/*
* OGL01Shape3D.cpp: 3D Shapes
*/
#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <iostream>
#include <string>
//#include <soil.h>

/* Global variables */
char title[] = "3D Shapes";
static GLfloat g_fViewDistance = 5;
static BOOL g_bButton1Down = FALSE;
static int g_yClick = 0;
//variables to read own bmp and not use soil
unsigned char header[54];
unsigned int dataPos;
unsigned int width, height;
unsigned int imageSize;
unsigned char * data;
//int width, height;

/* Initialize OpenGL Graphics */
void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling	
}

/*Loads in texture files manually-checks they are legitimate BMP files then sets them to 2D textures*/
GLuint loadBMP_custom(const char * imagepath) {
	FILE * file = fopen(imagepath, "rb");
	if (!file) { printf("Image could not be opened\n"); return 0; }

	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	if (imageSize == 0) imageSize = width * height * 3;
	if (dataPos == 0) dataPos = 54;

	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);

	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix									
	glLoadIdentity();                 // load the model-view matrix
	//CHANGED these keys to achieve various model views, by changing camera/eye point, the reference point, and the UP point
	//gluLookAt(0, 0, g_fViewDistance, 0, 0, -1, 0, 1, 0);
	//gluLookAt(0, 4, g_fViewDistance, 0, 0, -1, 0, 1, 0);
	//gluLookAt(4, 4, g_fViewDistance, 0, -1, -1, 0, 1, 0);
	//gluLookAt(-5, 4, g_fViewDistance, 0, -1, -1, 0, 1, 0);
	gluLookAt(0, 31.75, 75, 0, 0, 0, 0, 5, 0.66);

	//glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads

	//~~~~~~~TOP~~~~~~~~~~~~~ 
	GLuint Texture = loadBMP_custom("wood2.bmp");
	glBegin(GL_POLYGON);
	//glColor3f(0.0f, 1.0f, 0.0f);     // Green
	glTexCoord2d(0.5, 1);
	glVertex3f(21.0f, 11.0f, -3005.0f);
	glTexCoord2d(0.0, 0.0);
	glVertex3f(-21.0f, 11.0f, -3005.0f);
	glTexCoord2d(1, 0);
	glVertex3f(-21.0f, 11.0f, 3005.0f);
	glTexCoord2d(1, 1);
	glVertex3f(21.0f, 11.0f, 3005.0f);
	glEnd();

	//~~~~~~~BOTTOM~~~~~~~~~~~~~ 
	Texture = loadBMP_custom("wood2.bmp");
	glBegin(GL_POLYGON);
	//Bottom face (y = -1.0f)
	//glColor3f(1.0f, 0.5f, 0.0f);     // Orange
	glTexCoord2d(0.5, 1);
	glVertex3f(10.0f, -10.0f, 10.0f);
	glTexCoord2d(0.0, 0.0);
	glVertex3f(-10.0f, -10.0f, 10.0f);
	glTexCoord2d(1, 0);
	glVertex3f(-10.0f, -10.0f, -10.0f);
	glTexCoord2d(1, 1);
	glVertex3f(10.0f, -10.0f, -10.0f);
	glEnd();

	//~~~~~~~FRONT~~~~~~~~~~~~~ 
	Texture = loadBMP_custom("wood2.bmp");
	glBegin(GL_POLYGON);
	// Front face  (z = 1.0f)
	//glColor3f(1.0f, 0.0f, 0.0f);     // Red
	glTexCoord2d(0.5, 1);
	glVertex3f(10.0f, 10.0f, 10.0f);
	glTexCoord2d(0.0, 0.0);
	glVertex3f(-10.0f, 10.0f, 10.0f);
	glTexCoord2d(1, 0);
	glVertex3f(-10.0f, -10.0f, 10.0f);
	glTexCoord2d(1, 1);
	glVertex3f(10.0f, -10.0f, 10.0f);
	glEnd();

	//~~~~~~~BACK~~~~~~~~~~~~~ 
	Texture = loadBMP_custom("wood2.bmp");
	glBegin(GL_POLYGON);
	// Back face (z = -1.0f)
	//glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
	glTexCoord2d(0.5, 1);
	glVertex3f(10.0f, -10.0f, -10.0f);
	glTexCoord2d(0.0, 0.0);
	glVertex3f(-10.0f, -10.0f, -10.0f);
	glTexCoord2d(1, 0);
	glVertex3f(-10.0f, 10.0f, -10.0f);
	glTexCoord2d(1, 1);
	glVertex3f(10.0f, 10.0f, -10.0f);
	glEnd();

	//~~~~~~~LEFT~~~~~~~~~~~~~ 
	Texture = loadBMP_custom("wood2.bmp");
	glBegin(GL_POLYGON);
	// Left face (x = -1.0f)
	//glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	glTexCoord2d(0.5, 1);
	glVertex3f(-10.0f, 10.0f, 10.0f);
	glTexCoord2d(0.0, 0.0);
	glVertex3f(-10.0f, 10.0f, -10.0f);
	glTexCoord2d(1, 0);
	glVertex3f(-10.0f, -10.0f, -10.0f);
	glTexCoord2d(1, 1);
	glVertex3f(-10.0f, -10.0f, 10.0f);
	glEnd();

	//~~~~~~~RIGHT~~~~~~~~~~~~~ 
	Texture = loadBMP_custom("wood2.bmp");
	glBegin(GL_POLYGON);
	// Right face (x = 1.0f)
	//glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
	glTexCoord2d(0.5, 1);
	glVertex3f(10.0f, 10.0f, -10.0f);
	glTexCoord2d(0.0, 0.0);
	glVertex3f(10.0f, 10.0f, 10.0f);
	glTexCoord2d(1, 0);
	glVertex3f(10.0f, -10.0f, 10.0f);
	glTexCoord2d(1, 1);
	glVertex3f(10.0f, -10.0f, -10.0f);
	glEnd();  // End of drawing color-cube
	

//	glColor3f(1.0f, 0.0f, 0.0f);
//	glutWireTeapot(1);

	glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

/* Handler for window re-size event. Called back when the window first appears and
whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
											   // Compute aspect ratio of the new window
	if (height == 0) height = 1;                // To prevent divide by 0
		GLfloat aspect = (GLfloat)width / (GLfloat)height;
		glViewport(0, 0, width, height);
	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();
	gluPerspective(45.0f, aspect, 0.01f, 100.0f); // Enable perspective projection with fovy, aspect, zNear and zFar
}

void MouseButton(int button, int state, int x, int y)
{
	// Respond to mouse button presses.
	// If button1 pressed, mark this state so we know in motion function.
	if (button == GLUT_LEFT_BUTTON)
	{
		g_bButton1Down = (state == GLUT_DOWN) ? TRUE : FALSE;
		g_yClick = y - g_fViewDistance;
		printf("g_yClick is %d\n", g_yClick);

	}
}
void MouseMotion(int x, int y)
{
	// If button1 pressed, zoom in/out if mouse is moved up/down.
	if (g_bButton1Down)
	{
		g_fViewDistance = (y - g_yClick);
		printf("The eye location is %f and the mouse co-ordinates are (%d, %d) and \n", g_fViewDistance,x,y);
		if (g_fViewDistance < 0)
			g_fViewDistance = 0;
		glutPostRedisplay();
	}
}


/*
GLuint loadtextures(char* filename) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	unsigned char *image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	return textureId;
}

void initRendering() {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	GLUquadric* quad = gluNewQuadric();
	GLUquadric* quad2 = gluNewQuadric();
	GLuint _textureId = loadtextures("textures/bubbles.bmp");
	GLuint _textureId1 = loadtextures("textures/camo.bmp");
	GLuint _textureId2 = loadtextures("textures/grass.bmp");
	GLuint _textureId3 = loadtextures("textures/stone.bmp");
	GLuint _textureId4 = loadtextures("textures/wood1.bmp");
	GLuint _textureId5 = loadtextures("textures/wood2.bmp");
}
*/

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
	glutInit(&argc, argv);				// Initialize GLUT
	glutInitWindowSize(640, 480);		// Set the window's initial width & height	
	glutCreateWindow(title);			// Create window with the given title
	glutDisplayFunc(display);			// Register callback handler for window re-paint event
	glutReshapeFunc(reshape);		    // Register callback handler for window re-size event
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	//loadtextures("textures");
	initGL();							// Our own OpenGL initialization
	glutMainLoop();						// Enter the infinite event-processing loop
	return 0;
}