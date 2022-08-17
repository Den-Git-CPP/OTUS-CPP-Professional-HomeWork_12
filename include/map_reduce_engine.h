#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <limits>

 template<typename MapperCls, typename ReducerCls>
  class MapReduceEngine {
    public:
        using map_result_t = typename decltype(std::declval<MapperCls>()("", ""))::value_type; // vector -> value_type
        using map_value_t = typename decltype(std::declval<MapperCls>()("", ""))::value_type::second_type;
        using reduce_result_t = decltype(std::declval<ReducerCls>()("", {}));

        MapReduceEngine(std::string filename_,int num_threads_map_,int num_threads_reduce_,std::string path_to_save_reduce_files_ = "") 
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
        //чтение файла для получения позиции символов "\n" для разделения данных для Mapper        
            std::ifstream infile(filename);
            std::string buf;
            lines_indices.push_back(0);
            while (std::getline(infile, buf)) {
                auto position = infile.tellg();
                if (position >= 0){
                      lines_indices.emplace_back(position);
                }else{
                    break;
                }
            }
            infile.close();
            lines_indices.push_back(std::numeric_limits<int>::max());

            auto total_blocks = static_cast<int>(lines_indices.size()) - 1;
            auto num_threads = std::min(num_threads_map, total_blocks);
            double step = static_cast<double>(total_blocks) / num_threads;  
            std::vector<std::thread> map_threads;
            for (int i = 0, j = 0, j_next = 0; i < num_threads; i++) {
                j = j_next;
                if (i == num_threads - 1){ 
                    j_next = total_blocks;
                }else{
                    j_next = static_cast<int>(step * (i + 1));
                }
                map_threads.emplace_back([this, j, j_next, i] {
                    this->run_single_mapper(lines_indices[j], lines_indices[j_next], i);
                });
            }

            for (auto& t: map_threads){
                t.join();
            }
        }
        void run_reduce() {
        }

        void run_single_reducer(int start_i, int end_i, int thread_idx) {
            ReducerCls reducer(path_to_save_reduce_files + "reduce_" + std::to_string(thread_idx) + ".txt");
            for (int i = start_i; i < end_i; i++){ 
                reduce_results[thread_idx] = reducer(data_for_reducer[i].key, data_for_reducer[i].values);
            }               
        }

        void run_single_mapper(int i_start, int i_end, int container_idx) {
            std::ifstream file(filename);
            file.seekg(i_start);
            std::string current_email;
            MapperCls map_func{};
            while ((file.tellg() < i_end) && 
                   (file >> current_email)) {
                        if (file.tellg() <= i_end) {
                           auto map_result = map_func(filename, current_email);
                                map_results[container_idx].reserve(map_results[container_idx].size() + map_result.size());
                                std::move(map_result.begin(), map_result.end(), std::back_inserter(map_results[container_idx]));
                        }
            }            
            file.close();
            std::stable_sort(map_results[container_idx].begin(), map_results[container_idx].end());
        }
    };