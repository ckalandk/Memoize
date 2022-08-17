# Memoize

A simple header only library for memoizing functions.
To run the tests you need to have google-benchmark library
installed in your system.

## Usages

To memoize a function you need to wrap it in a std::function
and pass it to memoize function defined in the namespace mem.

### Examples

``` cpp
size_t fibo(size_t n) 
{
    if(n <= 1) return n;
    return fibo(n-1) + fibo(n-2);
}

struct MemFunction
{
    size_t foo(size_t n) {
        return 2 * n;
    }
};

struct Functor
{
    int operator()(int x, int y) 
    {
        return x + y;
    }
}
int main()
{
    auto memfibo = memoize(fibo);
    MemFunction fun;
    std::function<size_t(size_t)> f = std::bind(&function::foo, &fun);
    auto memf = memoize(f); 
    auto memFunctor = memoize(std::function<int(int, int)>(Functor{}));
}
```
