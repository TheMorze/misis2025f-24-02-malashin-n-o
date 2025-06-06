#include "blur.hpp"
#include <thread>
#include <algorithm>

static Color averageAt(const Image& img, int x, int y, int k) {
    int half = k/2;
    int cnt = 0;
    int sr=0, sg=0, sb=0;
    for(int i = std::max(0, x-half); i <= std::min((int)img.size()-1, x+half); ++i)
        for(int j = std::max(0, y-half); j <= std::min((int)img[0].size()-1, y+half); ++j) {
            sr += img[i][j].r;
            sg += img[i][j].g;
            sb += img[i][j].b;
            ++cnt;
        }
    return {uint8_t(sr/cnt), uint8_t(sg/cnt), uint8_t(sb/cnt)};
}

Image sequentialBlur(const Image& input, int k) {
    int h = input.size(), w = input[0].size();
    Image output(h, std::vector<Color>(w));
    for(int i=0;i<h;++i)
        for(int j=0;j<w;++j)
            output[i][j] = averageAt(input, i, j, k);
    return output;
}

Image parallelBlurThreads(const Image& input, int k, int numThreads) {
    int h = input.size(), w = input[0].size();
    Image output(h, std::vector<Color>(w));
    std::vector<std::thread> threads;
    auto worker = [&](int startRow, int endRow){
        for(int i = startRow; i < endRow; ++i)
            for(int j = 0; j < w; ++j)
                output[i][j] = averageAt(input, i, j, k);
    };
    int rowsPer = h / numThreads;
    for(int t=0; t<numThreads; ++t) {
        int start = t*rowsPer;
        int end   = (t==numThreads-1) ? h : start+rowsPer;
        threads.emplace_back(worker, start, end);
    }
    for(auto& th: threads) th.join();
    return output;
}


