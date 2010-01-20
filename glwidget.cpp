/*! \file glwidget.cpp
	\brief GLWidget source file.

	Contains the source code for the GLWidget class, which is the main display widget, showing the different flow visualisation options.
*/

#include "glwidget.h"
#include <devil/include/il.h>

#include <qapplication.h>
#include <qtimer.h>
#include <QMouseEvent>
#include <QDebug>
#include <QDoubleSpinBox>
#include <QSpinBox>

GLWidget::GLWidget(int timerInterval, QWidget *parent) : QGLWidget(parent)
{
    if(timerInterval == 0)
        timer = 0;
    else
    {
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
        timer->start(timerInterval);
    }

	pong = false;
	ball = Ball(290, 260);
	paused = false;

	tf = new TFTexture(this);

	updateGL();

	dataset = new FlowData();
	//loadDataSet("dat\\c_block");
	loadDataSet("dat\\hurricane_p_tc_singletime10");

}

GLWidget::~GLWidget() {
	check_gl_error("deleting textures...");
	glDeleteTextures(1,&transferTexture);
	check_gl_error("...deleting textures");
}

QSize GLWidget::sizeHint() const
{
    return QSize(800, 600);
}

/* OpenGL debugging helper function */
void GLWidget::check_gl_error (std::string from) {
	std::cout << from + " - ";
	char buf[100];
	GLenum i;
	const char *e;
	//qDebug() << "";
	//qDebug() << from.c_str();
	switch ((i = glGetError())) {
		case GL_NO_ERROR: std::cout << "no error"; break;
		case GL_INVALID_ENUM:          qDebug() << "invalid enum";      break;
		case GL_INVALID_VALUE:         qDebug() <<  "invalid value";     break;
		case GL_INVALID_OPERATION:     qDebug() <<  "invalid operation"; break;
		case GL_STACK_OVERFLOW:        qDebug() <<  "stack overflow";    break;
		case GL_STACK_UNDERFLOW:       qDebug() <<  "stack underflow";   break;
		case GL_OUT_OF_MEMORY:         qDebug() <<  "out of memory";     break;
		default:
			qDebug() << "unknown error";
			break;
	}
}

TFTexture* GLWidget::transferFunction()
{
	return tf;
}

void GLWidget::toggleArrowPlot(bool enabled)
{
	arrowPlot = enabled;
}

void GLWidget::toggleArrowScale(bool enabled)
{
	arrowScale = enabled;
}

void GLWidget::setNumArrows(int num)
{
	numArrows = num;
}

void GLWidget::toggleStreamlines(bool enabled)
{
	streamlines = enabled;
}

void GLWidget::setRK(bool enabled)
{
	rk = enabled;
}

void GLWidget::setNumLines(int num)
{
	numLines = num;
}

void GLWidget::setNumSteps(int num)
{
	numSteps = num;

	if(lockedSteps)
	{
		stepSize = min(50.0/(float)num, 1.0);
		QDoubleSpinBox* spinBox = qFindChild<QDoubleSpinBox *>(parent(), "stepSizeBox");
		if(spinBox != 0)
			spinBox->setValue(stepSize);
		else
			qDebug() << "Spin Box not found";
	}
}

void GLWidget::setStepSize(double step)
{
	stepSize = step;

	if(lockedSteps)
	{
		numSteps = 50/stepSize;
		QSpinBox* spinBox = qFindChild<QSpinBox *>(parent(), "numStepBox");
		if(spinBox != 0)
			spinBox->setValue(numSteps);
		else
			qDebug() << "Spin Box not found";
	}
}

void GLWidget::lockStepSize(bool enabled)
{
	lockedSteps = enabled;
}

void GLWidget::togglePong(bool enabled)
{
	pong = enabled;

	if(pong)
	{
		setCursor(QCursor(Qt::BlankCursor));
		setMouseTracking(true);
	}
	else
	{
		setCursor(QCursor(Qt::ArrowCursor));
		setMouseTracking(false);
	}
}

void GLWidget::resetPong()
{
	ball.setPos(rand() % width(), rand() % height());
	ball.setVel();
}

void GLWidget::pausePong()
{
	paused = !paused;
}

char* GLWidget::readShader(char *fn) {

	FILE *fp;
	char *content = NULL;
	int count=0;

	if (fn != NULL) {

		fp = fopen(fn,"rt");

		if (fp != NULL) {
										      
			fseek(fp, 0, SEEK_END);
        			count = ftell(fp);
        			rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}

			fclose(fp);
		}
	}
	return content;
}

void GLWidget::setShaders(void)
{
	glGenFramebuffersEXT(1, &fbo_transfer);
	check_gl_error("generate FBO");
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_transfer);
	check_gl_error("bind FBO");

	glGenRenderbuffersEXT(1, &depth_rb);
	check_gl_error("generate renderbuffer");
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
	check_gl_error("bind renderbuffer");
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 2048, 2048);
	check_gl_error("buffer storage");
	
	glGenTextures(1, &transferTexture);
	check_gl_error("generate transfer texture");
	glBindTexture(GL_TEXTURE_2D, transferTexture);
	check_gl_error("bind transfer texture");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, 2048, 2048, 0, GL_BGRA, GL_FLOAT, NULL);
	check_gl_error("teximage2d transfer texture");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	check_gl_error("tex parameters filter");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	check_gl_error("tex parameters wrapping");

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, transferTexture, 0);
	check_gl_error("framebuffer");
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_rb);
	check_gl_error("renderbuffer");

	if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)!=GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		//qDebug() << "the FBO is not complete";
		//qDebug() << "Error: " << glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	}

	tf->setFBO(fbo_transfer, transferTexture);

	glViewport(0, 0, width(), height());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	fragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);
	arrowShaderV = glCreateShaderObjectARB(GL_VERTEX_SHADER);
	arrowShaderVScale = glCreateShaderObjectARB(GL_VERTEX_SHADER);
	arrowShaderF = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);
	check_gl_error("create shaders");

	GLcharARB* fragmentSource;
	GLcharARB* vertexSource;
	GLcharARB* arrowSourceV;
	GLcharARB* arrowSourceVScale;
	GLcharARB* arrowSourceF;

	vertexSource = readShader("GLSL/color.vert");
	fragmentSource = readShader("GLSL/color.frag");
	arrowSourceV = readShader("GLSL/arrow.vert");
	arrowSourceVScale = readShader("GLSL/arrowScale.vert");
	arrowSourceF = readShader("GLSL/arrow.frag");

	glShaderSource(fragmentShader, 1, const_cast<const GLchar**>(&fragmentSource), 0);
	glShaderSource(vertexShader, 1, const_cast<const GLchar**>(&vertexSource), 0);
	glShaderSource(arrowShaderV, 1, const_cast<const GLchar**>(&arrowSourceV), 0);
	glShaderSource(arrowShaderVScale, 1, const_cast<const GLchar**>(&arrowSourceVScale), 0);
	glShaderSource(arrowShaderF, 1, const_cast<const GLchar**>(&arrowSourceF), 0);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	glCompileShader(arrowShaderV);
	glCompileShader(arrowShaderVScale);
	glCompileShader(arrowShaderF);
	check_gl_error("compile shaders");

	gridProgram = glCreateProgram();
	//check_gl_error("create program");
	glAttachShader(gridProgram, vertexShader);
	glAttachShader(gridProgram, fragmentShader);
	//check_gl_error("attach shaders");
	glLinkProgram(gridProgram);
	check_gl_error("link program");

	arrowProgram = glCreateProgram();
	check_gl_error("create program");
	glAttachShader(arrowProgram, arrowShaderV);
	glAttachShader(arrowProgram, arrowShaderF);
	check_gl_error("attach shaders");
	glLinkProgram(arrowProgram);
	check_gl_error("link program");

	arrowScaleProgram = glCreateProgram();
	check_gl_error("create program");
	glAttachShader(arrowScaleProgram, arrowShaderVScale);
	glAttachShader(arrowScaleProgram, arrowShaderF);
	check_gl_error("attach shaders");
	glLinkProgram(arrowScaleProgram);
	check_gl_error("link program");

	GLint compiledv, compiledf, linked;

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiledv);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiledf);

	glGetProgramiv(gridProgram, GL_LINK_STATUS, &linked);

	//if (!compiledv)
		//qDebug() << "vertex shader not compiled";
	//if (!compiledf)
		//qDebug() << "fragment shader not compiled";

	//if (!linked)
		//qDebug() << "not linked ";

	GLchar log[40960];
	GLint len;
	glGetShaderInfoLog(vertexShader, 40960, &len, log); 
	//qDebug() << log;

	glGetShaderInfoLog(fragmentShader, 40960, &len, log); 
	//qDebug() << log;

	glGetShaderiv(arrowShaderV, GL_COMPILE_STATUS, &compiledv);
	glGetShaderiv(arrowShaderF, GL_COMPILE_STATUS, &compiledf);

	glGetProgramiv(arrowProgram, GL_LINK_STATUS, &linked);

	//if (!compiledv)
		//qDebug() << "vertex shader not compiled";
	//if (!compiledf)
		//qDebug() << "fragment shader not compiled";

	//if (!linked)
		//qDebug() << "not linked ";

	glGetShaderInfoLog(arrowShaderV, 40960, &len, log); 
	//qDebug() << log;

	glGetShaderInfoLog(arrowShaderVScale, 40960, &len, log); 
	//qDebug() << log;

	glGetShaderInfoLog(arrowShaderF, 40960, &len, log); 
	//qDebug() << log;

	glUseProgram(gridProgram);
	check_gl_error("use program");

	tf->generate();
}

void GLWidget::initializeGL()
{
	glViewport(0, 0, width(), height());

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Initialize GLEW
	std::cout << "- Initializing GLEW ..." << std::endl;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		// glewInit failed, something is seriously wrong
		std::cerr << "Error initializing GLEW: " << glewGetErrorString(err) << std::endl;
		//qDebug() << "Error initializing GLEW";
		exit(1);
	}
	std::cout << "- GLEW initialized." << std::endl << std::endl;

	if (glewIsSupported("GL_VERSION_2_0")) {
		printf("Ready for OpenGL 2.0\n");
		//qDebug() << "Ready for OpenGL 2.0";
	}else {
		printf("OpenGL 2.0 not supported\n");
		//qDebug() << "OpenGL 2.0 not supported";
		exit(1);
	}

	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader) {
		printf("Ready for GLSL\n");
		//qDebug() << "Ready for GLSL";
	} else {
		printf("Not totally ready :( \n");
		//qDebug() << "Not totally ready";
		exit(1);
	}

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_RECTANGLE_ARB);

	glDepthFunc(GL_LEQUAL);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	ilInit();
	
	ILuint spriteImage;
	ilGenImages(1, &spriteImage);
	ilBindImage(spriteImage);
	ilLoadImage("textures/arrow.gif");
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	
	glGenTextures(1, &sprite);
	glBindTexture(GL_TEXTURE_2D, sprite);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
	check_gl_error("generate arrow sprite");
	ilDeleteImages(1, &spriteImage);

	setShaders();
}

void GLWidget::resizeGL(int width, int height)
{
    height = height ? height : 1;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLWidget::paintGL()
{
	static bool init = false;

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, float(width()), float(height()));
	glOrtho(0.0f,width(),height(),0.0f,-1.0f,1.0f);				
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	//check_gl_error("test");
	if (init) {

		glColor3f(1, 1, 1);

		glUseProgram(gridProgram);

		glEnable(GL_TEXTURE_RECTANGLE_ARB);
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, gridTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, channel3Texture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, transferTexture);

		glUniform1iARB(glGetUniformLocation(gridProgram, "tex_grid"), 0);
		glUniform1iARB(glGetUniformLocation(gridProgram, "tex_channel3"), 1);
		glUniform1iARB(glGetUniformLocation(gridProgram, "tex_transfer"), 2);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0,0);
		glTexCoord2f(1, 0.0f);
		glVertex2f(width(),0);
		glTexCoord2f(1, 1);
		glVertex2f(width(),height());
		glTexCoord2f(0.0f, 1);
		glVertex2f(0,height());
		glEnd();

		glUseProgram(0);

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(0,0,-0.8);
			glTexCoord2f(314, 0.0f);
			glVertex3f(width(),0,-0.8);
			glTexCoord2f(314, 538);
			glVertex3f(width(),height(),-0.8);
			glTexCoord2f(0.0f, 538);
			glVertex3f(0,height(),-0.8);
		glEnd();

		if(pong)
		{
			if(!paused)
				updatePong();

			drawPong();
		}

		if(streamlines)
			drawStreamlines();

		if(arrowPlot)
			drawArrows();
	}
	init = true;
}

void GLWidget::drawArrows()
{
	int program;
	float maxSize = min(width(), height())/numArrows;

	if(arrowScale)
		program = arrowScaleProgram;
	else
		program = arrowProgram;

	glUseProgram(program);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, velocityTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite);

	glUniform1i(glGetUniformLocation(program, "arrow"), 0);
	glUniform1i(glGetUniformLocation(program, "velocity"), 1);
	glUniform1f(glGetUniformLocation(program, "maxSize"), maxSize);

	glEnable(GL_POINT_SPRITE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glTexEnvf(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBegin(GL_POINTS);
	for(int i = 1; i <= numArrows; i++)
	{
		for(int j = 1; j <= numArrows; j++)
		{
			glVertex2f(i/(float)(numArrows + 1) * width(), j/(float)(numArrows + 1) * height());
		}
	}
	glEnd();
}

void GLWidget::drawStreamlines()
{
	float x, y, mag;
	int index;

	glUseProgram(0);

	for(int i = 1; i <= numLines; i++)
	{
		for(int j = 1; j <= numLines; j++)
		{
			x = i/(float)(numLines + 1) * width();
			y = j/(float)(numLines + 1) * height();

			glBegin(GL_LINE_STRIP);

			for(int k = 0; k <= numSteps; k++)
			{
				index = ((int)x + (int)y * geometry->getDimX()) * 3;
				if(index >= 0 && index < geometry->getDimX() * geometry->getDimY() * 3 - 3)
					mag = velocity[index + 2]/dataset->getChannel(vel)->getMax();

				glColor3f(mag, 0, 0);
				glVertex2f(x, y);
				if(rk)
					rungeKutta(&x, &y);
				else
					euler(&x, &y);
			}

			glEnd();
		}
	}
}
void GLWidget::euler(float *x, float *y)
{
	float nextX, nextY, nextMag;
	int index;

	index = ((int)*x + (int)*y * geometry->getDimX()) * 3;
	if(index >= 0 && index < geometry->getDimX() * geometry->getDimY() * 3 - 3)
	{
		nextX = *x + stepSize * velocity[index + 1];
		nextY = *y + stepSize * velocity[index];
		*x = nextX;
		*y = nextY;
	}
}

void GLWidget::rungeKutta(float *x, float *y)
{
	float tmpX, nextX, tmpY, nextY;
	int index;

	index = ((int)*x + (int)*y * geometry->getDimX()) * 3;
	if(index >= 0 && index < geometry->getDimX() * geometry->getDimY() * 3 - 3)
	{
		tmpX = *x + stepSize * velocity[index + 1] / 2.0;
		tmpY = *y + stepSize * velocity[index] / 2.0;

		index = ((int)tmpX + (int)tmpY * geometry->getDimX()) * 3;
		if(index >= 0 && index < geometry->getDimX() * geometry->getDimY() * 3 - 3)
		{
			nextX = *x + stepSize * velocity[index + 1];
			nextY = *y + stepSize * velocity[index];
			*x = nextX;
			*y = nextY;
		}
	}
}

void GLWidget::updatePong()
{
	int index = ((int)ball.x() + (int)ball.y() * geometry->getDimX()) * 3;
	if(index >= 0 && index < geometry->getDimX() * geometry->getDimY() * 3 - 3)
		ball.update(vec3(velocity[index + 1], velocity[index]), player.pos(), player.mode());
	else
		ball.update();
}

void GLWidget::drawPong()
{
	glUseProgram(0);
	player.draw();
	ball.draw();
}

void GLWidget::timeOut()
{
    updateGL();
}

void GLWidget::timeOutSlot()
{
    timeOut();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	if(paused)
		event->ignore();
	else
	{
		player.setPos(event->x(), event->y());
	}
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	if(paused)
		event->ignore();
	else
	{
		if(event->button() == Qt::LeftButton)
			player.setMode(PONG_ATTRACTION);

		if(event->button() == Qt::RightButton)
			player.setMode(PONG_REPULSION);
	}
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if(paused)
		event->ignore();
	else
	{
		if(event->buttons() == Qt::NoButton)
			player.setMode(PONG_NEUTRAL);
	}
}

const int GLWidget::GetNextPowerOfTwo(const int iNumber)
{
	int iPowerOfTwo = 1;

	while (iPowerOfTwo < iNumber)
		iPowerOfTwo = iPowerOfTwo * 2;

	return iPowerOfTwo;
}

void GLWidget::loadDataSet(std::string fileName)
{
	int i = 0;
	while (!dataset->freeChannel[i]) {
		dataset->deleteChannel(i);
		i++;
	}

	dataset->loadDataset(fileName, false);
	chX = dataset->createChannelGeometry(0);
	chY = dataset->createChannelGeometry(1);
	vel = dataset->createChannelVectorLength(0,1,2);

	geometry = dataset->getGeometry();

	//check_gl_error("test");
	glGenTextures(1, &gridTexture);
	check_gl_error("generate grid texture");
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, gridTexture);
	check_gl_error("bind grid texture");
/*
	qDebug() << "Xmin: " << geometry->getMinX();
	qDebug() << "Xmax: " << geometry->getMaxX();
	qDebug() << "Ymin: " << geometry->getMinY();
	qDebug() << "Ymax: " << geometry->getMaxY();*/
	//qDebug() << "range: " << channel3Range;

	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB, geometry->getDimX(), geometry->getDimY(), 0, GL_RGB, GL_FLOAT, geometry->geometryData);
	check_gl_error("teximage2d grid texture");

	//qDebug() << "dimX: " << geometry->getDimX();
	//qDebug() << "dimY: " << geometry->getDimY();

	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	check_gl_error("grid tex parameters filter");

	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	check_gl_error("grid tex parameters wrapping");
	
	FlowChannel* channel3 = dataset->getChannel(3);
	float channel3Min = channel3->getMin();
	float channel3Max = channel3->getMax();
	float channel3Range = channel3->getRange();

	float* channel3_v = (float*) malloc(sizeof(float) * geometry->getDimX() * geometry->getDimY());

	for (int i = 0; i < geometry->getDimX()*geometry->getDimY(); i++) {
		channel3_v[i] = (channel3->getRawValue(i) - channel3Min) / channel3Range;
	}

	glGenTextures(1, &channel3Texture);
	check_gl_error("generate data texture");
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, channel3Texture);
	check_gl_error("bind data texture");

	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_LUMINANCE16F_ARB, geometry->getDimX(), geometry->getDimY(), 0, GL_LUMINANCE, GL_FLOAT, channel3_v);
	check_gl_error("teximage2d data texture");

	int vel = dataset->createChannelVectorLength(0,1,2);

	FlowChannel* v1 = dataset->getChannel(0);
	FlowChannel* v2 = dataset->getChannel(1);
	FlowChannel* v3 = dataset->getChannel(vel);

	velocity = (float*) malloc(sizeof(float) * geometry->getDimX() * geometry->getDimY() * 3);
	for (int i = 0; i < geometry->getDimX()*geometry->getDimY(); i++) {
		velocity[i*3] = v1->getRawValue(i);
		velocity[i*3+1] = v2->getRawValue(i);
		velocity[i*3+2] = v3->getRawValue(i);
	}

	glGenTextures(1, &velocityTexture);
	check_gl_error("generate velocity texture");
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, velocityTexture);
	check_gl_error("bind velocity texture");
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB32F_ARB, geometry->getDimX(), geometry->getDimY(), 0, GL_RGB, GL_FLOAT, velocity);

	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);
}
