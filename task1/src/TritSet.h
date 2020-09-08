#pragma once

#include <cstdint>

enum Trit
{
    False = 0, Unknown = 1, True = 2
};

Trit uintToTrit(unsigned int x);

class TritSet
{
private:
    unsigned int* _array;
    size_t _capacity;
    size_t _maxTritsCount;

    void enlarge(size_t newTritsCount);
    void setAt(size_t index, Trit trit);

public:
    class TritProxy
    {
    private:
        TritProxy(TritSet& tritSet, size_t index);
        TritSet& _tritSet;
        size_t _index;
        friend class TritSet;

    public:
        explicit operator Trit() const;
        TritProxy& operator=(Trit val);
    };

    explicit TritSet(size_t tritsCount);

    // Returns allocated size of internal array
    [[nodiscard]] size_t capacity() const;

    [[nodiscard]] size_t maxTritsCount() const;

    // setter
    TritProxy operator[](size_t index);

    // getter
    Trit operator[](size_t index) const;
};
