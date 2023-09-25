#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, char** argv){
  cv::Mat image;
  int width, height;
  cv::VideoCapture cap;
  std::vector<cv::Mat> planes;
  cv::Mat histatual, histanterior;
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
  cv::Mat histImgR(histh, histw, CV_8UC3, cv::Scalar(0,0,0));

    cap >> image;

  /*o slit é usado para separar somente a cor de interesse do programa,
  que no caso é vermelho*/
  cv::split(image, planes);

  /*calculando histograma da imagem considerando a cor vermelha*/
  cv::calcHist(&planes[0], 1, 0, cv::Mat(), histatual, 1,
           &nbins, &histrange,
           uniform, acummulate);

  /*normalizando histograma*/ 
  cv::normalize(histatual, histatual, 0, histImgR.rows, cv::NORM_MINMAX, -1, cv::Mat());

  while(1){
    /*o histograma anterior recebe o atual*/
    histatual.copyTo(histanterior);

    cap >> image;

    cv::split(image,planes);

    cv::calcHist(&planes[0], 1, 0, cv::Mat(), histatual, 1, &nbins, &histrange, uniform, acummulate);
    
    cv::normalize(histatual, histatual, 0, histImgR.rows, cv::NORM_MINMAX, -1, cv::Mat());

    histImgR.setTo(cv::Scalar(0));

    double comp = cv::compareHist(histatual, histanterior, cv::HISTCMP_CORREL);
    
    /*definição do valor que define o movimento*/
    if(comp < 0.99){
      std::cout<< "Movimento detectado\n";
    }

    for(int i=0; i<nbins; i++){
        cv::line(histImgR,
               cv::Point(i, histh),
               cv::Point(i, histh-cvRound(histatual.at<float>(i))),
               cv::Scalar(0, 0, 255), 1, 8, 0);
    }

    histImgR.copyTo(image(cv::Rect(0,0,nbins, histh)));
    cv::imshow("Detector de movimento", image);
    key = cv::waitKey(30);
    if(key == 27) break;
  }
  return 0;
}
