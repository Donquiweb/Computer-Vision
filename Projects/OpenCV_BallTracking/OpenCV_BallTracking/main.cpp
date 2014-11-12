//big thanks to Kyle Hounslow for the tutorial that made this all so easy!
//https://www.youtube.com/watch?v=cgo0UitHfp8&list=PLvwB65U8V0HHCEyW2UTyOJym5FsdqfbHQ
//very good stuff!!!

#include <sstream>
#include <string>
#include <iostream>
#include <opencv\highgui.h>
#include <opencv\cv.h>


using namespace cv;
using namespace std;

//HSV filter values, controlled using trackbars
int H_MIN = 0;
int H_MAX = 256;

int S_MIN = 0;
int S_MAX = 256;

int V_MIN = 0;
int V_MAX = 256;

//default capture width and height
const int FRAME_WIDTH  = 640;
const int FRAME_HEIGHT = 480;

//max number of objects to be detected in the frame
const int MAX_NUM_OBJECTS = 50;

//minimum and maximum object area
const int MIN_OBJECT_AREA = 20 * 20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT * FRAME_WIDTH / 1.5;

//names that will appear along the top of each window
const string windowName  = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Threshold Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";

//gets called whenever a trackbar position is changed
void on_trackbar(int, void*)
{

}

//simple type conversion function to convert integers to strings
string intToString(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

//create a window and add a series of trackbars for controlling the various HSV values
void createTrackbars()
{
	namedWindow(trackbarWindowName, 0);

	//create some memory to store a trackbar name on window
	char TrackbarName[50];

	sprintf( TrackbarName, "H_MIN", H_MIN );
	sprintf( TrackbarName, "H_MAX", H_MAX );
	sprintf( TrackbarName, "S_MIN", S_MIN );
	sprintf( TrackbarName, "S_MAX", S_MAX );	
	sprintf( TrackbarName, "V_MIN", V_MIN );
	sprintf( TrackbarName, "V_MAX", V_MAX );

	createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
	createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
	createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
	createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
	createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
	createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );
}

//helper function to draw overlays for the image (cross hairs, boxes, etc)
void drawObject(int x, int y, Mat &frame)
{
	//draw a circle (Mat, Location, Radius, Color, Width)   
	circle(frame, Point(x,y), 20, Scalar(0,255,0), 2);

	//create a line that doesn't extend beyond the window
	if(y-25 > 0)
		line(frame, Point(x,y), Point(x, y-25), Scalar(0,255,0), 2);
	else 
		line(frame, Point(x,y), Point(x, 0), Scalar(0,255,0), 2);

	if(y+25 < FRAME_HEIGHT)
		line(frame, Point(x,y), Point(x, y+25), Scalar(0,255,0), 2);
	else
		line(frame, Point(x,y), Point(y, FRAME_HEIGHT), Scalar(0,255,0), 2);

	if(x-25 > 0)
		line(frame, Point(x,y), Point(x-25, y), Scalar(0,255,0), 2);
	else 
		line(frame, Point(x,y), Point(0, y), Scalar(0,255,0), 2);

	if(x+25 < FRAME_WIDTH)
		line(frame, Point(x,y), Point(x+25, y), Scalar(0,255,0), 2);
	else
		line(frame, Point(x,y), Point(FRAME_WIDTH, y), Scalar(0,255,0), 2);

	//add some text to the screen (Mat, Xloc, YLoc, Origin, Fontface, Fontscale, Color, Width)
	putText(frame, intToString(x)+","+intToString(y), Point(x,y+30), 1, 1, Scalar(0,255,0), 2);
}


//create a structuring element that will dilate and erode image, use 3x3px rectangle
void morphOps( Mat &thresh) 
{
	Mat erodeElement  = getStructuringElement(MORPH_RECT, Size(3,3));
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8,8));

	// (InputArray, OutputArray, InputArray Kernel)
	erode(thresh, thresh, erodeElement);
	erode(thresh, thresh, erodeElement);

	// (InputArray, OutputArray, InputArray Kernel)
	dilate(thresh, thresh, dilateElement);
	dilate(thresh, thresh, dilateElement);
}


void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed)
{
	Mat temp; //create a temporary Mat to work with
	threshold.copyTo(temp);

	//these two vectors are needed for output of findContours
	vector< vector<Point> >contours;
	vector<Vec4i> hierarchy;

	//find contours of filtered image using OpenCV findContours function (image, Ouput Array of Arrays Contours Vector, Output Array Hierarchy Vector, mode, method) 
	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) 
	{
			int numObjects = hierarchy.size();

		//if the number of objects is > MAX_NUM_OBJECTS we have a noisy filter and it needs to be adjusted
		if(numObjects < MAX_NUM_OBJECTS)
		{
			for(int i = 0; i >= 0; i = hierarchy[i][0])
			{
				Moments moment = moments((cv::Mat)contours[i]);
				double area = moment.m00;

				//if the area is less than 20x20pxm then it is likely noise
				//if the area is the same as the 3:2 of the image size, then it is probably a bad filter
				//only want the object with the largest area so we save a ref area and compare it ea. iteration
				if(area > MIN_OBJECT_AREA && area < MAX_OBJECT_AREA && area > refArea)
				{
					x = moment.m10 / area;
					y = moment.m01 / area;
					objectFound = true;
					refArea = area;
				}
				else objectFound = false;

			}

			//let the user know an object was found
			if(objectFound == true) 
			{
				//add text overlay (Mat, Text, Origin, FontFace, FontScale, Color, Width)
				putText(cameraFeed, "Tracking Object", Point(0,50), 2, 1, Scalar(0,255,0), 2);

				//draw object on sccreen 
				drawObject(x,y,cameraFeed);
			}
		
		}
		else putText(cameraFeed, "TOO MUCH NOISE!  ADJUST FILTER", Point(0,50), 1, 2, Scalar(0,255,0), 2);
	}
}

int main(int argc, char* argv[])
{
	bool trackObjects = true;
	bool useMorphOps = true;

	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;

	//Matix for HSV image
	Mat HSV;

	//Matrix for binary threshold image
	Mat threshold;

	//x & y values for the location of the object
	int x=0, y=0;

	//create slider bars for HSV filterins
	createTrackbars();

	//video capture object to acquire webcam feed
	VideoCapture capture;

	//open capture object at location zero (default location for the webcam
	capture.open(0);

	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

	//start an infinite loop where the webcam feed is copied to the cameraFeed matrix
	//all operations begin in this loop
	while(1)
	{
		//store the image matrix
		capture.read(cameraFeed);

		//convert frame form BGR to HSV (input matrix, ouptut matrix, code to perform)
		cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);

		//filter HSV image between values and store image to threshold matrix
		inRange(HSV, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold);

		//perform morphological operations on threshold image to eliminate noise and emphasize filtered objs
		if(useMorphOps)
			morphOps(threshold);

		//pass in the threshold frame to the out obj tracking function: rtrns x,y coords of filtered object
		if(trackObjects)
			trackFilteredObject(x,y, threshold, cameraFeed);

		//showFrames
		imshow(windowName2, threshold);
		imshow(windowName, cameraFeed);
		imshow(windowName1, HSV);

		//delay 30ms so the screen can refresh
		waitKey(30);

	}

	return 0;

}