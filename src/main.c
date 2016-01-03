#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const int _grid_size=9;
const int false=0;
const int true=1;

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
    int r,c;
    int cell=grid[row][col];
    /* init possible number */
    for(r=0;r<_grid_size;r++)
      possible_numbers[r]=r+1;
    /* remove impossible number */
    for(r=0;r<_grid_size;r++)
        for(c=0;c<_grid_size;c++){
          /* if same row or same col, but not same cell */
          if(r==row ^ c==col && grid[r][c]==cell)
            possible_numbers[grid[r][c]]=0;
        }
    /* move non-zero number into header and count number of possible number */
    int count=0;
    for(r=0;r<_grid_size;r++)
      if(possible_numbers[r]!=0)
        possible_numbers[count++]=possible_numbers[r];
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
  printf("program start\n");

  printf("Reading from file\n");
  char input_filename[] = "res/input.txt";
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
  while (!solved){
    //TODO
    solved=true;
    for(row=0;row<_grid_size;row++)
      for(col=0;col<_grid_size;col++){
        n_possible_numbers = find_possible_numbers(grid_solution, row, col, possible_numbers[row][col]);
        if(n_possible_numbers==1){
          grid_solution[row][col]=possible_numbers[row][col][0];
          printf(".");
        }else if(n_possible_numbers>0){
          //TODO
          printf("possible position (%d,%d): ",row,col);
          for(i=0;i<n_possible_numbers;i++){
            printf("%d",possible_numbers[row][col][i]);
          }
          printf("\n");
          grid_solution[row][col]=possible_numbers[row][col][0];
          printf(".");
        }else{
          //n==0
          solved=false;
        }
      }
    gotoxy(0,0);
    printf("current grid:\n");
    print_grid(grid_solution);
  }
  printf("\n");

  printf("solution found:\n");
  print_grid(grid_solution);

  printf("program end\n");
  return(EXIT_SUCCESS);
}
