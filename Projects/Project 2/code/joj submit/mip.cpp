//  @author zysssdk[odysseyzhou@126.com]
#include <iostream>
#include <fstream> 
#include <sstream>
#include <string>
#include <string.h> 
#include <cstdlib>
#include <vector> 
#include <algorithm>
#include "constants.h"
#include "image.h"

using namespace std;

//Actually the number 3 is 2s + 1, but in this project we define s = 1.
//a function pointer that takes a 2D array of chars and return an aggregated value as a char.
typedef unsigned char (*Agg) (const unsigned char values[3][3]);

void fterror() {
    cout << FILE_TYPE_ERROR << endl;
    return;
}

void fneerror() {
    cout << FILE_NOT_EXIST_ERROR << endl;
    return;
}

void terror() {
    cout << TRANSFORM_ERROR << endl;
    return;
}

void hm() {
    cout << HELP_MESSAGE << endl;
    return;
}

void readImage(istream &is, Image &imI) {
    //The image is read from the input stream and is stored in the second argument. 
    //The caller can then access this image from the image array.
    string P_type;
    char size;//No use, we only need to throw this string away.
    is >> P_type;//Of course this is P6, or it's not the file we need.
    is >> imI.w;
    is >> imI.h;
    is >> size;//Of course this is 255 by default in this project.
    //Read the RGB data from "is".
    //Mat[i][j] means the ith row and the jth column.
    //Actually the .ppm data storage is from top to bottom, and then from left to right.
    string Rgb_temp((istreambuf_iterator<char>(is)), istreambuf_iterator<char>());
    for(unsigned int i = 0U; i < imI.h; i++) {
        for(unsigned int j = 0U; j < imI.w; j++) {
            //The first three Rgb elements are not needed.
            imI.image[i][j].red = Rgb_temp[i * 3 * imI.w + j * 3 + 3]; 
            imI.image[i][j].green = Rgb_temp[i * 3 * imI.w + j * 3 + 4];
            imI.image[i][j].blue = Rgb_temp[i * 3 * imI.w + j * 3 + 5];    
        }
    }
    return;
}

int readImage_for_cin(istream &is, Image &imI) {
    char P_type[2];
    char size;
    is >> P_type;
    //Check the file header:
    if(strcmp(P_type, "P6") != 0) {
        fterror();
        return 1;
    }
    else {
        is >> imI.w;
        is >> imI.h;
        is >> size;
        string Rgb_temp((istreambuf_iterator<char>(is)), istreambuf_iterator<char>());
        for(unsigned int i = 0U; i < imI.h; i++) {
            for(unsigned int j = 0U; j < imI.w; j++) {
                imI.image[i][j].red = Rgb_temp[i * 3 * imI.w + j * 3 + 3]; 
                imI.image[i][j].green = Rgb_temp[i * 3 * imI.w + j * 3 + 4];
                imI.image[i][j].blue = Rgb_temp[i * 3 * imI.w + j * 3 + 5];    
            }
        }
        return 0;
    }
}

void writeImage(ostream &os, const Image &imI) {
    os << "P6" << endl;
    os << imI.w << ' ';
    os << imI.h << endl;
    os << "255" << endl;
    //Transpose the second time.
    for(unsigned int i = 0U; i < imI.h; i++) {
        for(unsigned int j = 0U; j < imI.w; j++) {
            os << (char) imI.image[i][j].red;
            os << (char) imI.image[i][j].green;
            os << (char) imI.image[i][j].blue;  
        }
    }
    return;
}

//vertically flips an image
void verticalFlip(const Image &imI, Image &imJ) {
    imJ.w = imI.w;
    imJ.h = imI.h;
    for(unsigned int i = 0U; i < imI.h; i++) {
        for(unsigned int j = 0U; j < imI.w; j++) {
            imJ.image[imI.h - 1 - i][j].red = imI.image[i][j].red;
            imJ.image[imI.h - 1 - i][j].green = imI.image[i][j].green;
            imJ.image[imI.h - 1 - i][j].blue = imI.image[i][j].blue;
        }
    }
    return;
}

//rotates an image by 90 degrees (clockwise)
void rotate90(const Image &imI, Image &imJ) {
    imJ.w = imI.h;
    imJ.h = imI.w; 
    for(unsigned int i = 0U; i < imI.w; i++) {
        for(unsigned int j = 0U; j < imI.h; j++) {
            imJ.image[i][j].red = imI.image[imI.h - j - 1][i].red;
            imJ.image[i][j].green = imI.image[imI.h - j - 1][i].green;
            imJ.image[i][j].blue = imI.image[imI.h - j - 1][i].blue;
        }
    }
    return;
}

//inverts the intensity of the RGB values
void intensityInversion(const Image &imI, Image &imJ) {
    imJ.w = imI.w;
    imJ.h = imI.h;
    for(unsigned int i = 0U; i < imI.h; i++) {
        for(unsigned int j = 0U; j < imI.w; j++) {
            imJ.image[i][j].red = M - imI.image[i][j].red;
            imJ.image[i][j].green = M - imI.image[i][j].green;
            imJ.image[i][j].blue = M - imI.image[i][j].blue;
        }
    }
    return;
}

//Max function
unsigned char max(const unsigned char values[3][3]) {
    unsigned char temp = values[0][0];
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(values[i][j] > temp) {
                temp = values[i][j];
            }
        }
    }
    return temp;
}

//Mean function
unsigned char mean(const unsigned char values[3][3]) {
    int temp = 0;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            temp = temp + (int) values[i][j];
        }
    }
    temp = temp / 9;//Here temp becomes an int.
    return (unsigned char) temp;
}

//Median function
unsigned char median(const unsigned char values[3][3]) {
    //create a array for sort() function
    unsigned char temp[9];
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            temp[3 * i + j] = values[i][j];
        }
    }
    sort(temp, temp + 9);
    return temp[4];
}

//apply the filtering operation
//include max, mean and median
//typedef unsigned char (*Agg) (const unsigned char values[3][3]);
void filter(const Image &imI, Image &imJ, Agg f) {
    imJ.w = imI.w;
    imJ.h = imI.h;
    unsigned char values_red[3][3];
    unsigned char values_green[3][3];
    unsigned char values_blue[3][3];
    for(unsigned int i = 0U; i < imI.h; i++) {
        for(unsigned int j = 0U; j < imI.w; j++) {
            //fill in the matrix (actually 3 in 1 is better...)
            //red
            for(unsigned int r = 0U; r < 3U; r++) {
                for(unsigned int c = 0U; c < 3U; c++) {
                    //boundary conditions
                    if(((i + r - 1U) < 0U) || ((i + r - 1U) >= imI.h) || ((j + c - 1U) < 0U) || ((j + c - 1U) >= imI.w)){
                        values_red[r][c] = 0U;
                    }
                    else{
                        values_red[r][c] = imI.image[i + r - 1U][j + c - 1U].red;
                    }
                }
            }
            //green
            for(unsigned int r = 0U; r < 3U; r++) {
                for(unsigned int c = 0U; c < 3U; c++) {
                    //boundary conditions
                    if(((i + r - 1U) < 0U) || ((i + r - 1U) >= imI.h) || ((j + c - 1U) < 0U) || ((j + c - 1U) >= imI.w)){
                        values_green[r][c] = 0U;
                    }
                    else{
                        values_green[r][c] = imI.image[i + r - 1U][j + c - 1U].green;
                    }
                }
            }
            //blue
            for(unsigned int r = 0U; r < 3U; r++) {
                for(unsigned int c = 0U; c < 3U; c++) {
                    //boundary conditions
                    if(((i + r - 1U) < 0U) || ((i + r - 1U) >= imI.h) || ((j + c - 1U) < 0U) || ((j + c - 1U) >= imI.w)){
                        values_blue[r][c] = 0U;
                    }
                    else{
                        values_blue[r][c] = imI.image[i + r - 1U][j + c - 1U].blue;
                    }
                }
            }
            //process the data
            imJ.image[i][j].red = f(values_red);
            imJ.image[i][j].green = f(values_green);
            imJ.image[i][j].blue = f(values_blue);
        }
    }
    return;
}

int main(int argc, char* argv[]) {
    Image imI;//input temp
    Image imJ;//output temp
    //For IO
    int ofile_flag = 0;//Choose cout or os
    int ifile_flag = 0;//Choose cin or is
    ifstream is;//Create istream
    ofstream os;//Create ostream
    //We should check whether "-h" exists at the very beginning.
    for(int m = 1; m < argc; m++) {
        if((strcmp(argv[m], "-h") == 0) || (strcmp(argv[m], "--help") == 0)) {
            hm();
            return 0;
        }
    }
    //Then, we should choose cin or is and try opening the file, because preloading is necessary!
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-i") == 0) {//Using file input
            ifile_flag = 1;
            //step 1: try opening the file
            string name = argv[i + 1];
            int len = name.length();
            is.open(argv[i + 1], ios_base::in | ios_base::binary);
            if(is.is_open() == false) {
                fneerror();
                return 0;
            }
            //step 2: test whether it is a ppm file
            else {
                if(((name[len - 3] == 'p') && (name[len - 2] == 'p') && (name[len - 1] == 'm'))) {
                    readImage(is, imI);
                    is.close();
                }
                else {
                    is.close();
                    fterror();
                    return 0;
                }
            }
            break;
        }            
    }
    if(ifile_flag == 0) {//Using std input
        int status = readImage_for_cin(cin, imI);
        if(status == 1) {
            return 0;
        }
    }
    //After that, we check the command "-t".
    imJ = imI;//In case there is no "-t" command
    for(int k = 1; k < argc; k++) {
        if(strcmp(argv[k], "-t") == 0) {
            if(k == argc - 1) {
                terror();
                return 0;
            } 
            else if(strcmp(argv[k + 1], "verticalFlip") == 0) {//verticalFlip
                verticalFlip(imI, imJ);
            }
            else if(strcmp(argv[k + 1], "rotate90") == 0) {//rotate90
                rotate90(imI, imJ);
            }
            else if(strcmp(argv[k + 1], "intensityInversion") == 0) {//intensityInversion
                intensityInversion(imI, imJ);
            }
            else if(strcmp(argv[k + 1], "maxFilter") == 0) {//maxFilter
                filter(imI, imJ, max);
            }
            else if(strcmp(argv[k + 1], "meanFilter") == 0) {//meanFilter
                filter(imI, imJ, mean);
            }
            else if(strcmp(argv[k + 1], "medianFilter") == 0) {//medianFilter
                filter(imI, imJ, median);
            }
            else {//Other cases
                terror();
                return 0;
            }             
        }
    }
    //Finally, we consider output (if the program runs here, the previous steps is sure to be valid): Try finding a file for output, or we should use cout!
    for(int j = 1; j < argc; j++) {
        if(strcmp(argv[j], "-o") == 0) {
            ofile_flag = 1;
            os.open(argv[j + 1], ios_base::out | ios_base::binary);//keep writing os open and close it at the end of the main function
            break;
        }
    }
    if(ofile_flag == 1) {
            writeImage(os, imJ);
            os.close();
    }
    else {
        writeImage(cout, imJ);
    }
    return 0;
}


