/******************************************************************************/
/*!
\file	MeshBuilder.cpp
\author 3000 Lines Studio
\brief
Functions to build meshes
*/
/******************************************************************************/

#include <vector>
#include "MyMath.h"
#include "MeshBuilder.h"
#include <GL\glew.h>
#include "Vertex.h"
#include "LoadOBJ.h"

#include "SharedData.h"

/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
    std::vector<Vertex> vertex_buffer_data;
    std::vector<GLuint> index_buffer_data;
    
    Vertex v;
    v.pos.Set(-lengthX, 0, 0);
    v.color.Set(1, 0, 0);
    vertex_buffer_data.push_back(v);

    v.pos.Set(lengthX, 0, 0);
    v.color.Set(1, 0, 0);
    vertex_buffer_data.push_back(v);

    v.pos.Set(0, -lengthY, 0);
    v.color.Set(0, 1, 0);
    vertex_buffer_data.push_back(v);

    v.pos.Set(0, lengthY, 0);
    v.color.Set(0, 1, 0);
    vertex_buffer_data.push_back(v);

    v.pos.Set(0, 0, -lengthZ);
    v.color.Set(0, 0, 1);
    vertex_buffer_data.push_back(v);

    v.pos.Set(0, 0, lengthZ);
    v.color.Set(0, 0, 1);
    vertex_buffer_data.push_back(v);

    index_buffer_data.push_back(0);
    index_buffer_data.push_back(1);
    index_buffer_data.push_back(2);
    index_buffer_data.push_back(3);
    index_buffer_data.push_back(4);
    index_buffer_data.push_back(5);

	Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_LINES;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color, float lengthX, float lengthY)
{
    //for reference only
    /*const GLfloat vertex_data[] = {
        0.5f, 0.5f, 0,      //top right
        -0.5f, 0.5f, 0,     //top left
        0.5f, -0.5f, 0,     //bottom right
        -0.5f, -0.5f, 0,    //bottom left

        -0.5f, 0.5f, -0.5f,      //top left
        0.5f, 0.5f, -0.5f,       //top right
        -0.5f, -0.5f, -0.5f,     //bottom left
        0.5f, -0.5f, -0.5f,      //bottom right
	};*/

    std::vector<Vertex> vertex_buffer_data;
    std::vector<GLuint> index_buffer_data;

    Vertex v;
    v.color = color;
    v.pos.Set(lengthX * 0.5f, lengthY * 0.5f, 0);   v.normal.Set(0, 0, 1);     v.texCoord.Set(1, 1);   vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * -0.5f, lengthY * 0.5f, 0);  v.normal.Set(0, 0, 1);     v.texCoord.Set(0, 1);   vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * 0.5f, lengthY * -0.5f, 0);  v.normal.Set(0, 0, 1);     v.texCoord.Set(1, 0);   vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * -0.5f, lengthY * -0.5f, 0); v.normal.Set(0, 0, 1);     v.texCoord.Set(0, 0);   vertex_buffer_data.push_back(v);

    for (int i = 0; i < 4; ++i) {
        index_buffer_data.push_back(i);
    }

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateCircle(const std::string &meshName, Color color, float radius, unsigned numSlices)
{
    std::vector<Vertex> vertex_buffer_data;
    std::vector<GLuint> index_buffer_data;

    float angle = Math::TWO_PI / numSlices;     //angle per slice

    Vertex v;
    v.color.Set(color.r, color.g, color.b);
    for (unsigned i = 0; i < numSlices + 1; ++i) {    //+1 to complete the loop
        v.pos.Set(radius * cos(angle * i), 0, radius * sin(angle * i));
        v.normal.Set(0, 1, 0);
        vertex_buffer_data.push_back(v);
    }
    v.pos.Set(0, 0, 0);
    v.normal.Set(0, 1, 0);
    vertex_buffer_data.push_back(v);

    for (unsigned i = 0; i < numSlices + 1; ++i) {
        index_buffer_data.push_back(i);
        index_buffer_data.push_back(numSlices + 1);
    }

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

    return mesh;
}

Mesh* MeshBuilder::GenerateRing(const std::string &meshName, Color color, float outerRadius, float innerRadius)
{
    std::vector<Vertex> vertex_buffer_data;
    std::vector<GLuint> index_buffer_data;

    int points = 36;
    float angle = Math::TWO_PI / points;

    Vertex v;
    v.color.Set(color.r, color.g, color.b);
    for (int i = 0; i < points + 1; ++i) {    //+1 to complete the loop
        v.pos.Set(outerRadius * cos(angle * i), 0, outerRadius * sin(angle * i));
        v.normal.Set(0, 1, 0);
        vertex_buffer_data.push_back(v);

        v.pos.Set(innerRadius * cos(angle * i), 0, innerRadius * sin(angle * i));
        v.normal.Set(0, 1, 0);
        vertex_buffer_data.push_back(v);
    }

    for (int i = 0; i < 2 * (points + 1); ++i) {
        index_buffer_data.push_back(i);
    }

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
    
    return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCube(const std::string &meshName, Color color, float lengthX, float lengthY, float lengthZ)
{
    std::vector<Vertex> vertex_buffer_data;
    std::vector<GLuint> index_buffer_data;
    Vertex v;

    v.color = color;
    //left
    v.pos.Set(lengthX * -0.5f, lengthY * -0.5f, lengthZ * -0.5f);   v.normal.Set(-1, 0, 0);     vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * -0.5f, lengthY * -0.5f, lengthZ * 0.5f);    v.normal.Set(-1, 0, 0);     vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * -0.5f, lengthY * 0.5f, lengthZ * 0.5f);     v.normal.Set(-1, 0, 0);     vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * -0.5f, lengthY * -0.5f, lengthZ * -0.5f);   v.normal.Set(-1, 0, 0);     vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * -0.5f, lengthY * 0.5f, lengthZ * 0.5f);     v.normal.Set(-1, 0, 0);     vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * -0.5f, lengthY * 0.5f, lengthZ * -0.5f);    v.normal.Set(-1, 0, 0);     vertex_buffer_data.push_back(v);

    //back
    v.pos.Set(lengthX * 0.5f, lengthY * 0.5f, lengthZ * -0.5f);     v.normal.Set(0, 0, -1);     vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * -0.5f, lengthY * -0.5f, lengthZ * -0.5f);   v.normal.Set(0, 0, -1);     vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * -0.5f, lengthY * 0.5f, lengthZ * -0.5f);    v.normal.Set(0, 0, -1);     vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * 0.5f, lengthY * 0.5f, lengthZ * -0.5f);     v.normal.Set(0, 0, -1);     vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * 0.5f, lengthY * -0.5f, lengthZ * -0.5f);    v.normal.Set(0, 0, -1);     vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * -0.5f, lengthY * -0.5f, lengthZ * -0.5f);   v.normal.Set(0, 0, -1);     vertex_buffer_data.push_back(v);

    //bottom
    v.pos.Set(lengthX * 0.5f, lengthY * -0.5f, lengthZ * 0.5f);     v.normal.Set(0, -1, 0);     vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * -0.5f, lengthY * -0.5f, lengthZ * -0.5f);   v.normal.Set(0, -1, 0);     vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * 0.5f, lengthY * -0.5f, lengthZ * -0.5f);    v.normal.Set(0, -1, 0);     vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * 0.5f, lengthY * -0.5f, lengthZ * 0.5f);     v.normal.Set(0, -1, 0);     vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * -0.5f, lengthY * -0.5f, lengthZ * 0.5f);    v.normal.Set(0, -1, 0);     vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * -0.5f, lengthY * -0.5f, lengthZ * -0.5f);   v.normal.Set(0, -1, 0);     vertex_buffer_data.push_back(v);

    //front
    v.pos.Set(lengthX * 0.5f, lengthY * 0.5f, lengthZ * 0.5f);      v.normal.Set(0, 0, 1);      vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * -0.5f, lengthY * 0.5f, lengthZ * 0.5f);     v.normal.Set(0, 0, 1);      vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * 0.5f, lengthY * -0.5f, lengthZ * 0.5f);     v.normal.Set(0, 0, 1);      vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * -0.5f, lengthY * 0.5f, lengthZ * 0.5f);     v.normal.Set(0, 0, 1);      vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * -0.5f, lengthY * -0.5f, lengthZ * 0.5f);    v.normal.Set(0, 0, 1);      vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * 0.5f, lengthY * -0.5f, lengthZ * 0.5f);     v.normal.Set(0, 0, 1);      vertex_buffer_data.push_back(v);

    //right
    v.pos.Set(lengthX * 0.5f, lengthY * -0.5f, lengthZ * -0.5f);    v.normal.Set(1, 0, 0);      vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * 0.5f, lengthY * 0.5f, lengthZ * 0.5f);      v.normal.Set(1, 0, 0);      vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * 0.5f, lengthY * -0.5f, lengthZ * 0.5f);     v.normal.Set(1, 0, 0);      vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * 0.5f, lengthY * 0.5f, lengthZ * 0.5f);      v.normal.Set(1, 0, 0);      vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * 0.5f, lengthY * -0.5f, lengthZ * -0.5f);    v.normal.Set(1, 0, 0);      vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * 0.5f, lengthY * 0.5f, lengthZ * -0.5f);     v.normal.Set(1, 0, 0);      vertex_buffer_data.push_back(v);

    //top
    v.pos.Set(lengthX * 0.5f, lengthY * 0.5f, lengthZ * 0.5f);      v.normal.Set(0, 1, 0);      vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * 0.5f, lengthY * 0.5f, lengthZ * -0.5f);     v.normal.Set(0, 1, 0);      vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * -0.5f, lengthY * 0.5f, lengthZ * -0.5f);    v.normal.Set(0, 1, 0);      vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * 0.5f, lengthY * 0.5f, lengthZ * 0.5f);      v.normal.Set(0, 1, 0);      vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * -0.5f, lengthY * 0.5f, lengthZ * -0.5f);    v.normal.Set(0, 1, 0);      vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * -0.5f, lengthY * 0.5f, lengthZ * 0.5f);     v.normal.Set(0, 1, 0);      vertex_buffer_data.push_back(v);

    for (unsigned i = 0; i < 36; ++i) {
        index_buffer_data.push_back(i);
    }

	Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

Mesh* MeshBuilder::GenerateSphere(const std::string &meshName, Color color, float radius, unsigned numSlices, int numStacks)
{
    std::vector<Vertex> vertex_buffer_data;
    std::vector<GLuint> index_buffer_data;

    float angleSlice = Math::TWO_PI / numSlices;
    float angleStack = Math::PI / numStacks;
    //start from angle of 90 then subtract down / start from -90 then add up

    Vertex v;
    v.color.Set(color.r, color.g, color.b);
    for (int i = -numStacks / 2; i < numStacks / 2 + 1; ++i) {
        for (unsigned j = 0; j < numSlices + 1; ++j) {
            v.pos.Set(radius * cos(angleStack * i) * cos(angleSlice * j), radius * sin(angleStack * i), radius * cos(angleStack * i) * sin(angleSlice * j));
            v.normal.Set(cos(angleStack * i) * cos(angleSlice * j), sin(angleStack * i), cos(angleStack * i) * sin(angleSlice * j));
            v.normal.Normalize();
            vertex_buffer_data.push_back(v);

            /*v.pos.Set(radius * cos(angleStack * (i + 1)) * cos(angleSlice * j), radius * sin(angleStack * (i + 1)), radius * cos(angleStack * (i + 1)) * sin(angleSlice * j));
            v.normal.Set(radius * cos(angleStack * (i + 1)) * cos(angleSlice * j), radius * sin(angleStack * (i + 1)), radius * cos(angleStack * (i + 1)) * sin(angleSlice * j));
            v.normal.Normalize();
            vertex_buffer_data.push_back(v);*/
        }
    }

    for (int i = 0; i < numStacks; ++i) {
        for (unsigned j = 0; j < numSlices + 1; ++j) {
            index_buffer_data.push_back(i * (numSlices + 1) + j);
            index_buffer_data.push_back((i + 1) * (numSlices + 1) + j);
        }
    }

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

    return mesh;
}

Mesh* MeshBuilder::GenerateHemisphere(const std::string &meshName, Color color, float radius, unsigned numSlices, int numStacks)
{
    std::vector<Vertex> vertex_buffer_data;
    std::vector<GLuint> index_buffer_data;

    float angleSlice = Math::TWO_PI / numSlices;
    float angleStack = Math::HALF_PI / numStacks;

    Vertex v;
    v.color.Set(color.r, color.g, color.b);

    //base
    for (unsigned i = 0; i < numSlices + 1; ++i) {    //+1 to complete the loop
        v.pos.Set(radius * cos(angleSlice * i), 0, radius * sin(angleSlice * i));
        v.normal.Set(0, -1, 0);
        vertex_buffer_data.push_back(v);
    }

    v.pos.Set(0, 0, 0);
    v.normal.Set(0, -1, 0);
    vertex_buffer_data.push_back(v);

    for (unsigned i = 0; i < numSlices + 1; ++i) {
        index_buffer_data.push_back(numSlices + 1);
        index_buffer_data.push_back(i);
    }
    
    for (int i = 0; i < numStacks + 1; ++i) {
        for (unsigned j = 0; j < numSlices + 1; ++j) {
            v.pos.Set(radius * cos(angleStack * i) * cos(angleSlice * j), radius * sin(angleStack * i), radius * cos(angleStack * i) * sin(angleSlice * j));
            v.normal.Set(radius * cos(angleStack * i) * cos(angleSlice * j), radius * sin(angleStack * i), radius * cos(angleStack * i) * sin(angleSlice * j));
            v.normal.Normalize();
            vertex_buffer_data.push_back(v);
            
            /*v.pos.Set(radius * cos(angleStack * (i + 1)) * cos(angleSlice * j), radius * sin(angleStack * (i + 1)), radius * cos(angleStack * (i + 1)) * sin(angleSlice * j));
            v.normal.Set(radius * cos(angleStack * (i + 1)) * cos(angleSlice * j), radius * sin(angleStack * (i + 1)), radius * cos(angleStack * (i + 1)) * sin(angleSlice * j));
            v.normal.Normalize();
            vertex_buffer_data.push_back(v);*/
        }
    }

    //(numSlices + 2) is the offset
    for (int i = 0; i < numStacks; ++i) {
        for (unsigned j = 0; j < numSlices + 1; ++j) {
            index_buffer_data.push_back(i * (numSlices + 1) + j + (numSlices + 2));
            index_buffer_data.push_back((i + 1) * (numSlices + 1) + j + (numSlices + 2));
        }
    }

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

    return mesh;
}

Mesh* MeshBuilder::GenerateCylinder(const std::string &meshName, Color color, float radius, float height, unsigned numSlices, unsigned numStacks)
{
    std::vector<Vertex> vertex_buffer_data;
    std::vector<GLuint> index_buffer_data;

    float angle = Math::TWO_PI / numSlices;
    float stackHeight = height / numStacks;
    float halfHeight = height / 2;    //cause cylinder's center will be at origin

    Vertex v;
    v.color.Set(color.r, color.g, color.b);

    //bottom
    for (unsigned i = 0; i < numSlices + 1; ++i) {
        v.pos.Set(radius * cos(angle * i), -halfHeight, radius * sin(angle * i));
        v.normal.Set(0, -1, 0);
        vertex_buffer_data.push_back(v);
    }

    v.pos.Set(0, -halfHeight, 0);
    v.normal.Set(0, -1, 0);
    vertex_buffer_data.push_back(v);

    for (unsigned i = 0; i < numSlices + 1; ++i) {
        index_buffer_data.push_back(numSlices + 1);
        index_buffer_data.push_back(i);
    }

    //stack - start from bottom
    for (unsigned i = 0; i < numStacks + 1; ++i) {
        for (unsigned j = 0; j < numSlices + 1; ++j) {
            v.pos.Set(radius * cos(angle * j), -halfHeight + i * stackHeight, radius * sin(angle * j));
            v.normal.Set(cos(angle * j), 0, sin(angle * j));
            vertex_buffer_data.push_back(v);

            /*v.pos.Set(radius * cos(angle * j), -halfHeight + (i + 1) * stackHeight, radius * sin(angle * j));
            v.normal.Set(cos(angle * j), 0, sin(angle * j));
            vertex_buffer_data.push_back(v);*/
        }
    }

    //(numSlices + 2) is the offset
    for (unsigned i = 0; i < numStacks; ++i) {
        for (unsigned j = 0; j < numSlices + 1; ++j) {
            index_buffer_data.push_back(i * (numSlices + 1) + j + (numSlices + 2));
            index_buffer_data.push_back((i + 1) * (numSlices + 1) + j + (numSlices + 2));
        }
    }

    //top
    v.color.Set(color.r, color.g, color.b);
    for (unsigned i = 0; i < numSlices + 1; ++i) {
        v.pos.Set(radius * cos(angle * i), halfHeight, radius * sin(angle * i));
        v.normal.Set(0, 1, 0);
        vertex_buffer_data.push_back(v);
    }

    v.pos.Set(0, halfHeight, 0);
    v.normal.Set(0, 1, 0);
    vertex_buffer_data.push_back(v);

    //(numSlices + 2) + (stackNum + 1) * (numSlices + 1) is the offset
    for (unsigned i = 0; i < numSlices + 1; ++i) {
        index_buffer_data.push_back(i + (numSlices + 2) + (numStacks + 1) * (numSlices + 1));
        index_buffer_data.push_back(numSlices + 1 + (numSlices + 2) + (numStacks + 1) * (numSlices + 1));
    }

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

    return mesh;
}

Mesh* MeshBuilder::GenerateCone(const std::string &meshName, Color color, float radius, float height, unsigned numSlices, unsigned numStacks)
{
    std::vector<Vertex> vertex_buffer_data;
    std::vector<GLuint> index_buffer_data;

    float angle = Math::TWO_PI / numSlices;
    float stackHeight = height / numStacks;
    float halfHeight = height / 2;    //cone's center will be at origin
    float stackRadius = radius / numStacks;

    Vertex v;
    v.color.Set(color.r, color.g, color.b);
    
    //base
    for (unsigned i = 0; i < (numSlices + 1); ++i) {
        v.pos.Set(radius * cos(angle * i), -halfHeight, radius * sin(angle * i));
        v.normal.Set(0, -1, 0);
        vertex_buffer_data.push_back(v);
    }

    v.pos.Set(0, -halfHeight, 0);
    v.normal.Set(0, -1, 0);
    vertex_buffer_data.push_back(v);

    for (unsigned i = 0; i < numSlices + 1; ++i) {
        index_buffer_data.push_back(numSlices + 1);
        index_buffer_data.push_back(i);
    }
    
    //stacks
    for (unsigned i = 0; i < numStacks + 1; ++i) {
        for (unsigned j = 0; j < (numSlices + 1); ++j) {
            v.pos.Set((radius - stackRadius * i) * cos(angle * j), -halfHeight + stackHeight * i, (radius - stackRadius * i) * sin(angle * j));
            v.normal.Set(radius * cos(angle * j), radius, radius * sin(angle * j));
            v.normal.Normalize();
            vertex_buffer_data.push_back(v);

            /*v.pos.Set((radius - stackRadius * (i + 1)) * cos(angle * j), -halfHeight + stackHeight * (i + 1), (radius - stackRadius * (i + 1)) * sin(angle * j));
            v.normal.Set(radius * cos(angle * j), radius, radius * sin(angle * j));
            v.normal.Normalize();
            vertex_buffer_data.push_back(v);*/
        }
    }
    
    //(numSlices + 2) is the offset
    for (unsigned i = 0; i < numStacks; ++i) {
        for (unsigned j = 0; j < numSlices + 1; ++j) {
            index_buffer_data.push_back(i * (numSlices + 1) + j + (numSlices + 2));
            index_buffer_data.push_back((i + 1) * (numSlices + 1) + j + (numSlices + 2));
        }
    }

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

    return mesh;
}

Mesh* MeshBuilder::GenerateTorus(const std::string &meshName, Color color, float outerRadius, float innerRadius, unsigned numSlices, unsigned numStacks)
{
    std::vector<Vertex> vertex_buffer_data;
    std::vector<GLuint> index_buffer_data;

    float angleStack = Math::TWO_PI / numStacks;
    float angleSlice = -Math::TWO_PI / numSlices;

    Vertex v;
    v.color.Set(color.r, color.g, color.b);
    for (unsigned i = 0; i < numStacks; ++i) {
        for (unsigned j = 0; j < numSlices + 1; ++j) {
            v.pos.Set((outerRadius + innerRadius * cos(angleStack * i)) * sin(angleSlice * j), innerRadius * sin(angleStack * i), (outerRadius + innerRadius * cos(angleStack * i)) * cos(angleSlice * j));
            v.normal.Set(cos(angleStack * i) * sin(angleSlice * j), sin(angleStack * i), cos(angleStack * i) * cos(angleSlice * j));
            v.normal.Normalize();
            vertex_buffer_data.push_back(v);

            v.pos.Set((outerRadius + innerRadius * cos(angleStack * (i + 1))) * sin(angleSlice * j), innerRadius * sin(angleStack * (i + 1)), (outerRadius + innerRadius * cos(angleStack * (i + 1))) * cos(angleSlice * j));
            v.normal.Set(cos(angleStack * (i + 1)) * sin(angleSlice * j), sin(angleStack * (i + 1)), cos(angleStack * (i + 1)) * cos(angleSlice * j));
            v.normal.Normalize();
            vertex_buffer_data.push_back(v);
        }
    }

    for (unsigned i = 0; i < numStacks * 2 * (numSlices + 1); ++i) {
        index_buffer_data.push_back(i);
    }

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

    return mesh;
}

Mesh* MeshBuilder::GenerateOBJ(const std::string &meshName, const std::string &file_path)
{
    //read vertices, texcoords & normals from OBJ
    std::vector<Position> vertices;
    std::vector<TexCoord> uvs;
    std::vector<Vector3> normals;
    bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);

    if (!success) {
        return NULL;
    }
    
    std::vector<Vertex> vertex_buffer_data;
    std::vector<GLuint> index_buffer_data;
    IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLES;

    return mesh;
}

Mesh* MeshBuilder::GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol)
{
    //for reference only
    /*const GLfloat vertex_data[] = {
        0.5f, 0.5f, 0,      //top right
        -0.5f, 0.5f, 0,     //top left
        0.5f, -0.5f, 0,     //bottom right
        -0.5f, -0.5f, 0,    //bottom left
    }; */

    std::vector<Vertex> vertex_buffer_data;
    std::vector<GLuint> index_buffer_data;

    Vertex v;
    v.color = Color(0, 1, 0);

    float width = 1.f / numCol;
    float height = 1.f / numRow;
    int offset = 0;
    
    for (unsigned i = 0; i < numRow; ++i) {
        for (unsigned j = 0; j < numCol; ++j) {
            //add 4 vertices into vertex_buffer_data
            v.pos.Set(-0.5f, -0.5f, 0);     v.normal.Set(0, 0, 1);  v.texCoord.Set(width * j, 1.f - height * (i + 1));     vertex_buffer_data.push_back(v);
            v.pos.Set(0.5f, -0.5f, 0);      v.normal.Set(0, 0, 1);  v.texCoord.Set(width * (j + 1), 1.f - height * (i + 1));   vertex_buffer_data.push_back(v);
            v.pos.Set(0.5f, 0.5f, 0);       v.normal.Set(0, 0, 1);  v.texCoord.Set(width * (j + 1), 1.f - height * i);   vertex_buffer_data.push_back(v);
            v.pos.Set(-0.5f, 0.5f, 0);      v.normal.Set(0, 0, 1);  v.texCoord.Set(width * j, 1.f - height * i);   vertex_buffer_data.push_back(v);

            //add 6 indices into index_buffer_data
            index_buffer_data.push_back(offset + 0);
            index_buffer_data.push_back(offset + 1);
            index_buffer_data.push_back(offset + 2);

            index_buffer_data.push_back(offset + 0);
            index_buffer_data.push_back(offset + 2);
            index_buffer_data.push_back(offset + 3);
            
            offset += 4;
        }
    }

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLES;

    return mesh;
}

Mesh* MeshBuilder::GenerateMinimap(const std::string &meshName, float lengthX, float lengthY)
{
    //for reference only
    /*const GLfloat vertex_data[] = {
    0.5f, 0.5f, 0,      //top right
    -0.5f, 0.5f, 0,     //top left
    0.5f, -0.5f, 0,     //bottom right
    -0.5f, -0.5f, 0,    //bottom left

    -0.5f, 0.5f, -0.5f,      //top left
    0.5f, 0.5f, -0.5f,       //top right
    -0.5f, -0.5f, -0.5f,     //bottom left
    0.5f, -0.5f, -0.5f,      //bottom right
    };*/

    std::vector<Vertex> vertex_buffer_data;
    std::vector<GLuint> index_buffer_data;

    float UVminX = SharedData::GetInstance()->player->position_.x / 1000;
    float UVmaxX = (SharedData::GetInstance()->player->position_.x + 500) / 1000;
    float UVminZ = SharedData::GetInstance()->player->position_.z / 1000;
    float UVmaxZ = (SharedData::GetInstance()->player->position_.z + 500) / 1000;

    Vertex v;
    v.pos.Set(lengthX * 0.5f, lengthY * 0.5f, 0);   v.normal.Set(0, 0, 1);     v.texCoord.Set(UVmaxX, UVmaxZ);   vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * -0.5f, lengthY * 0.5f, 0);  v.normal.Set(0, 0, 1);     v.texCoord.Set(UVminX, UVmaxZ);   vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * 0.5f, lengthY * -0.5f, 0);  v.normal.Set(0, 0, 1);     v.texCoord.Set(UVmaxX, UVminZ);   vertex_buffer_data.push_back(v);
    v.pos.Set(lengthX * -0.5f, lengthY * -0.5f, 0); v.normal.Set(0, 0, 1);     v.texCoord.Set(UVminX, UVminZ);   vertex_buffer_data.push_back(v);

    for (int i = 0; i < 4; ++i) {
        index_buffer_data.push_back(i);
    }

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

    return mesh;
}