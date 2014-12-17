#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "setup.h"

/* generates the floor for the project */
void make_floor(int x, int z);
/* makes a rectnagle when given 4 points in counter clockwise order */
void make_rectangle_textured(GLfloat *iv1, GLfloat *iv2, GLfloat *iv3, GLfloat *iv4, GLfloat *color);

