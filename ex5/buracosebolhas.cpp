#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv) {
    cv::Mat image;
    int width, height;
    int total_bolhas=0;
    int bolhas_cm_buraco=0;
    int bolhas_na_borda=0;

    cv::Point p;
    image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

    if (!image.data) {
        std::cout << "imagem nao carregou corretamente\n";
        return (-1);
    }

    cv::imshow("Imagem original", image);
    cv::imwrite("original.png", image);
    cv::waitKey();

    width = image.cols;
    height = image.rows;
    std::cout << width << "x" << height << std::endl;

    p.x = 0;
    p.y = 0;

    /*
    Removendo bolhas das bordas:
    o loop verifica os bjetos presentes nas bordas
    e pinta de preto
    */
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0 || i == (height - 1) || j == 0 || j == (width - 1))
            {
                if (image.at<uchar>(i, j) == 255)
                {
                    p.x = j;
                    p.y = i;
                    bolhas_na_borda++;
                    cv::floodFill(image, p, 0);
                }
            }
        }
    }

    cv::imshow("Sem bolhas na borda", image);
    cv::imwrite("borda.png", image);
    cv::waitKey();

    /*
    Contar bolhas com buraco: primeiro mudo a cor do fundo para
    um tom de cinza (100), assim só restará a cor preta dentro
    das bolhas com buraco permitindo a contagem.
    Depois o buraco é pintado de branco
    */
    p.x = 0;
    p.y = 0;
    cv::floodFill(image, p, 100);
    cv::imshow("Bolhas com buraco", image);
    cv::imwrite("buraco.png", image);
    cv::waitKey();

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (image.at<uchar>(i, j) == 0)
            {
                p.x = j;
                p.y = i;
                bolhas_cm_buraco++;
                cv::floodFill(image, p, 255);
            }
        }
    }

    p.x = 0;
    p.y = 0;
    /*volta o fundo para a cor original (preto)*/
    cv::floodFill(image, p, 0);

    /*
    utilizando o algoritmo labeling conto o total de bolhas na imagem
    */

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (image.at<uchar>(i, j) == 255)
            {
                // achou um objeto
                total_bolhas++;
                // para o floodfill as coordenadas
                // x e y são trocadas.
                p.x = j;
                p.y = i;
                // preenche o objeto com o contador
                cv::floodFill(image, p, total_bolhas);
            }
        }
    }

    int bolhas_sem_buraco=total_bolhas-bolhas_cm_buraco;

    cv::imshow("Bolhas", image);
    cv::imwrite("Bolhas.png", image);
    cv::waitKey();

    std::cout << "Total de bolhas na imagem: " << total_bolhas << std:: endl;
    std::cout << "Bolhas com buraco: " << bolhas_cm_buraco << std:: endl;
    std::cout << "Bolhas sem buraco: " << bolhas_sem_buraco << std:: endl;
    std::cout << "Bolhas que estavam na borda: " << bolhas_na_borda << std:: endl;

    return 0;
}