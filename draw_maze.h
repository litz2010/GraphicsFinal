#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "rectangle_generator.h"

void draw_maze_block(float *info, int height, int maze_x, int entry);