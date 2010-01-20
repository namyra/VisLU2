/*! \file glwidget.h
	\brief GLWidget header file.

	Contains the declarations for the GLWidget class, which is the main display widget, showing the different flow visualisation options.
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

	//! Slot that enables/disables the arrow plot.
	/*!
		\param enabled Whether to enable (true) or disable (false) the arrow plot.
	*/
	void toggleArrowPlot(bool enabled);

	//! Slot that enables/disables scaling of the arrow plot.
	/*!
		\param enabled Whether to enable (true) or disable (false) scaling of the arrow plot.
	*/
	void toggleArrowScale(bool enabled);

	//! Slot to set the number of arrows for the arrow plot.
	/*!
		\param num The new number of arrows (rows and colums) for the arrow plot.
	*/
	void setNumArrows(int num);

	//! Slot that enables/disables the streamlines.
	/*!
		\param enabled Whether to enable (true) or disable (false) the streamlines.
	*/
	void toggleStreamlines(bool enabled);

	//! Slot that toggles the algorithm used for approximating the streamlines.
	/*!
		This switches between Euler's method and the Runge-Kutta second order algorithm.
		\param enabled True to switch to Runge-Kutta, false to use Euler's method.
	*/
	void setRK(bool enabled);

	//! Slot to set the number of streamlines.
	/*!
		\param num The new number of streamlines (rows and colums).
	*/
	void setNumLines(int num);

	//! Slot to set the number of iteration steps for the streamlines.
	/*!
		\param num The new number of iteration steps for the streamlines.
	*/
	void setNumSteps(int num);

	//! Slot to set the size of each iteration step for the streamlines.
	/*!
		\param step The new size of each iteration step for the streamlines.
	*/
	void setStepSize(double step);

	//! Slot that locks/unlocks the ratio of step size to number of steps.
	/*!
		\param enabled Whether to lock (true) or unlock (false) the step size to the number of iteration steps.
	*/
	void lockStepSize(bool enabled);

	//! Slot that enables/disables the Pong game.
	/*!
		Also disables the cursor and enables mouse tracking when the game is enabled and vice versa.
		\param enabled Whether to enable (true) or disable (false) the Pong game.
	*/
	void togglePong(bool enabled);

	//! Slot that resets the Pong game.
	/*!
		This sets the ball at a random position within the display widget and resets its velocity to 0.
	*/
	void resetPong();

	//! Slot that toggles between pausing and unpausing the Pong game.
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
		Most of the work happens here. Displays any of the chosen functions, including an arrow plot, streamlines, and the Pong game.
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

	//! Handles mouse movement inside the widget.
	/*!
		Is called whenever the mouse moves inside the widget.
		If the Pong game is enabled and unpaused, this makes sure the Player is always at the cursor positon.
		\param event The mouse event.
	*/
	void mouseMoveEvent(QMouseEvent *event);

	//! Handles mouse clicks inside the widget.
	/*!
		Is called whenever a mouse button is clicked inside the widget.
		If the Pong game is enabled and unpaused, this switches to attraction mode when the left mouse button is pressed and repulsion mode when the right mouse button is pressed.
		\param event The mouse event.
	*/
	void mousePressEvent(QMouseEvent *event);

	//! Handles when a mouse button is released inside the widget.
	/*!
		Is called whenever a mouse button is released inside the widget.
		If the Pong game is enabled and unpaused, this switches to neutral mode when no mouse buttons are pressed anymore.
		\param event The mouse event.
	*/
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

	//! The flag that determines whether the arrow plot is enabled.
	bool arrowPlot;

	//! The flag that determines whether the arrows of the arrow plot should be scaled.
	bool arrowScale;

	//! The number of arrows (rows and columns) of the arrow plot.
	int numArrows;
	
	//! The flag that determines whether the streamlines are enabled.
	bool streamlines;

	//! The flag that determines which algorithm is used for the streamlines. True for Runge-Kutta second order, false for Euler's method.
	bool rk;

	//! The number of streamlines (rows and columns).
	int numLines;

	//! The number of iteration steps for the streamlines.
	int numSteps;

	//! The steps size of each iteration for the streamlines.
	float stepSize;

	//! The flag that determines whether the ratio of step size and number of steps is locked.
	bool lockedSteps;

	//! The flag that determines whether the Pong game is enabled.
	bool pong;

	//! The flag that determines whether the Pong game is paused.
	bool paused;

	//! The player representation (cursos) for the Pong game.
	Player player;

	//! The ball for the Pong game.
	Ball ball;

	//! The flow data.
	FlowData *dataset;

	//! The velocity data.
	float *velocity;

	//! The channel id for the x-coordinate of the velocity data.
	int chX;

	//! The channel id for the y-coordinate of the velocity data.
	int chY;

	//! The channel id for the magnitude of the velocity data.
	int vel;

	//! The flow geometry.
	FlowGeometry* geometry;
	//FlowChannel* channel3;

	//! The OpenGL id for the data channel 3 texture.
	GLuint channel3Texture;

	//! The OpenGL id for the velocity texture.
	GLuint velocityTexture;

	//! The OpenGL id for the vertex shader for the velocity magnitude image.
	GLuint vertexShader;

	//! The OpenGL id for the fragment shader for the velocity magnitude image.
	GLuint fragmentShader;

	//! The OpenGL id for the shader program for the velocity magnitude image.
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

	//! The OpenGL id for the frame buffer object used for rendering the grid texture.
	GLuint fbo_grid;

	//! The OpenGL id for the grid texture.
	GLuint gridTexture;

	//! The OpenGL id for the grid texture render buffer.
	GLuint depth_rb_grid;

	GLuint inverseGridXTexture;
	GLuint inverseGridYTexture;

	bool isFlipped;
	float texWidth;
	float texHeight;

	bool initVelocity;

	//! The OpenGL id for the velocity texture render buffer.
	GLuint velocityTextureFBO;
	GLuint fbo_velocity;
	GLuint depth_rb_velocity;

	void initializeVelocity(void);

	//! Draws the arrow plot.
	/*!
		Draws a grid of arrow point sprites, rotated and (optionally) scaled in the shader.
		Each arrow points in the direction of the velocity at that point and is optionally scaled in relation to the velocity magnitude.
	*/
	void drawArrows();

	//! Draws the streamlines.
	/*!
		Draws streamlines corresponding to the flow data.
		A grid of equidistant points serves as starting points, and then a number of iterations determines the path of the point along the flow,
		using either Euler's method or a Runge-Kutta second order algorithm to approximate the curve.
	*/
	void drawStreamlines();

	//! Approximates the next point in a curve using Euler's method.
	/*!
		Determines the next point in the flow curve starting from the given point and using the currently loaded velocity data.
		Euler's method is used to approximate the curve, using the step size set in the UI.
		\param x The x-coordinate of the starting point.
		\param y The y-coordinate of the starting point.
	*/
	void euler(float *x, float *y);

	//! Approximates the next point in a curve using a Runge-Kutta second order algorithm.
	/*!
		Determines the next point in the flow curve starting from the given point and using the currently loaded velocity data.
		A Runge-Kutta second order algorithm is used to approximate the curve, using the step size set in the UI.
		\param x The x-coordinate of the starting point.
		\param y The y-coordinate of the starting point.
	*/
	void rungeKutta(float *x, float *y);

	//! Updates the ball for the Pong game.
	/*!
		Calls the ball's update function with the velocity data at the ball's position.
	*/
	void updatePong();

	//! Draws the Pong game.
	/*!
		Calls the player's and ball's draw function.
	*/
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
