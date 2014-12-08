
//enemy movement
#include "enemy_movement.h"

void enemey_move(float maze[][3], float info[3][3], int worldX, int count){
	
	int i; 

	//count == 0 then we have to choose a new block for the enemies. 
	if (count == 0){
		for (i = 0; i < NUM_ENEMY; i++)
			move(maze, info[i], worldX);
	}
	else{
		//only need to continue forward movement until a decision must be made
		for (i = 0; i < NUM_ENEMY; i++){
			if (info[i][2] == 1){		 //left
				info[i][0] = info[i][0] + MOV_DISTANCE; 
			}
			else if (info[i][2] == 2){	//right
				info[i][0] = info[i][0] - MOV_DISTANCE; 
			}
			else if (info[i][2] == 3){	//close
				info[i][1] = info[i][1] - MOV_DISTANCE;
			}
			else if (info[i][2] == 4){	//far1
				info[i][1] = info[i][1] + MOV_DISTANCE; 
			}
		}
	}
}

/* randomly chooses where the enemies go next */
int move(float maze[][3], float info[3], int worldX){
	int i, xx, zz, block, choice, bad_direction =1;
	int return_info[2]; 

	for (i = 0; i < NUM_ENEMY; i++){

		xx = (int)info[0] / 10;
		zz = (int)info[1] / 10;
		block = zz * worldX + xx;

		choice = rand() % 4; 
		switch (choice){
		case 0: //exit far1
			if ((int)info[2] != 3){	//check to make sure enemy did not enter from far1

				//can the next block can be entered from this direction. 
				if (adjacent_block_layout_checker(maze, block, 0, 0, 0, 1)){ 
					 
					//does the current block have a wall that direction?
					if (layout_possibility_checker(maze, block, 0, 0, 0, 1)){
						info[2] = 4;
						return 1; 
					}
				}
			}
		case 1: //exit left
			if ((int)info[2] != 2){	//check to see if enemy entered from right
				
				//can the next block can be entered from this direction. 
				if (adjacent_block_layout_checker(maze, block, 1, 0, 0, 0)){

					//does the current block have a wall that direction?
					if (layout_possibility_checker(maze, block, 1, 0, 0, 0)){
						info[2] = 1;
						return 1;
					}
				}
			}
		case 2: //exit right
			if ((int)info[2] != 1){	//check to see if enemy entered from left

				//can the next block can be entered from this direction. 
				if (adjacent_block_layout_checker(maze, block, 0, 1, 0, 0)){

					//does the current block have a wall that direction?
					if (layout_possibility_checker(maze, block, 0, 1, 0, 0)){
						info[2] = 2;
						return 1;
					}
				}
			}
		case 3: //exit close
			if ((int)info[2] != 4){	//check to see if enemy entered from close

				//can the next block can be entered from this direction. 
				if (adjacent_block_layout_checker(maze, block, 0, 0, 1, 0)){

					//does the current block have a wall that direction?
					if (layout_possibility_checker(maze, block, 0, 0, 1, 0)){
						info[2] = 3;
						return 1;
					}
				}
			}
		default:
			/*random attempt failed. attempt every direction once again. then turn around if 
				no direction works */ 
			//far1
			if ((int)info[2] != 3){	//check to make sure enemy did not enter from far1
				//can the next block can be entered from this direction. 
				if (adjacent_block_layout_checker(maze, block, 0, 0, 0, 1)){
					//does the current block have a wall that direction?
					if (layout_possibility_checker(maze, block, 0, 0, 0, 1)){
						info[2] = 4;
						return 1;
					}
				}
			}
			//left
			if ((int)info[2] != 2){	
				if (adjacent_block_layout_checker(maze, block, 1, 0, 0, 0)){
					if (layout_possibility_checker(maze, block, 1, 0, 0, 0)){
						info[2] = 1; 
						return 1;
					}
				}
			}
			//right
			if ((int)info[2] != 1){	 
				if (adjacent_block_layout_checker(maze, block, 0, 1, 0, 0)){
					if (layout_possibility_checker(maze, block, 0, 1, 0, 0)){
						info[2] = 2;
						return 1;
					}
				}
			}
			//close- no need to check. It was garenteed to have been checked. 
			
			//time to turn around
			if (info[2] == 1){
				/*was traveling left to reach this point.*/
				info[2] = 2; 
			}
			else if (info[2] == 2){	//right
				info[2] = 1; 
			}
			else if (info[2] == 3){	//close
				info[2] = 4; 
			}
			else if (info[2] == 4){	//far1
				info[2] = 3; 
			}
		}//switch
	}//for
}