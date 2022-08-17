#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>


 template<typename MapperCls, typename ReducerCls>
  class MapReduceEngine {
    public:
        using map_result_t = typename decltype(std::declval<MapperCls>()("", ""))::value_type; // vector -> value_type
        using map_value_t = typename decltype(std::declval<MapperCls>()("", ""))::value_type::second_type;
        using reduce_result_t = decltype(std::declval<ReducerCls>()("", {}));

        MapReduceEngine(std::string filename_,int num_threads_map_,int num_threads_reduce_) 
               :filename(std::move(filename_)),
                num_threads_map(num_threads_map_),
                num_threads_reduce(num_threads_reduce_),
                path_to_save_reduce_files(std::move(path_to_save_reduce_files_)),
                map_results(static_cast<size_t>(num_threads_map_)) 
                {}

        std::vector<reduce_result_t> process() {
            run_map();
            run_reduce();
            return reduce_results;
        }

    private:
        int num_threads_map;
        int num_threads_reduce;

        const std::string filename;
        const std::string path_to_save_reduce_files;

        std::vector<int> lines_indices;

        std::vector<std::vector<map_result_t>> map_results;
        std::vector<map_result_t> map_results_flat;

        struct ReducerData {
            std::string key;
            std::vector<map_value_t> values;
        };

        std::vector<ReducerData> data_for_reducer;
        std::vector<reduce_result_t> reduce_results;


        void run_map() {
        }
        void run_reduce() {
        }
    };