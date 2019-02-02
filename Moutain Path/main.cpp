#include <iostream>
#include <fstream>
#include "functions.h"
using namespace std;

// Normally you should not use global variables. However, the stack is
//   limited in size and will not allow arrays of this size. We'll learn
//   later how to put these arrays on the heap instead. Regardless, for now
//   we'll use these and treat them as if they were declared in main. So
//   when used in functions, we'll pass them as we would any other array.
static int elevations[MAX_ROWS][MAX_COLS];
static Pixel image[MAX_ROWS][MAX_COLS];

int main() {
    int rows = 0;
    int cols = 0;
    char fileName[400];

    //reading in rows and cols and catching errors
    if(cin >> rows && cin >> cols){
        if(rows <= 0 || cols <= 0){//if row or col is negative
            cout << "Error: Problem reading in rows and columns.";
            return 1;//end the program
        }else if(rows > MAX_ROWS || cols > MAX_COLS){//if row or col is bigger than the constant created for MAX_...
            cout << "Error: Problem reading in rows and columns.";
            return 1;//end the program
        }
    }
    else{
        cout << "Error: Problem reading in rows and columns.";//case for when row or col is nnot an integer
        return 1;//end the program
    }
    
    //loading file
    cin >> fileName;
    ifstream inData(fileName);
    if (inData.is_open()) {
        loadData(elevations, rows, cols, inData);
    }
    else{
        cout << "Error: Unable to open file " << fileName << ".";//error file does not exist
        return 1;//end the program
    }
    
    //printing pixel file
    int max = elevations[0][0];//set max and min as the first value of the 2d array
    int min = elevations[0][0];
    findMaxMin(elevations, rows, cols, max, min);
    
    //concatenate the .ppm unto the name of the input file
    
    
    loadGreyscale(image, elevations, rows, cols, max, min);
    
    Pixel red(252,25,63);
    Pixel green(31,253,13);
    int pathLen[rows];
    
    int minPath = colorPath(elevations,image, rows, cols, red, 0);
    int minIndex = 0;
    
    for(int i =0; i < rows; i++){
        pathLen[i] = colorPath(elevations,image, rows, cols, red, i);//set all paths to red
        if(pathLen[i] < minPath){
            minPath = pathLen[i];
            minIndex = i;
        }
    }
    
    colorPath(elevations,image, rows, cols, green, minIndex);//set smallest path to green
    
    char ofileName[100];
    strcpy(ofileName,fileName);
    strcat(ofileName,".ppm");
    //outputing the .ppm file
    ofstream outData(ofileName);
    if(outData.is_open()){
        outputImage(image, rows, cols, outData);
    }
    return 0;
}
