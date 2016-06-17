#pragma once
#include <string>
#include <vector>
using namespace std;
class Model_OBJ
{
public:
	float* normals;		//stores the normal
	float* Faces_Triangles;	//stores the triangles
	float* Faces_Triangles_vertex_normal;
	float* vertexBuffer;	//stores the points which make the object
	float* normalBuffer;	//stroes the normal which make the object
	long TotalConnectedPoints;	//stores the total number of connected verteces
	long TotalConnectedTriangles; //stores the total numer of connected triangles
	long TotalConnectedVertexNormals;
	string MtlFileName;
	class material {
	public:
		float Ka[4];
		float Kd[4];
		float Ks[4];
		int startidx;

		material();
		material(float* Ka, float* Kd, float* Ks, int startidx);
		~material();
		void set_material();
	};
	material* Faces_Materials;
	long TotalConnectedMaterials;
	

	Model_OBJ();
	~Model_OBJ();
	float* calculateNormal(float* coord1, float* coord2, float* coord3);
	int Load(char *filename);
	void Draw();
	void Release();
};

