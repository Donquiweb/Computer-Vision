#pragma once
#include <string>
#include <opencv\highgui.h>
#include <opencv\cv.h>

using namespace std; //saves you having to type "std::" all over the place
using namespace cv;

class Shape
{
public:
	Shape(void);
	~Shape(void);

	Shape(string name);

	int getXPos();
	int getYPos();
	void setXPos(int x);
	void setYPos(int y);

	Scalar getHSVMin();
	Scalar getHSVMax();

	void setHSVMin(Scalar min);
	void setHSVMax(Scalar max);

	string getType(){return type;}
	void setType(string t){type = t;}

	Scalar getColor()
	{
		return color;
	}

	void setColor(Scalar aColor)
	{
		color = aColor;
	}

private:
	int xPos, yPos;
	string type;
	cv::Scalar HSVMin, HSVMax;
	Scalar color;
};

