#pragma once

#include <fstream>
#include <vector>
#include <string>

namespace prefix_no_duplicates {
// Реализует оригинальный алгоритм когда нет повторяющихся писем
// Mapper:
// - принять email как значение
// - вернуть список пар <prefix, 1> для всех префиксов
// - при возврате списка пар <prefix, 1> информация о дублирующихся письмах отсутствует

// Reducer:
// берет ключ (prefix) и список значений (1)
// если количество значений > 1 => результат должен быть size(prefix) + 1

    using mapper_key_t = std::string;
    using mapper_val_t = int;
    using reducer_val_t = int;

    class PrefixMapper {
    public:
        using pair_kv_t = std::pair<mapper_key_t, mapper_val_t>;

        std::vector<pair_kv_t> operator()(const std::string& key, const std::string& email) {
            std::vector<pair_kv_t> result;
            if (!email.empty()) {
                result.reserve(email.size());
                for (size_t n_prefix_elem = 1; n_prefix_elem <= email.size(); n_prefix_elem++) {
                    result.emplace_back(email.substr(0, n_prefix_elem), 1);
                }
            }
            return result;
        }
    };

    class PrefixReducer {
    public:
        explicit PrefixReducer(const std::string& filename) 
        : f(filename), result(1) 
        {};

        int operator()(const mapper_key_t& key, const std::vector<mapper_val_t>& values) {
            f << key << "\n";
            if (values.size() > 1){
            result = std::max(result, static_cast<int>(key.size()) + 1);
            }
            return result;
        }

    private:
        std::ofstream f;
        reducer_val_t result;
    };
}

