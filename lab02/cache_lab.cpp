// Copyright 2021 Fe-Ti
// Cash checker

#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

size_t
get_fwd_average_latency(const size_t data_size,
                        uint8_t streak_size,
                        const size_t iterations = 1000)
{
    /*
     * Get forward run avg latency
     * data_size in bytes
     * average_latency in ns
     */
    size_t average_latency = 0;

    char n;
    char* data = new char[data_size];
    char* data1 = new char[data_size];
    for(size_t j = 0; j < data_size; j += streak_size)
        data[j] = rand();

    auto start_t = std::chrono::steady_clock::now();
    for(size_t i = 0; i < iterations; ++i) {
        for(size_t j = 0; j < data_size; j += streak_size)
            data1[j] = data[j];
    }
    auto end_t = std::chrono::steady_clock::now();
    delete[] data;
    delete[] data1;
    average_latency
      += std::chrono::duration_cast<std::chrono::nanoseconds>(end_t - start_t)
           .count()
         / iterations;
    return average_latency;
}

size_t
get_rvs_average_latency(const size_t data_size,
                        uint8_t streak_size,
                        const size_t iterations = 1000)
{
    /*
     * Get reverse run avg latency
     * data_size in bytes
     * average_latency in ns
     */
    size_t average_latency = 0;

    char n;
    char* data = new char[data_size];
    char* data1 = new char[data_size];
    for(size_t j = 0; j < data_size; j += streak_size)
        data[j] = rand();
    auto start_t = std::chrono::steady_clock::now();
    for(size_t i = 0; i < iterations; ++i) {
        for(size_t j = 0; j < data_size; j += streak_size) {
            data1[data_size - j - 1] = data[data_size - j - 1];
        }
    }
    auto end_t = std::chrono::steady_clock::now();
    delete[] data;
    delete[] data1;
    average_latency
      += std::chrono::duration_cast<std::chrono::nanoseconds>(end_t - start_t)
           .count()
         / iterations;
    return average_latency;
}

size_t
get_rnd_average_latency(const size_t data_size,
                        uint8_t streak_size,
                        const size_t iterations = 1000)
{
    /* Get random run avg latency
     * data_size in bytes
     * average_latency in ns
     */
    size_t average_latency = 0;

    char n;
    char* data = new char[data_size];
    char* data1 = new char[data_size];
    for(size_t j = 0; j < data_size; j += streak_size)
        data[j] = rand();

    auto start_t = std::chrono::steady_clock::now();
    for(size_t i = 0; i < iterations; ++i) {
        for(size_t j = 0; j < data_size; j += streak_size)
            data1[rand() % data_size] = data[rand() % data_size];
    }
    auto end_t = std::chrono::steady_clock::now();
    delete[] data;
    delete[] data1;
    average_latency
      += std::chrono::duration_cast<std::chrono::nanoseconds>(end_t - start_t)
           .count()
         / iterations;
    return average_latency;
}

void
output_percentage(const short int score)
{
    std::cout << "\033[1A[" << std::setw(3) << score << "% ]" << std::endl;
}

void
measure_fwd_latency(size_t ss)
{
    std::string str;
    std::cout << "[     ] Measuring forward run latency" << std::endl;
    for(size_t i = 1; i < 196608; i += 512) {
        str += std::to_string(get_fwd_average_latency(i, ss)) + "\n";
        output_percentage((i / 196608.) * 100); /// 3.);
    }
    for(size_t i = 196608; i < 393216; i += 8192) {
        str += std::to_string(get_fwd_average_latency(i, ss)) + "\n";
        output_percentage((i / 393216.) * 100); /// 3.);
    }
    for(size_t i = 393216; i < 1048576; i += 32768) {
        str += std::to_string(get_fwd_average_latency(i, ss)) + "\n";
        output_percentage((i / 1048576.) * 100);
    }
    for(size_t i = 1048576; i < 16777216; i += 524288) {
        str += std::to_string(get_fwd_average_latency(i, ss)) + "\n";
        output_percentage((i / 16777216.) * 100);
    }
    output_percentage(100);
    std::ofstream fwdout("fwd_results" + std::to_string(ss) + ".txt");
    fwdout << str;
    fwdout.close();
}

void
measure_rvs_latency(size_t ss)
{
    std::string str;
    std::cout << "[     ] Measuring reverse run latency" << std::endl;
    for(size_t i = 1; i < 196608; i += 512) {
        str += std::to_string(get_rvs_average_latency(i, ss)) + "\n";
        output_percentage((i / 196608.) * 100); /// 3.);
    }
    for(size_t i = 196608; i < 393216; i += 8192) {
        str += std::to_string(get_rvs_average_latency(i, ss)) + "\n";
        output_percentage((i / 393216.) * 100); /// 3.);
    }
    for(size_t i = 393216; i < 1048576; i += 32768) {
        str += std::to_string(get_rvs_average_latency(i, ss)) + "\n";
        output_percentage((i / 1048576.) * 100);
    }
    for(size_t i = 1048576; i < 16777216; i += 524288) {
        str += std::to_string(get_rvs_average_latency(i, ss)) + "\n";
        output_percentage((i / 16777216.) * 100);
    }
    output_percentage(100);

    std::ofstream rvsout("rvs_results" + std::to_string(ss) + ".txt");
    rvsout << str;
    rvsout.close();
}

void
measure_rnd_latency(size_t ss)
{
    std::string str;
    std::cout << "[     ] Measuring random latency" << std::endl;
    for(size_t i = 1; i < 196608; i += 512) {
        str += std::to_string(get_rnd_average_latency(i, ss)) + "\n";
        output_percentage((i / 196608.) * 100); /// 3.);
    }
    for(size_t i = 196608; i < 393216; i += 8192) {
        str += std::to_string(get_rnd_average_latency(i, ss)) + "\n";
        output_percentage((i / 393216.) * 100); /// 3.);
    }
    for(size_t i = 393216; i < 1048576; i += 32768) {
        str += std::to_string(get_rnd_average_latency(i, ss)) + "\n";
        output_percentage((i / 1048576.) * 100);
    }
    for(size_t i = 1048576; i < 16777216; i += 524288) {
        str += std::to_string(get_rnd_average_latency(i, ss)) + "\n";
        output_percentage((i / 16777216.) * 100);
    }
    output_percentage(100);
    std::ofstream rndout("rnd_results" + std::to_string(ss) + ".txt");
    rndout << str;
    rndout.close();
}

int
main()
{
    size_t ss_beg_val = 0;
    size_t ss_end_val = 0;
    bool answer = 0;

    std::cout << "Enter begin value (0 to skip)" << std::endl;
    std::cin >> ss_beg_val;

    std::cout << "Enter end value (0 to skip)" << std::endl;
    std::cin >> ss_end_val;

    if(ss_end_val)
        for(uint64_t ss = ss_beg_val; ss <= ss_end_val; ss *= 2) {
            std::cout << "Should I measure fwd for " << ss << "? (0 for not)"
                      << std::endl;
            std::cin >> answer;
            if(answer)
                measure_fwd_latency(ss);
            std::cout << "Should I measure rvs for " << ss << "? (0 for not)"
                      << std::endl;
            std::cin >> answer;
            if(answer)
                measure_rvs_latency(ss);
            std::cout << "Should I measure rnd for " << ss << "? (0 for not)"
                      << std::endl;
            std::cin >> answer;
            if(answer)
                measure_rnd_latency(ss);
            std::cout << "Results for " << ss << " are ready, check *_results"
                      << ss << "\\.txt" << std::endl;
        }
    return 0;
}
