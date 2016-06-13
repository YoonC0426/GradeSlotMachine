#include "Model_OBJ_Texture.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#define UVS_PER_VERTEX 2
#define POINTS_PER_VERTEX 3
#define TOTAL_FLOATS_IN_TRIANGLE 9
#define TOTAL_UV_FLOATS_IN_TRIANGLE 6
#define TOTAL_FLOATS_IN_QUAD 12
#define TOTAL_UV_FLOATS_IN_QUAD 8
#define VERTEX_PER_TRIANGLE 3
#define VERTEX_PER_QUAD 4
using namespace std;


Model_OBJ_Texture::Model_OBJ_Texture()
{
	this->TotalConnectedTriangles = 0;
	this->TotalConnectedQuads = 0;
	this->TotalConnectedPoints = 0;
	this->TotalConnectedVertexNormals = 0;
	this->TotalConnectedUvs = 0;
}


Model_OBJ_Texture::~Model_OBJ_Texture()
{
}

float * Model_OBJ_Texture::calculateNormal(float * coord1, float * coord2, float * coord3)
{
	/* calculate Vector1 and Vector2 */
	float va[3], vb[3], vr[3], val;
	va[0] = coord1[0] - coord2[0];
	va[1] = coord1[1] - coord2[1];
	va[2] = coord1[2] - coord2[2];

	vb[0] = coord1[0] - coord3[0];
	vb[1] = coord1[1] - coord3[1];
	vb[2] = coord1[2] - coord3[2];

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

int Model_OBJ_Texture::Load(char * filename)
{
	string line;
	ifstream objFile(filename);
	if (objFile.is_open())                                       // If obj file is open, continue
	{
		objFile.seekg(0, ios::end);                              // Go to end of the file, 
		long fileSize = objFile.tellg();                           // get file size
		objFile.seekg(0, ios::beg);                              // we'll use this to register memory for our 3d model
		uvBuffer = (float*)malloc(fileSize);
		vertexBuffer = (float*)malloc(fileSize);                     // Allocate memory for the verteces
		normalBuffer = (float*)malloc(fileSize);
		Faces_Triangles = (float*)malloc(fileSize * sizeof(float));         // Allocate memory for the triangles
		Faces_Triangles_vertex_normal = (float*)malloc(fileSize * sizeof(float));
		Faces_Triangles_uv = (float*)malloc(fileSize * sizeof(float));
		Faces_Quads = (float*)malloc(fileSize * sizeof(float));         // Allocate memory for the triangles
		Faces_Quads_vertex_normal = (float*)malloc(fileSize * sizeof(float));
		Faces_Quads_uv = (float*)malloc(fileSize * sizeof(float));
		normals = (float*)malloc(fileSize * sizeof(float));               // Allocate memory for the normals

		int triangle_index = 0;                                    // Set triangle index to zero
		int quad_index = 0;
		int normal_index = 0;                                    // Set normal index to zero
		int triangle_uv_index = 0;
		int quad_uv_index = 0;

		while (!objFile.eof())                                 // Start reading file data
		{
			getline(objFile, line);                                 // Get line from file
			if (line.c_str()[0] == 'v' && line.c_str()[1] != 'n' && line.c_str()[1] != 't')                              // The first character is a v: on this line is a vertex stored.
			{
				line[0] = ' ';                                    // Set first character to 0. This will allow us to use sscanf

				sscanf(line.c_str(), "%f %f %f ",                     // Read floats from the line: v X Y Z
					&vertexBuffer[TotalConnectedPoints],
					&vertexBuffer[TotalConnectedPoints + 1],
					&vertexBuffer[TotalConnectedPoints + 2]);

				TotalConnectedPoints += POINTS_PER_VERTEX;               // Add 3 to the total connected points
			}
			if (line.c_str()[0] == 'v'&&line.c_str()[1] == 'n')
			{
				line[0] = ' '; line[1] = ' ';
				sscanf(line.c_str(), "%f %f %f",
					&normalBuffer[TotalConnectedVertexNormals],
					&normalBuffer[TotalConnectedVertexNormals + 1],
					&normalBuffer[TotalConnectedVertexNormals + 2]);

				TotalConnectedVertexNormals += POINTS_PER_VERTEX;
			}
			if (line.c_str()[0] == 'v'&&line.c_str()[1] == 't')
			{
				line[0] = ' '; line[1] = ' ';
				sscanf(line.c_str(), "%f %f",
					&uvBuffer[TotalConnectedUvs],
					&uvBuffer[TotalConnectedUvs + 1]);
				TotalConnectedUvs += UVS_PER_VERTEX;
				uvBuffer[TotalConnectedUvs + 1] = 1.0 - uvBuffer[TotalConnectedUvs + 1];
			}
			if (line.c_str()[0] == 'f')                              // The first character is an 'f': on this line is a point stored
			{
				line[0] = ' ';   // Set first character to 0. This will allow us to use sscanf
				int vertexNumber[4] = { 0, 0 };
				int vertexNormalNumber[4] = { 0,0,0 };
				int uvNumber[4] = { 0,0 };
				int slash_cnt = 0;
				for (int i = 0; i < line.size(); i++) {
					if (line[i] == '/') slash_cnt++;
				}
				if (slash_cnt == 6)
				{
					sscanf(line.c_str(), "%i/%i/%i %i/%i/%i %i/%i/%i",
						&vertexNumber[0],
						&uvNumber[0],
						&vertexNormalNumber[0],
						&vertexNumber[1],
						&uvNumber[1],
						&vertexNormalNumber[1],
						&vertexNumber[2],
						&uvNumber[2],
						&vertexNormalNumber[2]);

					vertexNumber[0] -= 1;                              // OBJ file starts counting from 1
					vertexNumber[1] -= 1;                              // OBJ file starts counting from 1
					vertexNumber[2] -= 1;                              // OBJ file starts counting from 1
					vertexNormalNumber[0] -= 1;                              // OBJ file starts counting from 1
					vertexNormalNumber[1] -= 1;                              // OBJ file starts counting from 1
					vertexNormalNumber[2] -= 1;                              // OBJ file starts counting from 1
					uvNumber[0] -= 1;
					uvNumber[1] -= 1;
					uvNumber[2] -= 1;

					int tCounter = 0;
					int tuvCounter = 0;
					for (int i = 0; i < VERTEX_PER_TRIANGLE; i++)
					{
						Faces_Triangles[triangle_index + tCounter] = vertexBuffer[3 * vertexNumber[i]];
						Faces_Triangles[triangle_index + tCounter + 1] = vertexBuffer[3 * vertexNumber[i] + 1];
						Faces_Triangles[triangle_index + tCounter + 2] = vertexBuffer[3 * vertexNumber[i] + 2];
						Faces_Triangles_vertex_normal[triangle_index + tCounter] = normalBuffer[3 * vertexNormalNumber[i]];
						Faces_Triangles_vertex_normal[triangle_index + tCounter + 1] = normalBuffer[3 * vertexNormalNumber[i] + 1];
						Faces_Triangles_vertex_normal[triangle_index + tCounter + 2] = normalBuffer[3 * vertexNormalNumber[i] + 2];
						Faces_Triangles_uv[triangle_uv_index + tuvCounter] = uvBuffer[2 * uvNumber[i]];
						Faces_Triangles_uv[triangle_uv_index + tuvCounter + 1] = uvBuffer[2 * uvNumber[i] + 1];
						tCounter += POINTS_PER_VERTEX;
						tuvCounter += UVS_PER_VERTEX;
					}
					//uv 컨트롤 부분은 좀 있다가~
					triangle_index += TOTAL_FLOATS_IN_TRIANGLE;
					triangle_uv_index += TOTAL_UV_FLOATS_IN_TRIANGLE;
					TotalConnectedTriangles += TOTAL_FLOATS_IN_TRIANGLE;
				}
				else if (slash_cnt == 8)
				{
					sscanf(line.c_str(), "%i/%i/%i %i/%i/%i %i/%i/%i %i/%i/%i",
						&vertexNumber[0],
						&uvNumber[0],
						&vertexNormalNumber[0],
						&vertexNumber[1],
						&uvNumber[1],
						&vertexNormalNumber[1],
						&vertexNumber[2],
						&uvNumber[2],
						&vertexNormalNumber[2],
						&vertexNumber[3],
						&uvNumber[3],
						&vertexNormalNumber[3]);
					
					vertexNumber[0] -= 1;                              // OBJ file starts counting from 1
					vertexNumber[1] -= 1;                              // OBJ file starts counting from 1
					vertexNumber[2] -= 1;                              // OBJ file starts counting from 1
					vertexNumber[3] -= 1;
					vertexNormalNumber[0] -= 1;                              // OBJ file starts counting from 1
					vertexNormalNumber[1] -= 1;                              // OBJ file starts counting from 1
					vertexNormalNumber[2] -= 1;                              // OBJ file starts counting from 1
					vertexNormalNumber[3] -= 1;
					uvNumber[0] -= 1;
					uvNumber[1] -= 1;
					uvNumber[2] -= 1;
					uvNumber[3] -= 1;

					int qCounter = 0;
					int quvCounter = 0;
					for (int i = 0; i < VERTEX_PER_QUAD; i++)
					{
						Faces_Quads[quad_index + qCounter] = vertexBuffer[3 * vertexNumber[i]];
						Faces_Quads[quad_index + qCounter + 1] = vertexBuffer[3 * vertexNumber[i] + 1];
						Faces_Quads[quad_index + qCounter + 2] = vertexBuffer[3 * vertexNumber[i] + 2];
						Faces_Quads_vertex_normal[quad_index + qCounter] = normalBuffer[3 * vertexNormalNumber[i]];
						Faces_Quads_vertex_normal[quad_index + qCounter + 1] = normalBuffer[3 * vertexNormalNumber[i] + 1];
						Faces_Quads_vertex_normal[quad_index + qCounter + 2] = normalBuffer[3 * vertexNormalNumber[i] + 2];
						Faces_Quads_uv[quad_uv_index + quvCounter] = uvBuffer[2 * uvNumber[i]];
						Faces_Quads_uv[quad_uv_index + quvCounter + 1] = uvBuffer[2 * uvNumber[i] + 1];
						qCounter += POINTS_PER_VERTEX;
						quvCounter += UVS_PER_VERTEX;
					}
					quad_index += TOTAL_FLOATS_IN_QUAD;
					quad_uv_index += TOTAL_UV_FLOATS_IN_QUAD;
					TotalConnectedQuads += TOTAL_FLOATS_IN_QUAD;
				}	
			}
		}
		objFile.close();                                          // Close OBJ file
	}
	else
	{
		cout << "Unable to open obj file";
	}
	return 0;
}

void Model_OBJ_Texture::Draw()
{
	int tuv_count = 0, quv_count = 0;
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);
	int triangle_cnt = 0, quad_cnt = 0;
	//printf("Faces_Triangles_uv[0] : %f\n", Faces_Triangles_uv[0]);
	for (int i = 0; i < TotalConnectedTriangles; i += 3) {
		//vertex 1개에 대해
		glTexCoord2f(Faces_Triangles_uv[tuv_count], Faces_Triangles_uv[tuv_count+1]);
		glNormal3f(Faces_Triangles_vertex_normal[i], Faces_Triangles_vertex_normal[i + 1], Faces_Triangles_vertex_normal[i + 2]);
		glVertex3f(Faces_Triangles[i], Faces_Triangles[i + 1], Faces_Triangles[i + 2]);
		//printf("(Triangle %d) vertex : %f %f %f, uv : %f %f\n", ++triangle_cnt, Faces_Triangles[i], Faces_Triangles[i + 1], Faces_Triangles[i + 2], Faces_Triangles_uv[tuv_count], Faces_Triangles_uv[tuv_count + 1]);
		tuv_count += UVS_PER_VERTEX;
	}
	glEnd();
	glBegin(GL_QUADS);
	for (int i = 0; i < TotalConnectedQuads; i += 3) {
		glTexCoord2f(Faces_Quads_uv[quv_count], Faces_Quads_uv[quv_count+1]);
		glNormal3f(Faces_Quads_vertex_normal[i], Faces_Quads_vertex_normal[i + 1], Faces_Quads_vertex_normal[i + 2]);
		glVertex3f(Faces_Quads[i], Faces_Quads[i + 1], Faces_Quads[i + 2]);
		//printf("(QUAD %d) vertex : %f %f %f, uv : %f %f\n", ++quad_cnt, Faces_Quads[i], Faces_Quads[i + 1], Faces_Quads[i + 2], Faces_Quads_uv[quv_count], Faces_Quads_uv[quv_count + 1]);
		quv_count += UVS_PER_VERTEX;
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void Model_OBJ_Texture::Release()
{
	free(this->Faces_Triangles);
	free(this->normals);
	free(this->vertexBuffer);
}
