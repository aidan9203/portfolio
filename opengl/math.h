#ifndef MATH
#define MATH

#include "OpenGL/glew/glew.h"
#include "OpenGL/freeglut/glut.h"

#include <cmath>
#include <vector>
#include <algorithm>

class math {
public:
	#define DEGTORAD(angle) (angle * (3.14159f / 180.0f))

	#define NStoMS(time) (time / 1000000.0f)
	#define MStoS(time) (time / 1000.0f)
	#define MStoS(time) (time / 1000000000.0f)
	#define MStoNS(time) (time * 1000000.0f)
	#define StoMS(time) (time * 1000.0f)
	#define StoNS(time) (time * 1000000000.0f)

	//normalizes the vector (x, y)
	static void normalize(float*, float*);
	//normalizes the vector (x, y, z)
	static void normalize(float*, float*, float*);

	//applies a rotation to the vector (x, y, z) along the axis
	static void rotate(float*, float*, float*, float, float, float);
	static void rotateX(float*, float*, float*, float);
	static void rotateY(float*, float*, float*, float);
	static void rotateZ(float*, float*, float*, float);

	static void averageVector(std::vector<float>, int, int, int, int, float[]);

	static bool liesInRange(std::vector<float>, int, int, int, int, float[3], float[3], float[3], float, float);
	static void getMaxAndMin(float, float, float, float, float*, float*);

	static double average(int[], int n);

	static void convertPlayerDirection(float[2], float[3]);

	//updates the view matrix with the given pitch and yaw
	static void updateViewMatrix(GLfloat[16], float, float);
};
#endif