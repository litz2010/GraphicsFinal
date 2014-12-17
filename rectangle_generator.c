
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

void find_normal(GLfloat *iv1, GLfloat*iv2, GLfloat *iv3, GLfloat *destination) {
    GLfloat Ux = iv2[0] - iv1[0];
    GLfloat Uy = iv2[1] - iv1[1];
    GLfloat Uz = iv2[2] - iv1[2];
    
    GLfloat Vx = iv3[0] - iv1[0];
    GLfloat Vy = iv3[1] - iv1[1];
    GLfloat Vz = iv3[2] - iv1[2];
    
    destination[0] = Uy*Vz - Uz-Vy;
    destination[1] = Uz*Vx - Ux*Vz;
    destination[2] = Ux*Vy - Uy*Vx;
    
    return;
}

void make_rectangle_textured(GLfloat *iv1, GLfloat *iv2, GLfloat *iv3, GLfloat *iv4, GLfloat *color) {
   
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, tex_names[1]);
    
    GLfloat dest[3];
    find_normal(iv2,iv1,iv3,dest);
    glBegin(GL_POLYGON);
    {
        glColor3fv(color);
        glTexCoord2f(0,0);
        glNormal3f(dest[0], dest[1], dest[2]);
        glVertex3fv(iv1);
        glTexCoord2f(0,1);
        glNormal3f(dest[0], dest[1], dest[2]);
        glVertex3fv(iv2);
        glTexCoord2f(1,1);
        glNormal3f(dest[0], dest[1], dest[2]);
        glVertex3fv(iv3);
        glTexCoord2f(1,0);
        glNormal3f(dest[0], dest[1], dest[2]);
        glVertex3fv(iv4);
    }
    glEnd();
    glLightModelf(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
    glDisable(GL_TEXTURE_2D);
    
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