
#if defined(__APPLE__)
#include <GLUT/glut.h>
#define min(a,b) ((a) < (b)? a:b)
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define FALSE 0
#define TRUE  1
#define smallWidth      256
#define largeWidth     1024
#define NUM_TEXTURES 3
#define ROOFIMG_TEX  2
#define WALLIMG_TEX  0
#define SOUPIMG_TEX  1

//static GLuint  tex_names[NUM_TEXTURES];
//static GLubyte wall_img[smallWidth*smallWidth*3] ;
static GLubyte soup_img[smallWidth*smallWidth*3] ;

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
void load_bmp(char *fname, GLubyte img[], int size, GLuint *ptname);


extern GLuint tex_names[2];
extern GLubyte wall_img[786432];
extern GLubyte barrel_img[512*512*3];
extern int FLASH_COLOR;