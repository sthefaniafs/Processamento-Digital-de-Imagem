#include <iostream>
#include <vector>
#include <math.h>
#include <opencv2/opencv.hpp>

void swapQuadrants(cv::Mat& image) {
  cv::Mat tmp, A, B, C, D;

  // se a imagem tiver tamanho impar, recorta a regiao para o maior
  // tamanho par possivel (-2 = 1111...1110)
  image = image(cv::Rect(0, 0, image.cols & -2, image.rows & -2));

  int centerX = image.cols / 2;
  int centerY = image.rows / 2;

  // rearranja os quadrantes da transformada de Fourier de forma que 
  // a origem fique no centro da imagem
  // A B   ->  D C
  // C D       B A
  A = image(cv::Rect(0, 0, centerX, centerY));
  B = image(cv::Rect(centerX, 0, centerX, centerY));
  C = image(cv::Rect(0, centerY, centerX, centerY));
  D = image(cv::Rect(centerX, centerY, centerX, centerY));

  // swap quadrants (Top-Left with Bottom-Right)
  A.copyTo(tmp);
  D.copyTo(A);
  tmp.copyTo(D);

  // swap quadrant (Top-Right with Bottom-Left)
  C.copyTo(tmp);
  B.copyTo(C);
  tmp.copyTo(B);
}

void makeFilter(const cv::Mat &image, cv::Mat &filter){
  cv::Mat_<float> filter2D(image.rows, image.cols);
  int centerX = image.cols / 2;
  int centerY = image.rows / 2;
  /* Aumenta-se o contraste da imagem ...*/
  float YH = 1.5; /*e a refletância é aumentada (γH > 1)*/
  float YL = 0.25; /*se a iluminação é diminuída (0 < γL < 1)*/
  float c = 1.0; 

  /* Para fazer o filtro homomórfico considera-se a seguinte equação:
     H(u,v) = (γH − γL)(1 − e^(− c(D²(u,v)/D0²))) + γL
  */

  for (int i = 0; i < image.rows; i++){
    for (int j = 0; j < image.cols; j++){
      float D = sqrt(pow(i-centerY,2) + pow(j-centerX,2));
      float H = (YH - YL) * (1.0 - exp(-c * (pow(D,2) / pow(centerX,2)))) + YL;
      filter2D.at<float>(i, j) = H;
    }
  }
  cv::Mat planes[] = {cv::Mat_<float>(filter2D), cv::Mat::zeros(filter2D.size(), CV_32F)};
  cv::merge(planes, 2, filter);
}

int main(int argc, char** argv) {
  cv::Mat image, padded, complexImage;
  std::vector<cv::Mat> planos; 

  image = imread(argv[1], cv::IMREAD_GRAYSCALE);
  if (image.empty()) {
    std::cout << "Erro abrindo imagem" << argv[1] << std::endl;
    return EXIT_FAILURE;
  }


  // expande a imagem de entrada para o melhor tamanho no qual a DFT pode ser
  // executada, preenchendo com zeros a lateral inferior direita.
  int dft_M = cv::getOptimalDFTSize(image.rows);
  int dft_N = cv::getOptimalDFTSize(image.cols); 
  cv::copyMakeBorder(image, padded, 0, dft_M - image.rows, 0, dft_N - image.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

  // prepara a matriz complexa para ser preenchida
  // primeiro a parte real, contendo a imagem de entrada
  planos.push_back(cv::Mat_<float>(padded)); 
  // depois a parte imaginaria com valores nulos
  planos.push_back(cv::Mat::zeros(padded.size(), CV_32F));

  // combina os planos em uma unica estrutura de dados complexa
  cv::merge(planos, complexImage);  

  // calcula a DFT
  cv::dft(complexImage, complexImage); 
  swapQuadrants(complexImage);

  // cria o filtro ideal e aplica a filtragem de frequencia
  cv::Mat filter;
  makeFilter(complexImage, filter);
  cv::mulSpectrums(complexImage, filter, complexImage, 0);

  // calcula a DFT inversa
  swapQuadrants(complexImage);
  cv::idft(complexImage, complexImage);

  // planos[0] : Re(DFT(image)
  // planos[1] : Im(DFT(image)
  cv::split(complexImage, planos);

  // recorta a imagem filtrada para o tamanho original
  // selecionando a regiao de interesse (roi)
  cv::Rect roi(0, 0, image.cols, image.rows);
  cv::Mat result = planos[0](roi);

  // normaliza a parte real para exibicao
  cv::normalize(result, result, 0, 1, cv::NORM_MINMAX);

  cv::imshow("original", image);
  cv::imshow("filtrada", result);
  cv::imwrite("dft-filter.png", result * 255);

  cv::waitKey();
  return EXIT_SUCCESS;
}
