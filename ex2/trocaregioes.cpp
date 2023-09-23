#include <iostream>
#include <opencv2/opencv.hpp>

int main (){
    /*Definindo imagem original*/
    cv::Mat image;
    /*Definindo imagem com os quadrantes mudados.
      Ela terá o mesmo número de linhas e colunas da imagem
      original e o mesmo tipo*/

    image = cv::imread("biel.png", cv::IMREAD_GRAYSCALE);

    if(!image.data)
        std::cout << "A imagem nao abriu" << std::endl;

    cv::namedWindow("Imagem original", cv::WINDOW_AUTOSIZE);
    cv::imshow("Imagem original", image);
    cv::waitKey();

    cv::Mat trocaquadrante(image.rows, image.cols, image.type());

    /*quadrante superior esquerdo*/
    cv::Mat Q1 = image(cv::Rect(0, 0, image.rows/2, image.cols/2));
    /*quadrante inferior esquerdo*/
    cv::Mat Q2 = image(cv::Rect(0, image.cols/2, image.rows/2, image.cols/2));
    /*quadrante superior direito*/
    cv::Mat Q3 = image(cv::Rect(image.rows/2, 0, image.rows/2, image.cols/2));
    /*quadrante inferior direito*/
    cv::Mat Q4 = image(cv::Rect(image.rows/2, image.cols/2, image.rows/2, image.cols/2));

    Q4.copyTo(trocaquadrante(cv::Rect(0,0,image.rows/2,image.cols/2)));
    Q3.copyTo(trocaquadrante(cv::Rect(0,image.cols/2,image.rows/2,image.cols/2)));
    Q2.copyTo(trocaquadrante(cv::Rect(image.rows/2,0,image.rows/2,image.cols/2)));
    Q1.copyTo(trocaquadrante(cv::Rect(image.rows/2,image.cols/2,image.rows/2,image.cols/2)));


    cv::namedWindow("Imagem trocada", cv::WINDOW_AUTOSIZE);
    cv::imshow("Imagem trocada", trocaquadrante);
    cv::waitKey();

    return 0;
}