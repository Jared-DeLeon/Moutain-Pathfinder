#include <iostream>
#include <fstream>
#include <cmath>
#include "functions.h"
using namespace std;

int colorPath(const int elevations[MAX_ROWS][MAX_COLS], Pixel image[MAX_ROWS][MAX_COLS], int rows, int cols, Pixel color, int start_row){
    int lengthOfPath = 0;//pathlength counter
    int currentCol = 0;//start here
    int adjust = 0;//which row to go next
    int currentRow = start_row;//start here
    int currentElevation = elevations[currentRow][currentCol];//instantiate currentElevation
    image[currentRow][currentCol] = color;

    
    while(currentCol < cols-1){//until you hit the second to last column
        currentElevation = elevations[currentRow][currentCol];//set new current Elevation
        
        if(currentRow == 0){//if at top
            if(abs(elevations[currentRow][currentCol+1]-currentElevation)<=abs(elevations[currentRow+1][currentCol+1]-currentElevation)){
                lengthOfPath += abs(elevations[currentRow][currentCol+1]-currentElevation);//go mid
                adjust = 0;
            }else{
                lengthOfPath += abs(elevations[currentRow+1][currentCol+1]-currentElevation);//go bot
                adjust = 1;
            }//ignore top
        
        }else if(currentRow == rows-1){//if at bot
            if(abs(elevations[currentRow][currentCol+1]-currentElevation) <= abs(elevations[currentRow-1][currentCol+1]-currentElevation)){
                lengthOfPath += abs(elevations[currentRow][currentCol+1]- currentElevation);//go mid
                adjust = 0;
            }else{
                lengthOfPath += abs(elevations[currentRow-1][currentCol+1]-currentElevation);//go top
                adjust = -1;
            }//ignore bot
        
        }else{// somewhere in the middle
            if(abs(elevations[currentRow+1][currentCol+1]-currentElevation) <= abs(elevations[currentRow-1][currentCol+1]-currentElevation)){//bot is smaller
                if(abs(elevations[currentRow][currentCol+1]-currentElevation)<=abs(elevations[currentRow+1][currentCol+1]-currentElevation)){//is mid smaller
                    lengthOfPath += abs(elevations[currentRow][currentCol+1]-currentElevation);//go mid
                    adjust = 0;
                }else{
                    lengthOfPath += abs(elevations[currentRow+1][currentCol+1]-currentElevation);//go bot
                    adjust = 1;
                }
            }else{//if top is smaller
                if(abs(elevations[currentRow][currentCol+1]-currentElevation)<=abs(elevations[currentRow-1][currentCol+1]-currentElevation)){
                    lengthOfPath += abs(elevations[currentRow][currentCol+1]-currentElevation);//go mid
                    adjust = 0;
                }else{
                    lengthOfPath += abs(elevations[currentRow-1][currentCol+1]- currentElevation);//go top
                    adjust = -1;
                }
                
            }
        }
        currentCol++;//go to next column
        currentRow = currentRow + adjust;//decide where to go next
        image[currentRow][currentCol] = color;// set new color of pixel
               
    }
    
    return lengthOfPath;
}

void findMaxMin(const int elevations[MAX_ROWS][MAX_COLS], int rows, int cols, int& max, int& min) {
    max = elevations[0][0];//set as first again for mimir
    min = elevations[0][0];//set as first again for mimir
    //itterate through every value
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols;j++){
            if(elevations[i][j] > max){//if find a val larger than max set max to it
                max = elevations[i][j];
            }
            if(elevations[i][j] < min){//likewise for min
                min = elevations[i][j];
            }
        }
    }
}

void loadData(int elevations[MAX_ROWS][MAX_COLS], int rows, int cols, istream& inData) {
        int error = 0;
        for(int r = 0; r < rows;r++){
            for(int c = 0; c <cols ; c++){
                if(inData >> elevations[r][c]){                    
                }
                else{
                    if(inData.fail()){
                        if(r == rows-1 && c == cols-1){
                        cout << "Error: End of file reached prior to getting all required data";
                        //error for not enough numbers for size
                        exit(1);//stop program
                        }
                        cout << "Error: Read a non-integer value.";//error for not a innt read
                        exit(1);//stop program
                    }
                    
                }
            }
        }
        if(inData >> error){//if you can put another value into error then there are too many values in the file
            cout << "Error: Too many data points"; //error for too many digits for size
            exit(1);//stop program
        }
    
    
}

int scaleValue(int value, int max, int min) {
    double dscale = round(((value-(double)min)/(max-min))*255);//create a double val with round
    int scale = (int)dscale;//truncate the zeros by casting int
    return scale;//return int val
}

void loadGreyscale(Pixel image[MAX_ROWS][MAX_COLS], const int elevations[MAX_ROWS][MAX_COLS],int rows, int cols, int max, int min){
    //itterate through the whole pixel array and set the pixel rgb values
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            image[r][c].red = scaleValue(elevations[r][c],max,min);//calls red val
            image[r][c].blue = scaleValue(elevations[r][c],max,min);//calls blue val
            image[r][c].green = scaleValue(elevations[r][c],max,min);//calls green val
            //can do this because is a struct and default things are public
        }
    }
}

void outputImage(const Pixel image[MAX_ROWS][MAX_COLS], int rows, int cols, ostream& outData) {
   //formating things
    outData << "P3" << endl;
    outData << cols << " " << rows <<endl;
    outData << 255 << endl;
   
    //printing pixels variables into outData ostream
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            outData << image[r][c].red << " " <<image[r][c].green << " " << image[r][c].blue <<  " ";
        }
        outData << endl;
    }
}

