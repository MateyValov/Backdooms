#include <iostream>
#include <GL/glut.h>
#include <math.h>

#define PI 3.141593
#define PI_LARGE 3.1415926535
#define RAD_90 (PI / 2)
#define RAD_270 (3 * PI / 2)
#define DEGREE_RADIAN 0.017453

#define RESOLUTION_WINDOW_X 1024
#define RESOLUTION_WINDOW_Y 512

#define RESOLUTION_GAME_X 320
#define RESOLUTION_GAME_Y 160

#define PLAYER_SPEED 5
#define PLAYER_FOV 60

#define MAP_TILES_X 5
#define MAP_TILES_Y 5
#define TILE_SIZE 64

float PLAYER_X;
float PLAYER_Y;
float PLAYER_DELTA_X = 0.f;
float PLAYER_DELTA_Y = 0.f;
float PLAYER_ANGLE = 0.f;

int map[] =
{
	1,1,1,1,1,
	1,0,0,0,1,
	0,0,0,0,1,
	1,0,0,0,1,
	1,1,0,1,1
};

using namespace std;

float dist(float ax, float ay, float bx, float by, float ang)
{
	return (sqrt((bx-ax)*(bx-ax) + (by - ay) * (by - ay)));
}

void drawRays3D()
{
	int r, mx, my, mp, dof;
	float rx = 0, ry = 0, ra, xo = 0, yo = 0, disT = 0;

	ra = PLAYER_ANGLE - DEGREE_RADIAN * (PLAYER_FOV / 2);

	if (ra < 0) { ra += 2 * PI; }
	if (ra > 2 * PI) { ra -= 2 * PI; }

	for (r = 0; r < PLAYER_FOV; r++)
	{
		dof = 0;
		float disH = 1000000;
		float hx = PLAYER_X;
		float hy = PLAYER_Y;

		float aTan = -1 / tan(ra);
		if (ra > PI) 
		{ 
			ry = (((int)PLAYER_Y>>6)<<6) - 0.0001; 
			rx = (PLAYER_Y - ry) * aTan + PLAYER_X;
			yo = -TILE_SIZE; 
			xo = -yo * aTan;
		}
		if (ra < PI)
		{
			ry = (((int)PLAYER_Y >> 6) << 6) + TILE_SIZE;
			rx = (PLAYER_Y - ry) * aTan + PLAYER_X;
			yo = TILE_SIZE;
			xo = -yo * aTan;
		}
		if (ra == 0 || ra == PI)
		{
			rx = PLAYER_X;
			ry = PLAYER_Y;
			dof = 5;
		}
		while (dof < 5)
		{
			mx = (int)(rx) >> 6; my = (int)(ry) >> 6; mp = my * MAP_TILES_X + mx;
			if (mx >= 0 && mx < MAP_TILES_X && my >= 0 && my < MAP_TILES_Y && map[mp] == 1) { hx = rx; hy = ry; disH = dist(PLAYER_X, PLAYER_Y, hx, hy, ra); dof = 5; }
			else { rx += xo; ry += yo; dof += 1; }
		}

		dof = 0;
		float disV = 1000000;
		float vx = PLAYER_X;
		float vy = PLAYER_Y;

		float nTan = -tan(ra);
		if (ra > RAD_90 && ra < RAD_270)
		{
			rx = (((int)PLAYER_X >> 6) << 6) - 0.0001;
			ry = (PLAYER_X - rx) * nTan + PLAYER_Y;
			xo = -TILE_SIZE;
			yo = -xo * nTan;
		}
		if (ra < RAD_90 || ra > RAD_270)
		{
			rx = (((int)PLAYER_X >> 6) << 6) + TILE_SIZE;
			ry = (PLAYER_X - rx) * nTan + PLAYER_Y;
			xo = TILE_SIZE;
			yo = -xo * nTan;
		}
		if (ra == RAD_90 || ra == RAD_270)
		{
			rx = PLAYER_X;
			ry = PLAYER_Y;
			dof = 5;
		}
		while (dof < 5)
		{
			mx = (int)(rx) >> 6; my = (int)(ry) >> 6; mp = my * MAP_TILES_X + mx;
			if (mx >= 0 && mx < MAP_TILES_X && my >= 0 && my < MAP_TILES_Y && map[mp] == 1) { vx = rx; vy = ry; disV = dist(PLAYER_X, PLAYER_Y, vx, vy, ra); dof = 5; }
			else { rx += xo; ry += yo; dof += 1; }
		}

		if (disV < disH) { rx = vx; ry = vy; disT = disV; glColor3f(0.9, 0, 0); }
		else { rx = hx; ry = hy; disT = disH; glColor3f(0.7, 0, 0); }

		glLineWidth(3); glBegin(GL_LINES); glVertex2i(PLAYER_X, PLAYER_Y); glVertex2i(rx, ry); glEnd();

		float ca = PLAYER_ANGLE - ra; if (ca < 0) { ca += 2 * PI; } if (ca > 2 * PI) { ca -= 2 * PI; } disT = disT * cos(ca);
		float lineH = (TILE_SIZE * RESOLUTION_GAME_X) / disT; if (lineH > RESOLUTION_GAME_X) { lineH = RESOLUTION_GAME_X; }
		float lineO = RESOLUTION_GAME_Y - lineH / 2;

		glLineWidth(8); glBegin(GL_LINES); glVertex2i(r * 8 + 325, lineO); glVertex2i(r * 8 + 325, lineH + lineO); glEnd();

		ra+=DEGREE_RADIAN;
		if (ra < 0) { ra += 2 * PI; }
		if (ra > 2 * PI) { ra -= 2 * PI; }
	}
}

void drawPlayer()
{
	glColor3f(1, 1, 0);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2i(PLAYER_X, PLAYER_Y);
	glEnd();

	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2i(PLAYER_X, PLAYER_Y);
	glVertex2i(PLAYER_X + PLAYER_DELTA_X * 5, PLAYER_Y + PLAYER_DELTA_Y * 5);
	glEnd();
}

void drawMap()
{
	int x, y, xo, yo;
	for (y = 0; y < MAP_TILES_Y; y++)
	{
		for (x = 0; x < MAP_TILES_X; x++)
		{
			if (map[y * MAP_TILES_X + x] == 1) { glColor3f(1, 1, 1); }
			else { glColor3f(0, 0, 0); }
			xo = x * TILE_SIZE; yo = y * TILE_SIZE;
			glBegin(GL_QUADS);
			glVertex2i(xo + 1, yo + 1);
			glVertex2i(xo + 1, yo + TILE_SIZE - 1);
			glVertex2i(xo + TILE_SIZE - 1, yo + TILE_SIZE - 1);
			glVertex2i(xo + TILE_SIZE - 1, yo + 1);
			glEnd();
		}
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawMap();
	drawPlayer();
	drawRays3D();
	glutSwapBuffers();
}

void init()
{
	glClearColor(0.3, 0.3, 0.3, 0);
	gluOrtho2D(0, RESOLUTION_WINDOW_X, RESOLUTION_WINDOW_Y, 0);
	PLAYER_X = 200.f;
	PLAYER_Y = 200.f;
	PLAYER_DELTA_X = cos(PLAYER_ANGLE) * 5;
	PLAYER_DELTA_Y = sin(PLAYER_ANGLE) * 5;
}

void input(unsigned char key, int x, int y)
{
	if (key == 'w') { PLAYER_X += PLAYER_DELTA_X; PLAYER_Y += PLAYER_DELTA_Y; }
	if (key == 'a') 
	{
		PLAYER_ANGLE -= 0.1; 
		if (PLAYER_ANGLE < 0) { PLAYER_ANGLE += 2 * PI; }
		PLAYER_DELTA_X = cos(PLAYER_ANGLE) * 5;
		PLAYER_DELTA_Y = sin(PLAYER_ANGLE) * 5;
	}
	if (key == 's') { PLAYER_X -= PLAYER_DELTA_X; PLAYER_Y -= PLAYER_DELTA_Y; }
	if (key == 'd')
	{
		PLAYER_ANGLE += 0.1;
		if (PLAYER_ANGLE > 2*PI) { PLAYER_ANGLE -= 2 * PI; }
		PLAYER_DELTA_X = cos(PLAYER_ANGLE) * 5;
		PLAYER_DELTA_Y = sin(PLAYER_ANGLE) * 5;
	}

	glutPostRedisplay();
}

void resize(int w, int h)
{
	glutReshapeWindow(RESOLUTION_WINDOW_X, RESOLUTION_WINDOW_Y);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(RESOLUTION_WINDOW_X, RESOLUTION_WINDOW_Y);
	glutCreateWindow("Backdooms");

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(input);

	glutMainLoop();
}
