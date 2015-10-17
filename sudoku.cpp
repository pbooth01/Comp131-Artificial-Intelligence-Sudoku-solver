/*
 * Tufts Comp 131 Backtracking Assignment
 * Framework for sudoku solver by Anselm Blumer, 26 February, 2015
 * Should be able to handle 9x9, 16x16, and 25x25 (but you may not
 * want to wait for the larger sizes to finish)
 */

#include <iostream>
#include <fstream>
#include <cmath>
#include <math.h>
#include <cstdlib>

using namespace std;

#define MAXSIZE 25

/*
 * Globals:
 * grid is the sudoku grid
 * legal[i][j][k] is 1 if k+1 is a legal value for grid[i][j], 0 if illegal
 *    k+1 since arrays are 0-based
 * size is the edge length of the sudoku grid, 9, 16, or 25
 *    specified on the first line of the input file
 * blocksize is the size of the sub-blocks, sqrt( size )
 */

int grid[MAXSIZE][MAXSIZE];
int legal[MAXSIZE][MAXSIZE][MAXSIZE];
int size, blocksize;
int BTcounter;
int IVcounter;

/*
 * GetGrid reads the sudoku grid values from the file specified by its parameter
 * Empty cells are zeroes in the file, givens are 1-9 in 9x9 grids, a-p in 16x16,
 * and a-y in 25x25.  Entries can be separated by blanks.
 */

void GetGrid( char * filename ) {

ifstream inFile( filename );
if ( !inFile ) {
  cerr << "Cannot open " << filename << "\n";
  exit( 1 );
}
inFile >> size;
blocksize = round( sqrt( size ) );
cout << "size = " << size << '\n';
int lowchar = '1';
if (size > 9) { lowchar = 'a'; }
char c = ' ';
while (c != '\n') { c = inFile.get(); }
for (int i=0; i<size; i++) {
  for (int j=0; j<size; j++) {
    c = inFile.get();
	if (c == ' ') { c = inFile.get(); }
	if (c == '0') { grid[i][j] = 0; }
	else {	
	  if ((c < lowchar) || (c > (lowchar+size-1))) {
	    cerr << c << " :Invalid char in file\n";
	    exit( 1 );
	  }
	  grid[i][j] = 1 + c - lowchar;
    }
  }
  while (c != '\n') { c = inFile.get(); }
}
}

/*
 * PrintGrid prints the sudoku grid.  Empty cells print as dots.
 */
void PrintGrid( void ) {

int lowchar = '1';
if (size > 9) { lowchar = 'a'; }
for (int i=0; i<size; i++) {
  for (int j=0; j<size; j++) {
    if (grid[i][j] == 0) { cout << ". "; }
	else {
	  char c = lowchar + grid[i][j] - 1;
	  cout << c << " ";
    }
  }
  cout << "\n";
}
}

/*
 * RemoveLegal sets entries in "legal" to 0 (false) for
 * values that occur in the same row, column, or sub-block
 */

void RemoveLegal( int r, int c ) {

int v = grid[r][c] - 1;
for (int i=0; i<size; i++) {
  legal[r][i][v] = 0;
  legal[i][c][v] = 0;
}
r = blocksize * (r/blocksize);
c = blocksize * (c/blocksize);
for (int i=r; i<(r+blocksize); i++) {
  for (int j=c; j<(c+blocksize); j++) {
    legal[i][j][v] = 0;
  }
}
}

/*
 * InitLegal initializes "legal" using RemoveLegal
 */

void InitLegal( void ) {

for (int i=0; i<size; i++) {
  for (int j=0; j<size; j++) {
    for (int k=0; k<size; k++) {
	  legal[i][j][k] = 1;
	}
  }
}
for (int i=0; i<size; i++) {
  for (int j=0; j<size; j++) {
    if (grid[i][j] > 0) {
	  RemoveLegal( i, j );
	}
  }
}
}

/*
 * CopyState saves or restores a copy of the grid when backtracking
 */

void CopyState( int g[MAXSIZE][MAXSIZE], int s[MAXSIZE][MAXSIZE] ) {

for (int i=0; i<size; i++) {
  for (int j=0; j<size; j++) {
    s[i][j] = g[i][j];
  }
}
}

//function finds the next open spot

void Nextopenspot(int &row, int &col){
        for(int i = 0; i < size; i++){  
          for(int j = 0; j < size; j++){
            if(grid[i][j] == 0){
              row = i;
              col = j;
              return;
            }
            
          }
        }
}
//function finds the number of open cells around specific coordinates
int NumSurrounding(int row, int col ){
  int TS = 0;
  if((row == 0) && (col == 0)){ // Top left corner
      if(grid[row][col + 1] == 0){
        TS++;
      }
      if(grid[row + 1][col] == 0){
        TS++;
      }
  }
   else if((row == 0) && (col == size -1)){ // Top right corner
      if(grid[row][col - 1] == 0){
        TS++;
      }
      if(grid[row + 1][col] == 0){
        TS++;
      }
  }
  else if((row == size -1) && (col == 0)){ // Bottom left
      if(grid[row][col + 1] == 0){
        TS++;
      }
      if(grid[row - 1][col] == 0){
        TS++;
      }
  }
    else if((row == size-1) && (col == size -1)){ // Bottom right corner
      if(grid[row][col - 1] == 0){
        TS++;
      }
      if(grid[row - 1][col] == 0){
        TS++;
      }
    }
  else if(row == 0){ //top row
    if(grid[row][col + 1] == 0){
      TS++;
      }
    if(grid[row + 1][col] == 0){
      TS++;
    }
    if(grid[row - 1][col] == 0){
      TS++;
    }
  }
  else if(row == size-1){ //bottom row
    if(grid[row][col - 1] == 0){
      TS++;
      }
    if(grid[row + 1][col] == 0){
      TS++;
    }
    if(grid[row - 1][col] == 0){
      TS++;
    }
  }
  else if(col == 0){ //Left side
    if(grid[row + 1][col] == 0){
      TS++;
      }
    if(grid[row -1][col] == 0){
      TS++;
    }
    if(grid[row][col + 1] == 0){
      TS++;
    }
  }
   else if(col == size-1){ //Left side
    if(grid[row + 1][col] == 0){
      TS++;
      }
    if(grid[row -1][col] == 0){
      TS++;
    }
    if(grid[row][col - 1] == 0){
      TS++;
    }
  }
  else{
    if(grid[row + 1][col] == 0){ //Right side
      TS++;
      }
    if(grid[row -1][col] == 0){
      TS++;
    }
    if(grid[row][col + 1] == 0){
      TS++;
    }
    if(grid[row][col - 1] == 0){
      TS++;
    }
  }
  return(TS);
}

//function chooses the cell with the most number of open
//spaces around it
void Degree(int &curmin, int &row, int &col){
  int tempnumsurrounding = 0;
  int curnumsurrounding= 0;

  int vals[MAXSIZE];
    for(int i = 0; i < size; i++){  
      for(int j = 0; j < size; j++){


        if(grid[i][j] == 0){
          for (int k=0; k<size; k++) { //initializes vals array to 0
            vals[k] = 0;
          }

          int v = 0;
          for (int k=0; k<size; k++) { //stores legal values in vals array
            if (legal[i][j][k]) { 
              v++;
            }
          }
          
          if(v = curmin){
            if(NumSurrounding(i, j) >= curnumsurrounding){
              curnumsurrounding = NumSurrounding(i, j);
              row = i;
              col = j;
            }
          }
        }
      }
    }
    return;
}

//Function Uses the MRV/MCV heuristic to choose the next open spot

void MRV(int &row, int &col){

        int curmin = size;
        int tempmin = size;
        int vals[MAXSIZE];

        for(int i = 0; i < size; i++){  
          for(int j = 0; j < size; j++){


            if(grid[i][j] == 0){
              for (int k=0; k<size; k++) { //initializes vals array to 0
                vals[k] = 0;
              }

              int v = 0;
              for (int k=0; k<size; k++) { //stores legal values in vals array
                if (legal[i][j][k]) { 
                    v++;
                }
              }
              tempmin = v;
              if(tempmin < curmin){
                curmin = tempmin;
                row = i;
                col = j;
              }
              if(tempmin == curmin){
                Degree(curmin, row, col);
                row = i;
                col = j;
              }
            }
        }
      }
        return;
}
//Function Uses the MRV/MCV heuristic to choose the next open spot
//Doesnt work/is too slow
void LCV(int &row, int &col){

        int curmax = 0;
        int tempmax = 0;
        int vals[MAXSIZE];

        for(int i = 0; i < size; i++){  
          for(int j = 0; j < size; j++){


            if(grid[i][j] == 0){
              for (int k=0; k<size; k++) { //initializes vals array to 0
                vals[k] = 0;
              }

              int v = 0;
              for (int k=0; k<size; k++) { //stores legal values in vals array
                if (legal[i][j][k]) { 
                    v++;
                }
              }
              tempmax = v;
              if(tempmax >= curmax){
                curmax = tempmax;
                row = i;
                col = j;
              }
              //if(tempmax == curmax){
                //Degree(curmax, row, col);
                //row = i;
                //col = j;
              //}
            }
        }
      }
        return;
}

// function looks through the grid to see if there are still open cells

bool Finished(){
  for(int i = 0; i < size; i++){
    for (int j = 0; j < size; j++){
      if(grid[i][j] == 0){
        return false;
      }
    }
  }
  return true;

}

/*
 * Outline for the backtracking solver
 * A return value of 0 means a successful solution, 1 means
 * backtracking is necessary
 */

//Fills all cells that have only one legal value
void SinglelegalFill(void){
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      int numlegvals = 0;
      for(int k = 0; k < size; k++){
        if(legal[i][j][k]){
          numlegvals++;
        }
      }
      if((numlegvals == 1) && (grid[i][j] == 0)){
        for(int k = 0; k < size; k++){
          if(legal[i][j][k]) {
            grid[i][j] = k + 1;
            RemoveLegal(i,j);
            SinglelegalFill();
          }
        }
      }
    }
  }
}


int Backtrack(void) {

BTcounter++;
int savegrid[MAXSIZE][MAXSIZE];  // local grid for backtracking
int vals[MAXSIZE];  // legal values for the current cell
int row = 0;
int col = 0;

// fill in code here to set i, j to the next cell to try,
// or return 1 if all cells are filled (shouldn't get here
// if grid is solved)
      
      if(Finished() == true){
        return 0;
      }

      else { //if not finished

        //traversing the grid
        //Nextopenspot(row, col);
        MRV(row, col);
        //LCV(row, col);

// The following segment finds the legal values for grid[i][j]
// You should add code to order the vals array according to a heuristic

        for (int k=0; k<size; k++) { //initializes vals array to 0
          vals[k] = 0;
        }

        int v = 0;
          for (int k=0; k<size; k++) { //stores legal values in vals array
            if (legal[row][col][k]) { 
              vals[v] = k+1;
              v++;
            }
          }

        if (v == 0) { //no legal values
          return 1;
        }

        // Backtracking below this
        //CopyState( grid, savegrid );  // Save grid so multiple values can be tried
        for (int k=0; k<v; k++) {
          if (vals[k] == 0) {
            return 1;  // ran out of values to try - need to backtrack
          } 
          else {
            grid[row][col] = vals[k];
            IVcounter++;
            RemoveLegal(row,col);

            if(Backtrack() == 0){
              return 0;
            } 
            else{          
              grid[row][col] = 0;
              InitLegal();
            } 
          } 
          }         
        return 1;
      }
}


int main (int argc, char * const argv[]) {

GetGrid( argv[1] );
cout << "Sudoku grid\n";
PrintGrid();
InitLegal();
//SinglelegalFill();
// You may want to call a function here to make easy inferences
// In some cases this may solve it without backtracking
Backtrack();
//cout << "\n";
PrintGrid();
cout << "\n";
cout << "BTcounter: "<< BTcounter;
cout << "\n";
cout << "IVcounter: "<< IVcounter << "\n";
}
