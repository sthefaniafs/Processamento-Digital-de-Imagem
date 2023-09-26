#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char**argv) {
  cv::Mat imagemPortadora, imagemFinal;
  cv::Vec3b valPortadora;
  int nbits = 3;

  imagemPortadora = cv::imread(argv[1], cv::IMREAD_COLOR);

  if (imagemPortadora.empty()) {
    std::cout << "imagem nao carregou corretamente" << std::endl;
    return (-1);
  }

  imagemFinal = imagemPortadora.clone();

  for (int i = 0; i < imagemPortadora.rows; i++) {
    for (int j = 0; j < imagemPortadora.cols; j++) {
      valPortadora = imagemPortadora.at<cv::Vec3b>(i, j);

    /*os bits menos significativos dos pixels da imagem fornecida irão compor 
    os bits mais significativos dos pixels da imagem recuperada*/
      valPortadora[0] = valPortadora[0] << (8-nbits);
      valPortadora[1] = valPortadora[1] << (8-nbits);
      valPortadora[2] = valPortadora[2] << (8-nbits);

      imagemFinal.at<cv::Vec3b>(i, j) = valPortadora;
    }
  }
 
  cv::imshow("Original", imagemPortadora);
  cv::waitKey();
  cv::imshow("Decodificada", imagemFinal);
  cv::waitKey();
  return 0;
}
