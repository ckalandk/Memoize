#ifndef MEMOIZE_HPP_
#define MEMOIZE_HPP_

#include <memory>
#include <concepts>
#include <list>
#include <type_traits>
#include <unordered_map>
#include <tuple>
#include <functional>
#include <optional>

namespace private_
{
    namespace
    {
        // boost::hash_combine
        template <class T>
        inline void hash_combine(std::size_t& seed, T const& v)
        {
            seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
    }
}

// Undefined behavior!move in another namespace.
namespace std
{
    namespace
    {
        template <typename Tuple, size_t...I>
        size_t hash_impl(Tuple&& t, std::index_sequence<I...>)
        {
            size_t seed = 0;
            (private_::hash_combine(seed, std::get<I>(std::forward<Tuple>(t))), ...);
            return seed;
        }
    }
    template<typename... Args>
    struct hash<tuple<Args...>>
    {
        typedef tuple<Args...> argument_type;
        typedef std::size_t result_type;

        constexpr result_type operator()(argument_type const& in) const
        {
            constexpr size_t tuple_size = std::tuple_size_v<argument_type>;
            return hash_impl(in, std::make_index_sequence<tuple_size>{});
        }
    };
}

namespace mem {
    namespace { inline constexpr size_t default_cache_size = 128;}

template <typename Key, typename Val>
struct LRUCache
{

    using Map = std::unordered_map<Key, typename std::list<Val>::const_iterator>;

public:
    constexpr explicit LRUCache(size_t capacity = default_cache_size)
        :m_cap(capacity)
    { }

    void cache(Key&& key, Val&& value)
    {
        if (!m_map.contains(key)) {
            if (m_cache.size() == m_cap) {
                m_cache.pop_back();
            }
        }
        else {
            m_cache.erase(m_map[key]);
        }
        m_cache.push_front(std::forward<Val>(value));
        m_map.emplace(std::forward<Key>(key), m_cache.cbegin());
    }

    std::optional<Val> operator[](Key const& key) {
       if (m_map.contains(key)) {
            return *m_map[key];
       }
       return {};
    }
private:
    std::list<Val> m_cache;
    Map m_map;
    const size_t m_cap;
};

template <typename R, typename... Args>
inline auto memoize(std::function<R(Args...)> fn, size_t cache_size = default_cache_size)
{
    using packed = std::tuple<Args...>;
    LRUCache<packed, R> cache(cache_size);
    return[cache = std::move(cache), fn = std::move(fn)](Args&&... args) mutable {
        auto pargs = std::make_tuple(std::forward<Args>(args)...);
        auto memo = cache[pargs];
        if (!memo) {
            auto result = std::apply(fn, pargs);
            cache.cache(std::move(pargs), std::move(result));
            return result;
        }
        return *memo;
    };
}

template <typename R, typename... Args>
inline auto memoize(R (&fn)(Args...), size_t cache_size = default_cache_size)
{
    return memoize<R, Args...>(std::function<R(Args...)>{fn}, cache_size);
}
}// end namespace mem
#endif //memoize.hpp