#pragma once
class Model_OBJ_Texture
{
public:
	float* normals;		//stores the normal
	float* Faces_Triangles;	//stores the triangles
	float* Faces_Triangles_vertex_normal;
	float* Faces_Triangles_uv;
	float* Faces_Quads;
	float* Faces_Quads_vertex_normal;
	float* Faces_Quads_uv;
	float* vertexBuffer;	//stores the points which make the object
	float* normalBuffer;	//stroes the normal which make the object
	float* uvBuffer;
	long TotalConnectedPoints;	//stores the total number of connected verteces
	long TotalConnectedQuads;
	long TotalConnectedTriangles; //stores the total numer of connected triangles
	long TotalConnectedVertexNormals;
	long TotalConnectedUvs;

	Model_OBJ_Texture();
	~Model_OBJ_Texture();
	float* calculateNormal(float* coord1, float* coord2, float* coord3);
	int Load(char *filename);
	void Draw();
	void Release();
};

