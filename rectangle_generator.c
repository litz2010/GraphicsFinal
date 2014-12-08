
#include "rectangle_generator.h"

GLfloat colors [][3] = {
	{0.0, 0.0, 0.0},  /* black   */
	{1.0, 0.0, 0.0},  /* red     */
	{1.0, 1.0, 0.0},  /* yellow  */
	{1.0, 0.5, 0.0},  /* orange  */
	{0.0, 1.0, 0.0},  /* green   */
	{0.0, 0.0, 1.0},  /* blue    */
	{1.0, 1.0, 1.0},   /* white   */
	{1.0, 0.0, 1.0},  /* magenta */
	{0.0, 1.0, 1.0},  /* cyan    */
	{0.5, 0.5, 0.5}  /* 50%grey */
};


void make_rectangle_textured(GLfloat *iv1, GLfloat *iv2, GLfloat *iv3, GLfloat *iv4, GLfloat *color) {
    glBegin(GL_POLYGON);
    {
        glColor3fv(color);
      //  glTexCoord2f(0,0);
        glVertex3fv(iv1);
     //   glTexCoord2f(0,1);
        glVertex3fv(iv2);
     //   glTexCoord2f(1,1);
        glVertex3fv(iv3);
      //  glTexCoord2f(1,0);
        glVertex3fv(iv4);
    }
    glEnd();
}

/* defines floor */
void make_floor(int x, int z)
{
    GLfloat checkedX;
    GLfloat checkedZ;
    checkedX = x;
    checkedZ = z;
    GLfloat dimensions[][3] = {
        {0.0, 0.0, 0.0},
        {0.0, 0.0, checkedZ},
        {checkedX, 0.0, checkedZ},
        {checkedX, 0.0, 0.0}
    };
    //a grey plane
    make_rectangle_textured(dimensions[0],dimensions[1],dimensions[2],dimensions[3], colors[4]);
}