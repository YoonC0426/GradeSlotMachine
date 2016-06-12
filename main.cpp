#include<iostream>
#include<glew.h>
#include<gl/glut.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<Windows.h>
#include "glm/glm.hpp"
#include<math.h>
#include "Model_OBJ.h"
#include "Model_OBJ_Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define PI 3.14151926535
using namespace std;
float xRot = 0.0, yRot = 0.0, zRot = 0.0;
float xCam = 0.8;
int blueAngle = 0;
GLubyte *pBytes; //데이터를 가리킬 포인터
BITMAPINFO *info; //비트맵 헤더 저장할 변수
GLuint textures[6];
GLuint slot_texture;
Model_OBJ obj,obj2;
Model_OBJ_Texture obj3;

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
bool LoadMeshFromFile(char *texFile) {
	FILE *fp = fopen(texFile, "rb");
	if (!fp) {
		printf("ERROR : NO %s.\n fail to bind %d\n", texFile, slot_texture);
		return false;
	}

	int width, height, channel;
	unsigned char *image = stbi_load_from_file(fp, &width, &height, &channel, 4);
	fclose(fp);

	//bind
	glBindTexture(GL_TEXTURE_2D, slot_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);

	return true;
}

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
		//top
		if (a == 0) {
			glBindTexture(GL_TEXTURE_2D, textures[1]);
			glBegin(GL_POLYGON);
				glNormal3f(0, 0, -1);
				glTexCoord2f(1, 1);
				glVertex3fv(vertices[a]);
				glTexCoord2f(0, 1);
				glVertex3fv(vertices[b]);
				glTexCoord2f(0, 0);
				glVertex3fv(vertices[c]);
				glTexCoord2f(1, 0);
				glVertex3fv(vertices[d]);
			glEnd();
		}
		//back
		else if (a == 2) {
			glBindTexture(GL_TEXTURE_2D, textures[2]);
			glBegin(GL_POLYGON);
			glNormal3f(-1, 0, 0);
			glTexCoord2f(1, 1);
			glVertex3fv(vertices[a]);
			glTexCoord2f(0, 1);
			glVertex3fv(vertices[b]);
			glTexCoord2f(0, 0);
			glVertex3fv(vertices[c]);
			glTexCoord2f(1, 0);
			glVertex3fv(vertices[d]);
			glEnd();
		}
		//left
		else if (a == 3) {
			glBindTexture(GL_TEXTURE_2D, textures[2]);
			glBegin(GL_POLYGON);
			glNormal3f(0, 1, 0);
			glTexCoord2f(1, 1);
			glVertex3fv(vertices[a]);
			glTexCoord2f(0, 1);
			glVertex3fv(vertices[b]);
			glTexCoord2f(0, 0);
			glVertex3fv(vertices[c]);
			glTexCoord2f(1, 0);
			glVertex3fv(vertices[d]);
			glEnd();
		}
		//bottom
		else if (a == 4) {
			glBindTexture(GL_TEXTURE_2D, textures[3]);
			glBegin(GL_POLYGON);
			glNormal3f(1, 0, 0);
			glTexCoord2f(1, 1);
			glVertex3fv(vertices[a]);
			glTexCoord2f(0, 1);
			glVertex3fv(vertices[b]);
			glTexCoord2f(0, 0);
			glVertex3fv(vertices[c]);
			glTexCoord2f(1, 0);
			glVertex3fv(vertices[d]);
			glEnd();
		}
		//front
		else if (a == 5) {
			glBindTexture(GL_TEXTURE_2D, textures[0]);
			glBegin(GL_POLYGON);
			glNormal3f(1, 0, 0);
			glTexCoord2f(1, 1);
			glVertex3fv(vertices[d]);
			glTexCoord2f(0, 1);
			glVertex3fv(vertices[c]);
			glTexCoord2f(0, 0);
			glVertex3fv(vertices[b]);
			glTexCoord2f(1, 0);
			glVertex3fv(vertices[a]);
			glEnd();
		}
		//right
		else if (a == 1) {
			glBindTexture(GL_TEXTURE_2D, textures[2]);
			glBegin(GL_POLYGON);
			glNormal3f(0, -1, 0);
			glTexCoord2f(1, 1);
			glVertex3fv(vertices[a]);
			glTexCoord2f(0, 1);
			glVertex3fv(vertices[b]);
			glTexCoord2f(0, 0);
			glVertex3fv(vertices[c]);
			glTexCoord2f(1, 0);
			glVertex3fv(vertices[d]);
			glEnd();
		}
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

GLubyte * LoadDIBitmap(const char *filename, BITMAPINFO **info)
{
	FILE *fp;
	GLubyte *bits;
	int bitsize, infosize;
	BITMAPFILEHEADER header;
	// 바이너리 읽기 모드로 파일을 연다
	if ((fp = fopen(filename, "rb")) == NULL)
		return NULL;
	// 비트맵 파일 헤더를 읽는다.
	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
		fclose(fp);
		return NULL;
	}
	// 파일이 BMP 파일인지 확인한다.
	if (header.bfType != 'MB') {
		fclose(fp);
		return NULL;
	}
	// BITMAPINFOHEADER 위치로 간다.
	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
	// 비트맵 이미지 데이터를 넣을 메모리 할당을 한다.
	if ((*info = (BITMAPINFO *)malloc(infosize)) == NULL) {
		fclose(fp);
		exit(0);
		return NULL;
	}
	// 비트맵 인포 헤더를 읽는다.
	if (fread(*info, 1, infosize, fp) < (unsigned int)infosize) {
		free(*info);
		fclose(fp);
		return NULL;
	}
	// 비트맵의 크기 설정
	if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
		bitsize = ((*info)->bmiHeader.biWidth *
		(*info)->bmiHeader.biBitCount + 7) / 8.0 *
		abs((*info)->bmiHeader.biHeight);
	// 비트맵의 크기만큼 메모리를 할당한다.
	if ((bits = (unsigned char *)malloc(bitsize)) == NULL) {
		free(*info);
		fclose(fp);
		return NULL;
	}
	// 비트맵 데이터를 bit(GLubyte 타입)에 저장한다.
	if (fread(bits, 1, bitsize, fp) < (unsigned int)bitsize) {
		free(*info); free(bits);
		fclose(fp);
		return NULL;
	}
	fclose(fp);
	return bits;
}

void myDisplay(void) {
	//텍스쳐 매핑 part1
	glGenTextures(6, textures);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	pBytes = LoadDIBitmap("texture1.bmp", &info);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 128, 128, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	pBytes = LoadDIBitmap("texture2.bmp", &info);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 128, 128, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);

	glBindTexture(GL_TEXTURE_2D, textures[2]);
	pBytes = LoadDIBitmap("texture3.bmp", &info);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 128, 128, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);

	glBindTexture(GL_TEXTURE_2D, textures[3]);
	pBytes = LoadDIBitmap("texture4.bmp", &info);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 128, 128, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);

	/*glBindTexture(GL_TEXTURE_2D, textures[4]);
	pBytes = LoadDIBitmap("texture5.bmp", &info);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 100, 100, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);

	glBindTexture(GL_TEXTURE_2D, textures[5]);
	pBytes = LoadDIBitmap("texture6.bmp", &info);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 100, 100, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);*/

	glEnable(GL_TEXTURE_2D);
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
		createCube();
		glPushMatrix();
		GLfloat amb_light[] = { 0.5, 0.5, 0.5, 1.0 };
		GLfloat diffuse[] = { 0.7, 0.7, 0.7, 1 };
		GLfloat specular[] = { 1, 1, 1, 1 };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);
		//glShadeModel(GL_SMOOTH);
		//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
		//glDepthFunc(GL_LEQUAL);
		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		//glEnable(GL_LIGHT0);
		float noMat[] = { 0.0,0.0,0.0,1.0 };
		float matAmb[] = { 0.3,0.3,0.3,1.0 };
		float matDif[] = { 1.0,1.0,1.0,1.0 };
		float matSpec[] = { 1.0,1.0,1.0,1.0 };
		float matShininess = 100.0f;

		glMaterialfv(GL_FRONT, GL_EMISSION, noMat);
		glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
		glMaterialf(GL_FRONT, GL_SHININESS, matShininess);
		
		glTranslatef(-0.7, 0, 0);
		glScalef(0.7, 0.7, 0.7);
		glRotatef(-90, 0, 1, 0);	
		glRotatef(180, 1, 0, 0);
		glRotatef(-90, 0, 0, 1);
		//glShadeModel(GL_FLAT);
		obj.Draw();
		obj2.Draw();
		if (LoadMeshFromFile("slot1.png"))
		obj3.Draw();
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
	//glDisable(GL_LIGHTING);
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
	obj.Load("Slot_Machine_momche.obj");
	obj2.Load("Slot_Machine_sonjabi.obj");
	obj3.Load("Slot_Machine_slot.obj");
	
	glutMainLoop();

	return 0;
}
