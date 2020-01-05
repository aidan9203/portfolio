#include "math.h"


/*
* Normalizes the vector (x, y)
*/
void math::normalize(float *x, float *y) {
	//compute length
	float length = sqrt((*x * *x) + (*y * *y));

	if (length == 0) { return; } //can't divide by zero

	//normalize each component
	*x = *x / length;
	*y = *y / length;
}

/*
* Normalizes the vector (x, y, z)
*/
void math::normalize(float *x, float *y, float *z) {
	//compute length
	float length_h = sqrt((*x * *x) + (*z * *z));
	float length = sqrt((length_h * length_h) + (*y * *y));

	if (length == 0) { return; } //can't divide by zero

	//normalize each component
	*x = *x / length;
	*y = *y / length;
	*z = *z / length;
}


/*
 * Sets result to the average of the four vectors
 */
void math::averageVector(std::vector<float> source, int v0, int v1, int v2, int v3, float result[]) {
	result[0] = (source[3 * v0 + 0] + source[3 * v1 + 0] + source[3 * v2 + 0] + source[3 * v3 + 0]) / 4.0f;
	result[1] = (source[3 * v0 + 1] + source[3 * v1 + 1] + source[3 * v2 + 1] + source[3 * v3 + 1]) / 4.0f;
	result[2] = (source[3 * v0 + 2] + source[3 * v1 + 2] + source[3 * v2 + 2] + source[3 * v3 + 2]) / 4.0f;
}


/*
 * Computes the average of the array
 */
double math::average(int arr[], int n) {
	double sum = 0;
	for (int i = 0; i < n; i++) {
		sum += arr[i];
	}
	return sum / n;
}


/*
* Converts the player's orientation to a normalized direction
*/
void math::convertPlayerDirection(float dir_player[2], float dir_result[3]) {
	float x = -1.0f;
	float y = 0.0f;
	float z = 0.0f;
	rotateZ(&x, &y, &z, dir_player[0]);
	rotateY(&x, &y, &z, dir_player[1]);
	dir_result[0] = x;
	dir_result[1] = y;
	dir_result[2] = z;
}


/*
 * Returns whether the point lies within the specified area
 */
bool math::liesInRange(std::vector<float> vertices, int v0, int v1, int v2, int v3, float point[3], float offset[3], float rotation[3], float scale, float err) {
	float x_max, y_max, z_max;
	float x_min, y_min, z_min;
	getMaxAndMin(vertices[3 * v0 + 0], vertices[3 * v1 + 0], vertices[3 * v2 + 0], vertices[3 * v3 + 0], &x_min, &x_max);
	getMaxAndMin(vertices[3 * v0 + 1], vertices[3 * v1 + 1], vertices[3 * v2 + 1], vertices[3 * v3 + 1], &y_min, &y_max);
	getMaxAndMin(vertices[3 * v0 + 2], vertices[3 * v1 + 2], vertices[3 * v2 + 2], vertices[3 * v3 + 2], &z_min, &z_max);
	//account for scaling and world position
	math::rotate(&x_min, &y_min, &z_min, rotation[0], rotation[1], rotation[2]);
	math::rotate(&x_max, &y_max, &z_max, rotation[0], rotation[1], rotation[2]);
	x_max = scale * x_max + offset[0] + err;
	x_min = scale * x_min + offset[0] - err;
	y_max = scale * y_max + offset[1] + err;
	y_min = scale * y_min + offset[1] - err;
	z_max = scale * z_max + offset[2] + err;
	z_min = scale * z_min + offset[2] - err;

	return (x_min <= point[0] && point[0] <= x_max) && (y_min <= point[1] && point[1] <= y_max) && (z_min <= point[2] && point[2] <= z_max);
}

/*
 * Calculates the maximum and minimum values in the range +/- err
 */
void math::getMaxAndMin(float x0, float x1, float x2, float x3, float* min, float* max) {
	*max = std::max(x0, x1);
	*max = std::max(*max, x2);
	*max = std::max(*max, x3);
	*min = std::min(x0, x1);
	*min = std::min(*min, x2);
	*min = std::min(*min, x3);
}



/*
* Rotates the coordinates around the X, Y, and Z axis (relative to the origin)
*/
void math::rotate(float* x, float* y, float* z, float rot_x, float rot_y, float rot_z) {
	rotateX(x, y, z, rot_x);
	rotateY(x, y, z, rot_y);
	rotateZ(x, y, z, rot_z);
}

/*
* Rotates the coordinates angle radians around the X axis (relative to the origin)
* https://petercollingridge.appspot.com/3D-tutorial/rotating-objects/ was my reference for the math
*/
void math::rotateX(float *x, float *y, float *z, float angle) {
	float s = sin(angle);
	float c = cos(angle);
	float _y = *y;
	float _z = *z;
	*y = (_y * c) - (_z * s);
	*z = (_z * c) + (_y * s);
}

/*
 * Rotates the coordinates angle radians around the Y axis (relative to the origin)
 * https://petercollingridge.appspot.com/3D-tutorial/rotating-objects/ was my reference for the math
 */
void math::rotateY(float *x, float *y, float *z, float angle) {
	float s = sin(angle);
	float c = cos(angle);
	float _x = *x;
	float _z = *z;
	*x = (_x * c) - (_z * s);
	*z = (_z * c) + (_x * s);
}

/*
 * Rotates the coordinates angle radians around the Z axis (relative to the origin)
 * https://petercollingridge.appspot.com/3D-tutorial/rotating-objects/ was my reference for the math
 */
void math::rotateZ(float *x, float *y, float *z, float angle) {
	float s = sin(angle);
	float c = cos(angle);
	float _x = *x;
	float _y = *y;
	*x = (_x * c) + (_y * s);
	*y = (_y * c) - (_x * s);
}


/*
* Updates the view matrix with the view position and rotation
* View matrix computations from https://gamedev.stackexchange.com/questions/168542/camera-view-matrix-from-position-yaw-pitch-worldup
*/
void math::updateViewMatrix(GLfloat viewMatrix[16], float horizontal, float vertical) {
	float yaw_cos = cos(horizontal);
	float yaw_sin = sin(horizontal);
	float pitch_cos = cos(-vertical);
	float pitch_sin = sin(-vertical);

	//x direction
	viewMatrix[0] = pitch_sin;
	viewMatrix[1] = pitch_cos * yaw_sin;
	viewMatrix[2] = pitch_cos * yaw_cos;

	viewMatrix[3] = 0;

	//y direction
	viewMatrix[4] = 0;
	viewMatrix[5] = yaw_cos;
	viewMatrix[6] = -yaw_sin;

	viewMatrix[7] = 0;

	//z direction
	viewMatrix[8] = pitch_cos;
	viewMatrix[9] = -pitch_sin * yaw_sin;
	viewMatrix[10] = -pitch_sin * yaw_cos;

	viewMatrix[11] = 0;

	//position
	viewMatrix[12] = 0;
	viewMatrix[13] = 0;
	viewMatrix[14] = 0;

	viewMatrix[15] = 1.0f;
}