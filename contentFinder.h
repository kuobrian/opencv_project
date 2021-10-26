#if !defined OFINDER
#define OFINDER


#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class ContentFinder {
    private:
        float hranges[2];
        const float* ranges[3];
        int channels[3];

        float threshold;
        cv::Mat histogram;
        cv::SparseMat shistogram;
        bool isSparse;

    public:
        ContentFinder() : threshold(0.1f), isSparse(false) {
            ranges[0]= hranges;  
            ranges[1]= hranges; 
            ranges[2]= hranges; 
        }
        // Sets the threshold on histogram values [0,1]
        void setThreshold(float t) {

            threshold= t;
        }

        // Gets the threshold
        float getThreshold() {

            return threshold;
        }
        // Sets the reference histogram
        void setHistogram(const cv::Mat& h) {

            isSparse= false;
            cv::normalize(h,histogram,1.0);
        }

        // Sets the reference histogram
        void setHistogram(const cv::SparseMat& h) {

            isSparse= true;
            cv::normalize(h,shistogram,1.0,cv::NORM_L2);
        }

        cv::Mat find(const cv::Mat& img) {
            cv::Mat result;

            hranges[0]= 0.0;	// default range [0,256[
            hranges[1]= 256.0;
            channels[0]= 0;		// the three channels 
            channels[1]= 1; 
            channels[2]= 2; 

            return find(img, hranges[0], hranges[1], channels);
        }
        cv::Mat find(const cv::Mat& image, float minValue, float maxValue, int *channels) {

		cv::Mat result;

		hranges[0]= minValue;
		hranges[1]= maxValue;

		if (isSparse) { // call the right function based on histogram type

		   for (int i=0; i<shistogram.dims(); i++)
			  this->channels[i]= channels[i];

		   cv::calcBackProject(&image,
                      1,            // we only use one image at a time
                      channels,     // vector specifying what histogram dimensions belong to what image channels
                      shistogram,   // the histogram we are using
                      result,       // the resulting back projection image
                      ranges,       // the range of values, for each dimension
                      255.0         // the scaling factor is chosen such that a histogram value of 1 maps to 255
		   );

		} else {

		   for (int i=0; i<histogram.dims; i++)
			  this->channels[i]= channels[i];

		   cv::calcBackProject(&image,
                      1,            // we only use one image at a time
                      channels,     // vector specifying what histogram dimensions belong to what image channels
                      histogram,    // the histogram we are using
                      result,       // the resulting back projection image
                      ranges,       // the range of values, for each dimension
                      255.0         // the scaling factor is chosen such that a histogram value of 1 maps to 255
		   );
		}

        // Threshold back projection to obtain a binary image
		if (threshold>0.0)
			cv::threshold(result, result, 255.0*threshold, 255.0, cv::THRESH_BINARY);

		return result;
	}


};







#endif