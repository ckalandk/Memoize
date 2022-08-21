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
#include <ranges>

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
        using argument_type = tuple<Args...>;
        using result_type   = std::size_t;

        constexpr result_type operator()(argument_type const& in) const
        {
            constexpr size_t tuple_size = std::tuple_size_v<argument_type>;
            return hash_impl(in, std::make_index_sequence<tuple_size>{});
        }
    };
}

namespace mem {
    
    namespace { inline constexpr size_t default_cache_size = 128;}

     namespace traits
    {
        template <typename T>
        concept Hashable = requires(T t)
        {
            {std::hash<T>{}(t)} -> std::convertible_to<std::size_t>;
        };
    }
template <typename Key, typename Val>
    requires traits::Hashable<Key>
struct LRUCache
{
    using key_type = std::remove_cvref_t<Key>;
    using mapped_type = Val;
    using Cache = std::list<std::pair<key_type, mapped_type>>;
    using iterator = typename Cache::const_iterator;
    using Map = std::unordered_map<key_type, iterator>;

public:
    constexpr explicit LRUCache(size_t capacity = default_cache_size)
        :m_cap(capacity)
    { }

    template <typename Key_, typename Val_>
        requires (std::is_convertible_v<Key_, Key>&& std::is_convertible_v<Val_, Val>)
    constexpr void cache(Key_&& key, Val_&& value)
    {
        if (!m_map.contains(key)) {
            if (m_cache.size() == m_cap) {
                m_map.erase(m_cache.back().first);
                m_cache.pop_back();
            }
        }
        else {
            if (m_map[key] == m_cache.cbegin())
                return;
            m_cache.erase(m_map[key]);
        }
        auto p = std::make_pair(std::forward<Key_>(key), std::forward<Val_>(value));
        m_cache.push_front(std::move(p));
        m_map[m_cache.cbegin()->first] = m_cache.cbegin();
    }

    constexpr std::optional<Val> operator[](Key const& key) {
        if (m_map.contains(key)) {
            return m_map[key]->second;
        }
        return {};
    }

    constexpr size_t capacity() const { return m_cap; }
    constexpr size_t size() const { return m_cache.size(); }

    auto cache_values() const { return m_cache | std::views::values; }

private:
    Cache m_cache;
    Map m_map;
    const size_t m_cap;
};

template <typename R, typename... Args>
requires (traits::Hashable<std::remove_cvref_t<Args>> && ...)
constexpr inline auto memoize(std::function<R(Args...)> fn, size_t cache_size = default_cache_size)
{
    using packed = std::tuple<Args...>;
    LRUCache<packed, R> cache(cache_size);
    return[cache = std::move(cache), fn = std::move(fn)](Args&&... args) mutable {
        auto pargs = std::make_tuple(std::forward<Args>(args)...);
        auto memo = cache[pargs];
        if (!memo) {
            auto result = std::apply(fn, pargs);
            cache.cache(std::move(pargs), result);
            return result;
        }
        return *memo;
    };
}

template <typename R, typename... Args>
constexpr inline auto memoize(R (&fn)(Args...), size_t cache_size = default_cache_size)
{
    return memoize<R, Args...>(std::function<R(Args...)>{fn}, cache_size);
}
}// end namespace mem
#endif //memoize.hpp