#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const int false=0;
const int true=1;

const int _sub_grid_size=3;
const int _grid_size=9;
const char input_filename[] = "res/input.txt";
const char output_filename[] = "res/output.txt";

void print_grid(int grid[_grid_size][_grid_size]){
  int row,col;
  for(row=0;row<_grid_size;row++){
    for(col=0;col<_grid_size;col++){
      if(grid[row][col]==0)
        printf("_");
      else
        printf("%d",grid[row][col]);
    }
    printf("\n");
  }
  for(col=0;col<_grid_size;col++)
    printf("*");
  printf("\n");
}

/**
 * @param grid : 2D array of the grid
 * @param row,col : index of the target cell in grid
 * @param possible_numbers : pointer to the 'output' array, to reduce memory usage
 * @return number of possible_numbers
 *
 * put all number to the array, then set impossible number to zero
*/
int find_possible_numbers(int grid[_grid_size][_grid_size], int row, int col, int possible_numbers[_grid_size]){
  /* return cell content directly if the cell is non-empty */
  if(grid[row][col]!=0){
    possible_numbers[0]=grid[row][col];
    return 1;
  }else{
    int r,c,num;
    /* init possible number */
    for(num=0;num<_grid_size;num++)
      possible_numbers[num]=num+1;
//    printf("\npossible_numbers:");
//    for(num=0;num<_grid_size;num++)
//        printf("%d",possible_numbers[num]);
//    printf("\n");
    /* remove impossible number */
    for(num=1;num<=_grid_size;num++)
      for(r=0;r<_grid_size;r++)
        for(c=0;c<_grid_size;c++){
          /* skip checking itself */
          if(c==col && r==row) continue;
          /* if same row or same col */
          if((r==row || c==col) && grid[r][c]==num)
            possible_numbers[num-1]=0;
          /* if in same sub grid */
          if((r/_sub_grid_size)==(row/_sub_grid_size) && (c/_sub_grid_size)==(col/_sub_grid_size) && grid[r][c]==num)
            possible_numbers[num-1]=0;
          //TODO
        }
    /* move non-zero number into header and count number of possible number */
//    printf("\npossible_numbers:");
//    for(num=0;num<_grid_size;num++)
//        printf("%d",possible_numbers[num]);
//    printf("\n");
    int count=0;
    for(r=0;r<_grid_size;r++)
      if(possible_numbers[r]!=0)
        possible_numbers[count++]=possible_numbers[r];
//    printf("\npossible_numbers:");
//    for(num=0;num<_grid_size;num++)
//        printf("%d",possible_numbers[num]);
//    printf("\n");
    return count;
  }
}

void cls(){
  printf("\e[2J\e[H");
}

void gotoxy(int x,int y){
  printf("%c[%d;%df",0x1B,y,x);
}

int main(){
  //printf("pre testing:\n");
  //printf("true ^ true %d\n",true ^ true);
  //printf("true ^ false %d\n",true ^ false);
  //printf("false ^ false %d\n",false ^ false);
  printf("program start\n");

  printf("Reading from file\n");
  /* read from file without checking */
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  FILE * input_file = fopen(input_filename, "rt");
  int grid[_grid_size][_grid_size];
  int row = 0;
  int col = 0;
  while ((read = getline(&line, &len, input_file)) != -1 && row < _grid_size){
    for (col=0; col<_grid_size; col++) {
      char cell = line[col];
      if(cell>='1' && cell<='9'){
        grid[row][col] = (int)(cell-'0');
      }else if(cell=='_'){
        grid[row][col] = 0;
      }else{
        printf("\nError: input is not digit or underscore");
        return(EXIT_FAILURE);
      }
    }
    row += 1;
  }
  fclose(input_file);

  printf("input grid :\n");
  print_grid(grid);

  printf("solving the sudoku");
  int grid_solution[_grid_size][_grid_size];
  int solved = false;
  int possible_numbers[_grid_size][_grid_size][_grid_size];
  /* init array */
  memset(possible_numbers, 0, _grid_size*_grid_size*_grid_size*sizeof(int));
  /* attempt to find unique possible cell, if not found, use deep first search */
  int n_possible_numbers;
  memcpy(&grid_solution, &grid, sizeof grid);
  cls();
  int i;
  int offset=0;
  while (!solved){
    //TODO
    solved=true;
    for(row=0;row<_grid_size;row++)
      for(col=0;col<_grid_size;col++){
        /* skip non-empty cell */
        if(grid_solution[row][col]!=0)
          continue;
        n_possible_numbers = find_possible_numbers(grid_solution, row, col, possible_numbers[row][col]);
        if(n_possible_numbers==1){
          grid_solution[row][col]=possible_numbers[row][col][0];
//          printf(".");
        }else if(n_possible_numbers>0){
          //TODO
          solved=false;
//          printf("possible position (%d,%d) [%d]: ",row,col,n_possible_numbers);
//          for(i=0;i<n_possible_numbers;i++){
//            printf("%d",possible_numbers[row][col][i]);
//          }
          grid_solution[row][col]=possible_numbers[row][col][(offset+rand()) % n_possible_numbers];
//          printf(" --> %d\n",grid_solution[row][col]);
//          printf(".");
        }else{
          /* n==0 */
//          printf("\nError : there is conflict!");
          /* reset */
          //TODO restore to last safe stage (maybe use stack)
          memcpy(&grid_solution, &grid, sizeof grid);
          offset++;
          //exit(EXIT_FAILURE);
        }
      }
    gotoxy(0,0);
    printf("\ncurrent grid:\n");
    print_grid(grid_solution);
  }
  printf("\n");

  printf("solution found:\n");
  print_grid(grid_solution);

  printf("\nwriting to file:\n");
  FILE * output_file = fopen(output_filename, "w");
  for(row=0;row<_grid_size;row++){
    for(col=0;col<_grid_size;col++){
      fprintf(output_file,"%d",grid_solution[row][col]);
    }
    fprintf(output_file,"\n");
  }
  fclose(output_file);

  printf("program end\n");
  return(EXIT_SUCCESS);
}
