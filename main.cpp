#include<iostream>
#include<gl/glut.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include "Vector.h"
#include "Matrix.h"
#include "Mesh.h"
#define PI 3.14151926535
using namespace std;
float xRot = 0.0, yRot = 0.0, zRot = 0.0;
float xCam = 0.8;

GLfloat vertices[][3] = {
	{ -1.0, -1.0,  1.0 },   // 0 
	{ -1.0,  1.0,  1.0 },   // 1
	{ 1.0,  1.0,  1.0 },   // 2
	{ 1.0, -1.0,  1.0 },   // 3
	{ -1.0, -1.0, -1.0 },   // 4
	{ -1.0,  1.0, -1.0 },   // 5
	{ 1.0,  1.0, -1.0 },   // 6
	{ 1.0, -1.0, -1.0 } };  // 7

GLfloat vertices2[][3] = {
	{ -1.0, -0.2,  -0.2 },   // 0 
	{ -1.0,  0.2,  -0.2 },   // 1
	{ -0.2,  0.2,  -0.2 },   // 2
	{ -0.2, -0.2,  -0.2 },   // 3
	{ -1.0, -0.2, -1.0 },   // 4
	{ -1.0,  0.2, -1.0 },   // 5
	{ -0.2,  0.2, -1.0 },   // 6
	{ -0.2, -0.2, -1.0 } };  // 7

GLfloat colors[][3] = {
	{ 1.0, 0.0, 0.0 },      // red
	{ 1.0, 1.0, 1.0 },      // white
	{ 1.0, 1.0, 0.0 },      // yellow
	{ 0.0, 1.0, 0.0 },      // green
	{ 0.0, 0.0, 1.0 },      // blue
	{ 1.0, 0.0, 1.0 } };     // magenta

void polygon(int a, int b, int c, int d, int num)
{
	if (num == 1) {
		glColor3fv(colors[a]);
		glBegin(GL_POLYGON);
		glVertex3fv(vertices[a]);
		glVertex3fv(vertices[b]);
		glVertex3fv(vertices[c]);
		glVertex3fv(vertices[d]);
		glEnd();
	}
	else if (num == 2) {
		glColor3fv(colors[a]);
		glBegin(GL_POLYGON);
		glVertex3fv(vertices2[a]);
		glVertex3fv(vertices2[b]);
		glVertex3fv(vertices2[c]);
		glVertex3fv(vertices2[d]);
		glEnd();
	}
}

// 6개의 면을 만든다.
void createCube(void)
{
	polygon(0, 3, 2, 1, 1);    // front
	polygon(2, 3, 7, 6, 1);    // right
	polygon(3, 0, 4, 7, 1);    // bottom
	polygon(4, 5, 6, 7, 1);    // back
	polygon(1, 2, 6, 5, 1);    // top
	polygon(5, 4, 0, 1, 1);    // right

	polygon(0, 3, 2, 1, 2);    // front
	polygon(2, 3, 7, 6, 2);    // right
	polygon(3, 0, 4, 7, 2);    // bottom
	polygon(4, 5, 6, 7, 2);    // back
	polygon(1, 2, 6, 5, 2);    // top
	polygon(5, 4, 0, 1, 2);    // right
}

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho (-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);
	gluPerspective(60.0, 1.0, 1.0, 100.0);  // 멀고 가까움을 표현.
}

void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(xCam, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

	/*glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float light_pos[] = { 0.0, 0.0, 0.0 };
	float light_dir[] = { -0.2, 0.2, -0.2 };
	float light_ambient[] = { 0.3,0.3,0.3,1.0 };
	float light_diffuse[] = { 0.5,0.5,0.5,1.0 };
	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_dir);
	
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 30.0);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 1.0);

	float noMat[] = { 0.0,0.0,0.0,1.0 };
	float matAmb[] = { 0.3,0.3,0.3,1.0 };
	float matDif[] = { 1.0,1.0,1.0,1.0 };
	float matSpec[] = { 1.0,1.0,1.0,1.0 };
	float matShininess = 100.0f;
	
	glMaterialfv(GL_FRONT, GL_EMISSION, noMat);
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, matShininess);*/

	glBegin(GL_LINES);  // X, Y, Z 선 표시
	glColor3f(1.0, 1.0, 1.0); // X축 
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(10.0, 0.0, 0.0);

	glColor3f(1.0, 1.0, 1.0); // Y축 
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 10.0, 0.0);

	glColor3f(1.0, 1.0, 1.0); // Z축 
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 10.0);
	glEnd();
	
	glPushMatrix();
		glRotatef(xRot, 1.0, 0.0, 0.0);
		glRotatef(yRot, 0.0, 1.0, 0.0);
		glRotatef(zRot, 0.0, 0.0, 1.0);
		createCube();   // 큐브 생성
	glPopMatrix();

	/*glPushMatrix();
		float Emission[] = { 1.0,1.0,1.0,1.0 };
		glMaterialfv(GL_FRONT, GL_EMISSION, Emission);
		glTranslatef(light_pos[0], light_pos[1], light_pos[2]);
		glutSolidSphere(0.3, 100, 100);
	glPopMatrix();
	
	glDisable(GL_LIGHTING);*/
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'x':
		xRot += 3.0;
		break;
	case 'y':
		yRot += 3.0;
		break;
	case'z':
		zRot += 3.0;
		break;
	case '1':
		xCam += 0.1;
		break;
	case '2':
		xCam -= 0.1;
		break;
	}
	glutPostRedisplay();
}
int main(int argc, char ** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("rotate!");
	init();
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;

}