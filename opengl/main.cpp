/*
 * Written by Aidan Buckner for the CS4600 final project.
 *
 * The assignment 3 code was used as reference for setting up the initial window and basic rendering
 */

#include "OpenGL/glew/glew.h"
#include "OpenGL/freeglut/glut.h"

#include "windows.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <limits>

#include "object3d.h"
#include "math.h"


//Game parameters
const char* WINDOW_NAME = "Game";
const bool FULLSCREEN = false;

const float MOVE_SPEED = 2.0f;
const float MOVE_SPEED_AIR = 2.0f;
const float JUMP_SPEED = 3.0f;
const float SENSITIVITY = 0.2f;

const int FOV = 60; //FOV in degrees

const float NEAR_PLANE = 0.01f;
const float FAR_PLANE = 100.0f;

const float PLAYER_HEIGHT = 0.5f;
const float PLAYER_RADIUS = 0.1f;
const float GRAVITY = 5.0f;
const float DRAG = 10.0f;
const int MIN_HEIGHT = -10.0f;

const float AMBIENT_COLOR[] = { 1.0f, 1.0f, 1.0f, 0.2f };

const float CHECKPOINT_1_POS[] = { 70, 0, 0 };
const float CHECKPOINT_2_POS[] = { 70, 0, 70 };
const float CHECKPOINT_3_POS[] = { 0, 0, 70 };



//game data
auto time_previous = std::chrono::steady_clock::now();
int frame_time[] = { 0, 0 }; //store multiple values for motion smoothing
float player_velocity[] = { 0.0f, 0.0f, 0.0f };
float player_position[] = { 0.0f, PLAYER_HEIGHT, 0.0f };
float player_rotation[] = { 0.0f, 0.0f};
float player_checkpoint[] = { 0.0f, 0.0f, 0.0f };
float move_flags[] = { 0, 0 };
bool standing = false;
int wall_x = 0;
int wall_z = 0;

std::vector<object3d*> objects;


//OpenGL information
GLfloat viewMatrix[16];

GLuint vertex_buffer, index_buffer;


//OpenGL functions
void handleKeyDown(unsigned char key, int x, int y);
void handleKeyUp(unsigned char key, int x, int y);
void resizeCallback(int, int);

//Game functions
void initObjects();
void updatePlayer();
object3d* raycast(float[3], float[3], unsigned int*, float*, float[3], float[3], float);

//Game rendering functions
void renderFrame();




/*
 * Creates the game window
 * Used initWindow function from CS4600 Assignment 3 for reference
*/
int main(int argc, char* argv) {
	//initialize OpenGL
	glutInit(&argc, &argv);
	//initialize the game window
	glutCreateWindow(WINDOW_NAME);
	glewInit();
	glutReshapeFunc(resizeCallback);
	glutDisplayFunc(renderFrame);
	glutIdleFunc(glutPostRedisplay);

	//maximize the window
	if (FULLSCREEN) {
		glutFullScreen();
	}
	else {
		HWND win = GetForegroundWindow();
		ShowWindow(win, SW_MAXIMIZE);
	}


	
	//set render parameters
	glClearColor(0, 0, 0, 0); //background color
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, AMBIENT_COLOR);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//set the mode to projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOV, (GLfloat)glutGet(GLUT_WINDOW_WIDTH) / (GLfloat)glutGet(GLUT_WINDOW_HEIGHT), NEAR_PLANE, FAR_PLANE);
	glMatrixMode(GL_MODELVIEW);

	//set window input handler
	glutKeyboardFunc(handleKeyDown);
	glutKeyboardUpFunc(handleKeyUp);

	//initialize objects
	initObjects();

	//hide the cursor
	glutSetCursor(GLUT_CURSOR_NONE);
	SetCursorPos(glutGet(GLUT_WINDOW_HEIGHT) / 2, glutGet(GLUT_WINDOW_WIDTH) / 2);

	//start the render loop
	glutMainLoop();
}







/*
 * Loads all game objects
 */
void initObjects() {
	//initialize objects
	objects.push_back(new object3d("level", "Resources/level-1/level-1-start.obj", "Resources/level-1-texture.bmp", true));
	objects[0]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-1/level-1-1.obj", "Resources/level-1-texture.bmp", true));
	objects[1]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-1/level-1-2.obj", "Resources/level-1-texture.bmp", true));
	objects[2]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-1/level-1-3.obj", "Resources/level-1-texture.bmp", true));
	objects[3]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-1/level-1-4.obj", "Resources/level-1-texture.bmp", true));
	objects[4]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-1/level-1-5.obj", "Resources/level-1-texture.bmp", true));
	objects[5]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-1/level-1-6.obj", "Resources/level-1-texture.bmp", true));
	objects[6]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-1/level-1-7.obj", "Resources/level-1-texture.bmp", true));
	objects[7]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-1/level-1-8.obj", "Resources/level-1-texture.bmp", true));
	objects[8]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-1/level-1-9.obj", "Resources/level-1-texture.bmp", true));
	objects[9]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-1/level-1-10.obj", "Resources/level-1-texture.bmp", true));
	objects[10]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-1/level-1-11.obj", "Resources/level-1-texture.bmp", true));
	objects[11]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-1/level-1-12.obj", "Resources/level-1-texture.bmp", true));
	objects[12]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-1/level-1-13.obj", "Resources/level-1-texture.bmp", true));
	objects[13]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-1/level-1-14.obj", "Resources/level-1-texture.bmp", true));
	objects[14]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-1/level-1-15.obj", "Resources/level-1-texture.bmp", true));
	objects[15]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-1/level-1-16.obj", "Resources/level-1-texture.bmp", true));
	objects[16]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-1/level-1-17.obj", "Resources/level-1-texture.bmp", true));
	objects[17]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-1/level-1-18.obj", "Resources/level-1-texture.bmp", true));
	objects[18]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-1/level-1-19.obj", "Resources/level-1-texture.bmp", true));
	objects[19]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	objects.push_back(new object3d("level", "Resources/level-2/level-2-start.obj", "Resources/level-2-texture.bmp", true));
	objects[20]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-2/level-2-1.obj", "Resources/level-2-texture.bmp", true));
	objects[21]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-2/level-2-2.obj", "Resources/level-2-texture.bmp", true));
	objects[22]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-2/level-2-3.obj", "Resources/level-2-texture.bmp", true));
	objects[23]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-2/level-2-4.obj", "Resources/level-2-texture.bmp", true));
	objects[24]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-2/level-2-5.obj", "Resources/level-2-texture.bmp", true));
	objects[25]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-2/level-2-6.obj", "Resources/level-2-texture.bmp", true));
	objects[26]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-2/level-2-7.obj", "Resources/level-2-texture.bmp", true));
	objects[27]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-2/level-2-8.obj", "Resources/level-2-texture.bmp", true));
	objects[28]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-2/level-2-9.obj", "Resources/level-2-texture.bmp", true));
	objects[29]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-2/level-2-10.obj", "Resources/level-2-texture.bmp", true));
	objects[30]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-2/level-2-11.obj", "Resources/level-2-texture.bmp", true));
	objects[31]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-2/level-2-12.obj", "Resources/level-2-texture.bmp", true));
	objects[32]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-2/level-2-13.obj", "Resources/level-2-texture.bmp", true));
	objects[33]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-2/level-2-14.obj", "Resources/level-2-texture.bmp", true));
	objects[34]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-2/level-2-15.obj", "Resources/level-2-texture.bmp", true));
	objects[35]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-2/level-2-16.obj", "Resources/level-2-texture.bmp", true));
	objects[36]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-2/level-2-17.obj", "Resources/level-2-texture.bmp", true));
	objects[37]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-2/level-2-18.obj", "Resources/level-2-texture.bmp", true));
	objects[38]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-2/level-2-19.obj", "Resources/level-2-texture.bmp", true));
	objects[39]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	objects.push_back(new object3d("level", "Resources/level-3/level-3-start.obj", "Resources/level-3-texture.bmp", true));
	objects[40]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-3/level-3-start.obj", "Resources/level-3-texture.bmp", true));
	objects[41]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-3/level-3-start.obj", "Resources/level-3-texture.bmp", true));
	objects[42]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-3/level-3-1.obj", "Resources/level-3-texture.bmp", true));
	objects[43]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-3/level-3-2.obj", "Resources/level-3-texture.bmp", true));
	objects[44]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-3/level-3-3.obj", "Resources/level-3-texture.bmp", true));
	objects[45]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-3/level-3-4.obj", "Resources/level-3-texture.bmp", true));
	objects[46]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-3/level-3-5.obj", "Resources/level-3-texture.bmp", true));
	objects[47]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-3/level-3-6.obj", "Resources/level-3-texture.bmp", true));
	objects[48]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-3/level-3-7.obj", "Resources/level-3-texture.bmp", true));
	objects[49]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-3/level-3-8.obj", "Resources/level-3-texture.bmp", true));
	objects[50]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-3/level-3-9.obj", "Resources/level-3-texture.bmp", true));
	objects[51]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-3/level-3-10.obj", "Resources/level-3-texture.bmp", true));
	objects[52]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-3/level-3-11.obj", "Resources/level-3-texture.bmp", true));
	objects[53]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-3/level-3-12.obj", "Resources/level-3-texture.bmp", true));
	objects[54]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-3/level-3-13.obj", "Resources/level-3-texture.bmp", true));
	objects[55]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-3/level-3-14.obj", "Resources/level-3-texture.bmp", true));
	objects[56]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-3/level-3-15.obj", "Resources/level-3-texture.bmp", true));
	objects[57]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-3/level-3-16.obj", "Resources/level-3-texture.bmp", true));
	objects[58]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-3/level-3-17.obj", "Resources/level-3-texture.bmp", true));
	objects[59]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-3/level-3-18.obj", "Resources/level-3-texture.bmp", true));
	objects[60]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-3/level-3-19.obj", "Resources/level-3-texture.bmp", true));
	objects[61]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("level", "Resources/level-3/level-3-end.obj", "Resources/level-3-texture.bmp", true));
	objects[62]->createInstance(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);



	objects.push_back(new object3d("checkpoint", "Resources/flag.obj", "Resources/flag-texture-checkpoint.bmp", true));
	objects[63]->createInstance(1.0f, 70.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	objects[63]->createInstance(1.0f, 70.0f, 0.0f, 70.0f, 0.0f, 0.0f, 0.0f);
	objects.push_back(new object3d("checkpoint", "Resources/flag.obj", "Resources/flag-texture-finish.bmp", true));
	objects[64]->createInstance(1.0f, 0.0f, 0.0f, 70.0f, 0.0f, 0.0f, 0.0f);
	



	int total_faces = 0;
	for (int o = 0; o < objects.size(); o++) {
		total_faces += objects[o]->num_faces;
	}
	size_t index_size = 4 * total_faces * sizeof(GLbyte);
	size_t vertex_size = 8 * 4 * total_faces * sizeof(GLfloat);

	//set up vertex/normal buffer
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_size, 0, GL_STATIC_DRAW);
	
	//set up index buffer
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, 0, GL_STATIC_DRAW);

	unsigned int last_index = 0;
	size_t vertex_offset = 0;
	size_t index_offset = 0;
	for (int o = 0; o < objects.size(); o++) {
		GLfloat *vertex_data = objects[o]->vertices_ordered;
		GLuint *index_data = objects[o]->indices_ordered;
		unsigned int faces = objects[o]->num_faces;
		for (int v = 0; v < 4 * faces; v++) { index_data[v] += last_index; }
		int v_size = 8 * 4 * faces * sizeof(GLfloat);
		int i_size = 4 * faces * sizeof(GLuint);
		glBufferSubData(GL_ARRAY_BUFFER, vertex_offset, v_size, vertex_data);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, index_offset, i_size, index_data);
		objects[o]->vertex_data_offset = vertex_offset;
		vertex_offset += v_size;
		index_offset += i_size;
		last_index += 4 * faces;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


/*
* Checks for ray hits on all objects
*/
object3d* raycast(float origin[3], float direction[3], unsigned int *instance, float* distance, float intersection[3], float normal[3], float error) {
	*distance = std::numeric_limits<float>::max();
	object3d* closest = NULL;
	unsigned int inst = -1;
	for (int o = 0; o < objects.size(); o++) {
		if (objects[o]->raycast_include) {
			for (int i = 0; i < objects[o]->num_instances; i++) {
				float dist;
				if (objects[o]->raycast(i, origin, direction, &dist, intersection, normal, error)) {
					if (dist < *distance) { closest = objects[o]; inst = i; *distance = dist; }
				}
			}
		}
	}
	*instance = inst;
	return closest;
}




/*
* Moves the player based on the movement flags
*/
void updatePlayer() {
	POINT mouse_pos;
	GetCursorPos(&mouse_pos);
	SetCursorPos(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);

	double frame_ms = NStoMS(math::average(frame_time, 2));
	player_rotation[0] += (mouse_pos.y - glutGet(GLUT_WINDOW_HEIGHT) / 2) * (frame_ms * -SENSITIVITY);
	player_rotation[1] += (mouse_pos.x - glutGet(GLUT_WINDOW_WIDTH) / 2) * (frame_ms * -SENSITIVITY);

	//clamp the vertical rotation to prevent the view from goint upside down
	player_rotation[0] = std::max(DEGTORAD(-90), std::min(DEGTORAD(90), player_rotation[0]));

	//raycast down to detect a floor
	float direction[] = { 0.0f, -1.0f, 0.0f };
	float distance_down;
	float intersection_down[3];
	float normal_down[3];
	float move_sin = sin(player_rotation[1]);
	float move_cos = cos(player_rotation[1]);
	unsigned int instance_down;
	if (raycast(player_position, direction, &instance_down, &distance_down, intersection_down, normal_down, PLAYER_RADIUS * 0.9f) && distance_down <= PLAYER_HEIGHT) {
		standing = true;
		
		player_position[1] = intersection_down[1] + PLAYER_HEIGHT;
		player_velocity[1] = std::max(0.0f, player_velocity[1]);

		//apply drag if player standing on something
		
		float drag = DRAG * frame_ms;
		if (player_velocity[0] > 0) { player_velocity[0] = std::max(0.0f, player_velocity[0] - drag); }
		else if (player_velocity[0] < 0) { player_velocity[0] = std::min(0.0f, player_velocity[0] + drag); }
		if (player_velocity[1] > 0) { player_velocity[1] = std::max(0.0f, player_velocity[1] - drag); }
		else if (player_velocity[1] < 0) { player_velocity[1] = std::min(0.0f, player_velocity[1] + drag); }
		if (player_velocity[2] > 0) { player_velocity[2] = std::max(0.0f, player_velocity[2] - drag); }
		else if (player_velocity[2] < 0) { player_velocity[2] = std::min(0.0f, player_velocity[2] + drag); }
	}
	else {
		if (standing == true) {
			player_velocity[0] = (-move_sin * move_flags[0] + move_cos * move_flags[1]) * MOVE_SPEED;
			player_velocity[2] = (move_cos * move_flags[0] + move_sin * move_flags[1]) * MOVE_SPEED;
		}
		standing = false;
		//apply gravity if nothing beneath the player
		player_velocity[1] -= GRAVITY * frame_ms;
	}

	//normalize the move controls so the player doesn't speed up when moving diagonally
	math::normalize(&move_flags[0], &move_flags[1]);

	float x = 0;
	float y = 0;
	float z = 0;

	if (standing) {
		x = move_flags[1] * MOVE_SPEED * frame_ms;
		z = move_flags[0] * MOVE_SPEED * frame_ms;
	}
	else
	{
		player_velocity[0] += (-move_sin * move_flags[0] + move_cos * move_flags[1]) * MOVE_SPEED_AIR * frame_ms;
		player_velocity[2] += (move_cos * move_flags[0] + move_sin * move_flags[1]) * MOVE_SPEED_AIR * frame_ms;
	}

	//apply the rotation of the player
	math::rotateY(&x, &y, &z, player_rotation[1]);

	//apply velocities
	x += player_velocity[0] * frame_ms;
	y += player_velocity[1] * frame_ms;
	z += player_velocity[2] * frame_ms;

	//raycast in the direction of movement to detect obstacles
	float origin[] = { player_position[0], player_position[1] - (PLAYER_HEIGHT / 2), player_position[2] };
	int xdir = 1;
	int zdir = 1;
	if (x < 0) { xdir = -1; }
	if (z < 0) { zdir = -1; }
	float direction_x[] = { 1.0f * xdir, 0.0f, 0.0f };
	float direction_z[] = { 0.0f, 0.0f, 1.0f * zdir };
	float distance_x;
	float distance_z;
	float intersection_x[3];
	float intersection_z[3];
	float normal_x[3];
	float normal_z[3];
	unsigned int instance_x, instance_y, instance_z;

	wall_x = 0;
	wall_z = 0;
	if (raycast(origin, direction_x, &instance_x, &distance_x, intersection_x, normal_x, PLAYER_RADIUS * 0.9f) && distance_x < abs(x) + PLAYER_RADIUS) {
		x = xdir * (distance_x - PLAYER_RADIUS);
		player_velocity[0] = 0.0f;
		wall_x = xdir;
	}
	if (raycast(origin, direction_z, &instance_y, &distance_z, intersection_z, normal_z, PLAYER_RADIUS * 0.9f) && distance_z < abs(z) + PLAYER_RADIUS) {
		z = zdir * (distance_z - PLAYER_RADIUS);
		player_velocity[2] = 0.0f;
		wall_z = zdir;
	}

	if (wall_x != 0 || wall_z != 0) { player_velocity[1] = std::max(player_velocity[1], -0.3f); }

	if (y > 0) {
		float direction_y[] = { 0.0f, 1.0f, 0.0f };
		float distance_y;
		float intersection_y[3];
		float normal_y[3];
		if (raycast(player_position, direction_y, &instance_z, &distance_y, intersection_y, normal_y, PLAYER_RADIUS * 0.9f) && distance_y < y + PLAYER_RADIUS) {
			y = distance_y - PLAYER_RADIUS;
			player_velocity[1] = 0.0f;
		}
	}

	//move the player
	player_position[0] += x;
	player_position[1] += y;
	player_position[2] += z;

	if (player_position[1] < MIN_HEIGHT) {
		player_position[0] = player_checkpoint[0];
		player_position[1] = player_checkpoint[1] + PLAYER_HEIGHT;
		player_position[2] = player_checkpoint[2];
		player_velocity[0] = 0.0f;
		player_velocity[1] = 0.0f;
		player_velocity[2] = 0.0f;
	}

	//checkpoint handling
	float checkpoint_1_diff_x = player_position[0] - CHECKPOINT_1_POS[0];
	float checkpoint_1_diff_z = player_position[2] - CHECKPOINT_1_POS[2];
	float checkpoint_1_distance = abs(sqrt((checkpoint_1_diff_x * checkpoint_1_diff_x) + (checkpoint_1_diff_z * checkpoint_1_diff_z)));

	if (checkpoint_1_distance < 2.0f) {
		player_checkpoint[0] = CHECKPOINT_1_POS[0];
		player_checkpoint[1] = CHECKPOINT_1_POS[1] + PLAYER_HEIGHT;
		player_checkpoint[2] = CHECKPOINT_1_POS[2] + 1;
	}

	float checkpoint_2_diff_x = player_position[0] - CHECKPOINT_2_POS[0];
	float checkpoint_2_diff_z = player_position[2] - CHECKPOINT_2_POS[2];
	float checkpoint_2_distance = abs(sqrt((checkpoint_2_diff_x * checkpoint_2_diff_x) + (checkpoint_2_diff_z * checkpoint_2_diff_z)));

	if (checkpoint_2_distance < 2.0f) {
		player_checkpoint[0] = CHECKPOINT_2_POS[0] - 1;
		player_checkpoint[1] = CHECKPOINT_2_POS[1] + PLAYER_HEIGHT;
		player_checkpoint[2] = CHECKPOINT_2_POS[2];
	}

	float checkpoint_3_diff_x = player_position[0] - CHECKPOINT_3_POS[0];
	float checkpoint_3_diff_z = player_position[2] - CHECKPOINT_3_POS[2];
	float checkpoint_3_distance = abs(sqrt((checkpoint_3_diff_x * checkpoint_3_diff_x) + (checkpoint_3_diff_z * checkpoint_3_diff_z)));

	if (checkpoint_3_distance < 2.0f) {
		player_checkpoint[0] = 0;
		player_checkpoint[1] = PLAYER_HEIGHT;
		player_checkpoint[2] = 0;
	}
}




/*
 * Renders in-game objects to the screen (call every frame)
 * http://www.songho.ca/opengl/gl_vbo.html was really helpful with figuring out how to render multiple objects
 */
void renderFrame() {
	//update frame timers
	auto time_current = std::chrono::steady_clock::now();
	frame_time[1] = frame_time[0];
	frame_time[0] = std::chrono::duration_cast<std::chrono::microseconds>(time_current - time_previous).count();
	time_previous = time_current;

	//move the player
	updatePlayer();

	//clear render buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	//set OpenGL view matrix
	math::updateViewMatrix(viewMatrix, player_rotation[0], player_rotation[1]);
	glLoadMatrixf(viewMatrix);

	//render the scene
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	for (int o = 0; o < objects.size(); o++)
	{
		for (int i = 0; i < objects[o]->num_instances; i++) {
			float scale = objects[o]->instances[i][0];
			float position[3] = { objects[o]->instances[i][1] - player_position[0], objects[o]->instances[i][2] - player_position[1], objects[o]->instances[i][3] - player_position[2] };
			float rotation[3] = { objects[o]->instances[i][4], objects[o]->instances[i][5], objects[o]->instances[i][6] };
			glPushMatrix();

			glTranslatef(position[0], position[1], position[2]);
			glRotatef(rotation[2], 0, 0, 1);
			glRotatef(rotation[1], 0, 1, 0);
			glRotatef(rotation[0], 1, 0, 0);
			glScalef(scale, scale, scale);

			glVertexPointer(3, GL_FLOAT, 0, (void*)(objects[o]->vertex_data_offset));
			glNormalPointer(GL_FLOAT, 0, (void*)(objects[o]->vertex_data_offset + objects[o]->vertex_data_size));
			glTexCoordPointer(2, GL_FLOAT, 0, (void*)(objects[o]->vertex_data_offset + 2 * (objects[o]->vertex_data_size)));

			glBindTexture(GL_TEXTURE_2D, objects[o]->texture_id);

			glDrawArrays(GL_QUADS, 0, 4 * objects[o]->num_faces);

			glBindTexture(GL_TEXTURE_2D, 0);

			glPopMatrix();
		}
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//push the buffers to the display
	glutSwapBuffers();
}





/*
 * Handles key presses
*/
void handleKeyDown(unsigned char key, int x, int y)
{
	//ESC quits the game
	if (key == 27) {
		exit(0);
	}

	//jumping
	if (key == ' ') {
		if (standing) {
			standing = false;
			float s = sin(player_rotation[1]);
			float c = cos(player_rotation[1]);
			math::normalize(&move_flags[0], &move_flags[1]);
			player_velocity[0] = (-s * move_flags[0] + c * move_flags[1]) * MOVE_SPEED;
			player_velocity[2] = (c * move_flags[0] + s * move_flags[1]) * MOVE_SPEED;
			player_velocity[1] = JUMP_SPEED;
		}
		else if (wall_x != 0 || wall_z != 0) {
			player_velocity[0] -= wall_x * (MOVE_SPEED * 0.8f);
			player_velocity[1] += JUMP_SPEED / 2.0f;
			player_velocity[2] -= wall_z * (MOVE_SPEED * 0.8f);
		}
	}

	//player movement flags
	if (key == 'w') {
		move_flags[1] = -1.0f;
	}
	else if (key == 's') {
		move_flags[1] = 1.0f;
	}
	else if (key == 'a') {
		move_flags[0] = -1.0f;
	}
	else if (key == 'd') {
		move_flags[0] = 1.0f;
	}
}


/*
 * Handles key releases
*/
void handleKeyUp(unsigned char key, int x, int y)
{
	//player movement flags
	if (key == 'w' && move_flags[1] < 0) {
		move_flags[1] = 0.0f;
	}
	else if (key == 's' && move_flags[1] > 0) {
		move_flags[1] = 0.0f;
	}
	else if (key == 'a' && move_flags[0] < 0) {
		move_flags[0] = 0.0f;
	}
	else if (key == 'd' && move_flags[0] > 0) {
		move_flags[0] = 0.0f;
	}
}


/*
 * Called when the window is resized
 */
void resizeCallback(int w, int h)
{
	glViewport(0, 0, w, h);
	gluPerspective(FOV, (GLfloat)w / (GLfloat)h, NEAR_PLANE, FAR_PLANE);
}