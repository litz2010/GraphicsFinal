
#include "draw_maze.h"

void draw_maze_block(float *info, int height, int maze_x, int entry){
//    GLfloat mat_emission[]  = {info[3],info[4],info[5], info[6]};
    GLfloat mat_amb[]  = {info[1],info[1],info[2],info[2]};
  //  GLfloat mat_diff[] = {info[11],info[12],info[13],info[14],info[15]};
   // GLfloat mat_specular[]  = {info[16],info[17],info[18],info[19]};
    //GLfloat mat_shininess[] = {info[2]};
    
    int close   =0; // draw wall closest to orgin?
    int far     =0; // draw wall farest from orgin?
    int left    =0; // draw wall +x to orgin?
    int right   =0; // draw wall -x from orgin?
    GLfloat a[3];
    GLfloat b[3];
    GLfloat c[3];
    GLfloat d[3];
    int x = entry % maze_x;
    int z = entry / maze_x;
    x = x*10; //world coordinates of block
    z = z*10; //world coordinates of block
    
    //test to see the layout
    int wall_layout = info[0];
    switch( wall_layout ){
        case 0:
            close = 0;
            far   = 0;
            left  = 0;
            right = 0;
            break;
        case 1:
            close = 1;
            far   = 0;
            left  = 0;
            right = 0;
            break;
        case 2:
            close = 0;
            far   = 0;
            left  = 1;
            right = 0;
            break;
        case 3:
            close = 0;
            far   = 1;
            left  = 0;
            right = 0;
            break;
        case 4:
            close = 0;
            far   = 0;
            left  = 0;
            right = 1;
            break;
        case 5:
            close = 1;
            far   = 0;
            left  = 1;
            right = 0;
            break;
        case 6:
            close = 1;
            far   = 1;
            left  = 0;
            right = 0;
            break;
        case 7:
            close = 1;
            far   = 0;
            left  = 0;
            right = 1;
            break;
        case 8:
            close = 0;
            far   = 0;
            left  = 1;
            right = 1;
            break;
        case 9:
            close = 0;
            far   = 1;
            left  = 1;
            right = 0;
            break;
        case 10:
            close = 0;
            far   = 1;
            left  = 0;
            right = 1;
            break;
        case 11:
            close = 0;
            far   = 1;
            left  = 1;
            right = 1;
            break;
        case 12:
            close = 1;
            far   = 1;
            left  = 0;
            right = 1;
            break;
        case 13:
            close = 1;
            far   = 0;
            left  = 1;
            right = 1;
            break;
        case 14:
            close = 1;
            far   = 1;
            left  = 1;
            right = 0;
            break;
        case 15:
            close = 1;
            far   = 1;
            left  = 1;
            right = 1;
            break;
    }
    if( right){
        //draw the outer wall
        a[0] = x;
        a[1] = 0;
        a[2] = z;
        b[0] = x;
        b[1] = 0;
        b[2] = z + 10;
        c[0] = x;
        c[1] = height;
        c[2] = z + 10;
        d[0] = x;
        d[1] = height;
        d[2] = z;
        make_rectangle_textured(a, b, c, d, mat_amb);
        //draw the inner wall
        a[0] = x + 1;
        a[1] = 0;
        a[2] = z;
        b[0] = x + 1;
        b[1] = 0;
        b[2] = z + 10;
        c[0] = x + 1;
        c[1] = height;
        c[2] = z + 10;
        d[0] = x + 1;
        d[1] = height;
        d[2] = z;
        make_rectangle_textured(a, b, c, d, mat_amb);
        //draw top of wall
        a[0] = x;
        a[1] = height;
        a[2] = z;
        b[0] = x;
        b[1] = height;
        b[2] = z + 10;
        c[0] = x + 1;
        c[1] = height;
        c[2] = z + 10;
        d[0] = x + 1;
        d[1] = height;
        d[2] = z;
        make_rectangle_textured(a, b, c, d, mat_amb);
        //draw close wall
        a[0] = x;
        a[1] = 0;
        a[2] = z;
        b[0] = x + 1;
        b[1] = 0;
        b[2] = z;
        c[0] = x + 1;
        c[1] = height;
        c[2] = z;
        d[0] = x;
        d[1] = height;
        d[2] = z;
        make_rectangle_textured(a, b, c, d, mat_amb);
        //draw far wall
        a[0] = x;
        a[1] = 0;
        a[2] = z + 10;
        b[0] = x + 1;
        b[1] = 0;
        b[2] = z + 10;
        c[0] = x + 1;
        c[1] = height;
        c[2] = z + 10;
        d[0] = x;
        d[1] = height;
        d[2] = z + 10;
        make_rectangle_textured(a, b, c, d, mat_amb);
    }
    if (left){
        //draw the outer wall
        a[0] = x + 10;
        a[1] = 0;
        a[2] = z;
        b[0] = x + 10;
        b[1] = 0;
        b[2] = z + 10;
        c[0] = x + 10;
        c[1] = height;
        c[2] = z + 10;
        d[0] = x + 10;
        d[1] = height;
        d[2] = z;
        make_rectangle_textured(a, b, c, d, mat_amb);
        //draw the inner wall
        a[0] = x + 9;
        a[1] = 0;
        a[2] = z;
        b[0] = x + 9;
        b[1] = 0;
        b[2] = z + 10;
        c[0] = x + 9;
        c[1] = height;
        c[2] = z + 10;
        d[0] = x + 9;
        d[1] = height;
        d[2] = z;
        make_rectangle_textured(a, b, c, d, mat_amb);
        //draw top of wall
        a[0] = x + 10;
        a[1] = height;
        a[2] = z;
        b[0] = x + 10;
        b[1] = height;
        b[2] = z + 10;
        c[0] = x + 9;
        c[1] = height;
        c[2] = z + 10;
        d[0] = x + 9;
        d[1] = height;
        d[2] = z;
        make_rectangle_textured(a, b, c, d, mat_amb);
        //draw close wall
        a[0] = x + 9;
        a[1] = 0;
        a[2] = z;
        b[0] = x + 10;
        b[1] = 0;
        b[2] = z;
        c[0] = x + 10;
        c[1] = height;
        c[2] = z;
        d[0] = x + 9;
        d[1] = height;
        d[2] = z;
        make_rectangle_textured(a, b, c, d, mat_amb);
        //draw far wall
        a[0] = x + 9;
        a[1] = 0;
        a[2] = z + 10;
        b[0] = x + 10;
        b[1] = 0;
        b[2] = z + 10;
        c[0] = x + 10;
        c[1] = height;
        c[2] = z + 10;
        d[0] = x + 9;
        d[1] = height;
        d[2] = z + 10;
        make_rectangle_textured(a, b, c, d, mat_amb);
    }
    if(close){
        //draw the outer wall
        a[0] = x;
        a[1] = 0;
        a[2] = z;
        b[0] = x;
        b[1] = height;
        b[2] = z;
        c[0] = x + 10;
        c[1] = height;
        c[2] = z;
        d[0] = x + 10;
        d[1] = 0;
        d[2] = z;
        make_rectangle_textured(a, b, c, d, mat_amb);
        //draw the inner wall
        a[0] = x;
        a[1] = 0;
        a[2] = z + 1;
        b[0] = x;
        b[1] = height;
        b[2] = z + 1;
        c[0] = x + 10;
        c[1] = height;
        c[2] = z + 1;
        d[0] = x + 10;
        d[1] = 0;
        d[2] = z + 1;
        make_rectangle_textured(a, b, c, d, mat_amb);
        //draw top of wall
        a[0] = x;
        a[1] = height;
        a[2] = z;
        b[0] = x;
        b[1] = height;
        b[2] = z + 1;
        c[0] = x + 10;
        c[1] = height;
        c[2] = z + 1;
        d[0] = x + 10;
        d[1] = height;
        d[2] = z;
        make_rectangle_textured(a, b, c, d, mat_amb);
        //draw left wall
        a[0] = x + 10;
        a[1] = 0;
        a[2] = z;
        b[0] = x + 10;
        b[1] = 0;
        b[2] = z + 1;
        c[0] = x + 10;
        c[1] = height;
        c[2] = z + 1;
        d[0] = x + 10;
        d[1] = height;
        d[2] = z;
        make_rectangle_textured(a, b, c, d, mat_amb);
        //draw right wall
        a[0] = x;
        a[1] = 0;
        a[2] = z;
        b[0] = x;
        b[1] = 0;
        b[2] = z + 1;
        c[0] = x;
        c[1] = height;
        c[2] = z + 1;
        d[0] = x;
        d[1] = height;
        d[2] = z;
        make_rectangle_textured(a, b, c, d, mat_amb);
    }
    if(far){
        //draw the outer wall
        a[0] = x;
        a[1] = 0;
        a[2] = z + 10;
        b[0] = x;
        b[1] = height;
        b[2] = z + 10;
        c[0] = x + 10;
        c[1] = height;
        c[2] = z + 10;
        d[0] = x + 10;
        d[1] = 0;
        d[2] = z + 10;
        make_rectangle_textured(a, b, c, d, mat_amb);
        //draw the inner wall
        a[0] = x;
        a[1] = 0;
        a[2] = z + 9;
        b[0] = x;
        b[1] = height;
        b[2] = z + 9;
        c[0] = x + 10;
        c[1] = height;
        c[2] = z + 9;
        d[0] = x + 10;
        d[1] = 0;
        d[2] = z + 9;
        make_rectangle_textured(a, b, c, d, mat_amb);
        //draw top of wall
        a[0] = x;
        a[1] = height;
        a[2] = z + 9;
        b[0] = x;
        b[1] = height;
        b[2] = z + 10;
        c[0] = x + 10;
        c[1] = height;
        c[2] = z + 10;
        d[0] = x + 10;
        d[1] = height;
        d[2] = z + 9;
        make_rectangle_textured(a, b, c, d, mat_amb);
        //draw left wall
        a[0] = x + 10;
        a[1] = 0;
        a[2] = z + 9;
        b[0] = x + 10;
        b[1] = 0;
        b[2] = z + 10;
        c[0] = x + 10;
        c[1] = height;
        c[2] = z + 10;
        d[0] = x + 10;
        d[1] = height;
        d[2] = z + 9;
        make_rectangle_textured(a, b, c, d, mat_amb);
        //draw right wall
        a[0] = x;
        a[1] = 0;
        a[2] = z + 9;
        b[0] = x;
        b[1] = 0;
        b[2] = z + 10;
        c[0] = x;
        c[1] = height;
        c[2] = z + 10;
        d[0] = x;
        d[1] = height;
        d[2] = z + 9;
        make_rectangle_textured(a, b, c, d, mat_amb);
    }
    
	//draw barrel?
}