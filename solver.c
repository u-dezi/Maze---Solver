// I've replaced x and y abi y and x with the row no. and the column no.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include<windows.h>
#include <conio.h>

void move_scenerio(), Move(), crossroad();
void menu();


char fileName[40];
char fileSave[40];

int sizeMaze[2];
int startMaze[2];
int finishMaze[2];

int mazeSize;
int maze_row;
int maze_column;

int cur_loc[2]; // shows where we are in the maze
int last_move = 0;
int move_to_take = 0;
int no_of_moves = 0;
int move_count = 0;

int UP = 1;
int RIGHT = 2;
int DOWN = 3;
int LEFT = 4;

int BORDER = 1;
int NO_BORDER = 0;

int *soln;
int inc_cross_moves = 0;
int crossroad_count = -1;

int *border_code;
int loop_again = 0;
void menu()
    {
        int option;
        printf("\n Enter 1 to display ");
        printf("\n Enter 2 to solve the puzzle, \n **Note:** Option 2 terminate program \n");
        scanf("%d",&option);
        printf("\n");
        if (option == 1)
        {
            //system.
            display_maze();
            menu();
        }
        else
        {
            avail_mov();
        }
        //if()

    }


struct box
    {
        // co-ordinates
        int row;
        int column;

        // border values
        int top;
        int bottom;
        int right;
        int left;

        int cross_moves;
    }**maze, cur_box, *CRs, **maze_toPrint; // maze array
//maze[maze_row][maze_column], cur_box, CRs[100]/*should be maze size*/, maze_toPrint[maze_row][maze_column]; // maze array

//function list to solve the puzzle
//#include<string.h>
void translate() // takes border_code[][] gets maze[][]
    {
       int trans_row, trans_column; // might as well be i and j or any name for that matter

       int border_position = 0;     //this tells us which border we are translating

       for(trans_row = 0; trans_row < maze_row; trans_row++)
       {
            for(trans_column = 0; trans_column < maze_column; trans_column++)
            {
                maze[trans_row][trans_column].row = trans_row;
                maze[trans_row][trans_column].column = trans_column;

               // top
               if(border_code[border_position] >= 8)
               {
                    maze[trans_row][trans_column].top = 1; // 1 means border, 0 means empty space
                    border_code[border_position]-=8;
               }

               // left
               if(border_code[border_position] >= 4)
               {
                     maze[trans_row][trans_column].left = 1;
                     border_code[border_position]-=4;
               }

               // bottom
               if(border_code[border_position] >= 2)
               {
                     maze[trans_row][trans_column].bottom = 1;
                     border_code[border_position]-=2;
               }

               // right
               if(border_code[border_position] >= 1)
               {
                     maze[trans_row][trans_column].right = 1;
                     border_code[border_position]-=1;
               }
               border_position++;
            }
       }
    }

 void addImgBorder()
{
    if(last_move == UP)
    {
        cur_box.bottom = 1;
    }
    if(last_move == RIGHT)
    {
        cur_box.left = 1;
    }
    if(last_move == DOWN)
    {
        cur_box.top = 1;
    }
    if(last_move == LEFT)
    {
        cur_box.right = 1;
    }
}

void cr_addImgBorder()
    {
             //this blocks entry into a crossroad in the actual maze
            if(last_move == UP)
            {
                //maze[][].bottom = 1;
                maze[cur_loc[0]][cur_loc[1]].bottom = 1;
                //printf("\n blocked bottom");
            }
            if(last_move == RIGHT)
            {
                //cur_box.left = 1;
                maze[cur_loc[0]][cur_loc[1]].left = 1;
                //printf("\n blocked left");
            }
            if(last_move == DOWN)
            {
                maze[cur_loc[0]][cur_loc[1]].top = 1;
                //printf("\n blocked top");
            }
            if(last_move == LEFT)
            {
                //cur_box.right = 1;
                maze[cur_loc[0]][cur_loc[1]].right = 1;
                //printf("\n blocked right");
            }
            //this blocks entry into a crossroad in the actual maze


        if(move_to_take == UP)
        {
            CRs[crossroad_count].top = 1;
            //maze[cur_loc[0]][cur_loc]].top = 1
            maze[cur_loc[0]][cur_loc[1]].top = 1;
        }
        if(move_to_take == RIGHT)
        {
             CRs[crossroad_count].right = 1;
             maze[cur_loc[0]][cur_loc[1]].right = 1;
        }
        if(move_to_take == DOWN)
        {
             CRs[crossroad_count].bottom = 1;
             maze[cur_loc[0]][cur_loc[1]].bottom = 1;
        }
        if(move_to_take == LEFT)
        {
             CRs[crossroad_count].left = 1;
             maze[cur_loc[0]][cur_loc[1]].left = 1;
        }
    }

void pick_move()
    {
        //printf(" \nCur_box values: up: %d right %d bottom %d left %d row col %d %d ",
        //          cur_box.top, cur_box.right, cur_box.bottom, cur_box.left, cur_box.row,cur_box.column);

        if(cur_box.top == NO_BORDER)
        {
            move_to_take = UP;
        }
        else if (cur_box.right == NO_BORDER)
        {
            move_to_take = RIGHT;
        }
        else if (cur_box.bottom == NO_BORDER)
        {
            move_to_take = DOWN;
        }
        else if (cur_box.left == NO_BORDER)
        {
            move_to_take = LEFT;
        }
        //printf("\nThe move to take from pickmove is %d", move_to_take);

        //printf("\n Move_to_take is now : %d \n",move_to_take );

    }

void count_move()
    {
        no_of_moves = 0;
        int border[4];

        border[0] = cur_box.top;
        border[1] = cur_box.right;
        border[2] = cur_box.bottom;
        border[3] = cur_box.left;
        int i;
        for(i=0; i <= 3; i++)
        {
            if(border[i] == NO_BORDER)
            {
                no_of_moves++;
            }

        }
   // printf("\n no_of_moves  is now : %d \n",no_of_moves );
    }

void hasItFinished()
    {
        if((cur_loc[0] == finishMaze[0]) && (cur_loc[1] == finishMaze[1]) )
        {
           //Creating a file point to write the solution to it using fileSave
           FILE *ffptr;
           ffptr=fopen(fileSave,"w");

           if(ffptr==NULL)
            {
              printf("Error!: Cant create a file ");
              exit(1);
           }

            printf("\nSolution: ");
            int i;
            // Print to both screen and file at the same time
          for( i = 0; i < move_count; i++)
          {
            if(soln[i] == UP)
            {
                printf("U ");
                fprintf(ffptr,"U ");
            }
            else if(soln[i] == RIGHT)
            {
                printf("R ");
                fprintf(ffptr,"R ");
            }
            else if(soln[i] == DOWN)
            {
                printf("D ");
                fprintf(ffptr,"D ");
            }
            else if(soln[i] == LEFT)
            {
                printf("L ");
                fprintf(ffptr,"L ");
            }
          }
          printf("\n");
          printf("Solution is in %s \n",fileSave);

          fclose(ffptr);
          Sleep(10000);
           exit(0);
        }
    }

void Move()
    {
        //move to take is top
        //printf("\n Previous location is %d%d",cur_loc[0],cur_loc[1]);
        if(move_to_take == UP)
        {
           cur_loc[0] -= 1;
          // cur_loc[1] =
        }
        else if (move_to_take == RIGHT)
        {
            //cur_loc[0]
            cur_loc[1] += 1;
        }
        else if (move_to_take == DOWN)
        {
            cur_loc[0] += 1;
            //cur_loc[1] =- 1
        }
        else if (move_to_take == LEFT)
        {
            //cur_loc[0]
            cur_loc[1] -= 1;
        }
        //printf("\n Current location is %d%d",cur_loc[0],cur_loc[1]);

        //end of actual movement

        soln[move_count] = move_to_take;
        last_move = move_to_take;
        move_count++;
        //checks whether any cross roads are available
        if(inc_cross_moves == 1)
        {
            CRs[crossroad_count].cross_moves++;
        }
        hasItFinished();

        /*if()
        {

        }*/
    }

 void avail_mov()
    {
        cur_box = maze[cur_loc[0]][cur_loc[1]];
        //print the values of cur_box
       // printf(" \nCur_box values: up: %d right %d bottom %d left %d row col %d %d ",
        //        cur_box.top, cur_box.right, cur_box.bottom, cur_box.left, cur_box.row,cur_box.column);
        /*
        if(last_move) = 4; //moves left
        addImgBorder(); //adds border right

        last_move = 1; // moves up
        addImgBorder();// adds border bottom
        /*
        last_move = 2; // moves right
        addImgBorder();
        */
        addImgBorder();
        pick_move();
        count_move();
        move_scenerio();
    }

void crossroad()
    {
        crossroad_count++;
        CRs[crossroad_count] = cur_box;
        while(1)
        {
           cur_box = CRs[crossroad_count];
           pick_move();
           inc_cross_moves = 1;
           cr_addImgBorder();
           Move();

           avail_mov();
        }
    }



void move_scenerio()
    {
        // take_only_one_move
        //printf("\n No of moves is %d",no_of_moves);
        if(no_of_moves == 1)
        {
           // printf("\nBefore move Cur_loc0 is %d cur_loc1 is %d", cur_loc[0],cur_loc[1]);
            Move();
            //printf("\nOnly one move");
           // printf("\nAfter move Cur_loc0 is %d cur_loc1 is %d", cur_loc[0],cur_loc[1]);
            //Sleep(1000);
            avail_mov();
        }
        // crossroad
        else if(no_of_moves > 1)
        {
            int sa;
            //printf("\nI am a cross road");

            //Sleep(1000);
            crossroad();
        }
        else
        {
            //no_Available();
                loop_again = 1;
                while (loop_again == 1)
                {
                    //printf("\n ****I am a dead end*****");
                    move_count -= CRs[crossroad_count].cross_moves;
                    CRs[crossroad_count].cross_moves = 0;
                    cur_box = CRs[crossroad_count];

                    count_move();
                    loop_again = 0;
                    //Sleep(1000);
                    if(no_of_moves == 0)
                    {
                        crossroad_count -- ;
                        loop_again = 1;
                        //printf("\ncross rofdad dead end");
                    }
                    else
                    {
                        cur_loc[0] = CRs[crossroad_count].row;
                        cur_loc[1] = CRs[crossroad_count].column;
                    }

                }
        }
    }
void display_maze()
{
     int r =0;
    int c =0;

    for(r = 0; r < maze_row; r++)
    {
        for(c = 0; c < maze_column; c++)
        {
            maze_toPrint[r][c] = maze[r][c];
        }
    }


/*
    printf("\n Showing the values in struct box \n");
    for(r=0; r< maze_row;r++)
    {

        for(c=0; c< maze_column; c++)
        {
            printf("\n The index : [%d][%d] :  \t",r,c );
            printf("The border values:  %d %d %d %d cr %d",
                    maze[r][c].top,maze[r][c].right, maze[r][c].bottom, maze[r][c].left,maze[r][c].cross_moves);


        }
    }
*/
// Code to print the maze
    int countt = 0;

    // loop to trick display
    for(r = 1; r < maze_row; r++)
    {
        for(c = 0; c < maze_column; c++)
        {
            if(maze_toPrint[r][c].top == BORDER)
            {
                maze_toPrint[r-1][c].bottom = BORDER;
            }
        }
    }

   // printf("\n");
    for(countt = 0; countt < maze_column; countt++)
    {
        if(countt == (maze_column-1))
        {
             printf(".__.\n");
        }
        else
        {
             printf(".__");
           //  printf("");
        }
    }

    for(r = 0; r < maze_row; r++)
    {
        for(c = 0; c < maze_column; c++)
        {
            if(maze_toPrint[r][c].left == BORDER)
            {
                if(c == 0)
                {
                    printf("|");
                }
                if((c != 0) && (maze_toPrint[r][c-1].right != BORDER))
                {
                    printf("|");
                }
            }
            else
            {
                if(maze_toPrint[r][c-1].right != NO_BORDER)
                {
                    printf(".");
                }
            }

            if(maze_toPrint[r][c].bottom == BORDER)
            {
                printf("__");
            }
            else
            {
                printf("  ");
            }


            if(maze_toPrint[r][c].right == BORDER)
            {
                printf("|");
            }
            else
            {
                printf(".");
            }
        }
        printf("\n");
    }
    // MAZE DISPLAY ENDS HERE

}

int main()
{
    FILE *myFile;

    printf("\nEnter name of file with extension e.g. test.txt\n");
    scanf("%s",fileName);

    printf("\nEnter  file name for solution will create and save to this file e.g. test.txt\n");
    scanf("%s",fileSave);
    //printf("\n%s",fileSave);

    myFile = fopen(fileName, "r");

    if (myFile == NULL)
    {
        printf("Error Reading File\n");
        exit (0);
    }

    fscanf(myFile, "%d,", &maze_row);
    fscanf(myFile, "%d,", &maze_column);
    fscanf(myFile, "%d,", &startMaze[0]);
    fscanf(myFile, "%d,", &startMaze[1] );
    fscanf(myFile, "%d,", &finishMaze[0] );
    fscanf(myFile, "%d,", &finishMaze[1] );

    mazeSize = maze_row * maze_column;
    // stores the border values


    //create dependent arrays with collected values
    border_code =  malloc(sizeof(int) * mazeSize);

    soln =  malloc(sizeof(int) * mazeSize);

    CRs =  malloc(sizeof(struct box) * mazeSize);

    int q;

    maze = (struct box**) malloc(maze_row*sizeof(struct box*));
    maze_toPrint = (struct box**) malloc(maze_row*sizeof(struct box*));

    for (q = 0; q < maze_row; q++)
    {
        maze[q] = (struct box*) malloc(maze_column*sizeof(struct box));
        maze_toPrint[q] = (struct box*) malloc(maze_column*sizeof(struct box));
    }
    // ahjdcbsikjhcbsdjkiacbakj
    int i = 0;
    int j = 0;
    for (i = 0; i <= mazeSize - 1; i++)
    {
        fscanf(myFile, "%d,", &border_code[i]);

    }
    fclose(myFile);

    // Shows the border values, it is a test
    /*
    for (i = 0; i <= mazeSize - 1; i++)
    {
        printf("\n no: %d num: %d",i, border_code[i]);

    }
    */


    int col = 0;
    int row = 0;
    //Set everything to 0 in the maze,  ;
    for(row=0; row< maze_row;row++)
    {
        for(col=0; col< maze_column; col++)
        {
            maze[row][col].top = 0;
            maze[row][col].right = 0;
            maze[row][col].bottom = 0;
            maze[row][col].left = 0;
            maze[row][col].cross_moves = 0;
            // to print to show that it is working fine
            /*
            printf("\n The index : [%d][%d] :  \t",row,col );
            printf("The border values:  %d %d %d %d cr %d ",
                   maze[row][col].top,maze[row][col].right, maze[row][col].bottom, maze[row][col].left,maze[row][col].cross_moves = 0);
            */
        }
    }

    translate();

    // print Everything to be sure, to be removed later
    //MAZE DISPLAY


    /*
    Start of solution part
    */
    // setting the start location  to the start moves
    cur_loc[0] = startMaze[0];
    cur_loc[1] = startMaze[1];

    //function list

    /*Testing has it finished
    cur_loc[0] = finishMaze[0];
    cur_loc[1] = finishMaze[1];

    hasItFinished();
    end test*/
    menu();




    //return 0;
}

