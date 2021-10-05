#include <opencv2\opencv.hpp>
#include <opencv2\xfeatures2d.hpp>
#include <opencv2\face.hpp>
 
using namespace cv;
using namespace std;
 
void test() {
    Mat img=imread("1.png");
    cv::imshow("image",img);
    cv::waitKey();
}



int main()
{
    // test();
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
    return 0;
}