#ifndef OBJECT3D
#define OBJECT3D

#include "OpenGL/glew/glew.h"
#include "OpenGL/freeglut/glut.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <limits>

#include "math.h"

class object3d {
public:
	const char* tag;

	bool raycast_include;

	//object instances
	unsigned int num_instances;
	std::vector<std::vector<float>> instances;

	//object data
	unsigned int num_vertices;
	unsigned int num_faces;

	std::vector<GLfloat> face_normals;

	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> textures;

	std::vector<GLubyte> vertices_index;
	std::vector<GLubyte> normals_index;
	std::vector<GLubyte> textures_index;

	GLfloat *vertices_ordered;
	GLuint *indices_ordered;

	size_t vertex_data_size;
	size_t index_data_size;

	size_t vertex_data_offset;

	//texture data
	GLuint texture_id;

	object3d(const char[], const char[], const char[], bool);

	void createInstance(float, float, float, float, float, float, float);

	bool raycast(unsigned int, float[3], float[3], float*, float[3], float[3], float);

private:
	static void parseFace(std::string, GLubyte*, GLubyte*, GLubyte*);
	void loadModel(const char[]);
	void loadTexture(const char[]);
};
#endif