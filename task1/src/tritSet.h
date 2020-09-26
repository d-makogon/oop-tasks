#pragma once

#include <cstdint>
#include <unordered_map>

#include "trit.h"

using namespace std;

class TritSet
{
private:
    typedef unsigned int cell_t;

    cell_t* _array;
    // size of _array
    size_t _capacity;
    size_t _maxTritsCount;
    size_t _initialTritsCount;
    // index of the last trit with not unknown value
    size_t _lastSetTrit;
    bool _isLastUpdated;


    void resize(size_t newTritsCount);

    // Sets _lastSetTrit to last not unknown trit index
    void updateLast();

    void fillFromTo(Trit t, size_t from, size_t to);

    // fills entire array with trit t
    void fill(Trit t);

    // Returns cell filled with trit t
    static cell_t getTritCell(Trit t);

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

        TritProxy& operator=(const Trit& val);

        TritProxy& operator&=(const Trit& other);

        TritProxy& operator|=(const Trit& other);

        friend ostream& operator<<(ostream& os, const TritProxy& tritProxy);

        bool operator==(Trit trit);
    };

    TritSet();

    explicit TritSet(size_t tritsCount);

    TritSet(const TritSet& other);

    // Returns size of internal array
    [[nodiscard]] size_t capacity() const;

    [[nodiscard]] size_t maxTritsCount() const;

    // Returns index of last not Unknown trit + 1
    [[nodiscard]] size_t length() const;

    [[nodiscard]] size_t cardinality(Trit value) const;

    [[nodiscard]] unordered_map<Trit, size_t, hash<Trit>> cardinality() const;

    void shrink();

    void trim(size_t lastIndex);

    TritSet merge(const TritSet& other, Trit (* tritMergeFunc)(Trit, Trit)) const;

    void setAt(size_t index, Trit trit);

    [[nodiscard]] Trit getAt(size_t index) const;

    TritProxy operator[](size_t index);

    Trit operator[](size_t index) const;

    TritSet operator&(const TritSet& other) const;

    TritSet operator|(const TritSet& other) const;

    TritSet operator~() const;

    TritSet& operator=(const TritSet& other);

    TritSet& operator&=(const TritSet& other);

    TritSet& operator|=(const TritSet& other);

    ~TritSet();
};
