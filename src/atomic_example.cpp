#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <chrono>

const int N = 10000000;
int counter_mutex = 0;
std::mutex mtx;
std::atomic<int> counter_atomic {0};

void incr_mutex() {
    for(int i=0;i<N;i++){
        std::lock_guard<std::mutex> lock(mtx);
        ++counter_mutex;
    }
}

void incr_atomic() {
    for(int i=0;i<N;i++){
        ++counter_atomic;
    }
}

template<typename F>
double measure(F f) {
    auto t1 = std::chrono::high_resolution_clock::now();
    f();
    auto t2 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(t2 - t1).count();
}

int main() {
    // С mutex
    {
        std::vector<std::thread> ths;
        for(int i=0;i<4;i++) ths.emplace_back(incr_mutex);
        for(auto& t: ths) t.join();
        std::cout << "Mutex result: " << counter_mutex
                  << ", time: " << measure([]{/* пусто */}) << " ms\n";
    }

    // C atomic
    {
        std::vector<std::thread> ths;
        for(int i=0;i<4;i++) ths.emplace_back(incr_atomic);
        for(auto& t: ths) t.join();
        std::cout << "Atomic result: " << counter_atomic
                  << ", time: " << measure([]{/* пусто */}) << " ms\n";
    }

    return 0;
}
