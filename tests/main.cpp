#include <iostream>
#include <memoize.hpp>

size_t fibo(size_t n)
{
    if (n <= 1) return n;
    return fibo(n - 1) + fibo(n - 2);
}

int main()
{
    auto memfibo = mem::memoize(fibo); 
    std::cout << memfibo(40) << std::endl;  
}