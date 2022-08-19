#include "gtest/gtest.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

#include "../include/prefix.h"
#include "../include/prefix_no_duplicates.h"
#include "../include/prefix_optimized.h"

#include "../include/map_reduce_engine.h"


using namespace std::string_literals;

struct TestingParametrs {
    std::string in_file;
    int num_threads_map;
    int num_threads_reduce;
    int expected_result;
};

class DataFromFile : public testing::TestWithParam<TestingParametrs> {
};

TEST_P(DataFromFile, Example) {
    auto task_runner = MapReduceEngine<
            prefix::PrefixMapper,
            prefix::PrefixReducer>(
            GetParam().in_file,
            GetParam().num_threads_map,
            GetParam().num_threads_reduce);
    std::vector<int> reduce_results = task_runner.process();
    auto result = std::max_element(reduce_results.cbegin(), reduce_results.cend());
    ASSERT_EQ(*result, GetParam().expected_result);
}

TEST_P(DataFromFile, ExampleOptimized) {
    auto task_runner = MapReduceEngine<
            prefix_optimized::PrefixMapper,
            prefix_optimized::PrefixReducer>(
            GetParam().in_file,
            GetParam().num_threads_map,
            GetParam().num_threads_reduce);
    std::vector<int> reduce_results = task_runner.process();
    auto result = std::max_element(reduce_results.cbegin(), reduce_results.cend());
    ASSERT_EQ(*result, GetParam().expected_result);
}

class DataFromFileNoDuplicates : public testing::TestWithParam<TestingParametrs> {
};

TEST_P(DataFromFileNoDuplicates, ExampleOptimized) {
    auto task_runner = MapReduceEngine<
            prefix_no_duplicates::PrefixMapper,
            prefix_no_duplicates::PrefixReducer>(
            GetParam().in_file,
            GetParam().num_threads_map,
            GetParam().num_threads_reduce);
    std::vector<int> reduce_results = task_runner.process();
    auto result = std::max_element(reduce_results.cbegin(), reduce_results.cend());
    ASSERT_EQ(*result, GetParam().expected_result);
}

INSTANTIATE_TEST_CASE_P(GroupTests, DataFromFile, ::testing::Values(
        TestingParametrs{"test1.txt"s, 1, 1, 13},
        TestingParametrs{"test1.txt"s, 2, 1, 13},
        TestingParametrs{"test2.txt"s, 1, 1, 1},
        TestingParametrs{"test2.txt"s, 2, 1, 1},
        TestingParametrs{"test3.txt"s, 1, 1, 8},
        TestingParametrs{"test3.txt"s, 2, 1, 8},
        TestingParametrs{"test3.txt"s, 1000, 1000, 8},
        TestingParametrs{"test4.txt"s, 1, 1, 3},
        TestingParametrs{"test4.txt"s, 2, 2, 3},
        TestingParametrs{"test4.txt"s, 3, 3, 3}
));


// only tests without duplicated emails
INSTANTIATE_TEST_CASE_P(GroupTests, DataFromFileNoDuplicates, ::testing::Values(
        TestingParametrs{"test1.txt"s, 1, 1, 13},
        TestingParametrs{"test1.txt"s, 1, 2, 13},
        TestingParametrs{"test1.txt"s, 2, 1, 13},
        TestingParametrs{"test1.txt"s, 4, 4, 13},
        TestingParametrs{"test4.txt"s, 1, 1, 3},
        TestingParametrs{"test4.txt"s, 2, 2, 3},
        TestingParametrs{"test4.txt"s, 3, 3, 3}
));

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

