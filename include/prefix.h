#pragma once

#include <fstream>
#include <vector>
#include <string>

namespace prefix {

// алгоритм из Assignment
// Mapper:
// - принять email как значение
// - вернуть список пар <prefix, email> для всех префиксов
// - NB: мы не можем вернуть список пар <prefix, email>, потому что мы потеряем информацию о дублирующихся письмах
// но для повторяющихся писем результат должен быть 1

// Reducer:
// принимает key (prefix) и list значений (e-mails)
// если количество значений > 1 => результат должен быть size(prefix) + 1,
// но если все значения равны, результат равен 1 (нам нужны оригинальные письма)

    using mapper_key_t = std::string;
    using mapper_val_t = std::string;
    using reducer_val_t = int;

    class PrefixMapper {
    public:
        using pair_kv_t = std::pair<mapper_key_t, mapper_val_t>;

        std::vector<pair_kv_t> operator()(const std::string& key, const std::string& email) {
            std::vector<pair_kv_t> result;
            if (!email.empty()) {
                result.reserve(email.size());
                for (size_t n_prefix_elem = 1; n_prefix_elem <= email.size(); n_prefix_elem++) {
                    result.emplace_back(email.substr(0, n_prefix_elem), email);
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
            if (values.size() > 1) {
                bool all_emails_equal = true;
                for (int i = 1; i < static_cast<int>(values.size()); i++){
                    if (values[i] != values[i - 1]) {
                        all_emails_equal = false;
                        break;
                    }
                }
                    
                int current_result = static_cast<int>(key.size()) + 1;
                if (all_emails_equal){
                    current_result = 1;  
                }
                    
                result = std::max(result, current_result);
            }
            return result;
        }

    private:
        std::ofstream f;
        reducer_val_t result;
    };
}
