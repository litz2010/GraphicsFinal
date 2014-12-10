#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void generate_maze(float maze1[][3], float info[3][3], int x_length, int z_length);
int adjacent_block_layout_checker(float maze[][3], int entry, int left, int right, int close, int far1);
int layout_possibility_checker(float maze[][3], int entry, int e_left, int e_right, int e_close, int e_far1);