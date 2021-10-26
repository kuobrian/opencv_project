#if !defined HISTOGRAM
#define HISTOGRAM

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

class Histogram1D {
    private:
        int histSize[1];
        float hranges[2];
        const float* ranges[1];
        int channels[1];

    public: 
        Histogram1D() {
            histSize[0] = 256;
            hranges[0] = 0.0;
            hranges[1] = 256.0;
            ranges[0] = hranges;
            channels[0] = 0;
        }

        cv::Mat getHistogram(const cv::Mat &img) {
            cv::Mat hist;

            cv::calcHist(&img, 1, channels, cv::Mat(), hist, 1, histSize, ranges);
            return hist;
        }

        cv::Mat getHistogramImage(const cv::Mat &img, int zoom = 1) {
            cv::Mat hist = getHistogram(img);

            return Histogram1D::getImageOfHistogram(hist, zoom);
        }
        
        // Equalizes the source image.
        static cv::Mat equalize(const cv::Mat &image) {

            cv::Mat result;
            cv::equalizeHist(image,result);

            return result;
        }

        
        static cv::Mat getImageOfHistogram(const cv::Mat &hist, int zoom) {

            // Get min and max bin values
            double maxVal = 0;
            double minVal = 0;
            cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);

            // get histogram size
            int histSize = hist.rows;

            // Square image on which to display histogram
            cv::Mat histImg(histSize*zoom, histSize*zoom, CV_8U, cv::Scalar(255));

            // set highest point at 90% of nbins (i.e. image height)
            int hpt = static_cast<int>(0.9*histSize);

            // Draw vertical line for each bin
            for (int h = 0; h < histSize; h++) {

                float binVal = hist.at<float>(h);
                if (binVal>0) {
                    int intensity = static_cast<int>(binVal*hpt / maxVal);
                    cv::line(histImg, cv::Point(h*zoom, histSize*zoom),
                        cv::Point(h*zoom, (histSize - intensity)*zoom), cv::Scalar(0), zoom);
                }
            }

        return histImg;
    }

    static cv::Mat applyLookUp(const cv::Mat& image, const cv::Mat& lookup) {
        cv::Mat result;

        cv::LUT(image, lookup, result);
        return result;
    }

    static cv::Mat applyLookUpWithIterator(const cv::Mat& image, const cv::Mat& lookup) {
        cv::Mat result(image.rows, image.cols, CV_8U);
        cv::Mat_<uchar>::iterator itr = result.begin<uchar>();

        cv::Mat_<uchar>::const_iterator it = image.begin<uchar>();
        cv::Mat_<uchar>::const_iterator itend = image.end<uchar>();

        for (;  it!=itend; ++it, ++itr) {
            *itr = lookup.at<uchar>(*it);
        }

        return result;
    }

    cv::Mat stretch(const cv::Mat &image, float percentils) {

        float number = image.total() * percentils;
        cv::Mat hist = getHistogram(image);

         // find left extremity of the histogram
        int imin = 0;
        for (float count=0.0; imin < 256; imin++) {
            // number of pixel at imin and below must be > number
            if ((count+=hist.at<float>(imin)) >= number)
                break;
        }

        // find right extremity of the histogram
        int imax = 255;
        for (float count=0.0; imax >= 0; imax--) {
            // number of pixel at imax and below must be > number
            if ((count += hist.at<float>(imax)) >= number)
                break;
        }

        int dims[1] = {256};
        cv::Mat lookup(1, dims, CV_8U);

        for (int i=0; i<256; i++) {
            if (i<imin) lookup.at<uchar>(i) = 0;
            else if (i>imax) lookup.at<uchar>(i) = 255;
            else lookup.at<uchar>(i) = cvRound(255.0*(i - imin) / (imax - imin));;
        }

        // Apply lookup table
        cv::Mat result;
        result = applyLookUp(image, lookup);

        return result;


    }



};

#endif