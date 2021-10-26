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
	cv::Mat image= cv::imread("baboon01.jpg");
	if (!image.data)
		return 0; 


	cv::waitKey();
	return 0;
}