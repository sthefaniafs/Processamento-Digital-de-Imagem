#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv){
  cv::Mat image, framequalizado;
  int width, height;
  cv::VideoCapture cap;
  std::vector<cv::Mat> planes;
  cv::Mat hist;
  int nbins = 64;
  float range[] = {0, 255};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;
  int key;

	cap.open("paisagem.mp4");
  
  if(!cap.isOpened()){
    std::cout << "video indisponível";
    return -1;
  }
  
  cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);  
  width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
  height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

  std::cout << "largura = " << width << std::endl;
  std::cout << "altura  = " << height << std::endl;

  int histw = nbins, histh = nbins/2;
  cv::Mat histimage(histh, histw, CV_8UC1, cv::Scalar(0));

  while(1){
    cap >> image;
    /*converter frame para cinza*/
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

    /*equalizar histograma com função do opencv*/
    cv::equalizeHist(image, framequalizado);

    cv::calcHist(&image, 1, 0, cv::Mat(), hist, 1, &nbins, &histrange, uniform, acummulate);
    
    cv::normalize(hist, hist, 0, histimage.rows, cv::NORM_MINMAX, -1, cv::Mat());

    histimage.setTo(cv::Scalar(0));
    
    for(int i=0; i<nbins; i++){
        cv::line(histimage,
               cv::Point(i, histh),
               cv::Point(i, histh-cvRound(hist.at<float>(i))),
               cv::Scalar(0, 0, 255), 1, 8, 0);
    }
    cv::imshow("Original", image);
    cv::imshow("Equalizado", framequalizado);
    key = cv::waitKey(30);
    if(key == 27) break;
  }
  return 0;
}
