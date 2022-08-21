#include <catch.hpp>
#include "../src/memoize.hpp"


size_t fibo(size_t n)
{
    if (n <= 1) return n;
    return fibo(n - 1) + fibo(n - 2);
}

int add(int const& a, int const& b)
{
    return a + b;
}

TEST_CASE("memoize function taking its arguments by value", "[memoize]")
{
    auto memfibo = mem::memoize(fibo);
    REQUIRE(memfibo(12) == 144);
}

TEST_CASE("memoize function taking its arguments by const ref", "[memoize]")
{
    auto memadd = mem::memoize(add);
    int a = 20;
    int b = 22;
    REQUIRE(memadd(a, b) == 42);
}
