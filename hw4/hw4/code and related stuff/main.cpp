/* 
 * city
 * 
 * File:   main.cpp
 * Author: rafields
 *
 * rafields - 99620151
 * 
 * Some messy code...
 */

#ifdef __APPLE__
#include <GL/glew.h>
#include <OpenGL/gl.h> 
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h> 
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include <string>
#include <cassert>
#include "array_points_FINAL.hpp"
//#include "car_points.hpp"
//#include "array_textures_FINAL.hpp"
#include "BezierCurve.h"
#include "car_points.hpp"

#define PI 3.14159265
#define X 0
#define Y 1
#define Z 2


using namespace std;

bool mouseDown = false;
float cur_x = 0,
	  cur_y = 0;
float rot_x = 0,
	  rot_y = 1; //originally 0
int prev_x = 0,
	prev_y = 0;
bool x_right = false,
	 y_up = false;
float x_trans = 0,
	  z_trans = 0;
// All global flags will go in here, and should be defined
// at the beginning; that way, later on, we know that if a flag
// isn't in the map, it doesn't exist.
map<string, bool> global_flags;

float camera[] = {0,		0,		3};
float focus[] =  {0,		1 ,	    1};
    // Note: the focus's y coordinate should just be fixed at 1.
float up[]    =  {0,		1,		0};
	/* I have a new appreciation for the people who made Up now, knowing they had to
	 * do this to make it.
	 */

//miscellaneous globals
GLuint prog, vert_shader, frag_shader, pos_info; //program, vert, and fragment IDs
size_t path_pos = 0;

float* checkpoint;

void render(); //render program
void stop(); //stops the simulation
void keyUp(unsigned char key, int, int); //key catcher
void update(); //updates the frame
void loadShader(); //loads the shader programs
void setUniformFloat(const char *varName, float value); //sets the uniforms
char* readFile(const char *); //reads the shaders
void mouse(int button, int state, int x, int y); //mouse button catcher
void mouseMotion(int x, int y); //mouse motion catcher
void normalize(const float coords[][3][3], float normalized[][3][3], int quadCount); //normalizes coords
void findNormal(int quadID, float structure[][3][3], int p1, int p2, int p3, float * normal); //returns the surface normal for a trimesh
void findNormal(int quadID, float structure[][3][3], int p1, int p2, float * point_4, float * normal); //returns the surface normal for a trimesh
void findNormalFan(int spine_1, int plane_1, int spine_2, int plane_2, float * normal); //returns the surface normals for a trifan
void setLighting(void); //sets up the lighting
void drawStructure(float structure[][3][3], int maxQuads);
float * generateFourthPoint(int quadID, float structure [][3][3], bool * success);
int samePoints(int quadID, float structure[][3][3]);
int diffPoints(int quadID, float structure[][3][3], int point_1, int point_2, int * pointChosen);
int diffPoints(int quadID, float structure[][3][3], int point_1, int point_2, int prevChosen, int * pointChosen);
int whichChosen(int quadID, float structure[][3][3], int XYZ);

/*
 * All the initializations occur here, directly (in-function) or indirectly (called function).
 */

int main(int argc, char** argv) {

    glutInit(&argc, argv); //initialize glut
    glutInitWindowSize(500, 500); //set window size
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); //going with RGB, double buffer, depth buffer
    glutCreateWindow("Vase - rafields - 996270151");
	

	//setLighting(); //set up the lighting


    glutKeyboardUpFunc(keyUp); //catches keypresses
    glutDisplayFunc(render); //the render function
    glutIdleFunc(update); //calls the render function to continuously update the image
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);

	//normalize(BUILDING_TEST, BUILDING_TEST_NORMALIZED, BUILDING_TEST_QUADAMT);
	
    GLenum err = glewInit();
    loadShader(); //loads our shader programsAll the initialization occurs here.

    // Initialize global flags here, since C++ is too retarded to
    // let us do it in the global area.
    global_flags["update camera"] = false;
    global_flags["mouse down"] = false;
    global_flags["follow car"] = false;
    glutMainLoop(); //summons a unicorn
    return 0;
}


/*
 * Sets up the lighting
 */
void setLighting(void)
{
	glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	//glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1);

    float light_pos[] = {-1.0f, 1.0f, 1.0f, 0.0f};
	float light_ambient[] = {0.5f, 0.5f, 0.5f, 0.5f};
	float light_diffuse[] = {0.1, 0.5, 0.8, 1.0 };

	
    GLfloat materialColor[] = {0.3f, 1.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glEnable(GL_LIGHT0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
	
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	 
    //float mat_ambientdiffuse[] = {0.5, 0, 0, 1};
    //float mat_spec[] = {1, 1, 0, 1};
    //float mat_shininess = 10;

    //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_ambientdiffuse);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec);
    //glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}


/*
 * Generates our mesh
 */

/*
 * draws our mesh
 */
void drawStructure(float structure[][3][3], int maxQuads)
{

	glColor4f(0.0, 0.3, 0.3, 1);



	//handles the normals
	float * normalArray = new float[3];
	float * p_4;
	float tempArr[3];
	bool * success = new bool;
	*success = true;
	
	//this is going to get messy
	//Format: meshPoints[spine_number][y-plane_number][x, y, or z]
	//for (int i = 0; i < maxQuads; i++)
	for (int i = 0; i < maxQuads; i++)
	{
		*success = true;
		p_4 = generateFourthPoint(i, structure, success);

		if (*success)
		{
			/*
			glBegin(GL_TRIANGLE_STRIP);


			findNormal(i, structure, 1, 0, 2, normalArray);
			glNormal3f(normalArray[X], normalArray[Z], normalArray[Y]);
			glVertex3i(structure[i][1][X], structure[i][1][Z], structure[i][1][Y]);
			glNormal3f(normalArray[X], normalArray[Z], normalArray[Y]);
			glVertex3i(structure[i][0][X], structure[i][0][Z], structure[i][0][Y]);
			glNormal3f(normalArray[X], normalArray[Z], normalArray[Y]);
			glVertex3i(structure[i][2][X], structure[i][2][Z], structure[i][2][Y]);
		
		
			//findNormal(i, structure, 2, 0, p_4, normalArray);

			glNormal3f(normalArray[X], normalArray[Z], normalArray[Y]);
			glVertex3i(			   p_4[X],			   p_4[Z],			   p_4[Y]);
			glEnd();*/


			glBegin(GL_LINE_LOOP);

			
			glVertex3i(structure[i][0][X], structure[i][0][Z], structure[i][0][Y]);
			glVertex3i(structure[i][1][X], structure[i][1][Z], structure[i][1][Y]);
			glVertex3i(structure[i][2][X], structure[i][2][Z], structure[i][2][Y]);
			glVertex3i(			   p_4[X],			   p_4[Z],			   p_4[Y]);
			glEnd();
		}



		delete p_4;
	}

	delete normalArray;
	delete success;
}

float * generateFourthPoint(int quadID, float structure [][3][3], bool * success)
{
	float * retPoints = new float[3];
	int oops;
	int count = 0;
	int debug[3];

	int * pointChosen = new int;

	switch(diffPoints(quadID, structure, 0, 1, pointChosen))
	{
	case X:
		retPoints[X] = structure[quadID][*pointChosen][X];
		count += 1;
		debug[0] = X;
		break;

	case Y:
		retPoints[Y] = structure[quadID][*pointChosen][Y];
		count += 10;
		debug[0] = Y;
		break;

	case Z:
		retPoints[Z] = structure[quadID][*pointChosen][Z];
		count += 100;
		debug[0] = Z;
		break;

	case -1:
		*success = false;
		break;
	}

	switch(diffPoints(quadID, structure, 0, 2, debug[0], pointChosen))
	{
	case X:
		retPoints[X] = structure[quadID][*pointChosen][X];
		count += 1;
		debug[1] = X;
		break;

	case Y:
		retPoints[Y] = structure[quadID][*pointChosen][Y];
		count += 10;
		debug[1] = Y;
		break;

	case Z:
		retPoints[Z] = structure[quadID][*pointChosen][Z];
		count += 100;
		debug[1] = Z;
		break;

	case -1:
		*success = false;
		break;
	}

	switch(samePoints(quadID, structure))
	{
	case X:
		retPoints[X] = structure[quadID][2][X];
		count += 1;
		debug[2] = X;
		break;

	case Y:
		retPoints[Y] = structure[quadID][2][Y];
		count += 10;
		debug[2] = Y;
		break;

	case Z:
		retPoints[Z] = structure[quadID][2][Z];
		count += 100;
		debug[2] = Z;
		break;

	case -1:
		*success = false;
		break;
	}

	if (count != 111)
	{
		*success = false;
	}

	delete pointChosen;

	return retPoints;
}

int samePoints(int quadID, float structure[][3][3])
{
		int pointID = -1;

		if ((structure[quadID][0][X] == structure[quadID][1][X]) && (structure[quadID][0][X] == structure[quadID][2][X]))
		{
			pointID = X;
		}
		else if ((structure[quadID][0][Y] == structure[quadID][1][Y]) && (structure[quadID][0][Y] == structure[quadID][2][Y]))
		{
			pointID = Y;
		}
		else if ((structure[quadID][0][Z] == structure[quadID][1][Z]) && (structure[quadID][0][Z] == structure[quadID][2][Z]))
		{
			pointID = Z;
		}

		return pointID;
}

int diffPoints(int quadID, float structure[][3][3], int point_1, int point_2, int * pointChosen)
{
		int pointID = -1;

		if ((int)structure[quadID][point_1][X] != (int)structure[quadID][point_2][X])
		{
			pointID = X;
			*pointChosen = whichChosen(quadID, structure, X);
		}
		else if ((int)structure[quadID][point_1][Y] != (int)structure[quadID][point_2][Y])
		{
			pointID = Y;
			*pointChosen = whichChosen(quadID, structure, Y);
		}
		else if ((int)structure[quadID][point_1][Z] != (int)structure[quadID][point_2][Z])
		{
			pointID = Z;
			*pointChosen = whichChosen(quadID, structure, Z);
		}

		return pointID;
}

int whichChosen(int quadID, float structure[][3][3], int XYZ)
{
		int pointChosen = -1;

		if ((structure[quadID][0][XYZ] != structure[quadID][1][XYZ]) && (structure[quadID][0][XYZ] != structure[quadID][2][XYZ]))
		{
				pointChosen = 0;
		}
		else if ((structure[quadID][1][XYZ] != structure[quadID][0][XYZ]) && (structure[quadID][1][XYZ] != structure[quadID][2][XYZ]))
		{
				pointChosen = 1;
		}
		else if ((structure[quadID][2][XYZ] != structure[quadID][0][XYZ]) && (structure[quadID][2][XYZ] != structure[quadID][1][XYZ]))
		{
				pointChosen = 2;
		}

		return pointChosen;
		
}

int diffPoints(int quadID, float structure[][3][3], int point_1, int point_2, int prevChosen, int * pointChosen)
{
		int pointID = -1;

		if (((int)structure[quadID][point_1][X] != (int)structure[quadID][point_2][X]) && (prevChosen != X))
		{
			pointID = X;
			*pointChosen = whichChosen(quadID, structure, X);
		}
		else if (((int)structure[quadID][point_1][Y] != (int)structure[quadID][point_2][Y]) && (prevChosen != Y))
		{
			pointID = Y;
			*pointChosen = whichChosen(quadID, structure, Y);
		}
		else if (((int)structure[quadID][point_1][Z] != (int)structure[quadID][point_2][Z]) && (prevChosen != Z))
		{
			pointID = Z;
			*pointChosen = whichChosen(quadID, structure, Z);
		}



		return pointID;
}

/*
 * Calculates the face normals for a triangle surface (non-fan) for flat shading
 */
void findNormal(int quadID, float structure[][3][3], int p1, int p2, int p3, float * normal)
{
	//using the psuedocode provided by http://www.opengl.org/wiki/Calculating_a_Surface_Normal

	//makes it easier to handle this
	const int x = 0,
			  y = 2,
			  z = 1;
	
	float u[3] = {
		structure[quadID][p2][x] - structure[quadID][p1][x],
		structure[quadID][p2][y] - structure[quadID][p1][y],
		structure[quadID][p2][z] - structure[quadID][p1][z]};
	
	float v[3] = {
		structure[quadID][p3][x] - structure[quadID][p1][x],
		structure[quadID][p3][y] - structure[quadID][p1][y],
		structure[quadID][p3][z] - structure[quadID][p1][z]};

	//cross product
	normal[x] = (u[y] * v[z]) - (u[z] * v[y]);
	normal[y] = (u[z] * v[x]) - (u[x] * v[z]);
	normal[z] = (u[x] * v[y]) - (u[y] * v[x]);

	//normalize
	float norm_len = sqrt( normal[x] * normal[x] 
						 + normal[y] * normal[y]
						 + normal[z] * normal[z]);
	normal[x] /= norm_len;
	normal[y] /= norm_len;
	normal[z] /= norm_len;

}

/*
 * Calculates the face normals for a triangle surface (non-fan) for flat shading
 */
void findNormal(int quadID, float structure[][3][3], int p1, int p2, float * point_four, float * normal)
{
	//using the psuedocode provided by http://www.opengl.org/wiki/Calculating_a_Surface_Normal

	//makes it easier to handle this
	const int x = 0,
			  y = 1,
			  z = 2;
	
	float u[3] = {
		structure[quadID][p2][x] - structure[quadID][p1][x],
		structure[quadID][p2][y] - structure[quadID][p1][y],
		structure[quadID][p2][z] - structure[quadID][p1][z]};
	
	float v[3] = {
		point_four[x] - structure[quadID][p1][x],
		point_four[y] - structure[quadID][p1][y],
		point_four[z] - structure[quadID][p1][z]};

	//cross product
	normal[x] = (u[y] * v[z]) - (u[z] * v[y]);
	normal[y] = (u[z] * v[x]) - (u[x] * v[z]);
	normal[z] = (u[x] * v[y]) - (u[y] * v[x]);

	//normalize
	float norm_len = sqrt( normal[x] * normal[x] 
						 + normal[y] * normal[y]
						 + normal[z] * normal[z]);
	normal[x] /= norm_len;
	normal[y] /= norm_len;
	normal[z] /= norm_len;

}

//on second thought, I probably should have just added an if statement into findNormal for when I'm dealing with a fan....
/*
 * Calculates the face normals for a triangle fan for flat shading
 */
void findNormalFan(int spine_1, int plane_1, int spine_2, int plane_2, float * normal)
{
	//see notes from the previous normal finder func
	const int x = 0,
			  y = 1,
			  z = 2;
	//midpoint
	/*float mid_coords[3] = {0, meshPoints[0][NUM_SEGS * 5 - 1][1], 0};

	float u[3] = {
		meshPoints[spine_2][plane_2][x] - meshPoints[spine_1][plane_1][x],
		meshPoints[spine_2][plane_2][y] - meshPoints[spine_1][plane_1][y],
		meshPoints[spine_2][plane_2][z] - meshPoints[spine_1][plane_1][z]};
	
	float v[3] = {
		mid_coords[x] - meshPoints[spine_1][plane_1][x],
		mid_coords[y] - meshPoints[spine_1][plane_1][y],
		mid_coords[z] - meshPoints[spine_1][plane_1][z]};

	normal[x] = (u[y] * v[z]) - (u[z] * v[y]);
	normal[y] = (u[z] * v[x]) - (u[x] * v[z]);
	normal[z] = (u[x] * v[y]) - (u[y] * v[x]);

	float norm_len = sqrt( normal[x] * normal[x] 
						 + normal[y] * normal[y]
						 + normal[z] * normal[z]);
	
	normal[x] /= norm_len;
	normal[y] /= norm_len;
	normal[z] /= norm_len;*/

}

/* Convert camera and focus into spherical coordinates,
 * then operate on them to achieve rotation.
 */
void camera_move()
{
    if (global_flags["follow car"])
    {
        checkpoint = path_points[path_pos];
        
    }
    else if (global_flags["update camera"])
    {
        camera[X] += x_trans;
        camera[Z] += z_trans;
        focus[X] += x_trans;
        focus[Z] += z_trans;
        x_trans = 0.0f; z_trans = 0.0f;
        global_flags["update camera"] = false;
    }
}


void render()
{    
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   //default "empty"/background color is set to white
    glClearColor(0.75, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);           //clears color values of framebuffer
   
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity();                                             //initializes projection matrix with identity
    gluPerspective(90,1,0.1,100);  //set up projection mode (field of view, aspect ratio, near and far clipping plane)
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();                       //initializes modelview matrix with identity

	//handle translating here
    camera_move();
    gluLookAt(
    	camera[X], camera[Y], camera[Z],
		focus[X],  focus[Y],  focus[Z],
		up[X],     up[Y],     up[Z]);  

	//glPushMatrix();
	//glUseProgram(prog);
	//rotate the vase
	//glRotatef(rot_y, 1, 0, 0);
	//glRotatef(rot_x, 0, 1, 0);
	//actually draw the mesh
	//drawMesh();
	//glUseProgram(0);
	//glPopMatrix();

	glPushMatrix();
	glRotatef(rot_y, 1, 0, 0);
	glRotatef(rot_x, 0, 1, 0);

	glPushMatrix();
	glScaled(0.002, 0.002, 0.002);
	drawStructure(building_1, building_1_length);
	drawStructure(building_2, building_2_length);
	drawStructure(building_3, building_3_length);
	drawStructure(building_4, building_4_length);
	drawStructure(building_5, building_5_length);
	drawStructure(building_6, building_6_length);
	drawStructure(building_7a, building_7a_length);
	drawStructure(building_7b, building_7b_length);
	drawStructure(building_8, building_8_length);
	drawStructure(building_9a, building_9a_length);
	drawStructure(building_9b, building_9b_length);
	//drawStructure(building_10, building_10_length);
	drawStructure(building_11, building_11_length);
	drawStructure(building_12, building_12_length);
	drawStructure(roads, roads_length);
	drawStructure(misc_ground, misc_ground_length);
	drawStructure(patch, patch_length);
    glPushMatrix();
    glTranslatef(-350, 0, 0);
    drawStructure(car_points, car_points_length);
    glPopMatrix();
    glColor4f(0.7, 0.5, 0.3, 1.0);
    glBegin(GL_LINE_STRIP); // Change to loop when you're done.
        // Up to end of first straightaway.
        glVertex3f(-230, 0, 0);
        glVertex3f(-230, 0, 800);
        // Starting to turn. 850 comes just to the intersection edge.
        glVertex3f(-210, 0, 900);
        glVertex3f(-150, 0, 950);
        glVertex3f(-120, 0, 950);
        /* control points for first Bezier curve.*/
        glVertex3f(950, 0, 950);
        glVertex3f(1000, 0, 900);
        glVertex3f(1000, 0, -395);
        glVertex3f(950, 0, -450);
        /* controls for second Bezier curve. */
        glVertex3f(-170, 0, -450);
        glVertex3f(-230, 0, -400);
        glVertex3f(-230, 0, 0);
        /* controls for third Bezier curve. */
    glEnd();
	glPopMatrix();
	
    // Drawing in the guide lines.
	glScaled(100, 100, 100);
	glColor4f(1.0, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex3f(-1, 0, 0);
	glVertex3f(1, 0, 0);
	glEnd();

	glColor4f(0.0, 1, 0, 1);
	glBegin(GL_LINES);
	glVertex3f(0, -1, 0);
	glVertex3f(0, 1, 0);
	glEnd();

	glColor4f(0.0, 0, 1, 1);
	glBegin(GL_LINES);
	glVertex3f(0, 0, -1);
	glVertex3f(0, 0, 1);
	glEnd();

    //glColor4f(0.3, 0.2, 0.5, 1);
    float inters[] = {
        1056, -464, 73,
        -192, -464, 73,
        -192, 960, 73,
        1056, 960, 73
    };

	glPopMatrix();
 

	glFlush();
	glutSwapBuffers();
 
}

/*
 * Mouse motion catcher
 */

void mouseMotion(int x, int y)
{
	//if the left mouse button is held down, rotate by the difference between (x, y when moving in a new direction) and cur(x, y)
	if (global_flags["mouse down"])
	{
		int diff_x = prev_x - x; //negative = going right
		int diff_y = prev_y - y; //negative = going up

		//y_up, x_right = were we moving in that direction previously?
		
		//if changing y directions, reset the initial y pos and flag the direction
		if (y_up && diff_y > 0)
		{
			y_up = false;
			cur_y = y;
		}
		else if (!y_up && diff_y < 0)
		{
			y_up = true;
			cur_y = y;
		}

		//if changing x directions, reset the initial x pos and flag the direction
		if (x_right && diff_x > 0)
		{
			x_right = false;
			cur_x = x;
		}
		else if (!x_right && diff_x < 0)
		{
			x_right = true;
			cur_x = x;
		}

		rot_x -= (cur_x - x) * 0.005; //originally 0.005
		rot_y -= (cur_y - y) * 0.005;

		prev_x = x;
		prev_y = y;

		glutPostRedisplay();
	}
}

//handles mouse presses
void mouse(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)   
    {
        global_flags["mouse down"] = true;
		cur_x = x;
		cur_y = y;
		prev_x = x;
		prev_y = y;
    }
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)   
    {
        global_flags["mouse down"] = false;
    }
}

/*
 * Key catcher.
 */
void keyUp(unsigned char key, int, int) 
{  
    if(key == 'q'       //"q"
    || (int)key == 27) //escape key
    { 
        stop(); //quit the main loop, exit prog
    } 
	if(key == 'w')
	{
		z_trans = -0.1;
        global_flags["update camera"] = true;
	}
	if(key == 'd')
	{
		x_trans = 0.1;
        global_flags["update camera"] = true;
	}
	if(key == 's')
	{
		z_trans = 0.1;
        global_flags["update camera"] = true;
	}
	if(key == 'a')
	{
		x_trans = -0.1;
        global_flags["update camera"] = true;
	}
    if (key == 'c')
    {
        global_flags["follow car"] = !global_flags["follow car"];
        checkpoint = path_points[path_pos];
    }
}


/*
 * Updates the image even when idle
 */
void update()
{
    glutPostRedisplay(); //marks window for redrawing
}


//from Demo4
//cleans up and deletes the program

void stop()
{
    glDetachShader(prog, vert_shader);
    glDetachShader(prog, frag_shader);
    
    glDeleteProgram(prog);
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
	//delete meshPoints;
    
    exit(0);
}

//normalizes our control points, not normals!!! As odd as it sounds....
void normalize(const float coords[][3][3], float normalized[][3][3], int quadCount)
{
	//max = [1536 1536 646.4], min = [-1536 -1536 0]
	const int MAX_X = 1536;
	const int MAX_Y = 1536;
	const int MAX_Z = 646.5;

	const int MIN_X = -1536;
	const int MIN_Y = -1536;
	const int MIN_Z = 0;

	const float RANGE_X = MAX_X - MIN_X;
	const float RANGE_Y = MAX_Y - MIN_Y;
	const float RANGE_Z = MAX_Z - MIN_Z;

	const float MIDPOINT_X = (MAX_X - MIN_X) / 2;
	const float MIDPOINT_Y = (MAX_Y - MIN_Y) / 2;
	const float MIDPOINT_Z = (MAX_Z - MIN_Z) / 2;

    
    for (int i = 0; i<quadCount; i++)
    {
		for (int j = 0; j < 3; j++)
		{


			normalized[i][j][X] = coords[i][j][X];
			normalized[i][j][Y] = coords[i][j][Y];
			normalized[i][j][Z] = coords[i][j][Z];



			/*normalized[i][j][0] = (coords[i][j][0] - MIN_X) / RANGE_X;
			if (normalized[i][j][0] > 1 || normalized[i][j][0] <= 0)
			{
				cout << "OH GOD";
			}

			normalized[i][j][1] = (coords[i][j][1] - MIN_Y) / RANGE_Y;
			if (normalized[i][j][1] > 1 || normalized[i][j][1] <= 0)
			{
				cout << "OH GOD";
			}

			normalized[i][j][2] = (coords[i][j][2] - MIN_Z) / RANGE_Z;
			if (normalized[i][j][2] > 1 || normalized[i][j][2] <= 0)
			{
				cout << "OH GOD";
			}*/


			/*normalized[i][j][0] = (MIDPOINT_X - coords[i][j][0]) / (RANGE_X / 2);
			normalized[i][j][1] = (MIDPOINT_Y - coords[i][j][1]) / (RANGE_Y / 2);
			normalized[i][j][2] = (MIDPOINT_Z - coords[i][j][2]) / (RANGE_Z / 2);*/
		}
    }
}

//loads the shaders
void loadShader()
{
    char * vs_code, * fs_code;      
    
    //read the code
    vs_code = readFile("shaders/vertexshader.vert");
    fs_code = readFile("shaders/fragmentshader.frag");
    
    vert_shader = glCreateShader(GL_VERTEX_SHADER_ARB); //creates the vertex shader
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER_ARB); //and fragment shader
    
    glShaderSource(vert_shader, 1, (const GLcharARB **)&vs_code,NULL); //attaches the shader sources...
    glShaderSource(frag_shader, 1, (const GLcharARB **)&fs_code,NULL);
    
    glCompileShader(vert_shader);    //and compile!
    glCompileShader(frag_shader);
    
    delete[] vs_code; //No one likes a memory leak. 
    delete[] fs_code; //Leeks though? Definitely good in a soup.
    
    //putting the key in the ignition
    prog = glCreateProgramObjectARB();
    
    glAttachShader(prog, vert_shader); //tells the program to use these shaders
    glAttachShader(prog, frag_shader);    
    

    glLinkProgram(prog); //turns the key
    glUseProgram(prog); //and puts the pedal to the floor
    
    pos_info = glGetAttribLocation(prog,"in_Position"); //links the position info, needed for fragment shaders
    
    glUseProgram(0); //turn off the program for now
    
    
}

/*
 * Handles Uniforms
 */
void setUniformFloat(const char *varName, float value)
{
    GLint loc = 0; //declare
    loc = glGetUniformLocation(prog, varName); //get the variable ID
    if (loc != -1)
    {
        glUniform1f(loc, value); //if valid, upload new data
    }
}

char* readFile(const char * fileName)
{
    FILE *file = fopen(fileName,"r");
    
    std::ifstream fileStream (fileName, std::ifstream::binary);
    if (fileStream) {
        fileStream.seekg(0, fileStream.end);
        int length = fileStream.tellg();
        fileStream.seekg(0, fileStream.beg);
        char *buffer = new char [length + 1];
        fileStream.read(buffer, length);
        buffer[length] = '\0';
        fclose(file);
        if (!fileStream)
        {
            return NULL;
        }
        return buffer;        
    }
    return NULL;
}
