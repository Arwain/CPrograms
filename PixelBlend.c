/*
 * @author - Arwain Giannini-Karlin
 * @date - 1/25/2017
 *
 * The following program takes two images and creates a blended image
 * and a checkerboard image of the two.
 */

#include <stdio.h>

void blendImages(FILE *in1, FILE *in2){
    int size,width,height,i;
    char header [2];
    char header2 [12];
    char header3 [28];
    unsigned char pixelColor;

    // Declare files for reading and writing.
    FILE *infile1;
    infile1 = in1;
    FILE *infile2;
    infile2 = in2;
    FILE *outfile1;
    outfile1 = fopen("blend.bmp","wb");

    // Read headers and set width and height variables.
    fread(header,sizeof(char),2,infile1);
    fread(&size,sizeof(int),1,infile1);
    fread(header2,sizeof(char), 12, infile1);
    fread(&width,sizeof(int), 1, infile1);
    fread(&height,sizeof(int),1,infile1);
    fread(header3,sizeof(char),28,infile1);

    // Read image from image1 into an array of adequate size.
    unsigned char image1 [size - 54];
    fread(image1, sizeof(char), (size_t) (size - 54), infile1);

    // Read header from image2.
    fread(&header,sizeof(char),2,infile2);
    fread(&size,sizeof(int),1,infile2);
    fread(header2,sizeof(char), 12, infile2);
    fread(&width,sizeof(int), 1, infile2);
    fread(&height,sizeof(int),1,infile2);
    fread(header3,sizeof(char),28,infile2);


    // Read image from image2 into an array of adequate size.
    unsigned char image2 [size - 54];
    fread(image2, sizeof(char), (size_t) (size - 54), infile2);

    // Write header for blended image to file.
    fwrite(header,sizeof(char),2,outfile1);
    fwrite(&size,sizeof(int),1,outfile1);
    fwrite(header2,sizeof(char),12,outfile1);
    fwrite(&width,sizeof(int),1,outfile1);
    fwrite(&height,sizeof(int),1,outfile1);
    fwrite(header3,sizeof(char),28,outfile1);

    // Algorithm for blended image below creates an array of the blended image.
    unsigned char blendedImage[size - 54];
    for (i = 0; i < sizeof(image1); ++i) {
        pixelColor = (unsigned char) ((image1[i] + image2[i]) / 2);
        blendedImage[i] = pixelColor;
    }

    // Write blended image to file.
    fwrite(blendedImage, (size_t) (size - 54), 1, outfile1);

}

void checkerImages(FILE *in1,FILE *in2) {
    int size,width,height,r,c,chkR,chkC;
    char header [2];
    char header2 [12];
    char header3 [28];

    // Declare files for reading and writing.
    FILE *infile1;
    infile1 = in1;
    FILE *infile2;
    infile2 = in2;
    FILE *outfile1;
    outfile1 = fopen("checker.bmp","wb");

    // Read headers and set width and height variables.
    fread(header,sizeof(char),2,infile1);
    fread(&size,sizeof(int),1,infile1);
    fread(header2,sizeof(char), 12, infile1);
    fread(&width,sizeof(int), 1, infile1);
    fread(&height,sizeof(int),1,infile1);
    fread(header3,sizeof(char),28,infile1);
    // Reduce the difference of the header and the image.
    size = size - 54;

    // Read image from image1 and create array of adequate size.
    unsigned char im1 [height][3 * width];
    fread(im1,sizeof(char),size,infile1);

    // Read header from image2.
    fread(&header,sizeof(char),2,infile2);
    fread(&size,sizeof(int),1,infile2);
    fread(header2,sizeof(char), 12, infile2);
    fread(&width,sizeof(int), 1, infile2);
    fread(&height,sizeof(int),1,infile2);
    fread(header3,sizeof(char),28,infile2);
    // Reduce the difference of the header and the image.
    size = size - 54;

    // Read image from image2 and create array of adequate size.
    unsigned char im2 [height][3 * width];
    fread(im2,sizeof(char),size,infile2);

    // Write header for checkered image to file.
    fwrite(header,sizeof(char),2,outfile1);
    fwrite(&size,sizeof(int),1,outfile1);
    fwrite(header2,sizeof(char),12,outfile1);
    fwrite(&width,sizeof(int),1,outfile1);
    fwrite(&height,sizeof(int),1,outfile1);
    fwrite(header3,sizeof(char),28,outfile1);

    // Algorithm for checkerboard array below.
    unsigned char board[height][3 * width];
    for (r = 0; r < height; r++) {
        for (c = 0; c < 3* width; c++) {
            chkR = r / (height / 8);
            chkC = c / (width * 3 / 8);
            if ((chkR + chkC) % 2 == 0) {
                board[r][c] = im1[r][c];
            } else {
                board[r][c] = im2[r][c];
            }
        }
    }

    // Write checkerboard array to file.
    fwrite(board,sizeof(board),1,outfile1);

}

int main(void) {

    // Declare files for reading.
    FILE *file1;
    FILE *file2;
    FILE *f1;
    FILE *f2;

    // Open files, call blend images and close files.
    file1 = fopen("in1.bmp","rb");
    file2 = fopen("in2.bmp","rb");
    blendImages(file1,file2);
    fclose(file1);
    fclose(file2);

    // Open files, call checker images and close files.
    f1 = fopen("in1.bmp","rb");
    f2 = fopen("in2.bmp","rb");
    checkerImages(f1,f2);
    fclose(f1);
    fclose(f2);
    return 0;
}
