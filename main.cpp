#include <opencv2\opencv.hpp>
#include <opencv2\xfeatures2d.hpp>
#include <opencv2\face.hpp>
 
using namespace cv;
 
void test() {
    Mat img=imread("1.png");
    cv::imshow("image",img);
    cv::waitKey();
}

void facedetect() {
    Mat frame;
    VideoCapture capture(1);
    CascadeClassifier face_cascade;
    face_cascade.load("C:\\opencv3\\build\\etc\\haarcascades\\haarcascade_frontalface_alt.xml");

    while (true)
    {
       
        capture >> frame;     
        std::vector<Rect> faces;
        face_cascade.detectMultiScale(frame, faces, 1.1, 3,0, Size(100, 100));
        for(size_t i = 0; i < faces.size(); i++) {

            rectangle(frame, faces[i], Scalar(255, 255, 255), 2, 2, 0);

        }

        if(!frame.empty()){
 
            imshow("window",frame);
        }
 
        if(waitKey(20) == 'q')
        break;
    }
   
    capture.release();  
    destroyAllWindows();
}

void salt(Mat img, int n) {
    int i, j;
    for(int k=0; k<n; k++) {
        i = std::rand() % img.cols;
        j = std::rand() % img.rows;
        
        if (img.type() == CV_8UC1) {
            img.at<uchar>(j, i) = 255;
        }
        else if (img.type() == CV_8UC3) {
            img.at<cv::Vec3b>(j, i)[0] = 255;
            img.at<cv::Vec3b>(j, i)[1] = 255;
            img.at<cv::Vec3b>(j, i)[2] = 255;
        }
    }
}

void colorRestore(Mat &image, Mat &result, int div=64) {
    
    Mat_ <cv::Vec3b>::iterator it = image.begin<cv::Vec3b>();
    Mat_ <cv::Vec3b>::iterator itend = image.end<cv::Vec3b>();


    for ( ; it!=itend; ++it) {
        (*it)[0] = (*it)[0]/div*div + div/2;
        (*it)[1] = (*it)[1]/div*div + div/2;
        (*it)[2] = (*it)[2]/div*div + div/2;
    }
    result = image;

}

void sharpen(const cv::Mat &image, cv::Mat &result) {
    result.create(image.size(), image.type());
    int nchannels = image.channels();
    for (int j=1; j<image.rows-1; j++) {
        const uchar* prev = image.ptr<const uchar>(j-1);
        const uchar* curr = image.ptr<const uchar>(j);
        const uchar* next = image.ptr<const uchar>(j+1);

        uchar* out = result.ptr<uchar>(j);

        for (int i=nchannels; i<(image.cols-1) * nchannels; i++) {
            *out ++ = cv::saturate_cast<uchar>(
                5*curr[i] - curr[i-nchannels] - curr[i+nchannels] - prev[i] - next[i]); 
        }
        
        
    }
}


int main()
{
    // test();
    // facedetect();

    const int64 start = cv::getTickCount();

    Mat img = imread("2.jpg");
    // salt(img, 3000);
    Mat result;
    result.create(img.rows, img.cols, img.type());
    colorRestore(img, result);
    // sharpen(img, result);
    
    double duration = (cv::getTickCount() - start) / cv::getTickFrequency();

    std::cout << "Time: " << duration << std::endl;
    
    
    cv::imshow("image", result);
    cv::waitKey();

    
    return 0;
}