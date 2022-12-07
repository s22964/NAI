#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>
int main( int argc, char** argv ) {
    using namespace std;
    std::chrono::time_point<std::chrono::system_clock> m_StartTime;
    std::chrono::time_point<std::chrono::system_clock> m_EndTime;

    bool capturing = true;
    // Question for you
     cv::VideoCapture cap( "nazwa.mp4" );
//     cv::VideoCapture cap(0);
    if ( !cap.isOpened() ) {
        std::cerr << "error opening frames source" << std::endl;
        return -1;
    }
    std::cout << "Video size: " << cap.get( cv::CAP_PROP_FRAME_WIDTH )
              << "x" << cap.get( cv::CAP_PROP_FRAME_HEIGHT ) << std::endl;
    do {
        cv::Mat frame;


        if ( cap.read( frame ) ) {
            m_StartTime = std::chrono::system_clock::now();
            cv::flip(frame,frame,1);
            cv::Mat f_g;
            cv::cvtColor(frame, f_g, cv::COLOR_BGR2GRAY);
            cv::Mat thresh;
            cv::threshold(f_g, thresh, 150, 255, cv::THRESH_BINARY);
            vector<vector<cv::Size>> contours;
            vector<cv::Vec4i> hierarchy;
            findContours(thresh, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
            cv::Mat image_copy = frame.clone();
            drawContours(image_copy, contours, -1, cv::Scalar(0, 255, 0), 2);
            double b=cv::contourArea(contours[0]);
            for (auto c : contours){
                auto a=cv::contourArea(c);
                if (a>b) b=a;
            }
            m_EndTime = std::chrono::system_clock::now();
            cout<<(std::chrono::duration_cast<std::chrono::milliseconds>(m_EndTime - m_StartTime).count())/1000.0<<endl;
            cv::imshow( "Not-yet smart windown", image_copy);



        } else {
            // stream finished
            capturing = false;
        }
        //czekaj na klawisz, sprawdz czy to jest 'esc'
        if( (cv::waitKey( 1000.0/60.0 )&0x0ff) == 27 ) capturing = false;
    } while( capturing );
    return 0;
}