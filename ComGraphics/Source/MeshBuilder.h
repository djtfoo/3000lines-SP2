#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	//axes
    static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
    //2D
    static Mesh* GenerateQuad(const std::string &meshName, Color color, float lengthX, float lengthY);
    static Mesh* GenerateCircle(const std::string &meshName, Color color, float radius, unsigned numSlices = 36);
    static Mesh* GenerateRing(const std::string &meshName, Color color, float outerRadius, float innerRadius);
    //3D
    static Mesh* GenerateCube(const std::string &meshName, Color color, float lengthX, float lengthY, float lengthZ);
    static Mesh* GenerateSphere(const std::string &meshName, Color color, float radius, unsigned numSlices = 36, int numStacks = 18);
    static Mesh* GenerateHemisphere(const std::string &meshName, Color color, float radius, unsigned numSlices = 36, int numStacks = 9);
    static Mesh* GenerateCylinder(const std::string &meshName, Color color, float radius, float height, unsigned numSlices = 36, unsigned numStacks = 1);
    static Mesh* GenerateCone(const std::string &meshName, Color color, float radius, float height, unsigned numSlices = 36, unsigned numStacks = 1);
    static Mesh* GenerateTorus(const std::string &meshName, Color color, float outerRadius, float innerRadius, unsigned numSlices = 36, unsigned numStacks = 36);

    //Load OBJ
    static Mesh* GenerateOBJ(const std::string &meshName, const std::string &file_path);
    
    //Text
    static Mesh* GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol);

    //Minimap
    static Mesh* GenerateMinimap(const std::string &meshName, float lengthX, float lengthY);
};

#endif