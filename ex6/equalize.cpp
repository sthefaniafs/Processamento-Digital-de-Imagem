#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv){
  cv::Mat image, framequalizado;
  int width, height;
  cv::VideoCapture cap;
  std::vector<cv::Mat> planes;
  cv::Mat hist, historiginal;
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
  cv::Mat hist1(histh, histw, CV_8UC1, cv::Scalar(0));
  cv::Mat hist2(histh, histw, CV_8UC1, cv::Scalar(0));

  while(1){
    cap >> image;

    /*converter frame colorido para cinza*/
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

    /*equalizar histograma com função do opencv*/
    cv::equalizeHist(image, framequalizado);

    /*Calcular o histograma das imagem original*/
    cv::calcHist(&image, 1, 0, cv::Mat(), historiginal, 1, &nbins, &histrange, uniform, acummulate);
    
    /*Calcular o histograma das equalizada*/    
    cv::calcHist(&framequalizado, 1, 0, cv::Mat(), hist, 1, &nbins, &histrange, uniform, acummulate);
    
    /*normalizar os histogramas*/
    cv::normalize(historiginal, historiginal, 0, hist1.rows, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(hist, hist, 0, hist2.rows, cv::NORM_MINMAX, -1, cv::Mat());

    hist1.setTo(cv::Scalar(0));
    hist2.setTo(cv::Scalar(0));
    
    for(int i=0; i<nbins; i++){
        cv::line(hist1,
               cv::Point(i, histh),
               cv::Point(i, histh-cvRound(historiginal.at<float>(i))),
               cv::Scalar(255, 255, 255), 1, 8, 0);
        cv::line(hist2,
               cv::Point(i, histh),
               cv::Point(i, histh-cvRound(hist.at<float>(i))),
               cv::Scalar(255, 255, 255), 1, 8, 0);
    }

    hist1.copyTo(image(cv::Rect(0,0,nbins, histh)));
    hist2.copyTo(framequalizado(cv::Rect(0,0,nbins, histh)));
    cv::imshow("Original", image);
    cv::imshow("Equalizado", framequalizado);
    key = cv::waitKey(30);
    if(key == 27) break;
  }
  return 0;
}
