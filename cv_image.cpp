#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include<fstream>
using namespace std;

int main()
{
    IplImage *img = cvLoadImage("./middle0.jpg", CV_LOAD_IMAGE_COLOR);
    uchar *data = (uchar *)img->imageData;
    int step = img->widthStep / sizeof(uchar);
    int channels = img->nChannels;
    int R, G, B;
    ofstream fout;
    fout.open("RGB.txt", ios::app);
    for(int i = 0; i < img->height; i++)
    {
        for(int j = 0; j < img->width; j++)
        {
            B = (int)data[i * step + j * channels + 0];
            G = (int)data[i * step + j * channels + 1];
            R = (int)data[i * step + j * channels + 2];
            fout << B << " " << G << " " << R << endl;
        }
    }
    fout.close();
    return 0;
}