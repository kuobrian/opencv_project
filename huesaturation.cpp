#include <opencv2\opencv.hpp>
#include <opencv2\xfeatures2d.hpp>
#include <opencv2\face.hpp>
 
void detectHScolor(const cv::Mat& img,
                    double minHue, double maxHue,
                    double minSat, double maxSat,
                    cv::Mat& mask)
{
    cv::Mat hsv;
    cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);

    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);

    // Hue masking
    cv::Mat mask1;
    cv::threshold(channels[0], mask1, maxHue, 255, cv::THRESH_BINARY_INV);
    cv::Mat mask2; // over minHue
	cv::threshold(channels[0], mask2, minHue, 255, cv::THRESH_BINARY);

    cv::Mat hueMask; // hue mask
	if (minHue < maxHue)
		hueMask = mask1 & mask2;
	else // if interval crosses the zero-degree axis
		hueMask = mask1 | mask2;

    
	// Saturation masking
	// below maxSat
	cv::threshold(channels[1], mask1, maxSat, 255, cv::THRESH_BINARY_INV);
	// over minSat
	cv::threshold(channels[1], mask2, minSat, 255, cv::THRESH_BINARY);

	cv::Mat satMask; // saturation mask
	satMask = mask1 & mask2;

	// combined mask
	mask = hueMask&satMask;



}



int main()
{
    cv::Mat image = cv::imread("./images/girl.jpg");
    if (!image.data)
		return 0; 

	// show original image
	cv::namedWindow("Original image");
	cv::imshow("Original image",image);

    // convert into HSV space
	cv::Mat hsv;
	cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);
    // channels[0] is the Hue
	// channels[1] is the Saturation
	// channels[2] is the Value

	// display value
	cv::namedWindow("Value");
	cv::imshow("Value",channels[2]);

	// display saturation
	cv::namedWindow("Saturation");
	cv::imshow("Saturation",channels[1]);

	// display hue
	cv::namedWindow("Hue");
	cv::imshow("Hue",channels[0]);

    channels[2] = 255;

    cv::merge(channels, hsv);

    cv::Mat newImage;
    cv::cvtColor(hsv, newImage, cv::COLOR_HSV2BGR);

    // display hue
	cv::namedWindow("re");
	cv::imshow("re",newImage);


    cv::Mat mask;
    detectHScolor(image,160, 10,25,  166, mask);
    
    // show masked image
	cv::Mat detected(image.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	image.copyTo(detected, mask);
	cv::imshow("Detection result",detected);


	// A test comparing luminance and brightness

	// create linear intensity image
	cv::Mat linear(100,256,CV_8U);
	for (int i=0; i<256; i++) {

		linear.col(i)= i;
	}

	// create a Lab image
	linear.copyTo(channels[0]);
	cv::Mat constante(100,256,CV_8U,cv::Scalar(128));
	constante.copyTo(channels[1]);
	constante.copyTo(channels[2]);
	cv::merge(channels,image);

	// convert back to BGR
	cv::Mat brightness;
	cv::cvtColor(image,brightness, cv::COLOR_Lab2BGR);
	cv::split(brightness, channels);

	// create combined image
	cv::Mat combined(200,256, CV_8U);
	cv::Mat half1(combined,cv::Rect(0,0,256,100));
	linear.copyTo(half1);
	cv::Mat half2(combined,cv::Rect(0,100,256,100));
	channels[0].copyTo(half2);

	cv::namedWindow("Luminance vs Brightness");
	cv::imshow("Luminance vs Brightness",combined);
    cv::waitKey();
    return 0;
}