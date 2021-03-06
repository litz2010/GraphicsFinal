#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#define FALSE 0
#define TRUE  1



void make_quad(int iv1, int iv2, int iv3, int iv4, int ic);
void make_cube();
void make_enemy();
void draw_head();
void draw_body();
void draw_arms();
void draw_legs();
int enemy_detected(GLfloat userPos[4], float enemyPos[][3]);