#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif



void make_quad(int iv1, int iv2, int iv3, int iv4, int ic);
void make_cube();
void make_enemy(GLfloat x, GLfloat z);
void draw_head();
void draw_body();
void draw_arms();
void draw_legs();
int enemy_detected();