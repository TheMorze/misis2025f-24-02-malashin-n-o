#include "blur.hpp"
#include <iostream>
#include <opencv2/opencv.hpp> // или stb_image + stb_image_write

// Загрузка и сохранение через OpenCV
Image loadImage(const std::string& path) {
    cv::Mat img = cv::imread(path);
    int h=img.rows, w=img.cols;
    Image out(h, std::vector<Color>(w));
    for(int i=0;i<h;++i)
        for(int j=0;j<w;++j) {
            auto c = img.at<cv::Vec3b>(i,j);
            out[i][j] = {c[2],c[1],c[0]};
        }
    return out;
}

void saveImage(const std::string& path, const Image& img) {
    int h=img.size(), w=img[0].size();
    cv::Mat out(h, w, CV_8UC3);
    for(int i=0;i<h;++i)
        for(int j=0;j<w;++j) {
            auto &c = img[i][j];
            out.at<cv::Vec3b>(i,j) = cv::Vec3b(c.b,c.g,c.r);
        }
    cv::imwrite(path, out);
}

int main() {
    auto input = loadImage("input.jpg");
    const int k = 3;
    const int threads = std::thread::hardware_concurrency();

    // Последовательное
    auto [seqImg, seqTime] = measure(sequentialBlur, input, k);
    std::cout << "Sequential: " << seqTime << " ms\n";
    saveImage("out_seq.jpg", seqImg);

    // Параллельное
    auto [parImg, parTime] = measure(parallelBlurThreads, input, k, threads);
    std::cout << "Parallel ("<<threads<<" threads): " << parTime << " ms\n";
    saveImage("out_par.jpg", parImg);

    return 0;
}
