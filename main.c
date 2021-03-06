/*
 cs1566 final project
 */
#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES // for C
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "rectangle_generator.h"
#include "draw_maze.h"
#include "maze_generator.h"
#include "setup.h"
#include "enemy_movement.h"
#include "collision_detection.h"
#include "model_enemy.h"

#define FALSE 0
#define TRUE  1
#define NUM_ENEMIES 3

#define DELTA_DEG  10
#define DELTA_TIME 50
#define WORLD_X 10
#define WORLD_Z 10


void glut_setup(void);
void my_keyboard( unsigned char key, int x, int y );
void my_display(void);
void draw_barrel(int index);	//delete~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int enemy_detected1();
void keyOperations(void);
void keyPressed(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void checkExited();

int keyStates2[256];// = new int[256]; // Create an array of boolean values of length 256 (0-255)
int step = 0;
int light0_theta = 0;
int viewPerspective = 0; //0 = 3rd person, 1 = first person
GLfloat light0_pos[] = { 5, 2, 2, 1 };
GLfloat light0_draw[] = { 0, 1, 0, 1 };
GLfloat light0_dir[] = { 0, -.2, 1 };

float maze[2500][3];
float enemy_pos[3][3];
float user_pos[3]; //not actual position. light0_pos is the user
int move_count = 0;
float timer = 0;

int numLives = 3;
int detectionCounter = 0;

int main(int argc, char **argv) {
	
    int jj = sizeof(maze) / sizeof(maze[0]);
    for(int j = 0; j<jj; j++){
        maze[j][0] = -1; //give all maze blocks a layout of -1
		maze[j][1] = .5; //maze color
    }
	
    /* General initialization for GLUT and OpenGL
     Must be called first */
    glutInit( &argc, argv ) ;
    
    /* we define these setup procedures */
    glut_setup() ;
    gl_setup() ;
    my_setup();
    generate_maze(maze, enemy_pos, WORLD_X, WORLD_Z);
	glutTimerFunc(2000, NPC_timer, 0);
    /* go into the main event loop */
    glutMainLoop() ;
    return(0) ;
}


/* This function sets up the windowing related glut calls */
void glut_setup(void) {
    
    /* specify display mode -- here we ask for a double buffer and RGB coloring */
    /* NEW: tell display we care about depth now */
    glutInitDisplayMode (GLUT_DOUBLE |GLUT_RGB |GLUT_DEPTH);
    
    glutInitWindowSize(800,800);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Maze Generation");
    
    /*initialize callback functions */
    glutDisplayFunc( my_display );
    glutReshapeFunc( my_reshape );
	glutKeyboardFunc(my_keyboard);
	return;
}

void my_display(void) {
	GLfloat colors[][3] = {
		{ 0, 0, 1 }
	};
    int xx, zz, block, i, j, radius = 60, entries, k, add_entry = 0;
    double temp_x, temp_z;
    double temp_theta = 0;
    int draw_entries[200];

	GLUquadricObj *qojb;
	qojb = gluNewQuadric();
	/* clear the buffer */
	/* NEW: now we have to clear depth as well */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
    if (FLASH_COLOR==1) {
        GLfloat light_color[] = {247.0f,255.0f,1.0f,1.0f};
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_color);
        //FLASH_COLOR=0;
        
    }
    else if (FLASH_COLOR==2) {
        GLfloat light_color[] = {255.0f,0.0f,0.0f,1.0f};
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_color);
        //FLASH_COLOR=0;
        
    }
    else {
        GLfloat light_color[] = {1.2f,1.2f,1.2f,1.0f};
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_color);
    }

	if (viewPerspective){ //first person
		gluLookAt(light0_pos[0], light0_pos[1], light0_pos[2],
			light0_pos[0] + sin(M_PI * light0_theta / 180), light0_pos[1], light0_pos[2] + cos(M_PI * light0_theta / 180),
			0.0, 1.0, 0.0);
	}
	else{
		gluLookAt(light0_pos[0], 100.0, light0_pos[2] - 20,  // x,y,z coord of the camera
			light0_pos[0], 2.0, light0_pos[2],  // x,y,z coord of the origin
			0.0, 1.0, 0.0); // the direction of up (default is y-axis)
	}
	make_floor(WORLD_X, WORLD_Z);

    //draw maze, 100 blocks surrounding the user
    xx = (int)light0_pos[0] / 10;
    zz = (int)light0_pos[2] / 10;
    block = zz * WORLD_X + xx;
    block = block + 5; //5 blocks left.
    block = block + 5 * WORLD_X;
 
    for(j = 0; j < 10; j++){
        for(i = 0; i < 10; i++){
            if(maze[block][0] != -1){
                if(block / WORLD_X < WORLD_Z){
                    draw_maze_block(maze[block], 10, WORLD_X, block);
                    if (maze[block][2] == 1)
                        draw_barrel(block);
                }
            }
            block = block - 1;
            if(block < 0)
                block = 0;
        }
        block = block - WORLD_X;
        block = block +10;
        if(block < 0)
            block = 0;
    }

/*
    attempt to do fov,
 
    xx = (int)light0_pos[0] / 10; //make sure these are set
    zz = (int)light0_pos[2] / 10;
    entries = 0;
    for(j = 0; j < 100; j++){
        draw_entries[j] = -1;
    }
    temp_theta = light0_theta;
//    if(light0_theta < 0)
//        temp_theta = temp_theta + 360;
    temp_theta = temp_theta + 10;
    
    for(j =0; j < 12; j++){ //radius / 10

        for(i = 0 ; i < 20; i++){
            add_entry = 1;
            temp_x = light0_pos[0] + radius * cos(temp_theta);
            temp_z = light0_pos[2] + radius * sin(temp_theta);
            if( ! (((temp_x / 10) >= WORLD_X) || (temp_z / WORLD_X >= WORLD_Z))){ //not a real block
                block = ((int)temp_z / 10) * WORLD_X + ((int)temp_x / 10);
                temp_theta = temp_theta - .5;
                //check if block is already in the array.
                for(k = 0; k < entries; k++){
                    if(draw_entries[k] == block){
                        add_entry = 0;
                        break;
                    }
                }
                if(add_entry && block > -1 && block < WORLD_X * WORLD_Z){
                    draw_entries[entries] = block;
                    entries++;
                }
            }
        }
 
        radius = radius - 5;
        temp_theta = temp_theta + 10;
    }
    //draw maze
    for(j = 0; j < entries; j++){
        block = draw_entries[j];
        if(maze[block][0] != -1){
            if(block / WORLD_X < WORLD_Z){
                draw_maze_block(maze[block], 10, WORLD_X, block);
                if (maze[block][2] == 1)
                    draw_barrel(block);
            }
        }
    }
 */
    glPushMatrix();
	{
		glTranslatef(light0_pos[0], light0_pos[1], light0_pos[2]);
		glRotatef(light0_theta, 0, 1, 0);
		glLightfv(GL_LIGHT0, GL_POSITION, light0_draw);
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_dir);
		glColor3fv(colors[0]);
		gluCylinder(qojb, .2f, .4f, 1, 32, 32);
	}
	glPopMatrix();
    
	for (int i = 0; i < 3; i++){
		glPushMatrix();
		{ //enemy

			glTranslatef(enemy_pos[i][0], 2, enemy_pos[i][1]);
			float enemyRotateAngle;
			
			if (enemy_pos[i][2] == 1)//left
			{
				enemyRotateAngle = 90;
			}
			else if (enemy_pos[i][2] == 2)//right
			{
				enemyRotateAngle = -90;
			}
			else if (enemy_pos[i][2] == 3)//close
			{
				enemyRotateAngle = 180;
			}
			else if (enemy_pos[i][2] == 4)//far1
			{
				enemyRotateAngle = 0;
			}
			glRotatef(enemyRotateAngle,0,1,0);
			make_enemy();			
		}
		glPopMatrix();
	}
	
	//int isDetected = enemy_detected(light0_pos, enemy_pos[3]);
	int isDetected = enemy_detected1();

	if (isDetected == TRUE & detectionCounter == 0)
	{
		numLives--;
		printf("Lives remaining: %d\n", numLives);
		detectionCounter = 10;
        
        if (numLives>0) {
            FLASH_COLOR = 1;
            glutPostRedisplay();
            glutTimerFunc(1000,my_timer,0);
        }
        else {
            FLASH_COLOR = 2;
            glutPostRedisplay();
            glutTimerFunc(1000,my_timer,0);
        }
	}

	if (numLives == 0)
	{
		//return back to beginning of maze;
		//numLives = 3
		numLives = 3;
		light0_theta = 0;
		light0_pos[0] = 5;
		light0_pos[1] = 2;
		light0_pos[2] = 2;
		light0_pos[3] = 1;
        
	}
    checkExited();
    /* buffer is ready */
    glutSwapBuffers();
}

void my_keyboard(unsigned char key, int x, int y) {
	int xx, zz, block;
	xx = 0;
	GLfloat globalAmb[] = { 0.5, 0.5, 0.5, 1 };
	GLfloat globalAmb2[] = { 0, 0, 0, 1 };
	switch (key) {
	case 's':
	case 'S':
		user_pos[0] = light0_pos[0] - sin(M_PI * light0_theta / 180); //x projected
		user_pos[1] = light0_pos[2] - cos(M_PI * light0_theta / 180); //z projected
		if ((int)user_pos[1] / WORLD_X != ((int)light0_pos[2] / WORLD_X))
			xx = 1; 
		if (detect_collision(maze, user_pos, WORLD_X, xx)){
			light0_pos[0] -= sin(M_PI * light0_theta / 180); //user real postion
			light0_pos[2] -= cos(M_PI * light0_theta / 180);
			glutPostRedisplay();
		}
		break;
	case 'w':
	case 'W':
		user_pos[0] = light0_pos[0] + sin(M_PI * light0_theta / 180);
		user_pos[1] = light0_pos[2] + cos(M_PI * light0_theta / 180);
		if ((int)user_pos[0] / WORLD_X != ((int)light0_pos[0] / WORLD_X))
			xx = 1;
		if (detect_collision(maze, user_pos, WORLD_X, xx)){
			light0_pos[0] += sin(M_PI * light0_theta / 180);
			light0_pos[2] += cos(M_PI * light0_theta / 180);
			glutPostRedisplay();
		}
		break;
	case 'd':
	case 'D':
		light0_theta = (light0_theta - DELTA_DEG) % 360;
		glutPostRedisplay();
		break;
	case 'i':
	case 'I':
		xx = (int)light0_pos[0] / 10;
		zz = (int)light0_pos[2] / 10;
		block = zz * WORLD_X + xx;
		printf("block=%d layout=%f x=%f z=%f\n", block, maze[block][0], light0_pos[0], light0_pos[2]);
		break;
	case 'o':
		glEnable(GL_LIGHT0);
		glutPostRedisplay();
		break;
	case 'O':
		glDisable(GL_LIGHT0);
		glutPostRedisplay();
		break;
	case 'a':
	case 'A':
		light0_theta = (light0_theta + DELTA_DEG) % 360;
		glutPostRedisplay();
		break;
	case 'v':
	case 'V':  //viewPerspective ==1 is first person
		if (viewPerspective)
		{
			viewPerspective = 0;
			gluPerspective(80.0, 1.0, 1.0, 20.0);
		}
		else
		{
			viewPerspective = 1;
			gluPerspective(90.0, 1.0, 0, 320.0);
		}
		break;
	case 'b':
		step += 1;
		glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1 + step);
		printf("spotexp = %d\n", step + 1);
		break;
	case 'B':
		step -= 1;
		glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1 + step);
		printf("spotexp = %d\n", step + 1);
		break;
	case 'g':
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);
		break;
	case 'G':
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb2);
		break;
	case 'r':
	case 'R':
		light0_theta = 0;
		light0_pos[0] = 5;
		light0_pos[1] = 2;
		light0_pos[2] = 2;
		light0_pos[3] = 1;
		break;
	case 'q':
	case 'Q':
		exit(0);
	default: break;
	}
	glutTimerFunc(DELTA_TIME, my_timer, 0);
	return;
}

void NPC_timer(int val) {
	move_count = (move_count + 1) % 5;
	if (detectionCounter > 0)
	{
		detectionCounter--;
	}
	glutPostRedisplay();
	enemey_move(maze, enemy_pos, WORLD_X, move_count);
	glutTimerFunc(300, NPC_timer, 0);
    timer = timer + 0.3;
	return;
}

void draw_barrel(int index){
	int xx, zz; 
	xx = index % WORLD_X;
	zz = index / WORLD_X;
	xx = xx * 10 + 2.5; 
	zz = zz * 10 + 2.5; 
	glPushMatrix();
	{ 
		glTranslatef(xx, 0, zz);
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, tex_names[0]);
		make_barrel(3.2f, 32);
        glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();
}

int enemy_detected1()
{
	float sightBoundLong;
	float sightBoundWide[2];
    
	int i, returnVal;
	returnVal = FALSE;
	
	for (i = 0; i<3; i++)
	{
		if (enemy_pos[i][2] == 1)//enemy facing left
		{
			sightBoundLong = enemy_pos[i][0] + 30; //x+30
			sightBoundWide[0] = enemy_pos[i][1] - 2.5;//z-5
			sightBoundWide[1] = enemy_pos[i][1] + 2.5;//z+5
			if (light0_pos[0] >= enemy_pos[i][0] && light0_pos[0] <= sightBoundLong && light0_pos[2] >= sightBoundWide[0] && light0_pos[2] <= sightBoundWide[1])
			{
				//check for objects/wall in between player and enemy
				//get enemy's current block
				int enemyBlock = get_enemy_block(i);
				//user current block
				int userBlock = light0_block();
				//printf("enemyBlock: %d userBlock: %d\n", enemyBlock, userBlock);
				int check;
				for (int blockToCheck = enemyBlock; blockToCheck != userBlock; blockToCheck++)
				{
					check = adjacent_block_layout_checker(maze, blockToCheck, 1, 0, 0, 0);
					printf("block to check: %d check: %d\n", blockToCheck, check);
					if (check == 0)
					{
						returnVal = FALSE;
						break;
					}
					returnVal = TRUE;
				}
				/*if (userBlock == light0_block())
                 {
                 returnVal = TRUE;
                 }*/
				//returnVal = TRUE;
				//printf("Detected enemy facing left!\n");
			}
		}
		else if (enemy_pos[i][2] == 2)//enemy facing right
		{
			sightBoundLong = enemy_pos[i][0] - 30;//x-30
			sightBoundWide[0] = enemy_pos[i][1] - 2.5;//z-5
			sightBoundWide[1] = enemy_pos[i][1] + 2.5;//z+5
			if (light0_pos[0] <= enemy_pos[i][0] && light0_pos[0] >= sightBoundLong && light0_pos[2] >= sightBoundWide[0] && light0_pos[2] <= sightBoundWide[1])
			{
				//check for objects/wall in between player and enemy
				//get enemy's current block
				int enemyBlock = get_enemy_block(i);
				//user current block
				int userBlock = light0_block();
				//printf("enemyBlock: %d userBlock: %d\n", enemyBlock, userBlock);
                
				int check;
				for (int blockToCheck = enemyBlock; blockToCheck != userBlock; blockToCheck--)
				{
					check = adjacent_block_layout_checker(maze, blockToCheck, 0, 1, 0, 0);
					printf("block to check: %d check: %d\n", blockToCheck, check);
					if (check == 0)
					{
						returnVal = FALSE;
						break;
					}
					returnVal = TRUE;
				}
				/*if (userBlock == light0_block())
                 {
                 returnVal = TRUE;
                 }*/
				//returnVal = TRUE;
				//printf("Detected enemy facing right!\n");
			}
		}
		else if (enemy_pos[i][2] == 3)//enemy facing close
		{
			sightBoundLong = enemy_pos[i][1] - 30;//z-30
			sightBoundWide[0] = enemy_pos[i][0] - 2.5;//x-5
			sightBoundWide[1] = enemy_pos[i][0] + 2.5;//x+5
			if (light0_pos[2] <= enemy_pos[i][1] && light0_pos[2] >= sightBoundLong && light0_pos[0] >= sightBoundWide[0] && light0_pos[0] <= sightBoundWide[1])
			{
				//check for objects/wall in between player and enemy
				//get enemy's current block
				int enemyBlock = get_enemy_block(i);
				//user current block
				int userBlock = light0_block();
				//printf("enemyBlock: %d userBlock: %d\n", enemyBlock, userBlock);
                
				int check;
				for (int blockToCheck = enemyBlock; blockToCheck != userBlock; blockToCheck=blockToCheck-10)
				{
					check = adjacent_block_layout_checker(maze, blockToCheck, 0, 0, 1, 0);
					//printf("block to check: %d check: %d\n", blockToCheck, check);
					if (check == 0)
					{
						returnVal = FALSE;
						break;
					}
					returnVal = TRUE;
                    
				}
				/*if (userBlock == light0_block())
                 {
                 returnVal = TRUE;
                 }*/
				//returnVal = TRUE;
				//printf("Detected enemy facing close!\n");
			}
		}
		else if (enemy_pos[i][2] == 4)//enemy facing far1
		{
			sightBoundLong = enemy_pos[i][1] + 30;//z+30
			sightBoundWide[0] = enemy_pos[i][0] - 2.5;//x-5
			sightBoundWide[1] = enemy_pos[i][0] + 2.5;//x+5
			if (light0_pos[2] >= enemy_pos[i][1] && light0_pos[2] <= sightBoundLong && light0_pos[0] >= sightBoundWide[0] && light0_pos[0] <= sightBoundWide[1])
			{
				//check for objects/wall in between player and enemy
				//get enemy's current block
				int enemyBlock = get_enemy_block(i);
				//user current block
				int userBlock = light0_block();
				//printf("enemyBlock: %d userBlock: %d\n", enemyBlock, userBlock);
                
				int check;
				for (int blockToCheck = enemyBlock; blockToCheck != userBlock; blockToCheck=blockToCheck + 10)
				{
					check = adjacent_block_layout_checker(maze, blockToCheck, 0, 0, 0, 1);
					//printf("block to check: %d check: %d\n", blockToCheck, check);
					if (check == 0)
					{
						returnVal = FALSE;
						break;
					}
					returnVal = TRUE;
				}
				/*if (userBlock == light0_block())
                 {
                 returnVal = TRUE;
                 }*/
				//returnVal = TRUE;
				//printf("Detected enemy facing far!\n");
			}
		}
	}
	return returnVal;
}

//new keyboard functions
void keyPressed(unsigned char key, int x, int y) {
	printf("keydown\n");
	keyStates2[key] = 1; // Set the state of the current key to pressed
	glutPostRedisplay();
	return;
}

void keyUp(unsigned char key, int x, int y) {
	printf("keyup\n");
	keyStates2[key] = 0; // Set the state of the current key to not pressed
	keyOperations();
	glutPostRedisplay();
	return;
}

void keyOperations(void) {
	int xx, zz, block;
	xx = 0;
	zz = 0;
	GLfloat globalAmb[] = { 0.5, 0.5, 0.5, 1 };
	GLfloat globalAmb2[] = { 0, 0, 0, 1 };
	printf("keyoperation \n");
	if (keyStates2['q'] || keyStates2['Q'])
	{
		exit(0);
	}
	if (keyStates2['i'] || keyStates2['I'])
	{
		xx = (int)light0_pos[0] / 10;
		zz = (int)light0_pos[2] / 10;
		block = zz * WORLD_X + xx;
		printf("block=%d layout=%f x=%f z=%f\n", block, maze[block][0], light0_pos[0], light0_pos[2]);
	}
	if (keyStates2['w'] || keyStates2['W'])
	{
		user_pos[0] = light0_pos[0] + sin(M_PI * light0_theta / 180); //x projected
		user_pos[1] = light0_pos[2] + cos(M_PI * light0_theta / 180); //z projected
		if ((int)user_pos[0] / WORLD_X != ((int)light0_pos[0] / WORLD_X))
			xx = 1;
		if (detect_collision(maze, user_pos, WORLD_X, xx)){
			light0_pos[0] += sin(M_PI * light0_theta / 180);
			light0_pos[2] += cos(M_PI * light0_theta / 180);
			glutPostRedisplay();
		}
	}
	if (keyStates2['s'] || keyStates2['S'])
	{
		user_pos[0] = light0_pos[0] - sin(M_PI * light0_theta / 180); //x projected
		user_pos[1] = light0_pos[2] - cos(M_PI * light0_theta / 180); //z projected
		if ((int)user_pos[1] / WORLD_X != ((int)light0_pos[2] / WORLD_X))
			xx = 1;
		if (detect_collision(maze, user_pos, WORLD_X, xx)){
			light0_pos[0] -= sin(M_PI * light0_theta / 180); //user real postion
			light0_pos[2] -= cos(M_PI * light0_theta / 180);
			glutPostRedisplay();
		}
	}
	if (keyStates2['d'] || keyStates2['D'])
	{
		light0_theta = (light0_theta - DELTA_DEG) % 360;
	}
	if (keyStates2['a'] || keyStates2['A'])
	{
		light0_theta = (light0_theta + DELTA_DEG) % 360;
	}
	if (keyStates2['o'])
	{
		glEnable(GL_LIGHT0);
	}
	if (keyStates2['O'])
	{
		glDisable(GL_LIGHT0);
	}
	if (keyStates2['g'])
	{
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);
	}
	if (keyStates2['G'])
	{
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb2);
	}
	if (keyStates2['v'] || keyStates2['V'])
	{
		if (viewPerspective)
		{
			viewPerspective = 0;
			gluPerspective(80.0, 1.0, 1.0, 320.0);
		}
		else
		{
			viewPerspective = 1;
			gluPerspective(80.0, 1.0, 0, 320.0);
		}
	}
	if (keyStates2['b'])
	{
		step += 1;
		glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1 + step);
		printf("spotexp = %d\n", step + 1);
	}
	if (keyStates2['B'])
	{
		step -= 1;
		glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1 + step);
		printf("spotexp = %d\n", step + 1);
	}
	return;
}

int get_enemy_block(int i)
{
	int xx, zz, block;
	xx = (int)enemy_pos[i][0] / 10;
	zz = (int)enemy_pos[i][1] / 10;
	block = zz * WORLD_X + xx;
	return block;
}

int light0_block()
{
	int xx, zz, block;
	xx = (int)light0_pos[0] / 10;
	zz = (int)light0_pos[2] / 10;
	block = zz * WORLD_X + xx;
	return block;
}


void checkExited()
{
	if (light0_pos[0] / 10 >= WORLD_X || light0_pos[2] / 10 >= WORLD_Z)
	{
		printf("You have exited the maze! It took you: %lf seconds \n", timer);
        light0_theta = 0;
        light0_pos[0] = 5;
        light0_pos[1] = 2;
        light0_pos[2] = 2;
        light0_pos[3] = 1;
        timer=0;
	/*	int jj = WORLD_X * WORLD_Z;
		for (int j = 0; j<jj; j++){
			maze[j][0] = -1; //give all maze blocks a layout of -1
			//maze[j][1] = .5; //maze color
		}
        //return back to beginning of maze;
		numLives = 3;
		light0_theta = 0;
		light0_pos[0] = 5;
		light0_pos[1] = 2;
		light0_pos[2] = 2;
		light0_pos[3] = 1;
        generate_maze(maze, enemy_pos, WORLD_X, WORLD_Z);
        glutTimerFunc(2000, NPC_timer, 0);
     */
	}	
}