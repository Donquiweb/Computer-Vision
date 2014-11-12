#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
 char* imageName = argv[1];

 Mat image;
 image = imread( imageName, 1 );

 if( argc != 2 || !image.data )
 {
   printf( " No image data \n " );
   return -1;
 }

 Mat F = image.clone(); //copies header and data
 
 Mat G;
 image.copyTo(G);  //copies the matrix data into a new one


 Mat gray_image;
 Mat image_sm(image, Rect(100,100,110,110)); //create a subsection of a matrix called image_sm

 Mat M(2,2, CV_8UC3, Scalar(0,0,255)); //create a simple matrix, 2R x 2C, 8-bit unsigned, 3 color channels, prepopulated 0,0,255
 cout << "M = " << endl << " " << M << endl << endl; //output matrix M to the console
 cout << "M (csv)     = " << endl << format(M,"csv"   ) << endl << endl;

 int sz3D[3] = {20,20,20}; //use a C++ array to define a matrix, in this case 3-dimensional
 Mat Q(3,sz3D, CV_8UC(1), Scalar::all(42));  //create a new matrix and configure it via the array

 int sz[2] = {40,40}; //use a C++ array to define a matrix, in this case 2-dimensional
 Mat L(2,sz, CV_8UC(1), Scalar::all(5));  //create a new matrix and configure it via the array
  cout << "L = " << endl << " " << L << endl << endl; //output matrix L to the console

 cvtColor( image, gray_image, CV_BGR2GRAY ); //convert original image to grayscale with BGR2GRAY

 imwrite( "Gray_Image.jpg", gray_image );

 namedWindow( imageName, CV_WINDOW_AUTOSIZE );
 namedWindow( "Gray image", CV_WINDOW_AUTOSIZE );
 namedWindow( "Section of image", CV_WINDOW_AUTOSIZE );
 namedWindow( "Cloned image F", CV_WINDOW_AUTOSIZE );
 namedWindow( "Copied image G", CV_WINDOW_AUTOSIZE );
 namedWindow( "Handmade matrix", CV_WINDOW_AUTOSIZE );

 imshow( imageName, image );
 imshow( "Gray image", gray_image );
 imshow( "Section of image", image_sm );
 imshow( "Cloned image F", F);
 imshow( "Copied image G", G);
 imshow( "Handmade matrix", L);

// waitKey(5000); //wait 5 sec then close...otherwise waitKey(0) waits indefinitely
 waitKey(0);  //waitKey(0) waits indefinitely

 return 0;
}