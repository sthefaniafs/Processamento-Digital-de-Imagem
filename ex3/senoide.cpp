#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>

int SIDE = 256;
int PERIODOS = 4;

int main(int argc, char** argv) {
  std::stringstream ss_img, ss_yml;
  cv::Mat image;

  ss_yml << "senoide-" << SIDE << ".yml";
  image = cv::Mat::zeros(SIDE, SIDE, CV_32FC1);

  cv::FileStorage fs(ss_yml.str(), cv::FileStorage::WRITE);

  for (int i = 0; i < SIDE; i++) {
    for (int j = 0; j < SIDE; j++) {
      image.at<float>(i, j) = 127 * sin(2 * M_PI * PERIODOS * j / SIDE) + 128;
    }
  }
    /*arquivo txt da imagem yml, será coletada uma linha para comparação com png*/
    std::ofstream Fileyml("img_yml.txt");
    

    if (!Fileyml.is_open()){
        std::cout << "Erro ao criar o arquivo .txt" << std::endl;
        return -1;
    }
    for (int i = 0; i < image.rows; i++)
    {
        float pixels = image.at<float>(i);
        Fileyml << pixels << " ";
    }
    Fileyml.close();

  fs << "mat" << image;
  fs.release();

  cv::normalize(image, image, 0, 255, cv::NORM_MINMAX);
  image.convertTo(image, CV_8U);
  ss_img << "senoide-" << SIDE << ".png";
  cv::imwrite(ss_img.str(), image);

  fs.open(ss_yml.str(), cv::FileStorage::READ);
  fs["mat"] >> image;

  cv::normalize(image, image, 0, 255, cv::NORM_MINMAX);
  image.convertTo(image, CV_8U);

    /*arquivo txt da imagem png, será coletada uma linha para comparação com yml*/
    std::ofstream Filepng("img_png.txt");

    if (!Filepng.is_open()){
            std::cout << "Erro ao criar o arquivo .txt" << std::endl;
            return -1;
        }
        for (int i = 0; i < image.rows; i++)
        {
            float pixels = image.at<uchar>(i);
            Filepng << pixels << " ";
            
        }
        Filepng.close();

  cv::imshow("image", image);
  cv::waitKey();

  return 0;
}
