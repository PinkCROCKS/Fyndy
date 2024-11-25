#include <cstddef>
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

typedef enum errors {
    zero_error = 0,
    input_error,
    amount_arguments_error,
    realpath_error,
    same_files_error,
    memory_error,
    open_error
} erors;

int find_error(enum errors key){
    switch (key) {
        case input_error:
            printf("Mistake with input");
            return 1;
        case amount_arguments_error:
            printf("Mistake with amount of arguments");
            return 1;
        case same_files_error:
            printf("All files must be different");
            return 1;
        case open_error:
            printf("No such file or Directory");
            return 1;
        default:
            printf("Unknown Mistake");
            return 1;
    }
}

errors same_files(const std::string& path1, const std::string& path2) {
    try {
        fs::path real_path1 = fs::canonical(path1);
        fs::path real_path2 = fs::canonical(path2);
        if (real_path2 == real_path1){
            return same_files_error;
        }
        return zero_error;
    } catch (const fs::filesystem_error& e) {
        return realpath_error;
    }
}

class not_open_stream : std::exception {};

class Encoder {
private:
    std::vector<std::byte> key;

    void key_init(std::vector<std::byte> &S) const {
        size_t key_length = key.size();
        for (size_t i = 0; i < 256; ++i) {
            S[i] = static_cast<std::byte>(i);
        }
        size_t j = 0;
        for (size_t i = 0; i < 256; ++i) {
            j = (j + static_cast<size_t>(S[i]) + static_cast<size_t>(key[i % key_length])) % 256;
            std::swap(S[i], S[j]);
        }
    }

    void process_stream(std::ifstream &input, std::ofstream &output) const {
        std::vector<std::byte> S(256);
        key_init(S);
        size_t i = 0, j = 0;
        char byte;
        while (input.get(byte)) {
            i = (i + 1) % 256;
            j = (j + static_cast<size_t>(S[i])) % 256;
            std::swap(S[i], S[j]);
            std::byte k = S[(static_cast<size_t>(S[i]) + static_cast<size_t>(S[j])) % 256];
            char output_byte = byte ^ static_cast<char>(k);
            output.put(output_byte);
        }
    }

public:
    Encoder(const std::vector<std::byte> &encryption_key) : key(encryption_key) {}

    void set_key(const std::vector<std::byte> &new_key) {
        key = new_key;
    }

    bool encode(const std::string &input_path, const std::string &output_path, bool encrypt) const {
        errors result;
        result = same_files(input_path, output_path);
        if (result){
            find_error(result);
            return false;
        }
        std::ifstream input(input_path, std::ios::binary);
        if (!input.is_open()) {
            find_error(open_error);
            return false;
        }

        std::ofstream output(output_path, std::ios::binary);
        if (!output.is_open()) {
            find_error(open_error);
            return false;
        }
        process_stream(input, output);
        return true;
    }
};

int main(){
    std::vector<std::byte> key = {std::byte{11}, std::byte{24}, std::byte{12}, std::byte{3}, std::byte{1}};
    std::vector<std::byte> new_key = {std::byte{234}};
    std::string input_fife = "cat.jpg";
    std::string output_file = "123.zip";
    std::string encrypted = "encrypted.txt";
    Encoder shifr{key};
    shifr.encode(output_file, encrypted, true);
    return 0;
}