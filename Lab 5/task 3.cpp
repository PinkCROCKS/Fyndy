#include <iostream>
#include <cstring>

class incorrect_index_array : std::exception {};

class LogicalValuesArray{
private:
    unsigned int value;
public:
    LogicalValuesArray(unsigned int y = 0){
        value = y;
    }
    unsigned int get() const{
        return value;
    }

    void arr_to_char(char* result) const {
        std::memset(result, '\0', sizeof(unsigned int) * 8);
        for (int i = sizeof(unsigned int) * 8 - 1, j = 0; i >= 0; --i, j++) {
            result[j] = (char)((char)((value >> i) & 1) + '0');
        }
    }

    static bool equal(const LogicalValuesArray first, const LogicalValuesArray second){
        return second.get() == first.get();
    }

    void to_char_arr(char* result) const {
        std::memset(result, '\0', sizeof(unsigned int) * 8);
        for (int i = sizeof(unsigned int) * 8 - 1, j = 0; i >= 0; --i, j++) {
            result[j] = (char)((char)((value >> i) & 1) + '0');
        }
    }
    bool get_bit(unsigned int index) const {
        if (index >= sizeof(unsigned int) * 8) {
            throw incorrect_index_array();
        }
        return (value & (1 << (sizeof(unsigned int) * 8 - index - 1)));
    }

    LogicalValuesArray operator~ () const{
        return LogicalValuesArray{~this->value};
    }

    LogicalValuesArray operator& (const LogicalValuesArray second) const{
        return LogicalValuesArray{value & second.get()};
    }

    LogicalValuesArray& operator&= (const LogicalValuesArray& second){
        value &= second.get();
        return *this;
    }

    LogicalValuesArray operator| (const LogicalValuesArray second) const{
        return LogicalValuesArray{~(~(*this) & ~second)};
    }

    LogicalValuesArray & operator|= (const LogicalValuesArray second) {
        value |= second.get();
        return *this;
    }

    LogicalValuesArray operator<= (const LogicalValuesArray second) const{
        return ~(*this) | second;
    }

    LogicalValuesArray operator> (const LogicalValuesArray second)const{
        return ~(*this <= second);
    }

    LogicalValuesArray operator^(LogicalValuesArray second) const {
        return LogicalValuesArray{(((~(*this)) & second) | (*this & (~second)))};
    }

    LogicalValuesArray operator==(const LogicalValuesArray second) const {
        return LogicalValuesArray{((~(*this)) | second) & (*this | (~(second)))};
    }

    LogicalValuesArray pierce_arrow(const LogicalValuesArray second) const {
        return LogicalValuesArray{~(*this | second)};
    }

    LogicalValuesArray sheffer(const LogicalValuesArray second) const {
        return LogicalValuesArray{~(*this & second)};
    }
};

std::ostream& operator<<(std::ostream& out, const LogicalValuesArray el) {
    for (int i = sizeof(unsigned int) * 8 - 1; i >= 0; --i) {
        out << ((el.get() >> i) & 1);
    }
    return out;
}

int main() {
    LogicalValuesArray first{3};
    LogicalValuesArray second{3};
    std::cout << LogicalValuesArray::equal(first, second);
    return 0;
}
