#include<iostream>
//#include<glew.h>
#include<gl/glut.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<Windows.h>
#include "glm/glm.hpp"
#include<math.h>
#include "Model_OBJ.h"
#include "Model_OBJ_Texture.h"
#define TEXTURES_NUMBER 6
#define PI 3.14151926535
using namespace std;
float xRot = 0.0, yRot = 0.0, zRot = 0.0;
float xCam = 3.0, yCam = -3.0, xDirect= xCam, yDirect= yCam+1;
int blueAngle = 0;
GLuint textures[TEXTURES_NUMBER];
Model_OBJ_Texture  obj_momche, obj_sonjabi, obj_slot[3], obj_view_slot, obj_chair;
GLfloat vertices[][3] = {
	{ -4.0, -4.0,  2.0 },   // 0 
	{ -4.0,  4.0,  2.0 },   // 1
	{ 4.0,  4.0,  2.0 },   // 2
	{ 4.0, -4.0,  2.0 },   // 3
	{ -4.0, -4.0, -1.0 },   // 4
	{ -4.0,  4.0, -1.0 },   // 5
	{ 4.0,  4.0, -1.0 },   // 6
	{ 4.0, -4.0, -1.0 } };  // 7


void polygon(int a, int b, int c, int d)
{
	glEnable(GL_TEXTURE_2D);
	//top
	if (a == 0) {
		glBindTexture(GL_TEXTURE_2D, textures[2]);
		glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);
		glTexCoord2f(3, 3);
		glVertex3fv(vertices[a]);
		glTexCoord2f(0, 3);
		glVertex3fv(vertices[b]);
		glTexCoord2f(0, 0);
		glVertex3fv(vertices[c]);
		glTexCoord2f(3, 0);
		glVertex3fv(vertices[d]);
		glEnd();
	}
	//back
	else if (a == 2) {
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glBegin(GL_POLYGON);
		glNormal3f(-1, 0, 0);
		glTexCoord2f(4, 4);
		glVertex3fv(vertices[a]);
		glTexCoord2f(0, 4);
		glVertex3fv(vertices[b]);
		glTexCoord2f(0, 0);
		glVertex3fv(vertices[c]);
		glTexCoord2f(4, 0);
		glVertex3fv(vertices[d]);
		glEnd();
	}
	//left
	else if (a == 3) {
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glBegin(GL_POLYGON);
		glNormal3f(0, 1, 0);
		glTexCoord2f(4, 4);
		glVertex3fv(vertices[a]);
		glTexCoord2f(0, 4);
		glVertex3fv(vertices[b]);
		glTexCoord2f(0, 0);
		glVertex3fv(vertices[c]);
		glTexCoord2f(4, 0);
		glVertex3fv(vertices[d]);
		glEnd();
	}
	//bottom
	else if (a == 4) {
		glBindTexture(GL_TEXTURE_2D, textures[3]);
		glBegin(GL_POLYGON);
		glNormal3f(1, 0, 0);
		glTexCoord2f(4, 4);
		glVertex3fv(vertices[a]);
		glTexCoord2f(0, 4);
		glVertex3fv(vertices[b]);
		glTexCoord2f(0, 0);
		glVertex3fv(vertices[c]);
		glTexCoord2f(4, 0);
		glVertex3fv(vertices[d]);
		glEnd();
	}
	//front
	else if (a == 5) {
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glBegin(GL_POLYGON);
		glNormal3f(1, 0, 0);
		glTexCoord2f(4, 0);
		glVertex3fv(vertices[d]);
		glTexCoord2f(0, 0);
		glVertex3fv(vertices[c]);
		glTexCoord2f(0, 4);
		glVertex3fv(vertices[b]);
		glTexCoord2f(4, 4);
		glVertex3fv(vertices[a]);
		glEnd();
	}
	//right
	else if (a == 1) {
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glBegin(GL_POLYGON);
		glNormal3f(0, -1, 0);
		glTexCoord2f(4, 4);
		glVertex3fv(vertices[a]);
		glTexCoord2f(0, 4);
		glVertex3fv(vertices[b]);
		glTexCoord2f(0, 0);
		glVertex3fv(vertices[c]);
		glTexCoord2f(4, 0);
		glVertex3fv(vertices[d]);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}

// 6개의 면을 만든다.
void createCube(void)
{
	polygon(0, 3, 2, 1);    
	polygon(2, 3, 7, 6);    
	polygon(3, 0, 4, 7);    
	polygon(4, 5, 6, 7);    
	polygon(1, 2, 6, 5);    
	polygon(5, 4, 0, 1);    
}

void init(void) {
	char filename1[] = "wall.jpg";
	char filename2[] = "wall.jpg";
	char filename3[] = "up.jpg";
	char filename4[] = "floor.jpg";
	//char filename5[] = "slot1.png";
	glGenTextures(4, textures);
	LoadMeshFromFile(filename1, textures[0]);
	LoadMeshFromFile(filename2, textures[1]);
	LoadMeshFromFile(filename3, textures[2]);
	LoadMeshFromFile(filename4, textures[3]);
	//LoadMeshFromFile(filename5, textures[4]);
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
void keyboard(unsigned char key, int x, int y) {
	float temp_x = xDirect - xCam;
	float temp_y = yDirect - yCam;
	float rot_x = 0;
	float rot_y = 0;
	switch (key) {
	case 'x':
		//xRot += 3.0;
		break;
	case 'y':
		//yRot += 3.0;
		break;
	case'z':
		//zRot += 3.0;
		break;
	case '1':
		//xCam += 0.1;
		break;
	case '2':
		//xCam -= 0.1;
		break;
	case 'a':
		rot_x = temp_x*cos(5 * PI / 180.0f) - temp_y*sin(5 * PI / 180.0f);
		rot_y = temp_y*cos(5 * PI / 180.0f) + temp_x*sin(5 * PI / 180.0f);
		xDirect = xCam + rot_x;
		yDirect = yCam + rot_y;
		break;
	case 's':
		rot_x = temp_x*cos(5 * PI / 180.0f) + temp_y*sin(5 * PI / 180.0f);
		rot_y = temp_y*cos(5 * PI / 180.0f) - temp_x*sin(5 * PI / 180.0f);
		xDirect = xCam + rot_x;
		yDirect = yCam + rot_y;
		break;
	}
	glutPostRedisplay();
}
void specialKeyboard(int key, int x, int y) {
	float temp_x = xDirect - xCam;
	float temp_y = yDirect - yCam;
	float cross_x = 0;
	float cross_y = 0;
	switch (key) {
	case GLUT_KEY_UP:
		xCam += temp_x / 10.0f;
		yCam += temp_y / 10.0f;
		xDirect += temp_x / 10.0f;
		yDirect += temp_y / 10.0f;
		break;
	case GLUT_KEY_DOWN:
		xCam -= temp_x / 10.0f;
		yCam -= temp_y / 10.0f;
		xDirect -= temp_x / 10.0f;
		yDirect -= temp_y / 10.0f;
		break;
	case GLUT_KEY_LEFT:
		cross_x = -temp_y;
		cross_y = temp_x;
		xCam += cross_x / 10.0f;
		yCam += cross_y / 10.0f;
		xDirect += cross_x / 10.0f;
		yDirect += cross_y / 10.0f;
		break;
	case GLUT_KEY_RIGHT:
		cross_x = temp_y;
		cross_y = -temp_x;
		xCam += cross_x / 10.0f;
		yCam += cross_y / 10.0f;
		xDirect += cross_x / 10.0f;
		yDirect += cross_y / 10.0f;
		break;
	}
	glutPostRedisplay();
}
void myDisplay(void) {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float temp_x = xDirect - xCam;
	float temp_y = yDirect - yCam;
	if (xCam < -3.0) {
		xCam = -3.0;
		xDirect = temp_x + xCam;
	}
	if (xCam > 3.0) {
		xCam = 3.0;
		xDirect = temp_x + xCam;
	}
	if (yCam < -3.0) {
		yCam = -3.0;
		yDirect = temp_y + yCam;
	}
	if (yCam > 3.0) {
		yCam = 3.0;
		yDirect = temp_y + yCam;
	}
	gluLookAt(xCam, yCam, 0.0, xDirect, yDirect , 0.0, 0.0, 0.0, 1.0);
	
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
		glRotatef(xRot, 1.0, 0.0, 0.0);
		glRotatef(yRot, 0.0, 1.0, 0.0);
		glRotatef(zRot, 0.0, 0.0, 1.0);
		createCube();
		glPushMatrix();
		GLfloat amb_light[] = { 0.5, 0.5, 0.5, 1.0 };
		GLfloat diffuse[] = { 0.7, 0.7, 0.7, 1 };
		GLfloat specular[] = { 1, 1, 1, 1 };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		
		//(+,+) 1번
		glPushMatrix();
		glScalef(0.8, 0.8, 0.8);
		glTranslatef(3.0, 3.5, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 0, 0, 1);
		obj_view_slot.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(3.5, 2.8, -1.0);
		glScalef(0.4, 0.4, 0.4);
		glRotatef(90, 1, 0, 0);
		obj_chair.Draw();
		glPopMatrix();

		glPushMatrix();
		glScalef(0.8, 0.8, 0.8);
		glTranslatef(3.0, 2.0, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 0, 0, 1);
		obj_view_slot.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(3.5, 1.6, -1.0);
		glScalef(0.4, 0.4, 0.4);
		glRotatef(90, 1, 0, 0);
		obj_chair.Draw();
		glPopMatrix();

		glPushMatrix();
		glScalef(0.8, 0.8, 0.8);
		glTranslatef(3.0, 0.5, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 0, 0, 1);
		obj_view_slot.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(3.5, 0.4, -1.0);
		glScalef(0.4, 0.4, 0.4);
		glRotatef(90, 1, 0, 0);
		obj_chair.Draw();
		glPopMatrix();

		glPushMatrix();
		glScalef(0.8, 0.8, 0.8);
		glTranslatef(3.0, -1.0, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 0, 0, 1);
		obj_view_slot.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(3.5, -0.8, -1.0);
		glScalef(0.4, 0.4, 0.4);
		glRotatef(90, 1, 0, 0);
		obj_chair.Draw();
		glPopMatrix();

		//(+,+) 2번
		glPushMatrix();
		glScalef(0.8, 0.8, 0.8);
		glTranslatef(2.0, 3.5, 0);
		glRotatef(-90, 0, 1, 0);
		glRotatef(-90, 0, 0, 1);
		obj_view_slot.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.7, 2.8, -1.0);
		glScalef(0.4, 0.4, 0.4);
		glRotatef(90, 1, 0, 0);
		obj_chair.Draw();
		glPopMatrix();

		glPushMatrix();
		glScalef(0.8, 0.8, 0.8);
		glTranslatef(2.0, 2.0, 0);
		glRotatef(-90, 0, 1, 0);
		glRotatef(-90, 0, 0, 1);
		obj_view_slot.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.7, 1.6, -1.0);
		glScalef(0.4, 0.4, 0.4);
		glRotatef(90, 1, 0, 0);
		obj_chair.Draw();
		glPopMatrix();

		glPushMatrix();
		glScalef(0.8, 0.8, 0.8);
		glTranslatef(2.0, 0.5, 0);
		glRotatef(-90, 0, 1, 0);
		glRotatef(-90, 0, 0, 1);
		obj_view_slot.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.7, 0.4, -1.0);
		glScalef(0.4, 0.4, 0.4);
		glRotatef(90, 1, 0, 0);
		obj_chair.Draw();
		glPopMatrix();

		glPushMatrix();
		glScalef(0.8, 0.8, 0.8);
		glTranslatef(2.0, -1.0, 0);
		glRotatef(-90, 0, 1, 0);
		glRotatef(-90, 0, 0, 1);
		obj_view_slot.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.7, -0.8, -1.0);
		glScalef(0.4, 0.4, 0.4);
		glRotatef(90, 1, 0, 0);
		obj_chair.Draw();
		glPopMatrix();
		//(-,+)
		glPushMatrix();
		glScalef(0.8, 0.8, 0.8);
		glTranslatef(-3.0, 3.5, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 0, 0, 1);
		obj_view_slot.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-1.4, 2.8, -1.0);
		glScalef(0.4, 0.4, 0.4);
		glRotatef(90, 1, 0, 0);
		obj_chair.Draw();
		glPopMatrix();
		
		glPushMatrix();
		glScalef(0.8, 0.8, 0.8);
		glTranslatef(-3.0, 2.0, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 0, 0, 1);
		obj_view_slot.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-1.4, 1.6, -1.0);
		glScalef(0.4, 0.4, 0.4);
		glRotatef(90, 1, 0, 0);
		obj_chair.Draw();
		glPopMatrix();

		glPushMatrix();
		glScalef(0.8, 0.8, 0.8);
		glTranslatef(-3.0, 0.5, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 0, 0, 1);
		obj_view_slot.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-1.4, 0.4 , -1.0);
		glScalef(0.4, 0.4, 0.4);
		glRotatef(90, 1, 0, 0);
		obj_chair.Draw();
		glPopMatrix();

		glPushMatrix();
		glScalef(0.8, 0.8, 0.8);
		glTranslatef(-3.0, -1.0, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 0, 0, 1);
		obj_view_slot.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-1.4, -0.8, -1.0);
		glScalef(0.4, 0.4, 0.4);
		glRotatef(90, 1, 0, 0);
		obj_chair.Draw();
		glPopMatrix();

		//(-,-)
		glPushMatrix();
		glScalef(0.8, 0.8, 0.8);
		glTranslatef(1.0, -4.0, 0.0);
		glRotatef(-90, 1, 0, 0);
		glRotatef(180, 0, 0, 1);
		obj_view_slot.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.8, -2.2, -1.0);
		glScalef(0.4, 0.4, 0.4);
		glRotatef(90, 1, 0, 0);
		obj_chair.Draw();
		glPopMatrix();

		glPushMatrix();
		glScalef(0.8, 0.8, 0.8);
		glTranslatef(-0.5, -4.0, 0.0);
		glRotatef(-90, 1, 0, 0);
		glRotatef(180, 0, 0, 1);
		obj_view_slot.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-0.4, -2.2, -1.0);
		glScalef(0.4, 0.4, 0.4);
		glRotatef(90, 1, 0, 0);
		obj_chair.Draw();
		glPopMatrix();

		glPushMatrix();
		glScalef(0.8, 0.8, 0.8);
		glTranslatef(-2.0, -4.0, 0.0);
		glRotatef(-90, 1, 0, 0);
		glRotatef(180, 0, 0, 1);
		obj_view_slot.Draw();
		glPopMatrix();
	
		glPushMatrix();
		glTranslatef(-1.6, -2.2, -1.0);
		glScalef(0.4, 0.4, 0.4);
		glRotatef(90, 1, 0, 0);
		obj_chair.Draw();
		glPopMatrix();

		glPushMatrix();
		glScalef(0.8, 0.8, 0.8);
		glTranslatef(-3.5, -4.0, 0.0);
		glRotatef(-90, 1, 0, 0);
		glRotatef(180, 0, 0, 1);
		obj_view_slot.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-2.8, -2.2, -1.0);
		glScalef(0.4, 0.4, 0.4);
		glRotatef(90, 1, 0, 0);
		obj_chair.Draw();
		glPopMatrix();


		//rotation slot machine
		glTranslatef(0, 3.0, 0);
		glRotatef(-90, 0, 0, 1);
		glTranslatef(-0.7, 0, 0);
		glScalef(0.8, 0.8, 0.8);
		glRotatef(-90, 0, 1, 0);	
		glRotatef(180, 1, 0, 0);
		glRotatef(-90, 0, 0, 1);
		obj_momche.Draw();
		obj_sonjabi.Draw();
		glPushMatrix();
			glTranslatef(-2, 0, 0);
		glPopMatrix();
		
		obj_slot[0].Draw();
		glPushMatrix();
			glTranslatef(-0.3, 0, 0);
			obj_slot[1].Draw();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.3, 0, 0);
			obj_slot[2].Draw();
		glPopMatrix();
		glPopMatrix();
	glPopMatrix();
	glDisable(GL_LIGHTING);
	glutSwapBuffers();

}
int main(int argc, char ** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(1200, 1400);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("rotate!");
	init();
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	obj_momche.Load("slotmachine_momche.obj");
	obj_sonjabi.Load("slotmachine_sonjabi.obj");
	obj_slot[0].Load("Slot_Machine_slot.obj");
	obj_slot[1].Load("Slot_Machine_slot.obj");
	obj_slot[2].Load("Slot_Machine_slot.obj");
	obj_view_slot.Load("slotmachine_reafinal.obj");
	obj_chair.Load("chair.obj");
	glutMainLoop();
	return 0;
}
