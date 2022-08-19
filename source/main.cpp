#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>

#include "prefix.h"
#include "prefix_no_duplicates.h"
#include "prefix_optimized.h"
#include "map_reduce_engine.h"

bool file_exists(const std::string& filename) {
    std::ifstream infile(filename);
    return infile.good();
}

int main(int argc, char *argv[]) {
    bool ex_correct = true;
    std::stringstream ss_error;
    std::string source_file;
    int num_threads_map{1};
    int num_threads_reduce{1};

    if (argc != 4) {
        ex_correct = false;
        ss_error << "Incorrect number of arguments";
    } else {
        try {
            source_file = argv[1];
            num_threads_map = std::stoi(argv[2]);
            num_threads_reduce = std::stoi(argv[3]);

            if ((num_threads_map <= 0) || 
                (num_threads_reduce <= 0)) {
                ss_error << "Number of threads must be positive";
                ex_correct = false;
            } else if (!file_exists(source_file)) {
                ss_error << "File " << source_file << " doesn't exist!";
                ex_correct = false;
            }
        } catch (std::exception& ex) {
            ex_correct = false;
            ss_error << ex.what();
        }
    }

    if (!ex_correct) {
        std::cout << "Error: " << ss_error.str()
        << "\nNeed 3 arguments: source file; num threads for map, num threads for reduce:"
        << "\nExample:EMail-Data-Storage.txt 4 4";
        exit(0);
    }

    using namespace prefix_optimized;

    auto engine_run = MapReduceEngine<PrefixMapper, PrefixReducer>(
            source_file, num_threads_map, num_threads_reduce
    );
    std::vector<int> reduce_results = engine_run.process();
    auto result = std::max_element(reduce_results.cbegin(), reduce_results.cend());
    if (result != reduce_results.cend()) {
        std::cout << *result << "\n";
    } else {
        std::cout << "empty result" << "\n";
    }

    return 0;
}
