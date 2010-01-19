/*! \file glwidget.h
	\brief GLWidget header file.

	Contains the declarations for the GLWidget class, which is the main display widget, showing the slices/volume renders.
*/

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <algorithm>

#include "common.h"
#include <qgl.h>
#include "FlowData.h"
#include "TFTexture.h"
#include "Ball.h"
#include "Player.h"

//! Display widget for the volume dataset.
/*!
	Provides the widget that takes care of all the rendering of the volume data.
	Most of the work happens here.
	Includes both the 2d (slicing) and 3d (direct volume rendering) processes.
*/
class GLWidget : public QGLWidget
{
    Q_OBJECT

public:

	//! Constructor.
	/*!
		\param timerInterval The interval (in milliseconds) the widget uses to redraw.
		\param parent This widget's parent widget.
		\sa ~GLWidget()
	*/
    GLWidget(int timerInterval=0, QWidget *parent=0);
	
	//! Default destructor.
	/*!
		Deletes all the generated textures.
		\sa GLWidget()
	*/
	~GLWidget();

	//! Tells Qt which size this widget prefers to have.
	/*!
		Overwritten from Widget.
	*/
    QSize sizeHint() const;

	//! Loads a volume dataset.
	/*!
		Also generates the texture containing the dataset (3d if supported by hardware, 2d otherwise).
		\param fileName The filename of the volume data (.dat).
	*/
	void loadDataSet(std::string fileName);

	//! Utility function for debugging.
	/*!
		Outputs the last occurring OpenGL error (at the time of the function call)
		as well as a custom label to indicate where in the code this function call is.
		\param from The custom label indicating where in the code a function call occurrs.
	*/
	void check_gl_error (std::string from);

	//! Access function for the transfer function.
	/*!
		\return The transfer function used by this widget.
	*/
	TFTexture* transferFunction();

public slots:
	void toggleArrowPlot(bool enabled);
	void toggleArrowScale(bool enabled);
	void setNumArrows(int num);
	void toggleStreamlines(bool enabled);
	void setRK(bool enabled);
	void setNumLines(int num);
	void setNumSteps(int num);
	void setStepSize(float step);
	void togglePong(bool enabled);
	void resetPong();
	void pausePong();

protected:

	//! Initialises OpenGL.
	/*!
		Overwritten from QGLWidget.
		Initialises GLEW and GLSL. Most of the actual initialisation happens in setShaders().
		\sa setShaders(), paintGL(), resizeGL()
	*/
	void initializeGL(void);

	//! Resizes the GL viewport on widget resize.
	/*!
		Overwritten from QGLWidget.
		\param width New viewport width.
		\param height New viewport height.
		\sa initializeGL(), paintGL()
	*/
    void resizeGL(int width, int height);

	//! Renders the scene.
	/*!
		Most of the work happens here. Displays the slice or direct volume render of the data set using the set transfer function.
		\sa drawBoundingBox(), initializeGL(), resizeGL()
	*/
    void paintGL();

	//! Calls for update of the widget.
	/*!
		Is called whenever the timer sends a signal to timeOutSlot().
		Calls updateGL(), which in turn automatically calls for a redraw via paintGL().
		\sa timeOutSlot(), paintGL()
	*/
    void timeOut();

	void mouseMoveEvent(QMouseEvent *event);

	void mousePressEvent(QMouseEvent *event);

	void mouseReleaseEvent(QMouseEvent *event);

protected slots:

	//! Slot for the peroidic timer signal.
	/*!
		Calls timeOut(), which in turn calls for a redraw of this widget.
		/sa timeOut(), paintGL()
	*/
    void timeOutSlot();

private:
	//! The timer taking care of continual updates to this widget.
    QTimer *timer;

	bool arrowPlot;

	bool arrowScale;
	int numArrows;
	
	bool streamlines;

	bool rk;

	int numLines;
	int numSteps;
	float stepSize;

	bool pong;

	bool paused;
	Player player;
	Ball ball;

	//! The flow data.
	FlowData *dataset;

	float *velocity;

	int chX;

	int chY;

	int vel;

	FlowGeometry* geometry;
	//FlowChannel* channel3;
	GLuint channel3Texture;
	GLuint velocityTexture;

	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint gridProgram;

	//! The OpenGL id for the frame buffer object used for rendering the transfer function texture.
	GLuint fbo_transfer;

	//! The OpenGL id for the transfer function texture.
	GLuint transferTexture;

	//! The OpenGL id for the transfer function render buffer.
	GLuint depth_rb;

	//! The transfer texture used by this widget.
	TFTexture *tf;

	//! The OpenGL id for the vertex shader for the arrow plot without scaling.
	GLuint arrowShaderV;

	//! The OpenGL id for the vertex shader for the arrow plot with scaling.
	GLuint arrowShaderVScale;

	//! The OpenGL id for the fragment shader for the arrow plot.
	GLuint arrowShaderF;

	//! The OpenGL id for the arrow plot shader program without scaling.
	GLuint arrowProgram;

	//! The OpenGL id for the arrow plot shader program with scaling.
	GLuint arrowScaleProgram;

	//! The OpenGL id for the arrow sprite texture.
	GLuint sprite;

	GLuint fbo_grid;
	GLuint gridTexture;
	GLuint depth_rb_grid;

	void drawArrows();
	void drawStreamlines();
	void euler(float *x, float *y, float *tex);
	void rungeKutta(float *x, float *y, float *tex);
	void updatePong();
	void drawPong();

	//! Utility function to get the next higher power of two for any given integer.
	/*!
		Used to generate an appropriately sized texture for an arbitrarily large volume dataset if non-power-of-two textures are not supported.
		\param iNumber The integer you want to know the next higher power of two for.
	*/
	const int GetNextPowerOfTwo(const int iNumber);

	//! Does most of initialisation work.
	/*!
		Generates all the required OpenGL objects (textures, buffers, shaders, etc.).
		Loads all the required shader files.
		\sa readShader(), initializeGL()
	*/
	void setShaders(void);

	//! Reads a shader file into a string.
	/*!
		\param fn The filename of the shader file.
		\return The character pointer containing the shader code.
	*/
	char* readShader(char *fn);
};

#endif // GLWIDGET_H
