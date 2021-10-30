#include <iostream>
#include <vector>
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>

#include "contentFinder.h"
#include "colorhistogram.h"

int main()
{
	// Read reference image
	cv::Mat image= cv::imread("images/baboon01.jpg");
	if (!image.data)
		return 0; 

	cv::Rect rect(110, 45, 35, 45);

	cv::rectangle(image, rect, cv::Scalar(0, 0, 255));

	cv::Mat imageROI = image(rect);


	cv::namedWindow("Image 1");
	cv::imshow("Image 1",imageROI);
	

	int minSet = 65;
	ColorHistogram hc;
	cv::Mat colorhist = hc.getHueHistogram(imageROI, minSet);

	ContentFinder finder;
	finder.setHistogram(colorhist);
	finder.setThreshold(0.2f);

	cv::Mat hsv;
	cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

	vector<cv::Mat> v;
	cv::split(hsv, v);


	cv::threshold(v[1], v[1], minSet, 255, cv::THRESH_BINARY);
	cv::namedWindow("Saturation mask");
	cv::imshow("Saturation mask", v[1]);


	image = cv::imread("./images/baboon02.jpg");

	cv::namedWindow("Image 2");
	cv::imshow("Image 2",image);

	cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

	// Get back-projection of hue histogram
	int ch[1] = {0};
	finder.setThreshold(-1.0f); // no thresholding
	cv::Mat result= finder.find(hsv, 0.0f, 180.0f, ch);

	// Display back projection result
	cv::namedWindow("Backprojection on second image");
	cv::imshow("Backprojection on second image",result);

	// initial window position
	cv::rectangle(image, rect, cv::Scalar(0,0,255));

	// search objet with mean shift
	cv::TermCriteria criteria(cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS,
		10,     // iterate max 10 times
		1);     // or until the change in centroid position is less than 1px
	cout << "meanshift= " << cv::meanShift(result,rect,criteria) << endl;



	// draw output window
	cv::rectangle(image, rect, cv::Scalar(0,255,0));

	// Display image
	cv::namedWindow("Image 2 result");
	cv::imshow("Image 2 result",image);

	cv::waitKey();

	return 0;
}