#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "maze_generator.h"

int detect_collision(float maze[][3], float user[7], int worldX, int row_change);