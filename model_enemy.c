#include "model_enemy.h"

#define BLACK   0
#define RED     1
#define YELLOW  2
#define ORANGE  3
#define GREEN   4
#define BLUE    5
#define WHITE   6
#define MAGENTA 7
#define CYAN    8
#define GREY    9

GLfloat colors1 [][3] = {
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

GLfloat vertices[][3] = {
	{-1,1,1}, {1,1,1}, {1,1,-1}, {-1,1,-1},
	{-1,-1,1}, {1,-1,1}, {1,-1,-1}, {-1,-1,-1}
};


GLfloat enemy_pos[][3] = {{0,0,1}};

void make_quad(int iv1, int iv2, int iv3, int iv4, int ic) {
  glBegin(GL_POLYGON); 
  {
    glColor3fv(colors1[ic]);
    glVertex3fv(vertices[iv1]);
    glVertex3fv(vertices[iv2]);
    glVertex3fv(vertices[iv3]);
    glVertex3fv(vertices[iv4]);
  }
  glEnd();
}

/* defines a cube */
void make_cube()
{
	make_quad(4,5,1,0,BLUE);
	make_quad(5,6,2,1,BLUE);
	make_quad(3,2,6,7,BLUE);
	make_quad(0,3,7,4,BLUE);
	make_quad(4,7,6,5,BLUE);
	make_quad(0,1,2,3,BLUE);

}
//spawn enemy at starting x and z
void make_enemy()
{

	glPushMatrix();
	{
		
		glTranslatef(0,1,0);
		glScalef(.25,.25,.25);
		draw_head();
		glTranslatef(0,-4,0);
		draw_body();
		draw_legs();
		glTranslatef(0,3.5,0);
		draw_arms();		
	}
	glPopMatrix();

}

void draw_head()
{
	glPushMatrix();
	{
		make_quad(4,5,1,0,RED);//face
		make_quad(5,6,2,1,BLUE);
		make_quad(3,2,6,7,BLUE);
		make_quad(0,3,7,4,BLUE);
		make_quad(4,7,6,5,BLUE);
		make_quad(0,1,2,3,BLUE);
	}
	glPopMatrix();
}

void draw_body()
{
	glPushMatrix();
	{	
		glScalef(2,3,1);
		make_cube();
	}
	glPopMatrix();
}

void draw_arms()
{
	glPushMatrix();
	{	
		glRotatef(45, 0,0,1);
		glScalef(2,.25,.95);
		glTranslatef(-1.5,0,0);		
		make_cube();
	}
	glPopMatrix();
	glPushMatrix();
	{	
		glRotatef(-45, 0,0,1);
		glScalef(2,.25,.95);
		glTranslatef(1.5,0,0);		
		make_cube();
	}
	glPopMatrix();
}

void draw_legs()
{
	glPushMatrix();
	{	
		glScalef(.66,2.25,.95);
		glTranslatef(-1.25,-2,0);
		make_cube();
		glTranslatef(2.5, 0, 0);
		make_cube();
	}
	glPopMatrix();
}

int enemy_detected()
{
	//temp vars
	GLfloat light0_pos[] = {5, 2, 2, 0};
	GLfloat light0_dir[] = {0, 0, 1};
	float maze[900][3];
	float enemy_pos[3][3];

		float sightBoundLong;
	float sightBoundWide[2];

	int i, numEnemies;

	numEnemies = sizeof(enemy_pos)/sizeof(enemy_pos[0]);	

	for(i=0;i<numEnemies;i++)
	{
		//light0_pos[0][2];
		if(enemy_pos[i][2] == 1)//enemy facing left
		{
			sightBoundLong = enemy_pos[i][0]+30;
			sightBoundWide[0] = enemy_pos[i][1]-5;
			sightBoundWide[1] = enemy_pos[i][1]+5;
			if(light0_pos[0] >= enemy_pos[i][0] && light0_pos[0] <= sightBoundLong)
			{
				
			}
		}
		else if(enemy_pos[i][2] == 2)//enemy facing right
		{
			sightBoundLong = enemy_pos[i][0]-30;
			if(light0_pos[0] >= enemy_pos[i][0] && light0_pos[0] <= sightBoundLong)
			{
				
			}
		}
		else if(enemy_pos[i][2] == 3)//enemy facing close
		{
			sightBoundLong = enemy_pos[i][2]+30;
			if(light0_pos[0] >= enemy_pos[i][0] && light0_pos[0] <= sightBoundLong)
			{
				
			}
		}
		else if(enemy_pos[i][2] == 4)//enemy facing far
		{
			sightBoundLong = enemy_pos[i][2]-30;
			if(light0_pos[0] >= enemy_pos[i][0] && light0_pos[0] <= sightBoundLong)
			{
				
			}
		}

	}
	return 0;
}
