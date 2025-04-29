// atomic_example.cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <chrono>

// Используем 64-битный беззнаковый для очень большого N
using u64 = unsigned long long;

// Задаём огромное число итераций (пример: 1e9)
constexpr u64 N = 1'000'000'000ULL;

std::mutex mtx;
int counter_mutex = 0;
std::atomic<u64> counter_atomic{0};

void incr_mutex() {
    for(u64 i = 0; i < N; ++i) {
        // Каждый инкремент защищён мьютексом
        std::lock_guard<std::mutex> lock(mtx);
        ++counter_mutex;
    }
}

void incr_atomic() {
    for(u64 i = 0; i < N; ++i) {
        // Атомарный инкремент без мьютекса
        ++counter_atomic;
    }
}

template<typename F>
double measure(F f) {
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto end   = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

int main() {
    const int numThreads = 4;  // или std::thread::hardware_concurrency()

    // Версия с mutex
    counter_mutex = 0;
    {
        std::vector<std::thread> threads;
        auto time_mutex = measure([&](){
            for(int t = 0; t < numThreads; ++t)
                threads.emplace_back(incr_mutex);
            for(auto &th : threads) th.join();
        });
        std::cout << "Mutex:   counter = " << counter_mutex
                  << ", time = " << time_mutex << " ms\n";
    }

    // Версия с atomic
    counter_atomic = 0;
    {
        std::vector<std::thread> threads;
        auto time_atomic = measure([&](){
            for(int t = 0; t < numThreads; ++t)
                threads.emplace_back(incr_atomic);
            for(auto &th : threads) th.join();
        });
        std::cout << "Atomic:  counter = " << counter_atomic.load()
                  << ", time = " << time_atomic << " ms\n";
    }

    return 0;
}
