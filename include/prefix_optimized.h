#pragma once

#include <fstream>
#include <vector>
#include <string>

namespace prefix_optimized {
// реализует оптимизированный алгоритм
// Mapper:
// - принять электронную почту как значение
// - вернуть список пар <первое письмо, email>
// Reducer:
// принимает ключ (первую букву) и список значений (электронные письма)
// сортирует их и находит кратчайший префикс для идентификации всех писем
    using mapper_key_t = std::string;
    using mapper_val_t = std::string;
    using reducer_val_t = int;

    class PrefixMapper {
    public:
        using pair_kv_t = std::pair<mapper_key_t, mapper_val_t>;

        std::vector<pair_kv_t> operator()(const std::string& key, const std::string& email) {
            std::vector<pair_kv_t> result({std::make_pair(std::string{email[0]}, email)});
            return result;
        }
    };

    class PrefixReducer {
    public:
        explicit PrefixReducer(const std::string& filename) 
        :f(filename), result(1) 
        {};

        int operator()(const mapper_key_t& key, std::vector<mapper_val_t> values) {
            f << key << "\n";
            if (values.size() > 1) {
                std::sort(values.begin(), values.end());
                reducer_val_t current_result;
                for (int i = 1; i < static_cast<int>(values.size()); i++) {
                    const auto& prev = values[i - 1];
                    const auto& cur = values[i];
                    current_result = 1;
                    if (prev != cur) { 
                        for (current_result = 1;
                             current_result <= static_cast<int>(std::min(prev.size(), cur.size()))
                             && prev[current_result - 1] == cur[current_result - 1];
                             current_result++) {}
                    }
                    result = std::max(result, current_result);
                }
            }
            return result;
        }

    private:
        std::ofstream f;
        reducer_val_t result;
    };
}
