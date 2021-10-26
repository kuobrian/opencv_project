#include <iostream>
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "histogram.h"


int main() 
{
    cv::Mat image = cv::imread("images/group.jpg", 0);

    if (!image.data)
        return 0;
    
    cv::imwrite("./outs/groupBW.jpg", image);

    cv::namedWindow("Image");
    cv::imshow("Image", image);

    Histogram1D h;

    cv::Mat histo = h.getHistogram(image);

    // for (int i=0; i<256; i++)
    //     cout << "Value " << i << " = " << histo.at<float>(i) <<endl;
    
    cv::namedWindow("Histogram");
	cv::imshow("Histogram",h.getHistogramImage(image));

    cv::Mat hi = h.getHistogramImage(image);
	cv::line(hi, cv::Point(70, 0), cv::Point(70, 255), cv::Scalar(128));
	cv::namedWindow("Histogram with threshold value");
	cv::imshow("Histogram with threshold value", hi);

    cv::Mat thresholded; // output binary image
	cv::threshold(image,thresholded,
		          70,    // threshold value
				  255,   // value assigned to pixels over threshold value
				  cv::THRESH_BINARY); // thresholding type
 
	// Display the thresholded image
	cv::namedWindow("Binary Image");
	cv::imshow("Binary Image",thresholded);
    thresholded = 255 - thresholded;
	cv::imwrite("./outs/binary.bmp",thresholded);

	// Equalize the image
	cv::Mat eq= h.equalize(image);

	// Show the result
	cv::namedWindow("Equalized Image");
	cv::imshow("Equalized Image",eq);

	// Show the new histogram
	cv::namedWindow("Equalized H");
	cv::imshow("Equalized H",h.getHistogramImage(eq));

	// Stretch the image, setting the 1% of pixels at black and 1% at white
	cv::Mat str= h.stretch(image,0.01f);

	// Show the result
	cv::namedWindow("Stretched Image");
	cv::imshow("Stretched Image",str);
	
	// Show the new histogram
	cv::namedWindow("Stretched H");
	cv::imshow("Stretched H",h.getHistogramImage(str));

	// Create an image inversion table
	cv::Mat lut(1,256,CV_8U); // 1x256 matrix

	// Or:
	// int dim(256);
	// cv::Mat lut(1,  // 1 dimension
	// 	&dim,          // 256 entries
	//	CV_8U);        // uchar

	for (int i=0; i<256; i++) {
		
		// 0 becomes 255, 1 becomes 254, etc.
		lut.at<uchar>(i)= 255-i;
	}

	// Apply lookup and display negative image
	cv::namedWindow("Negative image");
	cv::imshow("Negative image",h.applyLookUp(image,lut));


    cv::waitKey(0);
	return 0;

}