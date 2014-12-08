#include "collision_detection.h"



int detect_collision(float maze[][3], float user[7], int worldX){
    
    int xx, zz, block;
	double collision_point = 0; 
    xx = (int)user[0] / 10;
    zz = (int)user[1] / 10;
    block = zz * worldX+ xx;
    //check if the current block has a wall on the left.
    if(layout_possibility_checker(maze, block, 1, 0, 0, 0) == 0){
		collision_point = xx * 10 + 9; 
		if (collision_point - user[0] < 0.25)
			return 0; 
    }else{
        //no wall this block... check if next block to the left has a wall
        //on the right that we would run into.
        if(adjacent_block_layout_checker(maze, block, 1, 0, 0, 0) == 0){
			collision_point = xx * 10 + 10; 
			if (collision_point - user[0] < 0.25)
				return 0;
        }
    }
    //right wall?
    if(layout_possibility_checker(maze, block, 0, 1, 0, 0) == 0){
		collision_point = xx * 10 + 1; 
		if (user[0] - collision_point < 0.25)
			return 0; 
    }else{
        if(adjacent_block_layout_checker(maze, block, 0, 1, 0, 0) == 0){
   			collision_point = xx * 10;
			if (user[0] - collision_point < 0.25)
				return 0;
        }
    }
    //close wall?
    if(layout_possibility_checker(maze, block, 0, 0, 1, 0) == 0){
        collision_point = zz * 10 + 1;
		if (user[1] - collision_point < 0.25)
			return 0; 
    }else{
        if(adjacent_block_layout_checker(maze, block, 0, 0, 1, 0) == 0){
            collision_point = zz * 10;
			if (user[1] - collision_point < 0.25)
				return 0; 
        }
    }
    //far1 wall?
    if(layout_possibility_checker(maze, block, 0, 0, 0, 1) == 0){
        collision_point = zz * 10 + 9;
		if (collision_point - user[1] < 0.25)
			return 0; 
    }else{
        if(adjacent_block_layout_checker(maze, block, 0, 0, 0, 1) == 0){
            collision_point = zz * 10 + 10;
			if (collision_point - user[1] < 0.25)
				return 0;
        }
    }

	if (maze[block][2] == 1){
		double collision_point_z = 0;
		double delta_x, delta_z;
		collision_point = xx * 10 + 2.5;
		collision_point_z = zz * 10 + 2.5;
		delta_x = collision_point - user[0];
		delta_z = collision_point_z - user[1];
		if ((delta_x < 1.5) && (delta_x > -1.5) && (delta_z < 1.5) && (delta_z > -1.5))
			return 0; 
	}

	return 1; 
}