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

int main()
{
    // test();
    // facedetect();
    Mat img=imread("1.png");
    salt(img, 3000);
    cv::imshow("image",img);
    cv::waitKey();

    
    return 0;
}