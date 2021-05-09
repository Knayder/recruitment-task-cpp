#include <iostream>
#include <vector>
#include <array>



template<typename T>
void remove(std::vector<T>& vec, size_t index) {
    std::swap(vec[index], vec.back());
    vec.pop_back();
}

template<typename T, size_t S>
void remove(std::array<T, S>& arr, size_t index) {
    std::swap(arr[index], arr.back());
}

struct Test {
    int a;
    Test(int a) :a{a} {std::cout << "Constr " << a << '\n';}
    
    Test(Test&& t) : a{t.a} {t.a = -1;}
    Test(Test& t) : a{t.a} {}
    
    Test& operator=(Test&& t) {a = t.a; t.a = -1; return *this;}
    Test& operator=(Test& t) {a = t.a; return *this;}
    
    ~Test() {std::cout << "Destr " << a << '\n';}
};

int main() {
    //Kod testowy
    std::vector<Test> vec;
    
    vec.emplace_back(0);
    vec.emplace_back(1);
    vec.emplace_back(2);
    vec.emplace_back(3);
    
    for(auto& it : vec)
        std::cout << it.a << ' ';
    std::cout << '\n';
    
    remove(vec, 1);
    
    for(auto& it : vec)
        std::cout << it.a << ' ';
    std::cout << '\n';
    
    return 0;
}
