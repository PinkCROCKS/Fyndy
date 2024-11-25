#include <iostream>
#include <cstring>
#include <compare>
#include <iterator>

class incorrect_index : std::exception {};
class empty_vector : std::exception {};

void* my_realloc(void* ptr, size_t old_size, size_t new_size) {
    if (new_size == 0) {
        delete[] static_cast<double *>(ptr);
        return nullptr;
    }

    if (ptr == nullptr) {
        return new char[new_size];
    }

    auto * new_ptr = new double [new_size];
    size_t copy_size = (old_size < new_size) ? old_size : new_size;
    std::memcpy(new_ptr, ptr, copy_size);

    delete[] static_cast<double *>(ptr);
    return new_ptr;
}

class Vector{
private:
    size_t size;
    size_t capacity;
    double* content;
public:
    Vector(size_t amount = 0, double elem = 0.0){
        capacity = 2 * amount;
        size = amount;
        content = new double[capacity];
        for (int i = 0; i < size; i++){
            content[i] = elem;
        }
    }

    explicit Vector(std::initializer_list<double> init) {
        size = init.size();
        capacity = 2 * size;
        content = new double[capacity];
        std::copy(init.begin(), init.end(), content);
    }

    template<typename InputIt>
    Vector(InputIt first, InputIt last) {
        size = 0;
        for (InputIt it = first; it != last; ++it) {
            ++size;
        }
        capacity = 2 * size;
        content = new double[capacity];
        std::copy(first, last, content);
    }

    double& at(size_t index = 0){
        if (index >= size){
            throw incorrect_index();
        }
        return content[index];
    }

    const double& at(size_t index = 0) const{
        if (index >= size){
            throw incorrect_index();
        }
        return content[index];
    }

    double& front(){
        if (size == 0){
            throw empty_vector();
        }
        return content[0];
    }

    const double& front() const{
        if (size == 0){
            throw empty_vector();
        }
        return content[0];
    }

    double& back(){
        if (size == 0){
            throw empty_vector();
        }
        return content[size - 1];
    }

    const double& back() const{
        if (size == 0){
            throw empty_vector();
        }
        return content[size - 1];
    }

    double* data(){
        return content;
    }

    const double* data() const{
        return content;
    }

    bool empty() const{
        return size == 0;
    }

    size_t size_of_vector() const{
        return size;
    }

    void reserve(size_t num){
        if (num <= capacity) { return; }
        double* new_content = new double[num];
        std::copy(content, content + size, new_content);
        delete[] content;  // Освобождаем старую память
        content = new_content;
        capacity = num;
    }

    size_t capacity_of_vector() const{
        return capacity;
    }

    void shrink_to_fit(){
        if (capacity == size){
            return;
        }
        double* new_content = new double[size];
        std::copy(content, content + size, new_content);
        delete[] content;  // Освобождаем старую память
        content = new_content;
        capacity = size;
    }

    void clear(){
        size = 0;
    }

    void insert(size_t index, double elem, double between = 0.0){
        if (size == capacity){
            reserve(size * 2);
        }
        if (index >= capacity){
            reserve(index * 2);
            content[index] = elem;
            size = ++index;
        } else if (index >= size){
            for (size_t i = size; i < index; i++){
                content[i] = between;
            }
            content[index] = elem;
            size = ++index;
        } else {
            for (int i = static_cast<int>(size); i > static_cast<int>(index); --i) {
                content[i] = content[i - 1];
            }
            content[index] = elem;
            size++;
        }
    }

    void erase(size_t index){
        if (index >= size){
            return;
        }
        for (size_t i = (index + 1); i < size; i++){
            content[i - 1] = content[i];
        }
        size--;
    }

    void push_back(double elem){
        insert(size, elem);
    }

    void resize(size_t size1, double elem){
        if (size1 < size){
            size = size1;
        } else {
            insert(size1 - 1, elem, elem);
        }
    }

    std::partial_ordering operator<=> (const Vector& second) const{
        size_t min_size = std::min(size, second.size_of_vector());
        for (size_t i = 0; i < min_size; ++i) {
            if (content[i] != second.at(i)) {
                return content[i] <=> second.content[i];
            }
        }
        return size <=> second.size;
    }

    bool operator==(const Vector& other) const {
        return (*this <=> other) == 0;
    }

    class Iterator {
    private:
        double* ptr;

    public:
        explicit Iterator(double* prt) : ptr(prt){};

        double& operator*() const { return *ptr; }
        double* operator->() { return ptr;}
        Iterator& operator++()&{
            ++ptr;
            return *this;
        }
        Iterator operator++(int){
            Iterator tmp = *this;
            ++ptr;
            return tmp;
        }
        Iterator& operator--()&{
            --ptr;
            return *this;
        }
        Iterator operator--(int){
            Iterator copy = *this;
            ++(*this);
            return copy;
        }
        Iterator operator+(size_t n) const{
            return Iterator{ptr + n};
        }

        Iterator operator-(size_t n) const{
            return Iterator{ptr - n};
        }
        size_t operator-(const Iterator& other) const{
            return ptr - other.ptr;
        }
        bool operator==(const Iterator& other) const{
            return ptr == other.ptr;
        }
        bool operator!=(const Iterator& other) const{
            return !(*this == other);
        }
    };

    Iterator begin() {
        return Iterator{content};
    }

    Iterator begin() const{
        return Iterator{content};
    }

    Iterator end() {
        return Iterator{content + size};
    }

    Iterator end() const{
        return Iterator{content + size};
    }

    ~Vector(){
        delete[] content;
    }
};

std::ostream& operator<<(std::ostream& stream, const Vector& v){
    if (v.empty()){
        stream << "Capacity = " << v.capacity_of_vector();
        stream << "Vector is Empty\n";
        return stream;
    }
    stream << "Capacity = " << v.capacity_of_vector() << std::endl;
    stream << "Size = " << v.size_of_vector() << std::endl;
    for (size_t i = 0; i < v.size_of_vector(); i++){
        double y = v.at(i);
        stream << y << " ";
    }
    stream << std::endl;
    return stream;
}

int main() {
    Vector vi{2};
    vi.insert(0, 2);
    vi.push_back(3);
    std::cout << vi;
    return 0;
}