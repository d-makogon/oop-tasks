#include <limits>
#include <cerrno>

// signed types
template<typename T=int>
bool str2num(const std::string& str, T& ret)
{
    using TBigger = long long;
    const char* s = str.c_str();
    char* end;
    long long conv;
    errno = 0;
    conv = strtoll(s, &end, 10);
    if ((errno == ERANGE && conv == std::numeric_limits<TBigger>::max()) ||
        conv > std::numeric_limits<T>::max())
    {
        return false;
    }
    if ((errno == ERANGE && conv == std::numeric_limits<TBigger>::min()) ||
        conv < std::numeric_limits<T>::min())
    {
        return false;
    }
    if (*s == '\0' || *end != '\0')
    {
        return false;
    }
    ret = conv;
    return true;
}
