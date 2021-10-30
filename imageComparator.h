#if !defined ICOMPARATOR
#define ICOMPARATOR

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "colorhistogram.h"

class ImageComparator {
    private:
        cv::Mat refH;
        cv::Mat inputH;

        ColorHistogram hist;
        int nBins;
    
    public:
        ImageComparator(): nBins(8) {

        }

        void setNumberOfBins( int bins) {

		nBins= bins;
        }

        int getNumberOfBins() {

            return nBins;
        }

        // set and compute histogram of reference image
        void setReferenceImage(const cv::Mat& image) {

            hist.setSize(nBins);
            refH= hist.getHistogram(image);
        }

        // compare the image using their BGR histograms
        double compare(const cv::Mat& image) {

            inputH = hist.getHistogram(image);

            // histogram comparison using intersection
            return cv::compareHist(refH,inputH, cv::HISTCMP_INTERSECT);
        }
};

#endif