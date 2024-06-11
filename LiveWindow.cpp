/*********
ADDITIONAL FEATURES:
 1: Moving Clouds
 2: Repeating Clouds
 3: Ufo is blinking
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer
#define SPEED 2 //ufo's speed
#define CLOUDSPEED 0.44 //cloud's speed
#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
int ufolightflag = 0;  //for blinking ufo lights
float ufox = -310, ufoy = -210, cloudx = 160; // positions of ufo and cloud
bool animate = true; // for animation
//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char* string, void* font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

void wall() {

	int squareSide = 50; 
	//left side of the wall
	for (int w = -500; w < -300; w += squareSide) {
		for (int h = -300; h < 300; h += squareSide) {
			if ((w / squareSide + h / squareSide) % 2 == 0) {
				glColor3ub(204, 102, 153);

			}
			else {
				glColor3ub(255, 153, 204);
			}

			glRectf(w, h, w + squareSide, h + squareSide);
		}
	}
	//up side of the wall
	for (int w = -300; w < 300; w += squareSide) {
		for (int h = 200; h < 300; h += squareSide) {
			if ((w / squareSide + h / squareSide) % 2 == 0) {
				glColor3ub(204, 102, 153);

			}
			else {
				glColor3ub(255, 153, 204);
			}

			glRectf(w, h, w + squareSide, h + squareSide);
		}
	}
	//right side of the wall
	for (int w = 300; w < 500; w += squareSide) {
		for (int h = -300; h < 300; h += squareSide) {
			if ((w / squareSide + h / squareSide) % 2 == 0) {
				glColor3ub(204, 102, 153);

			}
			else {
				glColor3ub(255, 153, 204);
			}

			glRectf(w, h, w + squareSide, h + squareSide);
		}
	}
	//bottom side of the wall
	for (int w = -300; w < 300; w += squareSide) {
		for (int h = -300; h < -200; h += squareSide) {
			if ((w / squareSide + h / squareSide) % 2 == 0) {
				glColor3ub(204, 102, 153);

			}
			else {
				glColor3ub(255, 153, 204);
			}

			glRectf(w, h, w + squareSide, h + squareSide);
		}
	}
}

void background() {
	//gradient background
	glBegin(GL_POLYGON);

	glColor3ub(255, 153, 153);

	glVertex2f(-300, 200);
	glVertex2f(300, 200);
	glColor3ub(255, 204, 204);
	glVertex2f(300, -200);
	glVertex2f(-300, -200);
	glEnd();
	//sun
	glColor3ub(204, 0, 0);
	circle(0, 0, 120);

}
void mountain() {
	glBegin(GL_TRIANGLES);
	//gray part of mountain
	glColor3ub(68, 60, 64);
	glVertex2f(0, 0);
	glVertex2f(-150, -200);
	glVertex2f(150, -200);

	glEnd();
	//snowy part of mountain
	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 1);
	glVertex2f(0, 0);
	glVertex2f(-100, -133);
	glVertex2f(71, -95);
	glEnd();
}
void cloud() {
	//clouds
	glColor3ub(177, 145, 170);
	circle(cloudx - 20, 75, 15);
	circle(cloudx, 75, 20);
	circle(cloudx + 20, 75, 15);

	circle(-cloudx + 20, 85, 15);
	circle(-cloudx, 85, 20);
	circle(-cloudx - 20, 85, 15);

	circle(cloudx - 120, 150, 15);
	circle(cloudx - 100, 150, 20);
	circle(cloudx - 80, 150, 15);

	circle(cloudx - 120, -30, 15);
	circle(cloudx - 100, -30, 20);
	circle(cloudx - 80, -30, 15);
}
void tree() {
	//for background variety a sakura tree
	glBegin(GL_POLYGON);

	glColor3ub(153, 102, 51);

	glVertex2f(-300, 100);
	glVertex2f(-300, 75);
	glVertex2f(-250, 70);
	glVertex2f(-200, 60);
	glVertex2f(-100, 25);
	glVertex2f(100, 0);
	glVertex2f(-250, 75);
	glVertex2f(-300, 100);

	glEnd();
	//flowers
	glColor3ub(255, 0, 102);
	circle(-100, 50, 15);
	glColor3f(1, 1, 0);
	circle(-100, 50, 5);
	//flowers
	glColor3ub(255, 0, 102);
	circle(-200, 55, 15);
	glColor3f(1, 1, 0);
	circle(-200, 55, 5);
	//flowers
	glColor3ub(255, 0, 102);
	circle(50, 15, 10);
	glColor3f(1, 1, 0);
	circle(50, 15, 3);

}

void ufo() {
	//ufo's parts
	bool flag = ufolightflag / 15 == 1; //flag for ufo's blinking
	if (flag)
	glColor3ub(0, 204, 0);
	else
		glColor3ub(0, 255, 0);
	circle(ufox, ufoy, 25);
	glBegin(GL_POLYGON);
	glColor3ub(102, 0, 102);
	glVertex2f(ufox - 25, ufoy);
	glVertex2f(ufox + 25, ufoy);
	glVertex2f(ufox + 50, ufoy - 35);
	glVertex2f(ufox - 50, ufoy - 35);
	glEnd();
	//for coloring ufo
	if (flag)
		glColor3f(1, 1, 0);
	else
		glColor3f(0, 0, 0);
	circle(ufox - 25, ufoy - 15, 3);
	circle(ufox - 5, ufoy - 15, 3);
	circle(ufox + 15, ufoy - 15, 3);
	if (flag)
		glColor3f(0, 0, 0);
	else
		glColor3f(1, 1, 0);
	circle(ufox - 15, ufoy - 15, 3);
	circle(ufox + 5, ufoy - 15, 3);
	circle(ufox + 25, ufoy - 15, 3);
}
void title() {
	//title of the window
	glColor3ub(102, 51, 0);
	glRectf(-150, 250, 150, 200);
	glColor3ub(51, 26, 0);
	glRectf(-140, 240, 140, 210);
	glColor3f(1, 1, 1);
	vprint(-120, 220, GLUT_BITMAP_HELVETICA_12, "Mountain view by MUSTAFA CUREOGLU");
}
void window() {
	//windows
	glColor3ub(198, 140, 83);
	glRectf(300, 200, -300, 190);
	glRectf(-300, 200, -290, -200);
	glRectf(-300, -200, 300, -190);
	glRectf(300, -200, 290, 200);
	glRectf(0, 200, 10, -200);
	glRectf(0, 200, -10, -200);
	glRectf(-300, 5, 300, -5);
}
// 
// To display onto window using OpenGL commands
//
void display() {
	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);


	background();
	mountain();
	cloud();
	tree();
	ufo();
	wall();
	title();
	window();
	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	if (key == ' ')
		animate = !animate;

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP:
		up = true;
		break;
	case GLUT_KEY_DOWN:
		down = true;
		break;
	case GLUT_KEY_LEFT:
		left = true;
		break;
	case GLUT_KEY_RIGHT:
		right = true;
		break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP:
		up = false;
		break;
	case GLUT_KEY_DOWN:
		down = false;
		break;
	case GLUT_KEY_LEFT:
		left = false;
		break;
	case GLUT_KEY_RIGHT:
		right = false;
		break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	if (ufolightflag <= 30)
		ufolightflag += 1;
	else
		ufolightflag = 0;
	// Write your codes here.
	if (animate) {
		if (ufox<350 && ufox>-350) {
			ufox += SPEED;
			ufoy += SPEED * 2.0 / 3.0;
		}
		else {
			ufox = -310;
			ufoy = -210;
		}
	}
	cloudx -= CLOUDSPEED;
	if (cloudx < -350) {
		cloudx = 400;
	}
	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("There is an UFO turning around my mountain in Japan.");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}