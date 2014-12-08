#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "maze_generator.h"

#define NUM_ENEMY 3
#define MOV_DISTANCE 2.5

/*
	info[][0] = x coord
	info[][1] = z coord
	info[][2] = direction traveling... 1 = left, 2 = right, 3 = close, 4 = far
*/
void enemey_move(float maze[][3], float info[3][3], int worldX, int count);