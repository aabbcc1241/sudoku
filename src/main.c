#include <stdio.h>

const int _grid_size=9;

int main(){
  printf("program start\n");

  printf("Reading from file\n");
  char input_filename[] = "res/input.txt";
  /* read from file without checking */
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  FILE * input_file = fopen(input_filename, "rt");
  char * grid_pointers[_grid_size][_grid_size];
  int row = 0;
  int col = 0;
  while ((read = getline(&line, &len, input_file)) != -1 && row < _grid_size){
//    printf("%s", line);
    for (col=0; col<_grid_size; col++) {
//      printf("%c",line[col]);
      char cell = line[col];
      grid_pointers[row][col] = &cell;
    }
//    printf("\nfinished row-%d\n\n",row);
    row += 1;
  }

  printf("input grid :\n");
  for(row=0;row<_grid_size;row++){
    for(col=0;col<_grid_size;col++){
      printf("%c",*grid_pointers[row][col]);
    }
    printf("\n");
  }

  printf("program end\n");
  return 0;
}