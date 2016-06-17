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
#include "Model_OBJ.h"
#define POINTS_PER_VERTEX 3
#define TOTAL_FLOATS_IN_TRIANGLE 9
#define VERTEX_PER_TRIANGLE 3
using namespace std;
Model_OBJ::Model_OBJ()
{
	this->TotalConnectedTriangles = 0;
	this->TotalConnectedPoints = 0;
	this->TotalConnectedVertexNormals = 0;
	this->TotalConnectedMaterials = 0;
}

Model_OBJ::~Model_OBJ()
{
}

float* Model_OBJ::calculateNormal(float * coord1, float * coord2, float * coord3)
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

int Model_OBJ::Load(char * filename)
{
	string line;
	ifstream objFile(filename);
	if (objFile.is_open())                                       // If obj file is open, continue
	{
		objFile.seekg(0, ios::end);                              // Go to end of the file, 
		long fileSize = objFile.tellg();                           // get file size
		objFile.seekg(0, ios::beg);                              // we'll use this to register memory for our 3d model

		vertexBuffer = (float*)malloc(fileSize);                     // Allocate memory for the verteces
		normalBuffer = (float*)malloc(fileSize);
		Faces_Triangles = (float*)malloc(fileSize * sizeof(float));         // Allocate memory for the triangles
		Faces_Triangles_vertex_normal = (float*)malloc(fileSize * sizeof(float));
		normals = (float*)malloc(fileSize * sizeof(float));               // Allocate memory for the normals
		Faces_Materials = (material*)malloc(fileSize * sizeof(material));
		int triangle_index = 0;                                    // Set triangle index to zero
		int normal_index = 0;                                    // Set normal index to zero

		while (!objFile.eof())                                 // Start reading file data
		{
			getline(objFile, line);                                 // Get line from file
			if (line.c_str()[0] == 'v'&&line.c_str()[1] != 'n')                              // The first character is a v: on this line is a vertex stored.
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
			if(line.substr(0, 6) == "mtllib")
			{
				this->MtlFileName = line.substr(7);
			}
			if (line.substr(0, 6) == "usemtl")
			{
				string mat_name = line.substr(7);
				ifstream mtlFile(MtlFileName);
				if(mtlFile.is_open())
				{
					string mtlLine;
					mtlFile.seekg(0, ios::end);                              // Go to end of the file, 
					long mtlFileSize = mtlFile.tellg();                          // get file size
					mtlFile.seekg(0, ios::beg);									// we'll use this to register memory for our 3d model
					while (!mtlFile.eof())
					{
						getline(mtlFile, mtlLine);
						if (mtlLine.substr(0, 6) == "newmtl" && mtlLine.substr(7)==mat_name)
						{
							float Kd[4] = { 0 }, Ks[4] = { 0 }, Ka[4] = { 0 };
							while (!mtlFile.eof())
							{
								getline(mtlFile, mtlLine);
								if (mtlLine.size() == 0) break;
								if (mtlLine.substr(0, 6) == "newmtl"&& mtlLine.substr(7) != mat_name) break;
								
								//material 속성 부여
								if (mtlLine.substr(0, 2) == "Ka") {
									mtlLine[0] = ' '; mtlLine[1] = ' ';
									sscanf(mtlLine.c_str(), "%f %f %f",
										&Ka[0], &Ka[1], &Ka[2]); Ka[3] = 1;
								}
								if (mtlLine.substr(0, 2) == "Kd") {
									mtlLine[0] = ' '; mtlLine[1] = ' ';
									sscanf(mtlLine.c_str(), "%f %f %f",
										&Kd[0], &Kd[1], &Kd[2]); Kd[3] = 1;
								}
								if (mtlLine.substr(0, 2) == "Ks") {
									mtlLine[0] = ' '; mtlLine[1] = ' ';
									sscanf(mtlLine.c_str(), "%f %f %f",
										&Ks[0], &Ks[1], &Ks[2]); Ks[3] = 1;
								}
							}
							Faces_Materials[TotalConnectedMaterials++] = material(Ka, Kd, Ks, TotalConnectedTriangles);
						}
					}
					mtlFile.close();

				}
				else
				{
					cout << "Unable to open mtl file";
				}
				
			}
			if (line.c_str()[0] == 'f')                              // The first character is an 'f': on this line is a point stored
			{
				line[0] = ' ';                                    // Set first character to 0. This will allow us to use sscanf
				
				int vertexNumber[4] = { 0, 0, 0 };
				int vertexNormalNumber[4] = { 0,0,0 };
				sscanf(line.c_str(), "%i//%i %i//%i %i//%i",                       // Read integers from the line:  f 1 2 3
					&vertexNumber[0],								// First point of our triangle. This is an 
					&vertexNormalNumber[0],                              // pointer to our vertexBuffer list
					&vertexNumber[1],
					&vertexNormalNumber[1],
					&vertexNumber[2],
					&vertexNormalNumber[2]);                              // each point represents an X,Y,Z.

				vertexNumber[0] -= 1;                              // OBJ file starts counting from 1
				vertexNumber[1] -= 1;                              // OBJ file starts counting from 1
				vertexNumber[2] -= 1;                              // OBJ file starts counting from 1
				vertexNormalNumber[0] -= 1;                              // OBJ file starts counting from 1
				vertexNormalNumber[1] -= 1;                              // OBJ file starts counting from 1
				vertexNormalNumber[2] -= 1;                              // OBJ file starts counting from 1


																   /********************************************************************
																   * Create triangles (f 1 2 3) from points: (v X Y Z) (v X Y Z) (v X Y Z).
																   * The vertexBuffer contains all verteces
																   * The triangles will be created using the verteces we read previously
																   */

				int tCounter = 0;
				for (int i = 0; i < VERTEX_PER_TRIANGLE; i++)
				{
					Faces_Triangles[triangle_index + tCounter] = vertexBuffer[3 * vertexNumber[i]];
					Faces_Triangles[triangle_index + tCounter + 1] = vertexBuffer[3 * vertexNumber[i] + 1];
					Faces_Triangles[triangle_index + tCounter + 2] = vertexBuffer[3 * vertexNumber[i] + 2];

					Faces_Triangles_vertex_normal[triangle_index + tCounter] = normalBuffer[3 * vertexNormalNumber[i]];
					Faces_Triangles_vertex_normal[triangle_index + tCounter + 1] = normalBuffer[3 * vertexNormalNumber[i] + 1];
					Faces_Triangles_vertex_normal[triangle_index + tCounter + 2] = normalBuffer[3 * vertexNormalNumber[i] + 2];
					tCounter += POINTS_PER_VERTEX;
				}
				/*********************************************************************
				* Calculate all normals, used for lighting
				*/
				float coord1[3] = { Faces_Triangles[triangle_index], Faces_Triangles[triangle_index + 1],Faces_Triangles[triangle_index + 2] };
				float coord2[3] = { Faces_Triangles[triangle_index + 3],Faces_Triangles[triangle_index + 4],Faces_Triangles[triangle_index + 5] };
				float coord3[3] = { Faces_Triangles[triangle_index + 6],Faces_Triangles[triangle_index + 7],Faces_Triangles[triangle_index + 8] };
				float *norm = this->calculateNormal(coord1, coord2, coord3);

				tCounter = 0;
				for (int i = 0; i < VERTEX_PER_TRIANGLE; i++)
				{
					normals[normal_index + tCounter] = norm[0];
					normals[normal_index + tCounter + 1] = norm[1];
					normals[normal_index + tCounter + 2] = norm[2];
					tCounter += POINTS_PER_VERTEX;
				}

				triangle_index += TOTAL_FLOATS_IN_TRIANGLE;
				normal_index += TOTAL_FLOATS_IN_TRIANGLE;
				TotalConnectedTriangles += TOTAL_FLOATS_IN_TRIANGLE;
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

void Model_OBJ::Draw()
{
	/*glBegin(GL_TRIANGLES);
		for (int i = 0; i < TotalConnectedTriangles; i+=3) {
			//if (i % 3 == 0)glNormal3f(normals[i], normals[i + 1], normals[i + 2]);
			if (Faces_Materials[mat_cnt].startidx == i) {
				printf("Faces_Triangles: %f %f %f\n", Faces_Triangles[i], Faces_Triangles[i + 1], Faces_Triangles[i + 2]);
				Faces_Materials[mat_cnt++].set_material();
			}
			glNormal3f(Faces_Triangles_vertex_normal[i], Faces_Triangles_vertex_normal[i + 1], Faces_Triangles_vertex_normal[i + 2]);
			glVertex3f(Faces_Triangles[i], Faces_Triangles[i + 1], Faces_Triangles[i + 2]);
		}
	glEnd();*/
	int mat_cnt = 0;
	for (int i = 0; i < TotalConnectedTriangles; i += 9) {
		//glmaterialfv 적용
		if (i >= Faces_Materials[mat_cnt].startidx && i < Faces_Materials[mat_cnt + 1].startidx) Faces_Materials[mat_cnt].set_material();
			else if (mat_cnt != 0 && Faces_Materials[mat_cnt + 1].startidx == 0) Faces_Materials[mat_cnt].set_material();
			else if (mat_cnt < TotalConnectedMaterials - 1) {
				Faces_Materials[++mat_cnt].set_material();
				/*printf("Faces_Materials[%d]: start_idx = %d\n", mat_cnt, Faces_Materials[mat_cnt].startidx);
				printf("Ka: %f %f %f %f\n", Faces_Materials[mat_cnt].Ka[0], Faces_Materials[mat_cnt].Ka[1], Faces_Materials[mat_cnt].Ka[2], Faces_Materials[mat_cnt].Ka[3]);
				printf("Kd: %f %f %f %f\n", Faces_Materials[mat_cnt].Kd[0], Faces_Materials[mat_cnt].Kd[1], Faces_Materials[mat_cnt].Kd[2], Faces_Materials[mat_cnt].Kd[3]);
				printf("Ks: %f %f %f %f\n", Faces_Materials[mat_cnt].Ks[0], Faces_Materials[mat_cnt].Ks[1], Faces_Materials[mat_cnt].Ks[2], Faces_Materials[mat_cnt].Ks[3]);
				printf("Total connected materials : %d\n", TotalConnectedMaterials);*/
			}	
		//face rendering
		glBegin(GL_TRIANGLES);
			glNormal3f(Faces_Triangles_vertex_normal[i], Faces_Triangles_vertex_normal[i + 1], Faces_Triangles_vertex_normal[i + 2]);
			glVertex3f(Faces_Triangles[i], Faces_Triangles[i + 1], Faces_Triangles[i + 2]);
			glNormal3f(Faces_Triangles_vertex_normal[i+3], Faces_Triangles_vertex_normal[i + 4], Faces_Triangles_vertex_normal[i + 5]);
			glVertex3f(Faces_Triangles[i+3], Faces_Triangles[i + 4], Faces_Triangles[i + 5]);
			glNormal3f(Faces_Triangles_vertex_normal[i+6], Faces_Triangles_vertex_normal[i + 7], Faces_Triangles_vertex_normal[i + 8]);
			glVertex3f(Faces_Triangles[i+6], Faces_Triangles[i + 7], Faces_Triangles[i + 8]);
		glEnd();
	}
}

void Model_OBJ::Release()
{
	free(this->Faces_Triangles);
	free(this->normals);
	free(this->vertexBuffer);
}

Model_OBJ::material::material()
{
	for (int i = 0; i < 4; i++) {
		this->Ka[i] = 0;
		this->Kd[i] = 0;
		this->Ks[i] = 0;
	}
	this->startidx = 0;
	
}

Model_OBJ::material::material(float * Ka, float * Kd, float * Ks, int startidx)
{
	for (int i = 0; i < 4; i++) {
		this->Ka[i] = Ka[i];
		this->Kd[i] = Kd[i];
		this->Ks[i] = Ks[i];
		this->startidx = startidx;
	}
}

Model_OBJ::material::~material()
{
}

void Model_OBJ::material::set_material()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, this->Ka);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, this->Kd);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, this->Ks);
}
