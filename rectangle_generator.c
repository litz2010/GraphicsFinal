
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
    int i, j;
    checkedX = 0;
    checkedZ = 0;
    GLfloat dimensions[4][3] = {
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0}
    };
    for(i = 0; i < z; i++){
        for(j = 0; j < x; j++){
            dimensions[0][0] = checkedX;
            dimensions[0][2] = checkedZ;
            dimensions[1][0] = checkedX + 10;
            dimensions[1][2] = checkedZ;
            dimensions[2][0] = checkedX + 10;
            dimensions[2][2] = checkedZ + 10;
            dimensions[3][0] = checkedX;
            dimensions[3][2] = checkedZ + 10;
            //a green plane
            make_rectangle_textured(dimensions[0],dimensions[1],dimensions[2],dimensions[3], colors[4]);
            checkedX = checkedX + 10;
        }
        checkedZ = checkedZ + 10;
        checkedX = 0; 
    }
    
}