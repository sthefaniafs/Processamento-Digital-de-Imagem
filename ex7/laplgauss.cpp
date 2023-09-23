#include <iostream>
#include <opencv2/opencv.hpp>

int main(int, char **)
{
    cv::Mat image;

    //image = cv::imread("imgsalepimenta.png", cv::IMREAD_GRAYSCALE);
    image = cv::imread("mulher.png", cv::IMREAD_GRAYSCALE);


    if (!image.data)
        std::cout << "nao abriu mulher.png" << std::endl;

    cv::imshow("Original com ruido", image);

    float media[] = {0.1111, 0.1111, 0.1111, 0.1111, 0.1111,
                     0.1111, 0.1111, 0.1111, 0.1111};
    float gauss[] = {0.0625, 0.125, 0.0625, 0.125, 0.25,
                     0.125, 0.0625, 0.125, 0.0625};
    float vertical [] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};     // Máscara de Sobel vertical
    float horizontal [] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};   // Máscara de Sobel horizontal
    float mais45 [] = {0, 1, 2, -1, 0, 1, -2, -1, 0};       // Máscara de Sobel +45°
    float menos45 [] = {-2, -1, 0, -1, 0, 1, 0, 1, 2};      // Máscara de Sobel -45°
    float laplacian[] = {0, -1, 0, -1, 4, -1, 0, -1, 0};
    float boost[] = {0, -1, 0, -1, 5.2, -1, 0, -1, 0};      // Máscara de nitidez

    cv::Mat image32f, imageFiltered, secondFilteredimage;
    cv::Mat mask(3, 3, CV_32F);
    cv::Mat result;
    int absolut, laplgauss = 0;
    char key;

    mask = cv::Mat(3, 3, CV_32F, media);
    absolut = 1; // calcs abs of the image

    // Loop para o programa rodar até que ESC seja pressionada
    while(key != 27){

    key = (char)cv::waitKey();
    switch (key)
    {
    case 'a':
        absolut = !absolut;
        laplgauss = 0;
        break;
    case 'm':
        mask = cv::Mat(3, 3, CV_32F, media);
        laplgauss = 0;
        break;
    case 'g':
        mask = cv::Mat(3, 3, CV_32F, gauss);
        laplgauss = 0;
        break;
    case 'h':
        mask = cv::Mat(3, 3, CV_32F, horizontal);
        laplgauss = 0;
        break;
    case 'v':
        mask = cv::Mat(3, 3, CV_32F, vertical);
        laplgauss = 0;
        break;
    case 'j':
        mask = cv::Mat(3, 3, CV_32F, mais45);
        laplgauss = 0;
        break;
    case 'k':
        mask = cv::Mat(3, 3, CV_32F, menos45);
        laplgauss = 0;
        break;
    case 'l':
        mask = cv::Mat(3, 3, CV_32F, laplacian);
        laplgauss = 0;
        break;
    case 'b':
        mask = cv::Mat(3, 3, CV_32F, boost);
        laplgauss = 0;
        break;
    case 'p':
        // Opção para o filtro LoG (Laplaciano do Gaussiano)
        laplgauss = 1;
        break;
    default:
        break;
    }

    // Caso a seleção seja Laplaciano do Gaussiano
    if (laplgauss == 1)
    {
        image.convertTo(image32f, CV_32F);

        // Aplicando o filtro gaussiano
        mask = cv::Mat(3, 3, CV_32F, gauss);
        cv::filter2D(image32f, imageFiltered, image32f.depth(),
                     mask,
                     cv::Point(1, 1), 0);
        // Aplicando o filtro laplaciano
        mask = cv::Mat(3, 3, CV_32F, laplacian);
        cv::filter2D(imageFiltered, secondFilteredimage, image32f.depth(),
                     mask,
                     cv::Point(1, 1), 0);

        if (absolut)
        {
            secondFilteredimage = cv::abs(secondFilteredimage);
        }

        secondFilteredimage.convertTo(result, CV_8U);
    }
    else if (laplgauss == 0)
    {
        image.convertTo(image32f, CV_32F);

        cv::filter2D(image32f, imageFiltered, image32f.depth(),
                     mask,
                     cv::Point(1, 1), 0);
        if (absolut)
        {
            imageFiltered = cv::abs(imageFiltered);
        }

        imageFiltered.convertTo(result, CV_8U);
    }

    cv::namedWindow("ImagemFiltrada", cv::WINDOW_NORMAL);
    cv::imshow("Filtrado", result);
    }
    cv::waitKey();
    return 0;
}