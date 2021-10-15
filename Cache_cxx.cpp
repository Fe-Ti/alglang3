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
     * data_size in bytes
     * average_latency in ns
     */
    size_t average_latency = 0;

    int n;
    int* data = new int[data_size];
    for(size_t j = 0; j < data_size; j += streak_size)
        data[j] = rand();

    auto start_t = std::chrono::steady_clock::now();
    for(size_t i = 0; i < iterations; ++i) {
        for(size_t j = 0; j < data_size; j += streak_size)
            n = data[j];
    }
    auto end_t = std::chrono::steady_clock::now();
    delete [] data;
    average_latency
      += std::chrono::duration_cast<std::chrono::nanoseconds>(end_t - start_t)
           .count()
         / iterations;
    return average_latency;
}

void
output_percentage(const short int score)
{
    std::cout << "[1A[" << std::setw(3) << score << "% ]" << std::endl;
}
//
// uint8_t
// guess_cache_streak_size(const size_t iterations = 1000)
//{
//    uint8_t ss = 0;
//    size_t tzero = 10;
//    size_t tnext = 0;
//    char n = '\0';
//    while(std::abs(static_cast<int64_t>(tzero) - static_cast<int64_t>(tnext))
//          < static_cast<int64_t>(tzero)) {
//        ++ss;
//        tzero = 0;
//        tnext = 0;
//        for(size_t i = 0; i < iterations; ++i) {
//            char* data = new char[ss];
//
//            auto start_t = std::chrono::steady_clock::now();
//            n = data[0];
//            auto end_t = std::chrono::steady_clock::now();
//            tzero += std::chrono::duration_cast<std::chrono::nanoseconds>(
//                       end_t - start_t)
//                       .count();
//
//            start_t = std::chrono::steady_clock::now();
//            n = data[ss - 1];
//            end_t = std::chrono::steady_clock::now();
//            tnext += std::chrono::duration_cast<std::chrono::nanoseconds>(
//                       end_t - start_t)
//                       .count();
//                delete [] data;
//        }
//        tzero = tzero / iterations;
//        tnext = tnext / iterations;
//        std::cout << static_cast<int64_t>(tzero) - static_cast<int64_t>(tnext)
//                  << " cmp with " << 0 - tzero * 0.5 << std::endl;
//    }
//    return ss;
//}

int
main()
{
    std::string str;
    float scale = 2.0;
    
    size_t tzero = 10;
    size_t tnext = 0;
    char n = '\0';
    size_t ss_end_val = 0;
    std::cout << "Should I look at streak size? (0 to skip)" << std::endl;
    std::cin >> ss_end_val;
    size_t iterations = 1000000;//000;
    for(size_t ss = 0; ss < ss_end_val; ++ss){
        tzero = 0;
        tnext = 0;
        std::cout << "[     ] Checking " << ss << std::endl;
        for(size_t i = 0; i < iterations; ++i) {
            char* data = new char[ss];

            n = data[0];
            auto start_t = std::chrono::steady_clock::now();
            n = data[0];
            auto end_t = std::chrono::steady_clock::now();
            tzero += std::chrono::duration_cast<std::chrono::nanoseconds>(
                       end_t - start_t)
                       .count();

            start_t = std::chrono::steady_clock::now();
            n = data[ss - 1];
            end_t = std::chrono::steady_clock::now();
            tnext += std::chrono::duration_cast<std::chrono::nanoseconds>(
                       end_t - start_t)
                       .count();
            delete [] data;
            output_percentage(i * 100 / iterations);
        }
        tzero = tzero / iterations;
        tnext = tnext / iterations;
        std::cout << static_cast<int64_t>(tzero) << " and " << static_cast<int64_t>(tnext)<< std::endl;
    }
    
    do{
        std::cout << "Do u want to look at it yourself? (0 to skip)" << std::endl;
        std::cin >> ss_end_val;
        tzero = 0;
        tnext = 0;
        std::cout << "[     ] Checking " << ss_end_val << std::endl;
        for(size_t i = 0; i < iterations; ++i) {
            char* data = new char[ss_end_val];

            auto start_t = std::chrono::steady_clock::now();
            data[0] = 100;
            n = data[0];
            auto end_t = std::chrono::steady_clock::now();
            tzero += std::chrono::duration_cast<std::chrono::nanoseconds>(
                       end_t - start_t)
                       .count();

            start_t = std::chrono::steady_clock::now();
            data[0] = 101;
            n = data[ss_end_val - 1];
            end_t = std::chrono::steady_clock::now();
            tnext += std::chrono::duration_cast<std::chrono::nanoseconds>(
                       end_t - start_t)
                       .count();
            delete [] data;
            output_percentage(i * 100 / iterations);
        }
        tzero = tzero / iterations;
        tnext = tnext / iterations;
        std::cout << static_cast<int64_t>(tzero) << " and " << static_cast<int64_t>(tnext)<< std::endl;
    } while(ss_end_val);
    
    bool check_cache_latency = false;
    std::cout << "Should I test latency? (0 to skip)" << std::endl;
    std::cin >> check_cache_latency;
    
    if (check_cache_latency)
    for(uint64_t ss = 8;ss < 128; ss*=2) {
        //    guess_cache_streak_size();
        //    std::cout << "Cache streak size is " << static_cast<int>(ss) <<
        //    std::endl
        //              << std::endl;

        std::cout << "[     ] Measuring fwd latency" << std::endl;
        for(size_t i = 1; i < 196608; i += 512) {
            str += std::to_string(get_fwd_average_latency(i, ss)) + "\n";
            output_percentage((i / 196608.) * 100); /// 3.);
        }
        std::cout << std::endl;
        for(size_t i = 196608; i < 393216; i += 8192) {
            str += std::to_string(get_fwd_average_latency(i, ss)) + "\n";
            output_percentage((i / 393216.) * 100); /// 3.);
        }
        std::cout << std::endl;
        for(size_t i = 393216; i < 1048576; i += 32768) {
            str += std::to_string(get_fwd_average_latency(i, ss)) + "\n";
            output_percentage((i / 1048576.) * 100);
        }
        for(size_t i = 1048576; i < 16777216; i += 524288) {
            str += std::to_string(get_fwd_average_latency(i, ss)) + "\n";
            output_percentage((i / 16777216.) * 100);
        }

        output_percentage(100);

        std::ofstream out("results" + std::to_string(ss) + ".txt");
        out /*<< i << " bytes is " */ << str;
        str.clear();
        //<< " ns"
        //<< std::endl;
        out.close();
    }
    return 0;
}
