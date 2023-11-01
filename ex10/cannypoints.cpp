#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <opencv2/opencv.hpp>
#include <vector>

int STEP = 5;
int JITTER = 3;
int RAIO = 3;
int top_slider = 10;
int top_slider_max = 200;

char TrackbarName[50];

cv::Mat image, border, points;

void pointillism (){

    std::vector<int> yrange;
    std::vector<int> xrange;

    int width, height, gray;
    int x, y;

    width = image.cols;
    height = image.rows;

    xrange.resize(height / STEP);
    yrange.resize(width / STEP);

    std::iota(xrange.begin(), xrange.end(), 0);
    std::iota(yrange.begin(), yrange.end(), 0);

    for (uint i = 0; i < xrange.size(); i++) {
        xrange[i] = xrange[i] * STEP + STEP / 2;
    }

    for (uint i = 0; i < yrange.size(); i++) {
        yrange[i] = yrange[i] * STEP + STEP / 2;
    }

    points = cv::Mat(height, width, CV_8U, cv::Scalar(255));

    std::random_shuffle(xrange.begin(), xrange.end());

    for (auto i : xrange) {
        std::random_shuffle(yrange.begin(), yrange.end());
        for (auto j : yrange) {
            if (border.at<uchar>(i, j) == 255){
                x = i+rand()%(2*JITTER)-JITTER+1;
                y = j+rand()%(2*JITTER)-JITTER+1;
                gray = image.at<uchar>(x,y);
                circle(points, cv::Point(y, x), RAIO, CV_RGB(gray, gray, gray),
                cv::FILLED, cv::LINE_AA);
            }
            else{
                x = i+rand()%(2*JITTER)-JITTER+1;
                y = j+rand()%(2*JITTER)-JITTER+1;
                gray = image.at<uchar>(x,y);
                circle(points, cv::Point(y, x), RAIO, CV_RGB(gray, gray, gray),
                cv::FILLED, cv::LINE_AA);
            }
        }
    }

    cv::imshow("CannyPoints", points);
}

void on_trackbar_canny(int, void*){
    cv::Canny(image, border, top_slider, 3*top_slider);
    pointillism();
    cv::namedWindow("Canny",1);
    cv::imshow("Canny", border);
}

void on_trackbar_canny_points(int,void*){
    pointillism();
}


int main(int argc, char** argv) {

    image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

    std::srand(std::time(0));

    if (image.empty()) {
        std::cout << "Could not open or find the image" << std::endl;
        return -1;
    }


    sprintf( TrackbarName, "Threshold inferior", top_slider_max );

    cv::namedWindow("Canny",0);

    cv::createTrackbar("Threshold inferior", "CannyPoints",
                    &top_slider,
                    top_slider_max,
                    on_trackbar_canny );

    on_trackbar_canny(top_slider, 0);

    sprintf( TrackbarName, "Threshold inferior", top_slider_max );
    
    cv::createTrackbar( TrackbarName, "Canny",
                    &top_slider,
                    top_slider_max,
                    on_trackbar_canny );

    on_trackbar_canny(top_slider, 0 );

    /*cv::createTrackbar("STEP", "CannyPoints",
                    &STEP,
                    100,
                    on_trackbar_canny_points);

    on_trackbar_canny_points(STEP,0);

    cv::createTrackbar("JITTER", "CannyPoints",
                    &JITTER,
                    100,
                    on_trackbar_canny_points);

    on_trackbar_canny_points(JITTER,0);

    cv::createTrackbar("RAIO", "CannyPoints",
                    &RAIO,
                    100,
                    on_trackbar_canny_points);

    on_trackbar_canny_points(RAIO,0);*/

    cv::waitKey();
    cv::imwrite("cannyborders.png", border);

    return 1;

}
