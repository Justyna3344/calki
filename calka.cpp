#include <iostream>
#include <vector>
#include <thread>
#include <cmath>

double sum = 0.0;
double step;

void calculatePI(int thread_id, int num_threads, int num_steps) {
    double partial_sum = 0.0;
    double x;

    for (int i = thread_id; i < num_steps; i += num_threads) {
        x = (i + 0.5) * step;
        partial_sum += 4.0 / (1.0 + x * x);
    }

    sum += partial_sum;
}

int main() {
    int max_threads = 50;
    int num_threads;
    int num_steps;

    std::cout << "Podaj iloœæ kroków ca³kowania: ";
    std::cin >> num_steps;

    std::cout << "Podaj iloœæ w¹tków (od 1 do " << max_threads << "): ";
    std::cin >> num_threads;

    if (num_threads < 1 || num_threads > max_threads) {
        std::cout << "Niepoprawna liczba w¹tków!\n";
        return 1;
    }

    std::vector<std::thread> threads;

    step = 1.0 / static_cast<double>(num_steps);

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(calculatePI, i + 1, num_threads, num_steps);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    double pi = step * sum;
    std::cout << "Obliczona wartoœæ liczby PI: " << pi << std::endl;

    return 0;
}
