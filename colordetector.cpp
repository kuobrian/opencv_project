#include "colordetector.h"
#include <vector>
	
cv::Mat ColorDetector::process(const cv::Mat &image) {

	  // re-allocate binary map if necessary
	  // same size as input image, but 1-channel
	  result.create(image.size(), CV_8U);

	  // Converting to Lab color space 
	  if (useLab)
		  cv::cvtColor(image, converted, cv::COLOR_BGR2Lab);

	  // get the iterators
	  cv::Mat_<cv::Vec3b>::const_iterator it= image.begin<cv::Vec3b>();
	  cv::Mat_<cv::Vec3b>::const_iterator itend= image.end<cv::Vec3b>();
	  cv::Mat_<uchar>::iterator itout= result.begin<uchar>();

	  // get the iterators of the converted image 
	  if (useLab) {
		  it = converted.begin<cv::Vec3b>();
		  itend = converted.end<cv::Vec3b>();
	  }

	  // for each pixel
	  for ( ; it!= itend; ++it, ++itout) {
        
		// process each pixel ---------------------

		  // compute distance from target color
		  if (getDistanceToTargetColor(*it) < maxDist) {

			  *itout= 255;

		  } else {

			  *itout= 0;
		  }

        // end of pixel processing ----------------
	  }

	  return result;
}


cv::Mat ColorDetector::process2(const cv::Mat &image) {

	cv::Mat output;

	cv::absdiff(image, cv::Scalar(target), output);

	std::vector<cv::Mat> images;

	cv::split(output, images);

	output = images[0] + images[1] + images[2];

	cv::threshold(output,
					output,
					maxDist,
					255,
					cv::THRESH_BINARY_INV);
			
	return output;
}
