
#include "maze_generator.h"

int return_info[2];
int open_paths_counter = 0;
int open_paths_counter2 = 0;
 //[0] = sucessful add?, [1] = entered from what direction?
int maze_x_length;
int maze_z_length;
int maze_not_finished = 1;
int finish_entry = 0; //exit of the maze entry
int finish_entry_layout = 0;
int starting_entry = 0; //entrance of maze entry
int open_paths[2000][5]; //[0]entry, [1]exit left open?, [2]right?, [3]close? [4]far1?
int open_paths2[2000][5];

void maze_first_pass(float maze[][3], float info[3][3], int starting_x);
void maze_second_pass(float maze[][3]);
void maze_third_pass(float maze[][3]);
int first_pass_entrance_close(float maze[][3], int entry);
int first_pass_entrance_right(float maze[][3], int entry);
int first_pass_possibility_check(int entry, int m_left, int m_right, int m_close, int m_far1);
int first_pass_outer_wall_check(int entry, int layout);
int second_pass_path_expansion(float maze[][3], int entry, int return_info2[2]);
int second_pass_entrance_left(float maze[][3], int entry, int return_info2[2]);
int second_pass_entrance_right(float maze[][3], int entry, int return_info2[2]);
int second_pass_entrance_close(float maze[][3], int entry, int return_info2[2]);
int second_pass_entrance_far1(float maze[][3], int entry, int return_info2[2]);
int third_pass_path_expansion(float maze[][3], int entry, int return_info2[2]);
int third_pass_entrance_left(float maze[][3], int entry, int return_info2[2]);
int third_pass_entrance_right(float maze[][3], int entry, int return_info2[2]);
int third_pass_entrance_close(float maze[][3], int entry, int return_info2[2]);
int third_pass_entrance_far1(float maze[][3], int entry, int return_info2[2]);
int adjacent_block_layout_checker(float maze[][3], int entry, int left, int right, int close, int far1);
int layout_possibility_checker(float maze[][3], int entry, int m_left, int m_right, int m_close, int m_far1);
void complete_outer_wall(float maze[][3]);
void add_barrels(float maze[][3]); 

void generate_maze(float maze[][3], float info[3][3], int x_length, int z_length){
    
    maze_x_length = x_length;
    maze_z_length = z_length;
    
    //seed RNG
    srand((unsigned int)time(NULL));
    
    //maze always starts at {0, 0}
    maze_first_pass(maze, info, 0);
    maze_second_pass(maze);
    maze_third_pass(maze);
    complete_outer_wall(maze);
	add_barrels(maze);
}

/*
    Generates the path from beginning of the maze to the end. 
    For simplicity, can only progress forward(x & z stays the same or increases).
	info array is used to initialize the enemies. 
 */
void maze_first_pass(float maze[][3], float info[3][3],  int starting_x){
    
    //int entry_direction 1 = left, 2=right, 3=close, 4 = far1
    int entry = starting_x; //maze entry
	int place_enemy = maze_x_length / 3;  
	int enemies_placed = 0; 
	int xx, zz; 
    starting_entry = starting_x;

    //maze starts the same way everytime... walls on left and right at the origin
    maze[entry][0] = 8;
    return_info[1] = 3;

    printf("maze entry = %d and type =%f\n", entry, maze[entry][0]);
    while(maze_not_finished){
        if(return_info[1] == 2){ //enter from right
            entry = entry + 1;
            first_pass_entrance_right(maze, entry);
			maze[entry][1] = .8; //maze color
        }else if(return_info[1] == 3){ //enter from close
            entry = entry + maze_x_length;
            first_pass_entrance_close(maze, entry);
			maze[entry][1] = .8; //maze color
        }
        printf("maze entry = %d and type =%f\n", entry, maze[entry][0]);

		if ( (entry / maze_x_length > place_enemy) && (enemies_placed < 3) ){
			//we can put an enemy here
			xx = entry % maze_x_length;
			zz = entry / maze_x_length;
			xx = xx * 10;
			zz = zz * 10;
			info[enemies_placed][0] = xx + 5; //place in center of block 
			info[enemies_placed][1] = zz + 5;
			if (return_info[1] == 3){
				info[enemies_placed][2] = 3;
			}
			else{
				info[enemies_placed][2] = 2; 
			}
			enemies_placed++; 
			place_enemy = place_enemy + ((int)maze_x_length / 10); 
		}
    }
    maze[entry][0] = finish_entry_layout; //make the exit is an exit.
}

//10% chance for every maze block to have a barrel
void add_barrels(float maze[][3]){
	int jj = maze_x_length * maze_z_length; 
	int choice = 0; 

	for (int j = 0; j<jj; j++){
		if (maze[j][0] != -1){
			choice = rand() % 10;
			if (choice == 0)
				maze[j][2] = 1;
		}
	}
}

int first_pass_entrance_right(float maze[][3], int entry){
    //available_options = 6;
    int choice = rand() % 6;
    switch(choice){
        case 0:
            if(first_pass_outer_wall_check(entry, 0)){
                maze[entry][0] = 0;
                return_info[0] = 1;
                open_paths[open_paths_counter][0] = entry;
                open_paths[open_paths_counter][1] = 1;
                open_paths[open_paths_counter][2] = 0;
                open_paths[open_paths_counter][3] = 1;
                open_paths[open_paths_counter][4] = 1;
                choice = rand() % 2; //two options next entry
                if(choice == 0){
                    return_info[1] = 3;
                    open_paths[open_paths_counter][4] = 0;
                }else{
                    return_info[1] = 2;
                    open_paths[open_paths_counter][1] = 0;
                }
                open_paths_counter ++;
                break;
            }
        case 1:
            if(first_pass_outer_wall_check(entry, 1)){
                maze[entry][0] = 1;
                return_info[0] = 1;
                open_paths[open_paths_counter][0] = entry;
                open_paths[open_paths_counter][1] = 1;
                open_paths[open_paths_counter][2] = 0;
                open_paths[open_paths_counter][3] = 0;
                open_paths[open_paths_counter][4] = 1;
                choice = rand() % 2; //two options next entry
                if(choice == 0){
                    return_info[1] = 3;
                    open_paths[open_paths_counter][4] = 0;
                }else{
                    return_info[1] = 2;
                    open_paths[open_paths_counter][1] = 0;
                }
                open_paths_counter ++;
                break;
            }
        case 2:
            if(first_pass_outer_wall_check(entry, 2)){
                maze[entry][0] = 2;
                return_info[0] = 1;
                return_info[1] = 3;
                open_paths[open_paths_counter][0] = entry;
                open_paths[open_paths_counter][1] = 0;
                open_paths[open_paths_counter][2] = 0;
                open_paths[open_paths_counter][3] = 1;
                open_paths[open_paths_counter][4] = 0;
                open_paths_counter++;
                break;
            }
        case 3:
            if(first_pass_outer_wall_check(entry, 3)){
                maze[entry][0] = 3;
                return_info[0] = 1;
                return_info[1] = 2;
                open_paths[open_paths_counter][0] = entry;
                open_paths[open_paths_counter][1] = 0;
                open_paths[open_paths_counter][2] = 0;
                open_paths[open_paths_counter][3] = 1;
                open_paths[open_paths_counter][4] = 0;
                open_paths_counter++;
                break;
            }
        case 4:
            if(first_pass_outer_wall_check(entry, 5)){
                maze[entry][0] = 5;
                return_info[0] = 1;
                return_info[1] = 3;
                break;
            }
        case 5:
            if(first_pass_outer_wall_check(entry, 6)){
                maze[entry][0] = 6;
                return_info[0] = 1;
                return_info[1] = 2;
                break;
            }
        default:
            //if the wall checker failed then we will just assign it 5
            maze[entry][0] = 5;
            return_info[0] = 1;
            return_info[1] = 3;
            break;
    }
    return 0;
}

int first_pass_entrance_close(float maze[][3], int entry){
    //available_options = 6;
    int choice = rand() % 6;
    switch(choice){
        case 0:
            if(first_pass_outer_wall_check(entry, 0)){
                maze[entry][0] = 0;
                return_info[0] = 1;
                open_paths[open_paths_counter][0] = entry;
                open_paths[open_paths_counter][1] = 1;
                open_paths[open_paths_counter][2] = 1;
                open_paths[open_paths_counter][3] = 0;
                open_paths[open_paths_counter][4] = 1;
                choice = rand() % 2; //two options next entry
                if(choice == 0){
                    return_info[1] = 3;
                    open_paths[open_paths_counter][4] = 0;
                }else{
                    return_info[1] = 2;
                    open_paths[open_paths_counter][1] = 0;
                }
                open_paths_counter++;
                break;
            }
        case 1:
            if(first_pass_outer_wall_check(entry, 2)){
                maze[entry][0] = 2;
                return_info[0] = 1;
                return_info[1] = 3;
                open_paths[open_paths_counter][0] = entry;
                open_paths[open_paths_counter][1] = 0;
                open_paths[open_paths_counter][2] = 1;
                open_paths[open_paths_counter][3] = 0;
                open_paths[open_paths_counter][4] = 0;
                open_paths_counter++;
                break;
            }
        case 2:
            if(first_pass_outer_wall_check(entry, 3)){
                maze[entry][0] = 3;
                return_info[0] = 1;
                return_info[1] = 2;
                open_paths[open_paths_counter][0] = entry;
                open_paths[open_paths_counter][1] = 0;
                open_paths[open_paths_counter][2] = 1;
                open_paths[open_paths_counter][3] = 0;
                open_paths[open_paths_counter][4] = 0;
                open_paths_counter++;
                break;
            }
        case 3:
            if(first_pass_outer_wall_check(entry, 4)){
                maze[entry][0] = 4;
                return_info[0] = 1;
                open_paths[open_paths_counter][0] = entry;
                open_paths[open_paths_counter][1] = 1;
                open_paths[open_paths_counter][2] = 0;
                open_paths[open_paths_counter][3] = 0;
                open_paths[open_paths_counter][4] = 1;
                choice = rand() % 2; //two options next entry
                if(choice == 0){
                    return_info[1] = 3;
                    open_paths[open_paths_counter][4] = 0;
                }else{
                    return_info[1] = 2;
                    open_paths[open_paths_counter][1] = 0;
                }
                open_paths_counter++;
                break;
            }
        case 4:
            if(first_pass_outer_wall_check(entry, 8)){
                maze[entry][0] = 8;
                return_info[0] = 1;
                return_info[1] = 3;
                break;
            }
        case 5:
            if(first_pass_outer_wall_check(entry, 10)){
                maze[entry][0] = 10;
                return_info[0] = 1;
                return_info[1] = 2;
                break;
            }
        default:
            //if the wall checker failed then we will just assign it 8.  8 is
            //a valid option reguardless of which wall it failed to meet.
            maze[entry][0] = 8;
            return_info[0] = 1;
            return_info[1] = 3;
            break;
    }
    return 0;
}

/*
 Method requires that the the block being placed has a wall against the
 outer wall of the maze. Also checks to see if maze generation is done.
 */
int first_pass_outer_wall_check(int entry, int layout){
  
    if((entry % maze_x_length) == 0){
        //right must be 1
        if( !(layout==4 || layout==7 || layout ==8 || layout ==10) ){
            return 0;
        }
    }
    /*
    if( ((entry % maze_x_length) + 1) == maze_x_length){
        //left must be 1
        if( (layout==2 || layout==5 || layout==8 || layout ==9) )
            return 0;
    }
    */
    if( ((entry % maze_x_length) + 1) == maze_x_length){
        //first pass is done, reached left wall
        maze_not_finished = 0;
        finish_entry = entry;
        finish_entry_layout = 6;
        return 1;
    }
    if( ((entry / maze_x_length) + 1 == maze_z_length)){
        //first pass is done, reached far1 wall
        maze_not_finished = 0;
        finish_entry = entry;
        finish_entry_layout = 8;
        return 1;
    }
    return 1;
}

/*
 Method will access all the open paths left by first pass and create a
 path for them.  Method will continue till all paths have a dead end.
 */
void maze_second_pass(float maze[][3]){
    /*
     open_paths: [0]entry, [1]exit left open?, [2]right?, [3]close? [4]far1?
     If an exit is open to the left of the current block, then the player
     enters the next block from the right, i.e. 
     second_pass_entrance_right() is called.
     
     second pass does not have the restriction of always increasing z and x 
     so many more options are available.  Layouts with only 1 entry and exit
     (5, 6, 7, 8, 9, 10) will have twice the probability of the 3+ open wall 
     layouts
    */
    int next_block_entry = 0;
    int return_info2[2] = {1, 0};
    
    for(int i=0; i<open_paths_counter; i++){
        return_info2[0] = 1;
        next_block_entry = open_paths[i][0];
        if(open_paths[i][1]){ //exit left is open
            //next_block_entry = open_paths[i][0] + 1;
            return_info2[1] = 2;
            second_pass_path_expansion(maze, next_block_entry, return_info2);
        }
        return_info2[0] = 1;
        if(open_paths[i][2]){ //exit right is open
            //next_block_entry = open_paths[i][0] - 1;
            return_info2[1] = 1;
            second_pass_path_expansion(maze, next_block_entry, return_info2);
        }
        return_info2[0] = 1;
        if(open_paths[i][3]){ //exit close is open
            //next_block_entry = open_paths[i][0] - maze_x_length;
            return_info2[1] = 4;
            second_pass_path_expansion(maze, next_block_entry, return_info2);
        }
        return_info2[0] = 1;
        if(open_paths[i][4]){ //exit far1 is open
           // next_block_entry = open_paths[i][0] + maze_x_length;
            return_info2[1] = 3;
            second_pass_path_expansion(maze, next_block_entry, return_info2);
        }
    }
}

int second_pass_path_expansion(float maze[][3], int entry, int return_info2[2]){
   
    int next_entry = entry;
    while(return_info2[0] == 1){
        if(return_info2[1] == 1){ //enter next block from left
            next_entry = next_entry - 1;
            second_pass_entrance_left(maze, next_entry, return_info2);
        }else if(return_info2[1] == 2){ //enter next block from right
            next_entry = next_entry + 1;
            second_pass_entrance_right(maze, next_entry, return_info2);
        }else if(return_info2[1] == 3){ //enter next block from close
            next_entry = next_entry + maze_x_length;
            second_pass_entrance_close(maze, next_entry, return_info2);
        }else if(return_info2[1] == 4){ //enter next block from far1
            next_entry = next_entry - maze_x_length;
            second_pass_entrance_far1(maze, next_entry, return_info2);
        }
    }
    return 1;
}

/*
 Method will access all the open paths left by second pass and create a
 path for them.  Method will continue till all paths have a dead end. 
 Third pass does not allow for any open paths to be created.
 */
void maze_third_pass(float maze[][3]){
    /*
     open_paths: [0]entry, [1]exit left open?, [2]right?, [3]close? [4]far1?
     If an exit is open to the left of the current block, then the player
     enters the next block from the right, i.e.
     third_pass_entrance_right() is called.
     
     third pass does not have the restriction of always increasing z and x
     so many more options are available.  Layouts with only 1 entry and exit
     (5, 6, 7, 8, 9, 10) are possible.  If the chosen block does not work 
     the switch methods fall through until the default option which ends
     the current path.
     */
    int next_block_entry = 0;
    int return_info2[2] = {1, 0};
    
    for(int i=0; i<open_paths_counter2; i++){
        // printf("i = %d entry = %d layout %f\n", i, open_paths[i][0], maze[open_paths[i][0]][0]);
        return_info2[0] = 1;
        if(open_paths2[i][1]){ //exit left is open
            next_block_entry = open_paths2[i][0] + 1;
            return_info2[1] = 2;
            second_pass_path_expansion(maze, next_block_entry, return_info2);
            //printf("2p maze entry = %d and type =%f\n", next_block_entry, maze[next_block_entry][0]);
        }
        return_info2[0] = 1;
        if(open_paths2[i][2]){ //exit right is open
            next_block_entry = open_paths2[i][0] - 1;
            return_info2[1] = 1;
            second_pass_path_expansion(maze, next_block_entry, return_info2);
           // printf("2p maze entry = %d and type =%f\n", next_block_entry, maze[next_block_entry][0]);
        }
        return_info2[0] = 1;
        if(open_paths2[i][3]){ //exit close is open
            next_block_entry = open_paths2[i][0] - maze_x_length;
            return_info2[1] = 4;
            second_pass_path_expansion(maze, next_block_entry, return_info2);
           // printf("2p maze entry = %d and type =%f\n", next_block_entry, maze[next_block_entry][0]);
        }
        return_info2[0] = 1;
        if(open_paths2[i][4]){ //exit far1 is open
            next_block_entry = open_paths2[i][0] + maze_x_length;
            return_info2[1] = 3;
            second_pass_path_expansion(maze, next_block_entry, return_info2);
           // printf("2p maze entry = %d and type =%f\n", next_block_entry, maze[next_block_entry][0]);
        }
    }
}

int third_pass_path_expansion(float maze[][3], int entry, int return_info2[2]){
    
    int next_entry = entry;
    while(return_info2[0] == 1){
        if(return_info2[1] == 1){ //enter next block from left
            next_entry = next_entry - 1;
            third_pass_entrance_left(maze, next_entry, return_info2);
        }else if(return_info2[1] == 2){ //enter next block from right
            next_entry = next_entry + 1;
            third_pass_entrance_right(maze, next_entry, return_info2);
        }else if(return_info2[1] == 3){ //enter next block from close
            next_entry = next_entry + maze_x_length;
            third_pass_entrance_close(maze, next_entry, return_info2);
        }else if(return_info2[1] == 4){ //enter next block from far1
            next_entry = next_entry - maze_x_length;
            third_pass_entrance_far1(maze, next_entry, return_info2);
        }
    }
    return 1;
}

/*
    chooses a random layout for this block
    case 0 and case 1 done
 */
int second_pass_entrance_right(float maze[][3], int entry, int return_info2[2]){
    
    int second_choice = 0; //allows for options to fall through
    int second_break = 0;
    
    //is the new block already assigned? - do nothing
    if(maze[entry][0] != -1){
        return_info2[0] = 0;
        return 0;
    }
    
    /*
     outer wall check, can only attempt to leave maze through the left, far1,
     close walls.  If it touches any of these walls, this path is over. give
     this block a layout = 14; however, right is checked incase of bugs
    */
    if( (entry % maze_x_length) + 1 == maze_x_length){ //left
        maze[entry][0] = 14;
        return_info2[0] = 0; //done with branch
        return 1;
    }
    if( (entry / maze_x_length) + 1 == maze_z_length){ //far1
        maze[entry][0] = 14;
        return_info2[0] = 0;
        return 1;
    }
    if( (entry / maze_x_length) == 0){ //close
        maze[entry][0] = 14;
        return_info2[0] = 0;
        return 1;
    }
	if ((entry % maze_x_length) == 0){ //right
		maze[entry][0] = 12;
		return_info2[0] = 0; 
		return 1;
	}
    
    //randomly choose the layout for this block, if the choosen value does
    //not work then we fall through to the next case and try again
    int choice = rand() % 10;
    switch(choice){
        case 0:
            second_choice = rand() % 3; //3 directions
            switch(second_choice){
                case 0: //exit block far1
                    if(adjacent_block_layout_checker(maze, entry, 1, 0, 1, 1)){
                        maze[entry][0] = 0;
                        return_info2[0] = 1;
                        return_info2[1] = 3;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 1;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 1;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
                case 1: //exit block left
                    if(adjacent_block_layout_checker(maze, entry, 1, 0, 1, 1)){
                        maze[entry][0] = 0;
                        return_info2[0] = 1;
                        return_info2[1] = 2;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 1;
                        open_paths2[open_paths_counter2][4] = 1;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
                case 2: //exit block close
                    if(adjacent_block_layout_checker(maze, entry, 1, 0, 1, 1)){
                        maze[entry][0] = 0;
                        return_info2[0] = 1;
                        return_info2[1] = 4;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 1;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 1;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
            }
            if(second_break){
                second_break = 0;
                break;
            }
        case 1:
            second_choice = rand() % 2; //2 directions
            switch(second_choice){
                case 0: //exit block far1
                    if(adjacent_block_layout_checker(maze, entry, 1, 0, 0, 1)){
                        maze[entry][0] = 1;
                        return_info2[0] = 1;
                        return_info2[1] = 3;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 1;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
                case 1: //exit left
                    if(adjacent_block_layout_checker(maze, entry, 1, 0, 0, 1)){
                        maze[entry][0] = 1;
                        return_info2[0] = 1;
                        return_info2[1] = 2;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 1;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
            }
            if(second_break){
                second_break = 0;
                break;
            }
        case 2:
            second_choice = rand() % 2; //2 directions
            switch(second_choice){
                case 0: //exit far1
                    if(adjacent_block_layout_checker(maze, entry, 0, 0, 1, 1)){
                        maze[entry][0] = 2;
                        return_info2[0] = 1;
                        return_info2[1] = 3;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 1;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
                case 1: //exit close
                    if(adjacent_block_layout_checker(maze, entry, 0, 0, 1, 1)){
                        maze[entry][0] = 2;
                        return_info2[0] = 1;
                        return_info2[1] = 4;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 1;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
            }
            if(second_break){
                second_break = 0;
                break;
            }
        case 3:
            second_choice = rand() % 2; //2 directions
            switch(second_choice){
                case 0: //exit left
                    if(adjacent_block_layout_checker(maze, entry, 1, 0, 1, 0)){
                        maze[entry][0] = 3;
                        return_info2[0] = 1;
                        return_info2[1] = 2;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 1;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
                case 1: //exit close
                    if(adjacent_block_layout_checker(maze, entry, 1, 0, 1, 0)){
                        maze[entry][0] = 3;
                        return_info2[0] = 1;
                        return_info2[1] = 4;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 1;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
            }
            if(second_break){
                second_break = 0;
                break;
            }
        case 4:
        case 5:
            //exit far1
            if(adjacent_block_layout_checker(maze, entry, 0, 0, 0, 1)){
                maze[entry][0] = 5;
                return_info2[0] = 1;
                return_info2[1] = 3;
                break;
            }
        case 6:
        case 7:
            //exit left
            if(adjacent_block_layout_checker(maze, entry, 1, 0, 0, 0)){
                maze[entry][0] = 6;
                return_info2[0] = 1;
                return_info2[1] = 2;
                break;
            }
        case 8:
        case 9:
            //exit close
            if(adjacent_block_layout_checker(maze, entry, 0, 0, 1, 0)){
                maze[entry][0] = 9;
                return_info2[0] = 1;
                return_info2[1] = 4;
                break;
            }
        default:
            maze[entry][0] = 14;
            return_info2[0] = 0;
            return_info2[1] = 0;
    }
    return 1;
}

int second_pass_entrance_left(float maze[][3], int entry, int return_info2[2]){
    
    int second_choice = 0; //allows for options to fall through
    int second_break = 0;
    
    //is the new block already assigned? - do nothing
    if(maze[entry][0] != -1){
        return_info2[0] = 0;
        return_info2[1] = 0;
        return 0;
    }
    
    /*
     outer wall check, can only attempt to leave maze through the right, far1,
     close walls.  If it touches any of these walls, this path is over. give
     this block a layout = 12;
     */
    if( (entry % maze_x_length) == 0){ //right
        maze[entry][0] = 12;
        return_info2[0] = 0; //branch is finished
        return 1;
    }
    if( (entry / maze_x_length) + 1 == maze_z_length){ //far1
        maze[entry][0] = 12;
        return_info2[0] = 0;
        return 1;
    }
    if( (entry / maze_x_length) == 0){ //close
        maze[entry][0] = 12;
        return_info2[0] = 0;
        return 1;
    }
	if ((entry % maze_x_length) + 1 == maze_x_length){ //left (incase of bugs)
		maze[entry][0] = 14;
		return_info2[0] = 0;
		return 1;
	}
    
    //randomly choose the layout for this block, if the choosen value does
    //not work then we fall through to the next case and try again
    int choice = rand() % 10;
    switch(choice){
        case 0:
            second_choice = rand() % 3; //3 directions
            switch(second_choice){
                case 0: //exit block far1
                    if(adjacent_block_layout_checker(maze, entry, 0, 1, 1, 1)){
                        maze[entry][0] = 0;
                        return_info2[0] = 1;
                        return_info2[1] = 3;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 1;
                        open_paths2[open_paths_counter2][3] = 1;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
                case 1: //exit block right
                    if(adjacent_block_layout_checker(maze, entry, 0, 1, 1, 1)){
                        maze[entry][0] = 0;
                        return_info2[0] = 1;
                        return_info2[1] = 1;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 1;
                        open_paths2[open_paths_counter2][4] = 1;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
                case 2: //exit block close
                    if(adjacent_block_layout_checker(maze, entry, 0, 1, 1, 1)){
                        maze[entry][0] = 0;
                        return_info2[0] = 1;
                        return_info2[1] = 4;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 1;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 1;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
            }
            if(second_break){
                second_break = 0;
                break;
            }
        case 1:
            second_choice = rand() % 2; //2 directions
            switch(second_choice){
                case 0: //exit block far1
                    if(adjacent_block_layout_checker(maze, entry, 0, 1, 0, 1)){
                        maze[entry][0] = 1;
                        return_info2[0] = 1;
                        return_info2[1] = 3;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 1;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
                case 1: //exit right
                    if(adjacent_block_layout_checker(maze, entry, 0, 1, 0, 1)){
                        maze[entry][0] = 1;
                        return_info2[0] = 1;
                        return_info2[1] = 1;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 1;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
            }
            if(second_break){
                second_break = 0;
                break;
            }
        case 2:
            second_choice = rand() % 2; //2 directions
            switch(second_choice){
                case 0: //exit right
                    if(adjacent_block_layout_checker(maze, entry, 0, 1, 1, 0)){
                        maze[entry][0] = 3;
                        return_info2[0] = 1;
                        return_info2[1] = 1;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 1;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
                case 1: //exit close
                    if(adjacent_block_layout_checker(maze, entry, 0, 1, 1, 0)){
                        maze[entry][0] = 3;
                        return_info2[0] = 1;
                        return_info2[1] = 4;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 1;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
            }
            if(second_break){
                second_break = 0;
                break;
            }
        case 3:
            second_choice = rand() % 2; //2 directions
            switch(second_choice){
                case 0: //exit far1
                    if(adjacent_block_layout_checker(maze, entry, 0, 0, 1, 1)){
                        maze[entry][0] = 4;
                        return_info2[0] = 1;
                        return_info2[1] = 3;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 1;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
                case 1: //exit close
                    if(adjacent_block_layout_checker(maze, entry, 0, 0, 1, 1)){
                        maze[entry][0] = 4;
                        return_info2[0] = 1;
                        return_info2[1] = 4;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 1;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
            }
            if(second_break){
                second_break = 0;
                break;
            }
        case 4:
        case 5:
            //exit right
            if(adjacent_block_layout_checker(maze, entry, 0, 1, 0, 0)){
                maze[entry][0] = 6;
                return_info2[0] = 1;
                return_info2[1] = 1;
                break;
            }
        case 6:
        case 7:
            //exit far1
            if(adjacent_block_layout_checker(maze, entry, 0, 0, 0, 1)){
                maze[entry][0] = 7;
                return_info2[0] = 1;
                return_info2[1] = 3;
                break;
            }
        case 8:
        case 9:
            //exit close
            if(adjacent_block_layout_checker(maze, entry, 0, 0, 1, 0)){
                maze[entry][0] = 10;
                return_info2[0] = 1;
                return_info2[1] = 4;
                break;
            }
        default:
            maze[entry][0] = 12;
            return_info2[0] = 0;
            return_info2[1] = 0;
    }
    return 1;
}

int second_pass_entrance_close(float maze[][3], int entry, int return_info2[2]){
    
    int second_choice = 0; //allows for options to fall through
    int second_break = 0;
    
    //is the new block already assigned? - do nothing
    if(maze[entry][0] != -1){
        return_info2[0] = 0;
        return_info2[1] = 0;
        return 0;
    }
    
    /*
     outer wall check, can only attempt to leave maze through the right, left,
     far1, walls.  If it touches any of these walls, this path is over. give
     this block a layout = 11;
     */
    if( (entry % maze_x_length) == 0){ //right
        maze[entry][0] = 11;
        return_info2[0] = 0;
        return 1;
    }
    if( (entry / maze_x_length) + 1 == maze_z_length){ //far1
        maze[entry][0] = 11;
        return_info2[0] = 0;
        return 1;
    }
    if( (entry % maze_x_length) + 1 == maze_x_length){ //left
        maze[entry][0] = 11;
        return_info2[0] = 0;
        return 1;
    }
	if ((entry / maze_x_length) == 0){ //close (incase of bugs)
		maze[entry][0] = 13;
		return_info2[0] = 0;
		return 1;
	}
    
    //randomly choose the layout for this block, if the choosen value does
    //not work then we fall through to the next case and try again
    int choice = rand() % 10;
    switch(choice){
        case 0:
            second_choice = rand() % 3; //3 directions
            switch(second_choice){
                case 0: //exit block far1
                    if(adjacent_block_layout_checker(maze, entry, 1, 1, 0, 1)){
                        maze[entry][0] = 0;
                        return_info2[0] = 1;
                        return_info2[1] = 3;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 1;
                        open_paths2[open_paths_counter2][2] = 1;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
                case 1: //exit block right
                    if(adjacent_block_layout_checker(maze, entry, 1, 1, 0, 1)){
                        maze[entry][0] = 0;
                        return_info2[0] = 1;
                        return_info2[1] = 1;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 1;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 1;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
                case 2: //exit block left
                    if(adjacent_block_layout_checker(maze, entry, 1, 1, 0, 1)){
                        maze[entry][0] = 0;
                        return_info2[0] = 1;
                        return_info2[1] = 2;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 1;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 1;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
            }
            if(second_break){
                second_break = 0;
                break;
            }
        case 1:
            second_choice = rand() % 2; //2 directions
            switch(second_choice){
                case 0: //exit far1
                    if(adjacent_block_layout_checker(maze, entry, 0, 1, 0, 1)){
                        maze[entry][0] = 2;
                        return_info2[0] = 1;
                        return_info2[1] = 3;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 1;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
                case 1: //exit right
                    if(adjacent_block_layout_checker(maze, entry, 0, 1, 0, 1)){
                        maze[entry][0] = 2;
                        return_info2[0] = 1;
                        return_info2[1] = 1;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 1;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
            }
            if(second_break){
                second_break = 0;
                break;
            }
        case 2:
            second_choice = rand() % 2; //2 directions
            switch(second_choice){
                case 0: //exit right
                    if(adjacent_block_layout_checker(maze, entry, 1, 1, 0, 0)){
                        maze[entry][0] = 3;
                        return_info2[0] = 1;
                        return_info2[1] = 1;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 1;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
                case 1: //exit left
                    if(adjacent_block_layout_checker(maze, entry, 1, 1, 0, 0)){
                        maze[entry][0] = 3;
                        return_info2[0] = 1;
                        return_info2[1] = 2;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 1;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
            }
            if(second_break){
                second_break = 0;
                break;
            }
        case 3:
            second_choice = rand() % 2; //2 directions
            switch(second_choice){
                case 0: //exit far1
                    if(adjacent_block_layout_checker(maze, entry, 1, 0, 0, 1)){
                        maze[entry][0] = 4;
                        return_info2[0] = 1;
                        return_info2[1] = 3;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 1;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
                case 1: //exit left
                    if(adjacent_block_layout_checker(maze, entry, 1, 0, 0, 1)){
                        maze[entry][0] = 4;
                        return_info2[0] = 1;
                        return_info2[1] = 2;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 1;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
            }
            if(second_break){
                second_break = 0;
                break;
            }
        case 4:
        case 5:
            //exit far1
            if(adjacent_block_layout_checker(maze, entry, 0, 0, 0, 1)){
                maze[entry][0] = 8;
                return_info2[0] = 1;
                return_info2[1] = 3;
                break;
            }
        case 6:
        case 7:
            //exit right
            if(adjacent_block_layout_checker(maze, entry, 0, 1, 0, 0)){
                maze[entry][0] = 9;
                return_info2[0] = 1;
                return_info2[1] = 1;
                break;
            }
        case 8:
        case 9:
            //exit left
            if(adjacent_block_layout_checker(maze, entry, 1, 0, 0, 0)){
                maze[entry][0] = 10;
                return_info2[0] = 1;
                return_info2[1] = 2;
                break;
            }
        default:
            maze[entry][0] = 11;
            return_info2[0] = 0;
            return_info2[1] = 0;
    }
    return 1;
}

int second_pass_entrance_far1(float maze[][3], int entry, int return_info2[2]){
    
    int second_choice = 0; //allows for options to fall through
    int second_break = 0;
    
    //is the new block already assigned? - do nothing
    if(maze[entry][0] != -1){
        return_info2[0] = 0;
        return_info2[1] = 0;
        return 0;
    }
    
    /*
     outer wall check, can only attempt to leave maze through the right, left,
     close walls.  If it touches any of these walls, this path is over. give
     this block a layout = 13;
     */
    if( (entry % maze_x_length) == 0){ //right
        maze[entry][0] = 13;
        return_info2[0] = 0;
        return_info2[1] = 0;
        return 1;
    }
    if( (entry / maze_x_length) == 0){ //close
        maze[entry][0] = 13;
        return_info2[0] = 0;
        return_info2[1] = 0;
        return 1;
    }
    if( (entry % maze_x_length) + 1 == maze_x_length){ //left
        maze[entry][0] = 13;
        return_info2[0] = 0;
        return_info2[1] = 0;
        return 1;
    }
	if ((entry / maze_x_length) + 1 == maze_z_length){ //far1 (in case of bugs)
		maze[entry][0] = 11;
		return_info2[0] = 0;
		return_info2[1] = 0;
		return 1;
	}
    //randomly choose the layout for this block, if the choosen value does
    //not work then we fall through to the next case and try again
    int choice = rand() % 10;
    switch(choice){
        case 0:
            second_choice = rand() % 3; //3 directions
            switch(second_choice){
                case 0: //exit block close
                    if(adjacent_block_layout_checker(maze, entry, 1, 1, 1, 0)){
                        maze[entry][0] = 0;
                        return_info2[0] = 1;
                        return_info2[1] = 4;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 1;
                        open_paths2[open_paths_counter2][2] = 1;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
                case 1: //exit block right
                    if(adjacent_block_layout_checker(maze, entry, 1, 1, 1, 0)){
                        maze[entry][0] = 0;
                        return_info2[0] = 1;
                        return_info2[1] = 1;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 1;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 1;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
                case 2: //exit block left
                    if(adjacent_block_layout_checker(maze, entry, 1, 1, 1, 0)){
                        maze[entry][0] = 0;
                        return_info2[0] = 1;
                        return_info2[1] = 2;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 1;
                        open_paths2[open_paths_counter2][3] = 1;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
            }
            if(second_break){
                second_break = 0;
                break;
            }
        case 1:
            second_choice = rand() % 2; //2 directions
            switch(second_choice){
                case 0: //exit block left
                    if(adjacent_block_layout_checker(maze, entry, 1, 1, 0, 0)){
                        maze[entry][0] = 1;
                        return_info2[0] = 1;
                        return_info2[1] = 2;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 1;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
                case 1: //exit right
                    if(adjacent_block_layout_checker(maze, entry, 1, 1, 0, 0)){
                        maze[entry][0] = 1;
                        return_info2[0] = 1;
                        return_info2[1] = 1;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 1;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
            }
            if(second_break){
                second_break = 0;
                break;
            }
        case 2:
            second_choice = rand() % 2; //2 directions
            switch(second_choice){
                case 0: //exit close
                    if(adjacent_block_layout_checker(maze, entry, 0, 1, 1, 0)){
                        maze[entry][0] = 2;
                        return_info2[0] = 1;
                        return_info2[1] = 4;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 1;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
                case 1: //exit right
                    if(adjacent_block_layout_checker(maze, entry, 0, 1, 1, 0)){
                        maze[entry][0] = 2;
                        return_info2[0] = 1;
                        return_info2[1] = 1;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 1;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
            }
            if(second_break){
                second_break = 0;
                break;
            }
        case 3:
            second_choice = rand() % 2; //2 directions
            switch(second_choice){
                case 0: //exit close
                    if(adjacent_block_layout_checker(maze, entry, 1, 0, 1, 0)){
                        maze[entry][0] = 4;
                        return_info2[0] = 1;
                        return_info2[1] = 4;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 1;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 0;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
                case 1: //exit left
                    if(adjacent_block_layout_checker(maze, entry, 1, 0, 1, 0)){
                        maze[entry][0] = 4;
                        return_info2[0] = 1;
                        return_info2[1] = 2;
                        open_paths2[open_paths_counter2][0] = entry;
                        open_paths2[open_paths_counter2][1] = 0;
                        open_paths2[open_paths_counter2][2] = 0;
                        open_paths2[open_paths_counter2][3] = 1;
                        open_paths2[open_paths_counter2][4] = 0;
                        open_paths_counter2 ++;
                        second_break = 1;
                        break;
                    }
            }
            if(second_break){
                second_break = 0;
                break;
            }
        case 4:
        case 5:
            //exit right
            if(adjacent_block_layout_checker(maze, entry, 0, 1, 0, 0)){
                maze[entry][0] = 5;
                return_info2[0] = 1;
                return_info2[1] = 1;
                break;
            }
        case 6:
        case 7:
            //exit left
            if(adjacent_block_layout_checker(maze, entry, 1, 0, 0, 0)){
                maze[entry][0] = 7;
                return_info2[0] = 1;
                return_info2[1] = 2;
                break;
            }
        case 8:
        case 9:
            //exit close
            if(adjacent_block_layout_checker(maze, entry, 0, 0, 1, 0)){
                maze[entry][0] = 8;
                return_info2[0] = 1;
                return_info2[1] = 4;
                break;
            }
        default:
            maze[entry][0] = 13;
            return_info2[0] = 0;
            return_info2[1] = 0;
    }
    return 1;
}

/*
 Third pass does not allow any opening to remain.
 */
int third_pass_entrance_left(float maze[][3], int entry, int return_info2[2]){
    
    //is the new block already assigned? - do nothing
    if(maze[entry][0] != -1){
        return_info2[0] = 0;
        return_info2[1] = 0;
        return 0;
    }
    
    /*
     outer wall check, can only attempt to leave maze through the right, far1,
     close walls.  If it touches any of these walls, this path is over. give
     this block a layout = 12;
     */
    if( (entry % maze_x_length) == 0){ //right
        maze[entry][0] = 12;
        return_info2[0] = 0; //branch is finished
        return 1;
    }
    if( (entry / maze_x_length) + 1 == maze_z_length){ //far1
        maze[entry][0] = 12;
        return_info2[0] = 0;
        return 1;
    }
    if( (entry / maze_x_length) == 0){ //close
        maze[entry][0] = 12;
        return_info2[0] = 0;
        return 1;
    }
	if ((entry % maze_x_length) + 1 == maze_x_length){ //left for bugs
		maze[entry][0] = 14;
		return_info2[0] = 0; //branch is finished
		return 1;
	}

    int choice = rand() % 2;
    switch(choice){
        case 0:
            if(adjacent_block_layout_checker(maze, entry, 0, 1, 0, 0)){
                maze[entry][0] = 6;
                return_info2[0] = 1;
                return_info2[1] = 1;
                break;
            }
        case 1:
            if(adjacent_block_layout_checker(maze, entry, 0, 0, 0, 1)){
                maze[entry][0] = 7;
                return_info2[0] = 1;
                return_info2[1] = 3;
                break;
            }
        case 2:
            if(adjacent_block_layout_checker(maze, entry, 0, 0, 1, 0)){
                maze[entry][0] = 10;
                return_info2[0] = 1;
                return_info2[1] = 4;
                break;
            }
        default:
            maze[entry][0] = 12;
            return_info2[0] = 0;
            return_info2[1] = 0;
    }
    return 1;
}

int third_pass_entrance_right(float maze[][3], int entry, int return_info2[2]){
    
    //is the new block already assigned? - do nothing
    if(maze[entry][0] != -1){
        return_info2[0] = 0;
        return 0;
    }
    
    /*
     outer wall check, can only attempt to leave maze through the left, far1,
     close walls.  If it touches any of these walls, this path is over. give
     this block a layout = 14;
     */
    if( (entry % maze_x_length) + 1 == maze_x_length){ //left
        maze[entry][0] = 14;
        return_info2[0] = 0; //done with branch
        return 1;
    }
    if( (entry / maze_x_length) + 1 == maze_z_length){ //far1
        maze[entry][0] = 14;
        return_info2[0] = 0;
        return 1;
    }
    if( (entry / maze_x_length) == 0){ //close
        maze[entry][0] = 14;
        return_info2[0] = 0;
        return 1;
    }
	if ((entry % maze_x_length) == 0){ //right for bugs
		maze[entry][0] = 12;
		return_info2[0] = 0; //done with branch
		return 1;
	}
    
    //randomly choose the layout for this block, if the choosen value does
    //not work then we fall through to the next case and try again
    int choice = rand() % 2;
    switch(choice){
        case 0: //exit far1
            if(adjacent_block_layout_checker(maze, entry, 0, 0, 0, 1)){
                maze[entry][0] = 5;
                return_info2[0] = 1;
                return_info2[1] = 3;
                break;
            }
        case 1: //exit left
            if(adjacent_block_layout_checker(maze, entry, 1, 0, 0, 0)){
                maze[entry][0] = 6;
                return_info2[0] = 1;
                return_info2[1] = 2;
                break;
            }
        case 2: //exit close
            if(adjacent_block_layout_checker(maze, entry, 0, 0, 1, 0)){
                maze[entry][0] = 9;
                return_info2[0] = 1;
                return_info2[1] = 4;
                break;
            }
        default:
            maze[entry][0] = 12;
            return_info2[0] = 0;
            return_info2[1] = 0;
            
    }
    return 1;
}

int third_pass_entrance_close(float maze[][3], int entry, int return_info2[2]){
  
    //is the new block already assigned? - do nothing
    if(maze[entry][0] != -1){
        return_info2[0] = 0;
        return_info2[1] = 0;
        return 0;
    }
    
    /*
     outer wall check, can only attempt to leave maze through the right, left,
     far1, walls.  If it touches any of these walls, this path is over. give
     this block a layout = 11;
     */
    if( (entry % maze_x_length) == 0){ //right
        maze[entry][0] = 11;
        return_info2[0] = 0;
        return 1;
    }
    if( (entry / maze_x_length) + 1 == maze_z_length){ //far1
        maze[entry][0] = 11;
        return_info2[0] = 0;
        return 1;
    }
    if( (entry % maze_x_length) + 1 == maze_x_length){ //left
        maze[entry][0] = 11;
        return_info2[0] = 0;
        return 1;
    }
	if ((entry / maze_x_length) == 0){ //close for bugs
		maze[entry][0] = 13;
		return_info2[0] = 0;
		return 1;
	}
    
    //randomly choose the layout for this block, if the choosen value does
    //not work then we fall through to the next case and try again
    int choice = rand() % 2;
    switch(choice){
        case 0: //leave far1
            if(adjacent_block_layout_checker(maze, entry, 0, 0, 0, 1)){
                maze[entry][0] = 8;
                return_info2[0] = 1;
                return_info2[1] = 3;
                break;
            }
        case 1: //exit right
            if(adjacent_block_layout_checker(maze, entry, 0, 1, 0, 0)){
                maze[entry][0] = 9;
                return_info2[0] = 1;
                return_info2[1] = 1;
                break;
            }
        case 2: //exit left
            if(adjacent_block_layout_checker(maze, entry, 1, 0, 0, 0)){
                maze[entry][0] = 10;
                return_info2[0] = 1;
                return_info2[1] = 2;
                break;
            }
        default:
            maze[entry][0] = 11;
            return_info2[0] = 0;
            return_info2[1] = 0;
    }
    return 1;
}

int third_pass_entrance_far1(float maze[][3], int entry, int return_info2[2]){
    
    //is the new block already assigned? - do nothing
    if(maze[entry][0] != -1){
        return_info2[0] = 0;
        return_info2[1] = 0;
        return 0;
    }
    
    /*
     outer wall check, can only attempt to leave maze through the right, left,
     close walls.  If it touches any of these walls, this path is over. give
     this block a layout = 13;
     */
    if( (entry % maze_x_length) == 0){ //right
        maze[entry][0] = 13;
        return_info2[0] = 0;
        return_info2[1] = 0;
        return 1;
    }
    if( (entry / maze_x_length) == 0){ //close
        maze[entry][0] = 13;
        return_info2[0] = 0;
        return_info2[1] = 0;
        return 1;
    }
    if( (entry % maze_x_length) + 1 == maze_x_length){ //left
        maze[entry][0] = 13;
        return_info2[0] = 0;
        return_info2[1] = 0;
        return 1;
    }
	if ((entry / maze_x_length) + 1 == maze_z_length){ //far1 for bugs
		maze[entry][0] = 11;
		return_info2[0] = 0;
		return_info2[1] = 0;
		return 1;
	}
    
    //randomly choose the layout for this block, if the choosen value does
    //not work then we fall through to the next case and try again
    int choice = rand() % 2;
    switch(choice){
        case 0://exit right
            if(adjacent_block_layout_checker(maze, entry, 0, 1, 0, 0)){
                maze[entry][0] = 5;
                return_info2[0] = 1;
                return_info2[1] = 1;
                break;
            }
        case 1: //exit left
            if(adjacent_block_layout_checker(maze, entry, 1, 0, 0, 0)){
                maze[entry][0] = 7;
                return_info2[0] = 1;
                return_info2[1] = 2;
                break;
            }
        case 2: //exit close
            if(adjacent_block_layout_checker(maze, entry, 0, 0, 1, 0)){
                maze[entry][0] = 8;
                return_info2[0] = 1;
                return_info2[1] = 4;
                break;
            }
        default:
            maze[entry][0] = 13;
            return_info2[0] = 0;
            return_info2[1] = 0;
    }
    return 1;
}

/*
    adds the 1 sided wall to the outer wall of the maze for every 
    entry that is not assigned
 */
void complete_outer_wall(float maze[][3]){
    int entry = 0;
    int outer_wall_blocks = maze_x_length;
    int i = 0;
    //close wall
    for(; i<outer_wall_blocks; i++){
        if(maze[entry][0] == -1){
            maze[entry][0] = 1;
        }
        entry = entry + 1;
    }
    //far1 wall
    entry = maze_x_length * (maze_z_length -1);
    for(i = 0; i<outer_wall_blocks; i++){
        if(maze[entry][0] == -1){
            maze[entry][0] = 3;
        }
        entry = entry + 1;
    }
    
    /*check the corners... if we just assigned a value to them then we need
     to change it to a corner   */
    if(maze[0][0] == 1)
        maze[0][0] = 7;
    if(maze[maze_x_length-1][0] == 1)
        maze[maze_x_length-1][0] = 5;
    if(maze[maze_x_length * (maze_z_length-1)][0] == 3)
        maze[maze_x_length * (maze_z_length-1)][0] = 10;
    if(maze[maze_x_length * (maze_z_length-1) + maze_x_length - 1][0] == 3)
        maze[maze_x_length * (maze_z_length-1) + maze_x_length - 1][0] = 9;
    
    //left wall
    outer_wall_blocks = maze_z_length;
    entry = maze_x_length - 1;
    for(i = 0; i<outer_wall_blocks; i++){
        if(maze[entry][0] == -1){
            maze[entry][0] = 2;
        }
        entry = entry + maze_x_length;
    }
    //right wall
    entry = 0;
    for(i = 0; i<outer_wall_blocks; i++){
        if(maze[entry][0] == -1){
            maze[entry][0] = 4;
        }
        entry = entry + maze_x_length;
    }
}

/*
    Method takes a maze block and the directions the user wishes to leave the
    the block.  Then checks that all possible directions align well with the
    adjacent blocks to the provided entry. Returns 0 if there is a contradiction
 */
int adjacent_block_layout_checker(float maze[][3], int entry, int left, int right, int close, int far1){
    
    int next_entry = 0;
    
    if(left){
        //leave current block to the left. So you enter the next block from the right.
        if(entry % maze_x_length + 1 != maze_x_length){
            next_entry = entry + 1;
            if(layout_possibility_checker(maze, next_entry, 0, 1, 0, 0) == 0 )
                return 0;
        }
    }
    if(right){
        //leave current block to the right. So you enter the next block from the left.
        if(entry % maze_x_length != 0){
            next_entry = entry - 1;
            if(layout_possibility_checker(maze, next_entry, 1, 0, 0, 0) == 0)
                return 0;
        }
    }
    if(close){
        //leave current block close. Enter next block from far1.
        next_entry = entry - maze_x_length;
        if(layout_possibility_checker(maze, next_entry, 0, 0, 0, 1) == 0)
            return 0;
    }
    if(far1){
        //leave current block far1. enter next block from close.
        next_entry = entry + maze_x_length;
        if(layout_possibility_checker(maze, next_entry, 0, 0, 1, 0) == 0)
            return 0;
    }
    //block openings match with the existing blocks... does placing the block prevent
    //surrounding blocks from entering this one?
    
    return 1; // good entry
}

/*
    method checks if the provided maze block entry can be entered from specified
    direction.
    e_left=1; we wish to enter this block from the left.
    if this block wall on the left side then the layout cannot be used.
 
    returns 0 if block layout cannot work
 */
int layout_possibility_checker(float maze[][3], int entry, int e_left, int e_right, int e_close, int e_far1){
    
    int adjacent_wall_layout = maze[entry][0];
    int adjacent_left    =0;
    int adjacent_right   =0;
    int adjacent_close   =0;
    int adjacent_far1     =0;
    
    //is it a part of the outer wall? -doesnt matter. other checks in place for that

    switch( adjacent_wall_layout ){
        case -1:
            adjacent_close = 0;
            adjacent_far1   = 0;
            adjacent_left  = 0;
            adjacent_right = 0;
            break;
        case 0:
            adjacent_close = 0;
            adjacent_far1   = 0;
            adjacent_left  = 0;
            adjacent_right = 0;
            break;
        case 1:
            adjacent_close = 1;
            adjacent_far1   = 0;
            adjacent_left  = 0;
            adjacent_right = 0;
            break;
        case 2:
            adjacent_close = 0;
            adjacent_far1   = 0;
            adjacent_left  = 1;
            adjacent_right = 0;
            break;
        case 3:
            adjacent_close = 0;
            adjacent_far1   = 1;
            adjacent_left  = 0;
            adjacent_right = 0;
            break;
        case 4:
            adjacent_close = 0;
            adjacent_far1   = 0;
            adjacent_left  = 0;
            adjacent_right = 1;
            break;
        case 5:
            adjacent_close = 1;
            adjacent_far1   = 0;
            adjacent_left  = 1;
            adjacent_right = 0;
            break;
        case 6:
            adjacent_close = 1;
            adjacent_far1   = 1;
            adjacent_left  = 0;
            adjacent_right = 0;
            break;
        case 7:
            adjacent_close = 1;
            adjacent_far1   = 0;
            adjacent_left  = 0;
            adjacent_right = 1;
            break;
        case 8:
            adjacent_close = 0;
            adjacent_far1   = 0;
            adjacent_left  = 1;
            adjacent_right = 1;
            break;
        case 9:
            adjacent_close = 0;
            adjacent_far1   = 1;
            adjacent_left  = 1;
            adjacent_right = 0;
            break;
        case 10:
            adjacent_close = 0;
            adjacent_far1   = 1;
            adjacent_left  = 0;
            adjacent_right = 1;
            break;
        case 11:
            adjacent_close = 0;
            adjacent_far1   = 1;
            adjacent_left  = 1;
            adjacent_right = 1;
            break;
        case 12:
            adjacent_close = 1;
            adjacent_far1   = 1;
            adjacent_left  = 0;
            adjacent_right = 1;
            break;
        case 13:
            adjacent_close = 1;
            adjacent_far1   = 0;
            adjacent_left  = 1;
            adjacent_right = 1;
            break;
        case 14:
            adjacent_close = 1;
            adjacent_far1   = 1;
            adjacent_left  = 1;
            adjacent_right = 0;
            break;
        case 15:
            adjacent_close = 1;
            adjacent_far1   = 1;
            adjacent_left  = 1;
            adjacent_right = 1;
            break;
        default:
            adjacent_close = 0;
            adjacent_far1   = 0;
            adjacent_left  = 0;
            adjacent_right = 0; 
    }
    
    if(e_left){
        if(adjacent_left == 1)
            return 0;
    }
    if(e_right){
        if(adjacent_right == 1)
            return 0;
    }
    if(e_close){
        if(adjacent_close == 1)
            return 0;
    }
    if(e_far1){
        if(adjacent_far1 == 1)
            return 0;
    }
    
    //good entry
    return 1;
}