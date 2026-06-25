#include <fstream>
#include <string>
#include <unordered_map>

class ConfigFileBuilder {
    std::unordered_map<std::string, std::string> data;

  public:
    ConfigFileBuilder &add(const std::string &key, const std::string &value) {
        data[key] = value;
        return *this;
    }

    void build(const std::string &path = "config.txt") const {
        std::ofstream f(path, std::ios::trunc);

        for (const auto &[k, v] : data) {
            f << k << "=" << v << "\n";
        }
    }
};