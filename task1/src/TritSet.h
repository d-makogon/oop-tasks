#pragma once

#include <cstdint>

using namespace std;

enum Trit
{
    False = 0, Unknown = 1, True = 2
};

Trit operator&(Trit left, Trit right);
Trit operator|(Trit left, Trit right);
Trit operator~(Trit t);

Trit uintToTrit(unsigned int x);

class TritSet
{
private:
    unsigned int* _array{};
    size_t _capacity{};
    size_t _maxTritsCount{};
    size_t _lastSetTrit{};

    void resize(size_t newTritsCount);

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

        bool operator==(Trit trit);
    };

    TritSet();
    explicit TritSet(size_t tritsCount);
    TritSet(const TritSet& set);

    ~TritSet();

    // Returns size of internal array
    [[nodiscard]] size_t capacity() const;

    [[nodiscard]] size_t maxTritsCount() const;

    // setter
    TritProxy operator[](size_t index);

    // getter
    Trit operator[](size_t index) const;

    TritSet operator&(TritSet& b);

    void shrink();
};
