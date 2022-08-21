#include <catch.hpp>
#include "../src/memoize.hpp"

size_t fibo(size_t n)
{
    if (n <= 1) return n;
    return fibo(n - 1) + fibo(n - 2);
}
TEST_CASE("LRUCache construction", "[LRUCache]")
{
    mem::LRUCache<std::string, size_t> cache(3);
    REQUIRE(cache.capacity() == 3);
    REQUIRE(cache.size() == 0);
}

TEST_CASE("LRUCache::cache member function", "[LRUCache]") 
{
    mem::LRUCache<std::string, size_t> cache(3);
    REQUIRE(cache.capacity() == 3);
    REQUIRE(cache.size() == 0);

    SECTION("Caching a pair of values") {
        cache.cache("3", 3);
        cache.cache("2", 2);
        cache.cache("1", 1);
        auto&& view = cache.cache_values();
        std::vector<size_t> v(view.begin(), view.end());
        REQUIRE(v == std::vector<size_t>{ 1, 2, 3 });
        REQUIRE(cache.size() == 3);
    }

    SECTION("Adding value to a full cache") {
        cache.cache("3", 3);
        cache.cache("2", 2);
        cache.cache("1", 1);
        cache.cache("0", 0);
        auto&& view = cache.cache_values();
        std::vector<size_t> v(view.begin(), view.end());
        REQUIRE(v == std::vector<size_t>{ 0, 1, 2 });
        REQUIRE(cache.size() == 3);
    }

    SECTION("Adding value present int the cache") {
        cache.cache("3", 3);
        cache.cache("2", 2);
        cache.cache("1", 1);
        cache.cache("3", 3);
        auto&& view = cache.cache_values();
        std::vector<size_t> v(view.begin(), view.end());
        REQUIRE(v == std::vector<size_t>{ 3, 1, 2 });
        REQUIRE(cache.size() == 3);
    }
}