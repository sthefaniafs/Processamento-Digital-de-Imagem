#include <iostream>
#include <opencv2/opencv.hpp>

/*
Essa função foi feita para definir o funcionamento correto do for que tornará a imagem negativa. Afinal ele percorre do menor valor para o maior, incrementando a cada iteração, então é preciso garantir que ele inicie a contagem da menor coordenada para maior.
*/
void troca_val(int *a, int *b){

    int aux=0;

    if ((*a)>(*b)){
        aux=*a;
        *a=*b;
        *b=aux;
    }
    else
        return;
}

int main (){

    cv::Mat image;
    cv::Vec2i p1;
    cv::Vec2i p2;

    image = cv::imread("biel.png", cv::IMREAD_GRAYSCALE);
    if(!image.data)
        std::cout << "A imagem nao abriu" << std::endl;

    cv::namedWindow("Recorte negativo", cv::WINDOW_AUTOSIZE);

    std:: cout << "O tamanho da imagem é: " << image.rows << "x"<< image.cols << std:: endl;
    std:: cout << "Insira 2 pontos dentro do limite." << std:: endl;

    /*
    o do-while é utilizado para impedir que se insira uma coordenada fora do tamanho da imagem
    */

    do {
        std::cout << "Insira a coordenada x do 1º ponto:" << std:: endl;
        std::cin >> p1[0];
    } while (p1[0]>=image.rows);

    do {
        std::cout << "Insira a coordenada y do 1º ponto:" << std:: endl;
        std::cin >> p1[1];
    } while (p1[1]>=image.cols);

    do {
        std::cout << "Insira a coordenada x do 2º ponto:" << std:: endl;
        std::cin >> p2[0];
    } while (p2[0]>=image.rows);

    do {
        std::cout << "Insira a coordenada y do 2º ponto:" << std:: endl;
        std::cin >> p2[1];
    } while (p2[1]>=image.cols);

    /*
    se a coordenada x1 e x2  ou y1 e y2 forem iguais a condição do for não é atendida e a imagem  não ficará negativa, então se alguém preencher dessa forma o programa será encerrado com aviso de erro.
    */

    if (p1[0]==p2[0]|| p1[1]==p2[1]){
        std:: cout<< "Erro: coordenadas inválidas. x1 deve ser diferente de x2, y1 deve ser diferente de y2." << std:: endl;
        return 0;
    }

    troca_val(&p1[0],&p2[0]);
    troca_val(&p1[1],&p2[1]);

    for(int i=p1[0]; i<p2[0]; i++){
        for(int j=p1[1]; j<p2[1]; j++){
            image.at<uchar>(i,j)= 255 - image.at<uchar>(i,j);
        }
    }

    cv::imshow("Recorte negativo", image);
    cv::waitKey();

    return 0;
}