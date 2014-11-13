#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <iostream>

using namespace cv;
using namespace std;

//simple type conversion  helper function using stringstream conversion
string intToString(int number)
{
	stringstream ss;
	ss << number;
	return ss.str();
}


int main(int argc, char* argv[])
{
	
	//capture video from the camera at location 0 - usually the webcam
	VideoCapture cap(0); //open the video camera at location 0

	//create a video writer object to use later when writing the video
	VideoWriter writer;

	//check if video capture device is open and available
	if (!cap.isOpened())
	{
		cout << "ERROR INITIALIZING VIDEO CAPTURE" << endl;
		return -1;
	}

	//create a name for window we use to write the video
	char* windowName = "Webcam Feed";

	//create a window by name and set to autosize acccording to capture device input
	namedWindow(windowName, CV_WINDOW_AUTOSIZE);

	//filename string used for captured video
	string filename = "myVideo.avi";

	//fourcc (four character code) integer used by video encoder (see www.fourcc.org for the complete codec list)
	int fcc = CV_FOURCC('D', 'I', 'V', '3');

	//frames per sec integer
	int fps = 20;

	//Size of frame (part of "cv" namespace - use CTRL + SPACE to access list of cap.get() properties 
	cv::Size frameSize(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT));

	//construct a new video writer (filename, four character code fourcc codec, fps, frame size, bool isColor)
	writer = VideoWriter(filename, fcc, fps, frameSize);

	//test to see if we were able to create video writer
	if(!writer.isOpened())
	{
		cout << "ERROR INITIALIZING THE VIDEO WRITER" << endl;
		return -1;
	}

	while(1)
	{
		//create a new matrix to capture frames
		Mat frame;

		//read from the capture feed and write buffer into frame
		bool success = cap.read(frame);

		if(!success)
		{
			cout << "ERROR READING FROM CAMERA FEED" << endl;
			break;
		}

		//write video to the output file, if not avail
		writer.write(frame);

		//show the current frame in a window named after the windowName variable declared earlier
		imshow(windowName, frame);

		//respond to the ESC key (wait 10ms each time or you may miss it)'''''
		switch(waitKey(10))
		{
			case 27: return 0; //esc key exits the program
		}
	}

	return 0;

}
