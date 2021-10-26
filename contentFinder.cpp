#include <iostream>
#include <vector>
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include "histogram.h"
#include "contentFinder.h"
#include "colorhistogram.h"

int main()
{
	// Read reference image
	cv::Mat image= cv::imread("./images/waves.jpg", 0);
	if (!image.data)
		return 0;

    cv::Mat imageROI;
    imageROI= image(cv::Rect(216,33,24,30)); // Cloud region

	// Display reference patch
	cv::namedWindow("Reference");
	cv::imshow("Reference",imageROI);

// Find histogram of reference
	Histogram1D h;
	cv::Mat hist= h.getHistogram(imageROI);
	cv::namedWindow("Reference Hist");
	cv::imshow("Reference Hist",h.getHistogramImage(imageROI));


    ContentFinder finder;
    finder.setHistogram(hist);
    finder.setThreshold(-1.0f);

    // Get back-projection
	cv::Mat result1;
	result1= finder.find(image);

	// Create negative image and display result
	cv::Mat tmp;
	result1.convertTo(tmp,CV_8U,-1.0,255.0);
	cv::namedWindow("Backprojection result");
	cv::imshow("Backprojection result",tmp);

    // Get binary back-projection
	finder.setThreshold(0.12f);
	result1= finder.find(image);

	// Draw a rectangle around the reference area
	cv::rectangle(image, cv::Rect(216, 33, 24, 30), cv::Scalar(0, 0, 0));

	// Display image
	cv::namedWindow("Image");
	cv::imshow("Image",image);

	// Display result
	cv::namedWindow("Detection Result");
	cv::imshow("Detection Result",result1);

    // Load color image
	ColorHistogram hc;
    cv::Mat color= cv::imread("./images/waves.jpg");
	// extract region of interest
	imageROI= color(cv::Rect(0,0,100,45)); // blue sky area

    cv::namedWindow("Color image");
	cv::imshow("Color image", color);


	// Get 3D colour histogram (8 bins per channel)
	hc.setSize(8); // 8x8x8
	cv::Mat shist= hc.getHistogram(imageROI);

	// set histogram to be back-projected
	finder.setHistogram(shist);
	finder.setThreshold(0.05f);

	// Get back-projection of color histogram
	result1= finder.find(color);
    

	cv::namedWindow("Color Detection Result");
	cv::imshow("Color Detection Result",result1);


	cv::waitKey();
	return 0;
}