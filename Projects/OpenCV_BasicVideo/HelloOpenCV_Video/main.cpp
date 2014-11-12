#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat image;         //create a matrix object to store the color image
	Mat gray_image;    //create a matrix object to store the b&w image

	VideoCapture cap;  //create a video stream
	cap.open(0);       //get closest webcam

	namedWindow( "Gray image", CV_WINDOW_AUTOSIZE );
	namedWindow("VideoDisplay", 1);

	while(1)
	{
		cap>>image;
		cvtColor( image, gray_image, CV_BGR2GRAY ); //convert original image to grayscale with BGR2GRAY
		
		//display color image
		imshow("VideoDisplay", image);
		
		//display grayscale image
        imshow( "Gray Image", gray_image );
		
		waitKey(33);
	}

}
