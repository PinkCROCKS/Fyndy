#pragma once
#include <iostream>
#include <cmath>

class div_by_null : std::exception {};

struct some {
    int a = 2;
private:
    int sh;
public:
    int another;
};


class ComplexNumber{
    double real;
    double imaginary;
public:
    explicit ComplexNumber(double a = 0.0, double b = 0.0){
        real = a;
        imaginary = b;
    }

    double get_real(){
        return real;
    }

    double get_imaginary(){
        return imaginary;
    }

    void add(double a = 0.0, double b = 0.0){
        real +=a;
        imaginary += b;
    }

    ComplexNumber operator+ (ComplexNumber second) const{
        return ComplexNumber{real + second.get_real(), imaginary + second.get_imaginary()};
    }

    ComplexNumber& operator+= (ComplexNumber second){
        real += second.get_real();
        imaginary += second.get_imaginary();
        return *this;
    }

    ComplexNumber operator- () const{
        return ComplexNumber{-real, -imaginary};
    }

    void sub(double a = 0.0, double b = 0.0){
        add(-a, -b);
    }

    ComplexNumber operator- (ComplexNumber& second) const{
        return (*this) + -second;
    }

    ComplexNumber& operator-= (ComplexNumber second){
        return *this += -second;
    }

    void multiply(double a = 0.0, double b = 0.0){
        real = real * a - imaginary * b;
        imaginary = real * b + imaginary * a;
    }

    ComplexNumber operator* (ComplexNumber second) const{
        double c = second.get_real();
        double d = second.get_imaginary();
        return ComplexNumber{real * c - imaginary * d, real * d + imaginary * c};
    }

    ComplexNumber& operator*= (ComplexNumber second){
        double c = second.get_real();
        double d = second.get_imaginary();
        real = real * c - imaginary * d;
        imaginary = real * d + imaginary * c;
    }

    void divide(double c = 0.0, double d = 0.0){
        if ((c - 0.0) < 1e-6 && (d - 0.0) < 1e-6){
            throw div_by_null();
        }
        double r = c * c + d * d;
        real = (real * c + imaginary * d) / r;
        imaginary = (-real * d + imaginary * c) / r;
    }

    ComplexNumber operator/ (ComplexNumber second) const{
        double c = second.get_real();
        double d = second.get_imaginary();
        if ((c - 0.0) < 1e-6 && (d - 0.0) < 1e-6){
            throw div_by_null();
        }
        double r = c * c + d * d;
        return ComplexNumber{(real * c + imaginary * d) / r, (-real * d + imaginary * c) / r};
    }

    ComplexNumber& operator/= (ComplexNumber second){
        double c = second.get_real();
        double d = second.get_imaginary();
        if ((c - 0.0) < 1e-6 && (d - 0.0) < 1e-6){
            throw div_by_null();
        }
        double r = c * c + d * d;
        real = (real * c + imaginary * d) / r;
        imaginary = (-real * d + imaginary * c) / r;
        return *this;
    }

    double abs(){
        return sqrt(real * real + imaginary * imaginary);
    }

    double argument(){
        return atan2(imaginary, real);
    }
};

std::ostream& operator<<(std::ostream& stream, ComplexNumber counter){
    stream << "Value: ";
    stream << counter.get_real();
    if (counter.get_imaginary() > 0){
        stream << " + " << counter.get_imaginary() << "i";
        return stream;
    }
    stream << " - " << fabs(counter.get_imaginary()) << "i";
    return stream;
}

int main() {
    ComplexNumber y{1,2};
    ComplexNumber u{};
    std::cout << y / u;

    some another;
    int e = 2;
    return 0;
}
