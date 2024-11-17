#include <iostream>
#include <utility>
using namespace std;

int add(int a, int b){
    int carry;
    while (b != 0){
        carry = a & b;
        a = a ^ b;
        b = carry << 1;
    }
    return a;
}

int negative(int a){
    return add(~a, 1);
}

class BinaryInt {
private:
    bool value[32];
public:
    BinaryInt(){
        for (int i = 0; i < 32; i = add(i, 1)){
            value[i] = false;
        }
    }
    BinaryInt(int a){
        if (a == 0){
            for (int i = 0; i < 32; i = add(i, 1)){
                value[i] = false;
            }
            return;
        }
        value[0] = a < 0;
        for (int i = 31; i > 0; i = add(i, negative(1))){
            value[i] = (a & 1) != 0;
            a >>= 1;
        }
    }
    BinaryInt(const bool* a){
        for (int i = 0; i < 32; i = add(i, 1)){
            value[i] = a[i];
        }
    }

    bool* get_value(){
        return value;
    }

    //Operations
    BinaryInt operator+ (BinaryInt& second){
        bool carry = false;
        bool new_number[32];
        int amount = 0;
        for (int i = 31; i >= 0; i = add(i, negative(1))){
            new_number[i] = (value[i] ^ second.get_value()[i]) ^ carry;
            if (value[i]){
                amount = add(amount, 1);
            }
            if (carry){
                amount = add(amount, 1);
            }
            if (second.get_value()[i]){
                amount = add(amount, 1);
            }
            if (amount > 1){
                carry = true;
            }
            else {
                carry = false;
            }
            amount = 0;
        }
        return BinaryInt{new_number};
    }

    BinaryInt& operator += (BinaryInt& second){
        BinaryInt temp{};
        temp = (*this) + second;
        for (int i = 0; i < 32; i = add(i, 1)){
            value[i] = temp.get_value()[i];
        }
        return *this;
    }

    BinaryInt& operator++ (){
        BinaryInt second{1};
        (*this) += second;
        return *this;
    }

    BinaryInt operator++ (int){
        BinaryInt copy {*this};
        ++(*this);
        return copy;
    }

    BinaryInt& operator-- (){
        BinaryInt second{negative(1)};
        (*this) += second;
        return *this;
    }

    BinaryInt operator-- (int){
        BinaryInt copy {*this};
        ++(*this);
        return copy;
    }

    BinaryInt operator- (){
        bool new_number[32];
        for (int i = 0; i < 32; i = add(i, 1)){
            new_number[i] = !(value[i]);
        }
        BinaryInt negative{new_number};
        return ++negative;
    }

    BinaryInt& operator-= (BinaryInt& second) {
        BinaryInt temp{};
        temp = -second;
        (*this) += temp;
        return *this;
    }

    BinaryInt operator- (BinaryInt& second){
        BinaryInt temp{};
        temp = -second;
        return (*this) + temp;
    }

    BinaryInt operator*(BinaryInt& second) const {
        BinaryInt first{this->value};
        BinaryInt temp1{};
        BinaryInt temp2{};
        bool sign1 = false;
        bool sign2 = false;
        if (value[0]){
            temp1 = -first;
            sign1 = true;
        } else{
            temp1 = first;
            sign1 = false;
        }
        if (second.get_value()[0]){
            temp2 = -second;
            sign2 = true;
        } else{
            temp2 = second;
            sign2 = false;
        }
        BinaryInt answer{};
        while (!((--temp1).get_value()[0])){
            answer += temp2;
        }
        if (sign1 ^ sign2){
            return -answer;
        }
        return answer;
    }

    BinaryInt& operator*= (BinaryInt& second){
        BinaryInt temp{};
        temp = (*this) * second;
        for (int i = 0; i < 32; i = add(i, 1)){
            value[i] = temp.get_value()[i];
        }
        return *this;
    }

    BinaryInt operator<< (BinaryInt& second){
        bool new_number[32];
        new_number[0] = value[0];
        BinaryInt temp2{};
        if (second.get_value()[0]){
            temp2 = -second;
        } else{
            temp2 = second;
        }
        while (!((--temp2).get_value()[0])){
            for (int i = 1; i < 31; i = add(i, 1)){
                new_number[i] = value[add(i, 1)];
            }
            new_number[31] = false;
        }
        return BinaryInt{new_number};
    }

    BinaryInt& operator<<= (BinaryInt& second){
        BinaryInt temp{};
        temp = (*this) << second;
        for (int i = 0; i < 32; i = add(i, 1)){
            value[i] = temp.get_value()[i];
        }
        return *this;
    }

    BinaryInt operator>> (BinaryInt& second){
        bool new_number[32];
        new_number[0] = value[0];
        BinaryInt temp2{};
        if (second.get_value()[0]){
            temp2 = -second;
        } else{
            temp2 = second;
        }
        while (!((--temp2).get_value()[0])){
            for (int i = 31; i > 0; i = add(i, negative(1))){
                new_number[i] = value[add(i, -1)];
            }
            new_number[1] = false;
        }
        return BinaryInt{new_number};
    }

    BinaryInt& operator>>= (BinaryInt& second){
        BinaryInt temp{};
        temp = (*this) >> second;
        for (int i = 0; i < 32; i = add(i, 1)){
            value[i] = temp.get_value()[i];
        }
        return *this;
    }

    pair<BinaryInt, BinaryInt> half_cut(){
        bool new_element1[32];
        bool new_element2[32];
        for (int i = 0; i < 32; i = add(i, 1)){
            if (i > 15){
                new_element1[i] = false;
                new_element2[i] = value[i];
            }
            else{
                new_element1[i] = value[i];
                new_element2[i] = false;
            }
        }
        return make_pair(BinaryInt{new_element1}, BinaryInt{new_element2});
    }
};

ostream& operator<<(std::ostream& stream, BinaryInt counter){
    stream << "Value: ";
    bool* r = counter.get_value();
    for (int i = 0;i < 32; i++){
        stream << r[i];
    }
    return stream;
}

int main() {
    BinaryInt first{4};
    BinaryInt second{1};
    tuple<BinaryInt, BinaryInt> answer = first.half_cut();
    cout << get<0>(answer) << get<1>(answer);
    return 0;
}