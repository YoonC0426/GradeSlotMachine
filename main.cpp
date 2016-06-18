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
float xCam = 0.8;
int blueAngle = 0;
GLuint textures[TEXTURES_NUMBER];
Model_OBJ_Texture  obj_momche, obj_sonjabi, obj_slot[3], obj_view_slot;
GLfloat vertices[][3] = {
	{ -4.0, -4.0,  2.0 },   // 0 
	{ -4.0,  4.0,  2.0 },   // 1
	{ 4.0,  4.0,  2.0 },   // 2
	{ 4.0, -4.0,  2.0 },   // 3
	{ -4.0, -4.0, -1.0 },   // 4
	{ -4.0,  4.0, -1.0 },   // 5
	{ 4.0,  4.0, -1.0 },   // 6
	{ 4.0, -4.0, -1.0 } };  // 7
/*bool LoadMeshFromFile(char *texFile, GLuint &tex_id) {
	FILE *fp = fopen(texFile, "rb");
	if (!fp) {
		printf("ERROR : NO %s.\n fail to bind %d\n", texFile, tex_id);
		return false;
	}

	int width, height, channel;
	unsigned char *image = stbi_load_from_file(fp, &width, &height, &channel, 4);
	fclose(fp);

	//bind
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	return true;
}*/

float* getNormal(float * v1, float * v2, float * v3)
{
	/* calculate Vector1 and Vector2 */
	float va[3], vb[3], vr[3], val;
	va[0] = v1[0] - v2[0];
	va[1] = v1[1] - v2[1];
	va[2] = v1[2] - v2[2];

	vb[0] = v1[0] - v3[0];
	vb[1] = v1[1] - v3[1];
	vb[2] = v1[2] - v3[2];

	/* cross product */
	vr[0] = va[1] * vb[2] - vb[1] * va[2];
	vr[1] = vb[0] * va[2] - va[0] * vb[2];
	vr[2] = va[0] * vb[1] - vb[0] * va[1];

	/* normalization factor */
	val = sqrt(vr[0] * vr[0] + vr[1] * vr[1] + vr[2] * vr[2]);

	float norm[3];
	norm[0] = vr[0] / val;
	norm[1] = vr[1] / val;
	norm[2] = vr[2] / val;

	return norm;
}

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
	char filename1[] = "front.jpg";
	char filename2[] = "left.jpg";
	char filename3[] = "up.jpg";
	char filename4[] = "texture4.bmp";
	char filename5[] = "slot1.png";
	char filename6[] = "ku.png";
	glGenTextures(6, textures);
	LoadMeshFromFile(filename1, textures[0]);
	LoadMeshFromFile(filename2, textures[1]);
	LoadMeshFromFile(filename3, textures[2]);
	LoadMeshFromFile(filename4, textures[3]);
	LoadMeshFromFile(filename5, textures[4]);
	LoadMeshFromFile(filename6, textures[5]);

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

	
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHT2);
	//glEnable(GL_LIGHT3);

	//float light_pos0[] = { 1.0, -1.0, 1.0, 1.0 };
	//float light_dir[] = { -0.2, 0.2, -0.2, 1.0 };
	//float light_ambient[] = { 0.3,0.3,0.3, 1.0 };
	//float light_diffuse0[] = { 0.0,1.0,0.0, 1.0 };
	//float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
	
	//glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0); //green

	//float light_pos1[] = { 1.0,1.0,1.0,1.0 };
	//float light_diffuse1[] = { 1.0,0.0,0.0,1.0 };

	//glLightfv(GL_LIGHT1, GL_POSITION, light_pos1);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1); //red

	//float light_pos2[] = { 1.0,-1.0,-1.0,1.0 };
	//float light_diffuse2[] = { 0.0,0.0,1.0,1.0 };

	//glLightfv(GL_LIGHT2, GL_POSITION, light_pos2);
	//glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2); //red

	//float light_pos3[] = { -0.6, 0.0, -0.6, 1.0 };
	//float light_diffuse3[] = { 0.5,0.2,0.3,1.0 };

	//glLightfv(GL_LIGHT3, GL_POSITION, light_pos3);
	//glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse3); //red
	
	
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_dir);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60.0);
	//glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 30.0);

	//glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0);
	//glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0);
	//glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 1.0);

	/*float noMat[] = { 0.0,0.0,0.0,1.0 };
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
	glVertex3f(2.0, 0.0, 0.0);

	glColor3f(1.0, 1.0, 1.0); // Y축 
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 2.0, 0.0);

	glColor3f(1.0, 1.0, 1.0); // Z축 
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 2.0);
	glEnd();
	
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
		//glEnable(GL_COLOR_MATERIAL);
		//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
		//glDepthFunc(GL_LEQUAL);
		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		//glEnable(GL_LIGHT0);
		/*float noMat[] = { 0.0,0.0,0.0,1.0 };
		float matAmb[] = { 0.3,0.3,0.3,1.0 };
		float matDif[] = { 1.0,1.0,1.0,1.0 };
		float matSpec[] = { 1.0,1.0,1.0,1.0 };
		float matShininess = 100.0f;

		glMaterialfv(GL_FRONT, GL_EMISSION, noMat);
		glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
		glMaterialf(GL_FRONT, GL_SHININESS, matShininess);*/
		obj_view_slot.Draw();
		glTranslatef(-0.7, 0, 0);
		glScalef(1.0, 1.0, 1.0);
		glRotatef(-90, 0, 1, 0);	
		glRotatef(180, 1, 0, 0);
		glRotatef(-90, 0, 0, 1);
		//glShadeModel(GL_FLAT);
		obj_momche.Draw();
		obj_sonjabi.Draw();
		glPushMatrix();
			glTranslatef(-2, 0, 0);
		glPopMatrix();
		
		//glEnable(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D, textures[4]);
		obj_slot[0].Draw();
		glPushMatrix();
			glTranslatef(-0.3, 0, 0);
			obj_slot[1].Draw();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.3, 0, 0);
			obj_slot[2].Draw();
		glPopMatrix();
		//glDisable(GL_TEXTURE_2D);
		//glDisable(GL_LIGHT0);
		//glDisable(GL_LIGHTING);
		glPopMatrix();
	glPopMatrix();
	/*glPushMatrix();
		float Emission[] = { 1.0,1.0,1.0,1.0 };
		glMaterialfv(GL_FRONT, GL_EMISSION, Emission);
		glTranslatef(light_pos[0], light_pos[1], light_pos[2]);
		glutSolidSphere(0.3, 100, 100);
	glPopMatrix();*/
	glDisable(GL_LIGHTING);
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
	glutInitWindowSize(1100, 1100);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("rotate!");
	init();
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	obj_momche.Load("Slot_Machine_Momche.obj");
	obj_sonjabi.Load("Slot_Machine_sonjabi.obj");
	obj_slot[0].Load("Slot_Machine_slot.obj");
	obj_slot[1].Load("Slot_Machine_slot.obj");
	obj_slot[2].Load("Slot_Machine_slot.obj");
	//obj_view_slot.Load("chair.obj");
	glutMainLoop();
	return 0;
}
