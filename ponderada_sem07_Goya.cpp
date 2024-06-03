#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <utility>

// Função para criar e preencher um vetor com números aleatórios
std::vector<int> create_vector(int size) {
    std::vector<int> vec(size);
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < size; ++i) {
        vec[i] = rand() % 100; // Números aleatórios de 0 a 99
    }
    return vec;
}

// Função para calcular a soma de forma iterativa, passando por cada elemento da array de uma única vez
std::pair<long long, double> serial_sum(const std::vector<int>& numbers) {
    double start_time = omp_get_wtime();
    long long total = 0;
    for (int value : numbers) {
        total += value;
    }
    double end_time = omp_get_wtime() - start_time;
    return std::make_pair(total, end_time); // retorna a soma e o tempo de processamento
}

// Função para calcular a soma em paralelo usando OpenMP
std::pair<long long, double> parallel_sum(const std::vector<int>& numbers, int num_threads) {
    double start_time = omp_get_wtime();
    long long total = 0;
    int size = numbers.size();

    #pragma omp parallel for reduction(+:total) num_threads(num_threads)
    for (int i = 0; i < size; ++i) {
        total += numbers[i];
    }

    double end_time = omp_get_wtime() - start_time;
    return std::make_pair(total, end_time); // retorna a soma e o tempo de processamento
}

int main() {
    const int N = 1000000; // Tamanho do vetor
    std::vector<int> numbers = create_vector(N);

    // Soma serial
    auto [serial_res, serial_time] = serial_sum(numbers);
    std::cout << "Serial: \n";
    std::cout << "Resultado da soma: " << serial_res << "\n";
    std::cout << "Tempo de processamento: " << serial_time << " segundos\n\n";

    // Soma paralela
    std::cout << "Paralelismo: \n";
    // Compara os tempos de processamento de 1 a 11 threads, adicionando 2 threads a cada iteração
    for (int threads = 1; threads <= 11; threads += 2) {
        auto [parallel_res, parallel_time] = parallel_sum(numbers, threads);
        std::cout << "Resultado da soma com " << threads << " threads: " << parallel_res << "\n";
        std::cout << "Tempo de processamento com " << threads << " threads: " << parallel_time << " segundos\n";
    }

    return 0;
}
