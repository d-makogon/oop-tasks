#include "TritSet.h"

#include <cstring>
#include <exception>
#include <malloc.h>

using namespace std;

size_t min(size_t a, size_t b)
{
    return (a < b) ? a : b;
}

size_t max(size_t a, size_t b)
{
    return (a > b) ? a : b;
}

Trit operator&(Trit left, Trit right)
{
    if (left == False || right == False)
    {
        return False;
    }
    if (left == True && right == True)
    {
        return True;
    }
    return Unknown;
}

Trit operator|(Trit left, Trit right)
{
    if (left == True || right == True)
    {
        return True;
    }
    if (left == False && right == False)
    {
        return False;
    }
    return Unknown;
}

Trit operator~(Trit t)
{
    if (t == False) return True;
    if (t == True) return False;
    return Unknown;
}

Trit uintToTrit(unsigned int x)
{
    switch (x)
    {
        case 0:
            return Trit::False;
        case 1:
            return Trit::Unknown;
        case 2:
            return Trit::True;
        default:
            return Trit::Unknown;
    }
}

// Returns the number of unsigned ints enough to store
// tritsCount trits
size_t uintsForTrits(size_t tritsCount)
{
    size_t bytesForTrits = (tritsCount * 2) / 8;
    if ((tritsCount * 2) % 8 != 0)
    {
        bytesForTrits++;
    }

    size_t uintSize = sizeof(unsigned int);
    size_t uintsForTrits = bytesForTrits / uintSize;
    if (bytesForTrits % uintSize != 0)
    {
        uintsForTrits++;
    }
    return uintsForTrits;
}

TritSet::TritSet()
{
    _array = nullptr;
    _capacity = 0;
    _maxTritsCount = 0;
    _lastSetTrit = 0;
}

TritSet::TritSet(size_t tritsCount)
{
    size_t uintsToAlloc = uintsForTrits(tritsCount);
    auto tmp = (unsigned int*)malloc(uintsToAlloc * sizeof(unsigned int));
    if (tmp == nullptr)
    {
        throw bad_alloc();
    }
    _array = tmp;

    // fill array with unknown trits
    unsigned int defaultUint = 0;
    size_t tritsInUint = sizeof(unsigned int) * 8 / 2;
    for (int i = 0; i < tritsInUint; i++)
    {
        defaultUint = (defaultUint << (unsigned int)2) | (unsigned int)Trit::Unknown;
    }

    for (int i = 0; i < uintsToAlloc; i++)
    {
        _array[i] = defaultUint;
    }

    _capacity = uintsToAlloc;
    _maxTritsCount = tritsCount;
    _lastSetTrit = tritsCount;
}

void TritSet::resize(size_t newTritsCount)
{
    size_t newUintsCount = uintsForTrits(newTritsCount);

    if (capacity() == newUintsCount)
    {
        _maxTritsCount = newTritsCount;
        return;
    }

    auto tmp = (unsigned int*)realloc(_array, newUintsCount * sizeof(unsigned int));
    if (tmp == nullptr)
    {
        throw bad_alloc();
    }
    _array = tmp;
    _capacity = newUintsCount;
    _maxTritsCount = newTritsCount;
}

size_t TritSet::capacity() const
{
    return _capacity;
}

size_t TritSet::maxTritsCount() const
{
    return _maxTritsCount;
}

TritSet::TritProxy::TritProxy(TritSet& tritSet, size_t index) : _tritSet(tritSet), _index(index) {}

TritSet::TritProxy TritSet::operator[](size_t index)
{
    return TritProxy(*this, index);
}

TritSet::TritProxy::operator Trit() const
{
    return static_cast<const TritSet&>(_tritSet)[_index];
}

// Returns the index of number in uint array which contains
// trit on tritIndex
size_t getArrayIndex(size_t tritIndex)
{
    return (tritIndex * 2) / (8 * sizeof(unsigned int));
}

// getter
Trit TritSet::operator[](size_t index) const
{
    if (index < 0 || index > _maxTritsCount - 1)
    {
        return Trit::Unknown;
    }
    unsigned int num = _array[getArrayIndex(index)];
    size_t uintBits = sizeof(unsigned int) * 8;
    // num = 00 01 10 11 . 00 00 00 01
    //        0  1  2  3    4  5  6  7
    // index = 3
    // shift = 16 - 6 - 2
    size_t shift = uintBits - ((index * 2) % uintBits) - 2;
    return uintToTrit((unsigned int)0b11 & (num >> shift));
}

void TritSet::setAt(size_t index, Trit trit)
{
    size_t arrIndex = getArrayIndex(index);
    unsigned int num = _array[arrIndex];

    size_t uintBits = sizeof(unsigned int) * 8;
    //      00 01 10 11 . 00 00 00 11
    //       0  1  2  3    4  5  6  7
    // idx=3
    // shift=16-6-2=8
    size_t shift = uintBits - ((index * 2) % uintBits) - 2;
    unsigned int mask = ~((unsigned int)(0b11) << shift);
    num &= mask;

    unsigned int tritBits = trit;
    num |= (tritBits << shift);

    _lastSetTrit = index;
    _array[arrIndex] = num;
}

TritSet::~TritSet()
{
    if (_array) free(_array);
}

void TritSet::shrink()
{
    resize(_lastSetTrit + 1);
}

TritSet TritSet::operator&(TritSet& b)
{
    size_t setATritsCount = this->maxTritsCount();
    size_t setBTritsCount = b.maxTritsCount();
    size_t minTritsCount = min(setATritsCount, setBTritsCount);
    size_t maxTritsCount = max(setATritsCount, setBTritsCount);

    auto* resultSet = new TritSet(maxTritsCount);

    for (size_t i = 0; i < minTritsCount; i++)
    {
        (*resultSet)[i] = (Trit)(*this)[i] & (Trit)b[i];
    }

    return *resultSet;
}

TritSet::TritSet(const TritSet& set) : TritSet(set.maxTritsCount())
{
    
}

// setter logic
TritSet::TritProxy& TritSet::TritProxy::operator=(Trit val)
{
    if (_index > _tritSet.maxTritsCount() - 1)
    {
        if (val == Trit::Unknown)
        {
            return *this;
        }
        else
        {
            _tritSet.resize(_index + 1);
        }
    }
    _tritSet.setAt(_index, val);
    return *this;
}

bool TritSet::TritProxy::operator==(Trit trit)
{
    return (Trit)(*this) == trit;
}
