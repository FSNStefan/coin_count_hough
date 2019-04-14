// testopencv.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
	Mat src, src_gray;
	Mat grad;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	src = imread("coin.jpg", 1);
	GaussianBlur(src, src, Size(9, 9), 2, 2);
	cvtColor(src, src_gray, CV_BGR2GRAY);
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	/// Gradient X
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	/// Total Gradient (approximate)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
	vector<Vec3f> circles;
	HoughCircles(grad, circles, CV_HOUGH_GRADIENT, 1, grad.rows / 8, 200, 100, 0, 0);
	imshow("Hough Gray", src_gray);
	/// Draw the circles detected
	int ten = 0, one = 0, five = 0;
	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		cout << "radius : " << radius << endl;
		// circle center
		circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		if (radius > 185)
			one++;
		else if (radius <= 185 && radius > 180)
			five++;
		else {
			ten++;
		}
		// circle outline
		circle(src, center, radius, Scalar(255, 0, 0), 3, 8, 0);
	}
	cout << "The number of one-baht coins is " << one << "." << endl;
	cout << "The number of five-baht coins is " << five << "." << endl;
	cout << "The number of ten-baht coins is " << ten << "." << endl;
	cout << "Total is " << one + (5 * five) + (10 * ten) << "." << endl;
	/// Show your results
	imwrite("Result.jpg", src);
	namedWindow("Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE);
	imshow("Hough Circle Transform Demo", src);
	waitKey(0);
	return 0;
}

