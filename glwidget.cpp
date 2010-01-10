/*! \file glwidget.cpp
	\brief GLWidget source file.

	Contains the source code for the GLWidget class, which is the main display widget, showing the slices/volume renders.
*/

#include "glwidget.h"

#include <qapplication.h>
#include <qtimer.h>

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

	dataset = new FlowData();
	dataset->loadDataset("dat\\c_block", false);
	chX = dataset->createChannelGeometry(0);
	chY = dataset->createChannelGeometry(1);
	vel = dataset->createChannelVectorLength(0,1,2);

	tf = new TFTexture(this);

	updateGL();
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
	qDebug() << "";
	qDebug() << from.c_str();
	switch ((i = glGetError())) {
		case GL_NO_ERROR: std::cout << "no error"; break;
		case GL_INVALID_ENUM:          qDebug() << "invalid enum";      break;
		case GL_INVALID_VALUE:         qDebug() <<  "invalid value";     break;
		case GL_INVALID_OPERATION:     qDebug() <<  "invalid operation"; break;
		case GL_STACK_OVERFLOW:        qDebug() <<  "stack overflow";    break;
		case GL_STACK_UNDERFLOW:       qDebug() <<  "stack underflow";   break;
		case GL_OUT_OF_MEMORY:         qDebug() <<  "out of memory";     break;
		default:
		qDebug() << "unknown error"; break;
	}
}

TFTexture* GLWidget::transferFunction()
{
	return tf;
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
		qDebug() << "the FBO is not complete";
		qDebug() << "Error: " << glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	}

	tf->setFBO(fbo_transfer, transferTexture);

	glViewport(0, 0, width(), height());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	//fragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);
	//rayShaderV = glCreateShaderObjectARB(GL_VERTEX_SHADER);
	//rayShaderF = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);
	//check_gl_error("create shaders");

	//GLcharARB* fragmentSource;
	//GLcharARB* vertexSource;
	//GLcharARB* raySourceV;
	//GLcharARB* raySourceF;

	//vertexSource = readShader("GLSL/slice.vert");
	//fragmentSource = readShader("GLSL/slice.frag");
	//raySourceV = readShader("GLSL/raycast.vert");
	//raySourceF = readShader("GLSL/raycast.frag");

	//glShaderSource(fragmentShader, 1, const_cast<const GLchar**>(&fragmentSource), 0);
	//glShaderSource(vertexShader, 1, const_cast<const GLchar**>(&vertexSource), 0);
	//glShaderSource(rayShaderV, 1, const_cast<const GLchar**>(&raySourceV), 0);
	//glShaderSource(rayShaderF, 1, const_cast<const GLchar**>(&raySourceF), 0);

	//glCompileShader(vertexShader);
	//glCompileShader(fragmentShader);
	//glCompileShader(rayShaderV);
	//glCompileShader(rayShaderF);
	//check_gl_error("compile shaders");

	//transferProgram = glCreateProgram();
	//check_gl_error("create program");
	//glAttachShader(transferProgram, vertexShader);
	//glAttachShader(transferProgram, fragmentShader);
	//check_gl_error("attach shaders");
	//glLinkProgram(transferProgram);
	//check_gl_error("link program");

	//rayProgram = glCreateProgram();
	//check_gl_error("create program");
	//glAttachShader(rayProgram, rayShaderV);
	//glAttachShader(rayProgram, rayShaderF);
	//check_gl_error("attach shaders");
	//glLinkProgram(rayProgram);
	//check_gl_error("link program");

	//GLint compiledv, compiledf, linked;

	//glGetShaderiv(rayShaderV, GL_COMPILE_STATUS, &compiledv);
	//glGetShaderiv(rayShaderF, GL_COMPILE_STATUS, &compiledf);

	//glGetProgramiv(rayProgram, GL_LINK_STATUS, &linked);

	//if (!compiledv)
	//	qDebug() << "vertex shader not compiled";
	//if (!compiledf)
	//	qDebug() << "ray shader not compiled";

	//if (!linked)
	//	qDebug() << "not linked ";

	//GLchar log[40960];
	//GLint len;
	//glGetShaderInfoLog(vertexShader, 40960, &len, log); 
	//std::cout << log << std::endl;

	//glGetShaderInfoLog(fragmentShader, 40960, &len, log); 
	//std::cout << log << std::endl;

	//glGetShaderInfoLog(rayShaderV, 40960, &len, log); 
	//std::cout << log << std::endl;
	//qDebug() << log;

	//glGetShaderInfoLog(rayShaderF, 40960, &len, log); 
	//std::cout << log << std::endl;
	//qDebug() << log;

	//glUseProgram(transferProgram);
	//check_gl_error("use program");

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
		qDebug() << "Error initializing GLEW";
		exit(1);
	}
	std::cout << "- GLEW initialized." << std::endl << std::endl;

	if (glewIsSupported("GL_VERSION_2_0")) {
		printf("Ready for OpenGL 2.0\n");
		qDebug() << "Ready for OpenGL 2.0";
	}else {
		printf("OpenGL 2.0 not supported\n");
		qDebug() << "OpenGL 2.0 not supported";
		exit(1);
	}

	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader) {
		printf("Ready for GLSL\n");
		qDebug() << "Ready for GLSL";
	} else {
		printf("Not totally ready :( \n");
		qDebug() << "Not totally ready";
		exit(1);
	}

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, float(width()), float(height()));
	gluPerspective(40.0, float(height())/float(width()), 0.01, 50.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
}

void GLWidget::timeOut()
{
    updateGL();
}

void GLWidget::timeOutSlot()
{
    timeOut();
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
	dataset->loadDataset(fileName, false);
}

