#include "stdafx.h"
#include "MyMesh.h"

MyMesh::MyMesh(vector<MyVertex> vertices, vector<GLuint> indices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	this->setupMesh();
}

MyMesh::MyMesh(const ShapeData & data)
{

	glm::vec2 tex[4];
	tex[0] = glm::vec2(1.0f, 1.0f); 
	tex[1] = glm::vec2(0.0f, 1.0f); 
	tex[2] = glm::vec2(0.0f, 0.0f); 
	tex[3] = glm::vec2(1.0f, 0.0f); 
	
	for (uint32_t i = 0; i < data.numVertices; ++i)
	{
		MyVertex vert;
		vert.Normal = data.vertices[i].normal;
		vert.position = data.vertices[i].position;
		vert.TexCoords = tex[i % 4];
		vertices.push_back(vert);
	}

	for (uint32_t i = 0; i < data.numIndices; ++i)
		indices.push_back(data.indices[i]);

	for (uint32_t i = 0; i < indices.size(); i += 3)
	{
		glm::vec3 pos1 = vertices[indices[i]].position;
		glm::vec3 pos2 = vertices[indices[i+1]].position;
		glm::vec3 pos3 = vertices[indices[i+2]].position;
		glm::vec2 uv1 = vertices[indices[i]].TexCoords;
		glm::vec2 uv2 = vertices[indices[i + 1]].TexCoords;
		glm::vec2 uv3 = vertices[indices[i + 2]].TexCoords;
		// calculate tangent/bitangent vectors of both triangles
		glm::vec3 tangent1, bitangent1;
		// - triangle 1
		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
		
		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent1 = glm::normalize(tangent1);

		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent1 = glm::normalize(bitangent1);

		vertices[indices[i]].tangent = tangent1;
		vertices[indices[i+1]].tangent = tangent1;
		vertices[indices[i+2]].tangent = tangent1;
		vertices[indices[i]].bitangent = bitangent1;
		vertices[indices[i + 1]].bitangent = bitangent1;
		vertices[indices[i + 2]].bitangent = bitangent1;
	}
	this->setupMesh();
}

void MyMesh::Draw()
{
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void MyMesh::setupMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(MyVertex),
		&this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
		&this->indices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex),
		(GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex),
		(GLvoid*)offsetof(MyVertex, Normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertex),
		(GLvoid*)offsetof(MyVertex, TexCoords));
	// Vertex Tangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex),
		(GLvoid*)offsetof(MyVertex, tangent));
	// Vertex Bitangent
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex),
		(GLvoid*)offsetof(MyVertex, bitangent));
	// Vertex BoneIDs
	glEnableVertexAttribArray(6);
	glVertexAttribIPointer(6, 4, GL_INT,  sizeof(MyVertex),
		(GLvoid*)offsetof(MyVertex, boneIDs));
	// Vertex Weights
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(MyVertex),
		(GLvoid*)offsetof(MyVertex, weights));
	glBindVertexArray(0);
}

void MyMesh::calculatedTangent()
{
	for (int i = 0; i < indices.size(); i += 3)
	{
		glm::vec3 pos1 = vertices[indices[i]].position;
		glm::vec3 pos2 = vertices[indices[i + 1]].position;
		glm::vec3 pos3 = vertices[indices[i + 2]].position;
		glm::vec2 uv1 = vertices[indices[i]].TexCoords;
		glm::vec2 uv2 = vertices[indices[i + 1]].TexCoords;
		glm::vec2 uv3 = vertices[indices[i + 2]].TexCoords;
		// calculate tangent/bitangent vectors of both triangles
		glm::vec3 tangent1, bitangent1;
		// - triangle 1
		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent1 = glm::normalize(tangent1);

		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent1 = glm::normalize(bitangent1);

		vertices[indices[i]].tangent = tangent1;
		vertices[indices[i + 1]].tangent = tangent1;
		vertices[indices[i + 2]].tangent = tangent1;
		vertices[indices[i]].bitangent = bitangent1;
		vertices[indices[i + 1]].bitangent = bitangent1;
		vertices[indices[i + 2]].bitangent = bitangent1;
	}
}

//////////////////////////////////
ParticleMesh::ParticleMesh(vector<MyVertex> vertices, vector<GLuint> indices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	this->setupMesh();
}

ParticleMesh::ParticleMesh(const ShapeData & data)
{

	glm::vec2 tex[4];
	tex[0] = glm::vec2(1.0f, 1.0f);
	tex[1] = glm::vec2(0.0f, 1.0f);
	tex[2] = glm::vec2(0.0f, 0.0f);
	tex[3] = glm::vec2(1.0f, 0.0f);


	for (int i = 0; i < data.numVertices; ++i)
	{
		MyVertex vert;
		vert.Normal = data.vertices[i].normal;
		vert.position = data.vertices[i].position;
		vert.TexCoords = tex[i % 4];
		vertices.push_back(vert);
	}

	for (int i = 0; i < data.numIndices; ++i)
		indices.push_back(data.indices[i]);

	for (int i = 0; i < indices.size(); i += 3)
	{
		glm::vec3 pos1 = vertices[indices[i]].position;
		glm::vec3 pos2 = vertices[indices[i + 1]].position;
		glm::vec3 pos3 = vertices[indices[i + 2]].position;
		glm::vec2 uv1 = vertices[indices[i]].TexCoords;
		glm::vec2 uv2 = vertices[indices[i + 1]].TexCoords;
		glm::vec2 uv3 = vertices[indices[i + 2]].TexCoords;
		// calculate tangent/bitangent vectors of both triangles
		glm::vec3 tangent1, bitangent1;
		// - triangle 1
		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent1 = glm::normalize(tangent1);

		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent1 = glm::normalize(bitangent1);

		vertices[indices[i]].tangent = tangent1;
		vertices[indices[i + 1]].tangent = tangent1;
		vertices[indices[i + 2]].tangent = tangent1;
		vertices[indices[i]].bitangent = bitangent1;
		vertices[indices[i + 1]].bitangent = bitangent1;
		vertices[indices[i + 2]].bitangent = bitangent1;
	}
	this->setupMesh();
}

void ParticleMesh::Draw()
{
	glBindVertexArray(this->VAO);
	glDrawElementsInstanced(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0, instances);
	glBindVertexArray(0);
}

void ParticleMesh::setupMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(MyVertex),
		&this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
		&this->indices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex),
		(GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex),
		(GLvoid*)offsetof(MyVertex, Normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertex),
		(GLvoid*)offsetof(MyVertex, TexCoords));
	// Vertex Tangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex),
		(GLvoid*)offsetof(MyVertex, tangent));
	// Vertex Bitangent
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex),
		(GLvoid*)offsetof(MyVertex, bitangent));

	//Instancing
	glGenBuffers(1, &this->VBOinstanced);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBOinstanced);

	glBufferData(GL_ARRAY_BUFFER, SIZEOF * MAXPARTICLES,
		0, GL_DYNAMIC_DRAW);

	for (int i = 0; i < 4; ++i) {
		glEnableVertexAttribArray(i+6);
		glVertexAttribPointer(i+6, 4, GL_FLOAT, GL_FALSE, SIZEOF,
			(const GLvoid*)((sizeof(float) * 4)*i));
		glVertexAttribDivisor(i+6, 1);
	}
	/*glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, STEP,
		(GLvoid*)0);
	glVertexAttribDivisor(6, 1);
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, STEP,
		(GLvoid*)(sizeof(float)*4) );
	glVertexAttribDivisor(7, 1);
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, STEP,
		(GLvoid*)(sizeof(float) * 8));
	glVertexAttribDivisor(8, 1);
	glEnableVertexAttribArray(9);
	glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, STEP,
		(GLvoid*)(sizeof(float) * 12));
	glVertexAttribDivisor(9, 1);*/

	glEnableVertexAttribArray(10);
	glVertexAttribPointer(10, 2, GL_FLOAT, GL_FALSE, SIZEOF,
		(GLvoid*)(sizeof(float) * 16));
	glVertexAttribDivisor(10, 1);

	glEnableVertexAttribArray(11);
	glVertexAttribPointer(11, 2, GL_FLOAT, GL_FALSE, SIZEOF,
		(GLvoid*)(sizeof(float) * 18));
	glVertexAttribDivisor(11, 1);

	glEnableVertexAttribArray(12);
	glVertexAttribPointer(12, 2, GL_FLOAT, GL_FALSE, SIZEOF,
		(GLvoid*)(sizeof(float) * 20));
	glVertexAttribDivisor(12, 1);

	glBindVertexArray(0);

}

void ParticleMesh::updateInstancedData(std::vector<float>& buffer)
{
	//glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBOinstanced);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*buffer.size()/*size*/, &buffer[0]/*data*/, GL_DYNAMIC_DRAW);
	//glBindVertexArray(0);
}