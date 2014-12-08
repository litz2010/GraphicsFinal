
#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define FALSE 0
#define TRUE  1

void glut_setup(void);
void gl_setup(void);
void my_setup(void);
void my_display(void);
void my_reshape(int w, int h);
void lighting_setup();
void my_setup(void);
void texture_setup(void);
void my_timer(int val);
void NPC_timer(int val); 

