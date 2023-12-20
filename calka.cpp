# calki
#include <iostream>
#include <cmath>
#include <thread>
#include <vector>
#include <chrono>

// Funkcja obliczaj¹ca wartoœæ funkcji w punkcie x (funkcja zale¿na od liczby PI)
double function(double x) {
    return 4.0 / (1.0 + x * x);
}

// Funkcja wykonuj¹ca ca³kowanie numeryczne w okreœlonym przedziale z u¿yciem w¹tków
double calculatePI(double a, double b, int num_steps, int num_threads) {
    double step = (b - a) / num_steps;
    double sum = 0.0;

    std::vector<std::thread> threads;

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([=, &sum]() {
            double partial_sum = 0.0;
            for (int j = i; j < num_steps; j += num_threads) {
                double x = a + (j + 0.5) * step;
                partial_sum += function(x);
            }
            sum += partial_sum;
            });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    return sum * step;
}

int main() {
    int num_steps = 1000000000; // Mo¿esz zmieniaæ iloœæ kroków
    int max_threads = 50;

    // Tablica do przechowywania czasów dla ró¿nych iloœci w¹tków
    std::vector<double> times(max_threads);

    for (int num_threads = 1; num_threads <= max_threads; ++num_threads) {
        auto start = std::chrono::high_resolution_clock::now();
        double result = calculatePI(0.0, 1.0, num_steps, num_threads);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = end - start;
        times[num_threads - 1] = duration.count();

        std::cout << "Threads: " << num_threads << " Time: " << duration.count() << " Result: " << result << std::endl;
    }

    // Zapisz czasy do arkusza kalkulacyjnego (Excel)
    // Narysuj wykres czasu vs. iloœæ w¹tków

    return 0;
}
