#include "Shape.h"


Shape::Shape(void)
{
}

Shape::Shape(string name)
{
	setType(name);
	if(name == "green")
	{
		setHSVMin(Scalar(0,70,0));
		setHSVMax(Scalar(84,256,200));
	}
	if(name == "blue")
	{
		setHSVMin(Scalar(0,70,0));
		setHSVMax(Scalar(84,256,200));
	}
	if(name == "red")
	{
		setHSVMin(Scalar(0,70,0));
		setHSVMax(Scalar(84,256,200));
	}
}


Shape::~Shape(void)
{
}

int Shape::getXPos()
{
	return Shape::xPos;
}

int Shape::getYPos()
{
	return Shape::yPos;
}

void Shape::setXPos(int x)
{
	Shape::xPos = x;
}

void Shape::setYPos(int y)
{
	Shape::yPos = y;
}

Scalar Shape::getHSVMin()
{
	return Shape::HSVMin;
}

Scalar Shape::getHSVMax()
{
	return Shape::HSVMax;
}

void Shape::setHSVMin(Scalar min)
{
	Shape::HSVMin = min;
}

void Shape::setHSVMax(Scalar max)
{
	Shape::HSVMax = max;
}
