#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <chrono>

double sum = 0.0;
double step;

void calculatePI(int thread_id, int num_threads, long long num_steps) {
    double partial_sum = 0.0;
    double x;

    for (long long i = thread_id; i < num_steps; i += num_threads) {
        x = (i + 0.5) * step;
        partial_sum += 4.0 / (1.0 + x * x);
    }

    sum += partial_sum;
}

int main() {
    std::ofstream file("results.csv");
    file << "Threads,Num_Steps,Time\n";

    std::vector<long long> num_steps_values = { 100000000LL, 1000000000LL, 3000000000LL };
    int max_threads = 50;

    for (auto num_steps : num_steps_values) {
        for (int num_threads = 1; num_threads <= max_threads; ++num_threads) {
            sum = 0.0;

            auto start_time = std::chrono::high_resolution_clock::now();

            step = 1.0 / static_cast<double>(num_steps);

            std::vector<std::thread> threads;
            for (int i = 0; i < num_threads; ++i) {
                threads.emplace_back(calculatePI, i + 1, num_threads, num_steps);
            }

            for (auto& thread : threads) {
                thread.join();
            }

            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end_time - start_time;

            double pi = step * sum;
            file << num_threads << "," << num_steps << "," << duration.count() << "\n";

            std::cout << "Threads: " << num_threads << " Num_Steps: " << num_steps << " Time: " << duration.count() << " Result: " << pi << std::endl;
        }
    }

    file.close();
    std::cout << "Results saved to results.csv\n";

    return 0;
}
