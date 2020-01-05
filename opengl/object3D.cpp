#include "object3d.h"

#define MAX_RAYCAST_DISTANCE 10.0f

/*
* Loads object data from the .obj file located at path
* Assignment 3 loadObj code used as reference
*/
object3d::object3d(const char name[], const char modelPath[], const char texturePath[], bool raycast) {
	tag = name;
	raycast_include = raycast;
	num_instances = 0;

	loadModel(modelPath);
	loadTexture(texturePath);
}


/*
* Creates a new instance of the object
*/
void object3d::createInstance(float scale, float pos_x, float pos_y, float pos_z, float rot_x, float rot_y, float rot_z){
	std::vector<float> inst;
	inst.push_back(scale);
	inst.push_back(pos_x);
	inst.push_back(pos_y);
	inst.push_back(pos_z);
	inst.push_back(rot_x);
	inst.push_back(rot_y);
	inst.push_back(rot_z);
	instances.push_back(inst);
	num_instances++;
}



/*
* Sets vertex, texture, and normal to the values encoded in face
*/
void object3d::parseFace(std::string face, GLubyte *vertex, GLubyte *texture, GLubyte *normal) {
	int pos = 0;
	int del = face.find('/', pos);
	*vertex = std::stoi(face.substr(pos, del));
	pos = del + 1;
	del = face.find('/', pos);
	*texture = std::stoi(face.substr(pos, del));
	pos = del + 1;
	*normal = std::stoi(face.substr(pos, face.size()));
}




/*
 * Casts a ray from position in direction; distance is set if there is a collision with this object
 * https://samsymons.com/blog/math-notes-ray-plane-intersection/ used as reference for ray-plane intersection
 */
bool object3d::raycast(unsigned int instance, float origin[3], float direction[3], float *distance, float intersection[3], float norm[3], float error) {
	float scale = instances[instance][0];
	float position[3];
	position[0] = instances[instance][1];
	position[1] = instances[instance][2];
	position[2] = instances[instance][3];
	float rotation[3];
	rotation[0] = instances[instance][4];
	rotation[1] = instances[instance][5];
	rotation[2] = instances[instance][6];

	bool hit = false;
	*distance = std::numeric_limits<float>::max();
	for (int f = 0; f < num_faces; f++) {
		float normal[3];
		normal[0] = face_normals[3 * f + 0];
		normal[1] = face_normals[3 * f + 1];
		normal[2] = face_normals[3 * f + 2];

		float denominator = (normal[0] * direction[0]) + (normal[1] * direction[1]) + (normal[2] * direction[2]);
		//check for intersection if the ray and face are not (approximately) in the same line
		if (std::abs(denominator) > 0.001f) {
			int v0 = vertices_index[4 * f + 0];
			int v1 = vertices_index[4 * f + 1];
			int v2 = vertices_index[4 * f + 2];
			int v3 = vertices_index[4 * f + 3];

			float center[3];
			math::averageVector(vertices, v0, v1, v2, v3, center);
			//rotate
			math::rotate(&center[0], &center[1], &center[2], rotation[0], rotation[1], rotation[2]);
			//scale relative to the local coordinate space
			center[0] *= scale;
			center[1] *= scale;
			center[2] *= scale;
			//convert to the global coordinate space and take the difference
			center[0] += position[0];
			center[1] += position[1];
			center[2] += position[2];

			float difference[3];
			difference[0] = center[0] - origin[0];
			difference[1] = center[1] - origin[1];
			difference[2] = center[2] - origin[2];

			//end early if there is no chance of a hit
			if (abs(difference[0]) > MAX_RAYCAST_DISTANCE || abs(difference[1]) > MAX_RAYCAST_DISTANCE || abs(difference[2]) > MAX_RAYCAST_DISTANCE) {
				return false;
			}

			float t = ((difference[0] * normal[0]) + (difference[1] * normal[1]) + (difference[2] * normal[2])) / denominator;
			if (t > 0.001f && t < *distance) {
				float intersect[3];
				intersect[0] = origin[0] + t * direction[0];
				intersect[1] = origin[1] + t * direction[1];
				intersect[2] = origin[2] + t * direction[2];
				
				//check if the point of intersection is inside the face
				if (math::liesInRange(vertices, v0, v1, v2, v3, intersect, position, rotation, scale, error)) {
					hit = true;
					*distance = t;
					intersection[0] = intersect[0];
					intersection[1] = intersect[1];
					intersection[2] = intersect[2];
					norm[0] = normal[0];
					norm[1] = normal[1];
					norm[2] = normal[2];
				}
			}
		}
	}
	return hit;
}




/*
* Loads the model from an obj file
*/
void object3d::loadModel(const char path[]) {
	//open obj file and load it into vectors
	std::ifstream file;
	file.open(path);
	std::string s;

	while (file >> s) {
		if (s.compare("v") == 0) {
			//read vertices
			GLfloat x, y, z;
			file >> x >> y >> z;

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
		else if (s.compare("vn") == 0) {
			//read vertex normals
			GLfloat x, y, z;
			file >> x >> y >> z;

			normals.push_back(x);
			normals.push_back(y);
			normals.push_back(z);
		}
		else if (s.compare("vt") == 0) {
			//read vertex normals
			GLfloat u, v;
			file >> u >> v;

			textures.push_back(u);
			textures.push_back(v);
		}
		else if (s.compare("f") == 0) {
			//read faces
			std::string a, b, c, d;
			GLubyte vertex_a, vertex_b, vertex_c, vertex_d;
			GLubyte texture_a, texture_b, texture_c, texture_d;
			GLubyte normal_a, normal_b, normal_c, normal_d;
			file >> a >> b >> c >> d;

			parseFace(a, &vertex_a, &texture_a, &normal_a);
			parseFace(b, &vertex_b, &texture_b, &normal_b);
			parseFace(c, &vertex_c, &texture_c, &normal_c);
			parseFace(d, &vertex_d, &texture_d, &normal_d);

			vertices_index.push_back(vertex_a - 1);
			vertices_index.push_back(vertex_b - 1);
			vertices_index.push_back(vertex_c - 1);
			vertices_index.push_back(vertex_d - 1);

			textures_index.push_back(texture_a - 1);
			textures_index.push_back(texture_b - 1);
			textures_index.push_back(texture_c - 1);
			textures_index.push_back(texture_d - 1);

			normals_index.push_back(normal_a - 1);
			normals_index.push_back(normal_b - 1);
			normals_index.push_back(normal_c - 1);
			normals_index.push_back(normal_d - 1);
		}
		else {
			//skip unneeded lines
			std::getline(file, s);
		}
	}

	num_vertices = vertices.size() / 3;
	num_faces = vertices_index.size() / 4;

	//compute face normals from vertex normals
	for (int f = 0; f < num_faces; f++) {
		//find vertex indices for this face
		int n0 = normals_index[4 * f + 0];
		int n1 = normals_index[4 * f + 1];
		int n2 = normals_index[4 * f + 2];
		int n3 = normals_index[4 * f + 3];
		//compute average normal for the face
		float normal[3];
		math::averageVector(normals, n0, n1, n2, n3, normal);
		face_normals.push_back(normal[0]);
		face_normals.push_back(normal[1]);
		face_normals.push_back(normal[2]);
	}

	//organize data by vertex index (v_x, v_y, v_z, n_x, n_y, n_z, t_u, t_v), 8 floats per vert, 4 verts per face
	//essentially creates a copy of each vertex in a face so it is easier to read
	vertices_ordered = new GLfloat[num_faces * 4 * 8];
	indices_ordered = new GLuint[num_faces * 4];

	for (int f = 0; f < num_faces * 4; f++) {
		GLubyte v_index = vertices_index[f];
		GLubyte n_index = normals_index[f];
		GLubyte t_index = textures_index[f];
		indices_ordered[f] = f;
		vertices_ordered[3 * f + 0] = vertices[3 * v_index + 0];
		vertices_ordered[3 * f + 1] = vertices[3 * v_index + 1];
		vertices_ordered[3 * f + 2] = vertices[3 * v_index + 2];
		vertices_ordered[(12 * num_faces) + (3 * f + 0)] = normals[3 * n_index + 0];
		vertices_ordered[(12 * num_faces) + (3 * f + 1)] = normals[3 * n_index + 1];
		vertices_ordered[(12 * num_faces) + (3 * f + 2)] = normals[3 * n_index + 2];
		vertices_ordered[(24 * num_faces) + (2 * f + 0)] = textures[2 * t_index + 0];
		vertices_ordered[(24 * num_faces) + (2 * f + 1)] = textures[2 * t_index + 1];
	}

	vertex_data_size = 3 * 4 * num_faces * sizeof(GLfloat);
	index_data_size = 4 * num_faces * sizeof(GLuint);

	file.close();
	//report that the object was loaded
	std::cout << path << " loaded. Vertices: " << num_vertices << " Quads: " << num_faces << std::endl;
}

/*
* Loads the texture from a ppm file
* http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/ used as reference for BMP loading and texture creation
*/
void object3d::loadTexture(const char path[]) {
	FILE *file;
	fopen_s(&file, path, "rb");

	//image data
	unsigned char header[54];
	unsigned int pos;
	unsigned int size;
	unsigned int width;
	unsigned int height;
	unsigned char *image;

	//read header
	fread(header, 1, 54, file);
	pos = *(int*)&(header[0x0A]);
	size = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	image = new unsigned char[size];

	//read the image data
	fread(image, 1, size, file);

	fclose(file);

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, image);
	glBindTexture(GL_TEXTURE_2D, 0);

	free(image);
	
	//report that the texture was loaded
	std::cout << path << " loaded. Dimensions: " << width << " x " << height << std::endl;
}