#include "tritSet.h"

#include <cstring>
#include <exception>
#include <iostream>
#include <unordered_map>
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

// Returns the number of cells enough to store
// tritsCount trits
size_t cellsForTrits(size_t tritsCount, size_t cellSize)
{
    size_t bytesForTrits = (tritsCount * 2) / 8;
    if ((tritsCount * 2) % 8 != 0)
    {
        bytesForTrits++;
    }

    size_t cellsForTrits = bytesForTrits / cellSize;
    if (bytesForTrits % cellSize != 0)
    {
        cellsForTrits++;
    }
    return cellsForTrits;
}

// Returns the index of cell in array which contains
// trit on tritIndex
size_t getArrayIndex(size_t tritIndex, size_t cellSize)
{
    return (tritIndex * 2) / (8 * cellSize);
}

// Returns the index in TritSet of first trit located in
// arrIndex cell + shift (0 <= shift < cellSize / 2)
size_t getTritIndex(size_t arrIndex, size_t shift, size_t cellSize)
{
    return (arrIndex * cellSize * 4) + shift;
}

TritSet::cell_t TritSet::getTritCell(Trit t)
{
    cell_t tritCell = 0;
    size_t tritsInCell = sizeof(cell_t) * 8 / 2;
    for (int i = 0; i < tritsInCell; i++)
    {
        tritCell = (tritCell << 2) | (unsigned int)t;
    }
    return tritCell;
}

TritSet::TritSet()
{
    _array = nullptr;
    _capacity = 0;
    _initialTritsCount = 0;
    _maxTritsCount = 0;
    _lastSetTrit = 0;
    _isLastUpdated = false;
}

TritSet::TritSet(size_t tritsCount)
{
    size_t cellSize = sizeof(cell_t);
    size_t cellsToAlloc = cellsForTrits(tritsCount, cellSize);
    auto tmp = (cell_t*)malloc(cellsToAlloc * cellSize);
    if (tmp == nullptr)
    {
        throw bad_alloc();
    }
    _array = tmp;

    _capacity = cellsToAlloc;
    _maxTritsCount = tritsCount;
    _initialTritsCount = tritsCount;
    _lastSetTrit = 0;
    _isLastUpdated = false;

    fill(Trit::Unknown);
}

// Copy ctor
TritSet::TritSet(const TritSet& other) : TritSet(other.maxTritsCount())
{
    memcpy(_array, other._array, _capacity * sizeof(cell_t));
}

void TritSet::resize(size_t newTritsCount)
{
    if (newTritsCount == _maxTritsCount)
    {
        return;
    }
    size_t newCellsCount = cellsForTrits(newTritsCount, sizeof(cell_t));

    if (capacity() == newCellsCount)
    {
        _maxTritsCount = newTritsCount;
        return;
    }

    if (newCellsCount != 0)
    {
        auto tmp = (cell_t*)realloc(_array, newCellsCount * sizeof(cell_t));
        if (tmp == nullptr)
        {
            throw bad_alloc();
        }
        _array = tmp;
    }
    else
    {
        if (_array)
        {
            free(_array);
        }
        _array = nullptr;
    }

    bool needToUpdateLast = _isLastUpdated && (newTritsCount < _maxTritsCount) && (_lastSetTrit > newTritsCount - 1);

    size_t oldTritsCount = _maxTritsCount;
    _maxTritsCount = newTritsCount;
    _capacity = newCellsCount;

    if (newTritsCount > oldTritsCount)
    {
        fillFromTo(Trit::Unknown, oldTritsCount, newTritsCount - 1);
    }

    if (needToUpdateLast)
    {
        updateLast();
    }
}

void TritSet::updateLast()
{
    _isLastUpdated = false;
    _lastSetTrit = 0;
    cell_t unknownCell = getTritCell(Trit::Unknown);

    // check last cell first
    size_t lastCellTritIndex = getTritIndex(_capacity - 1, 0, sizeof(cell_t));
    for (size_t i = lastCellTritIndex; i < _maxTritsCount; i++)
    {
        if (getAt(i) != Trit::Unknown)
        {
            _isLastUpdated = true;
            _lastSetTrit = i;
        }
    }
    if (_isLastUpdated) return;

    // check other cells from the end
    for (size_t i = 1; i < _capacity; i++)
    {
        if (_array[_capacity - i - 1] != unknownCell)
        {
            size_t tritsInCell = sizeof(cell_t) * 8 / 2;
            size_t currTritIndex = getTritIndex(_capacity - i - 1, 0, sizeof(cell_t));
            for (size_t j = 0; j < tritsInCell; j++)
            {
                if (getAt(currTritIndex) != Trit::Unknown)
                {
                    _isLastUpdated = true;
                    _lastSetTrit = currTritIndex;
                }
                currTritIndex++;
            }
            if (_isLastUpdated) return;
        }
    }
}

TritSet::cell_t TritSet::insertTritInCell(cell_t cell, Trit t, size_t tritIndexInCell)
{
    size_t cellBits = sizeof(cell_t) * 8;

    if (tritIndexInCell * 2 >= cellBits) return cell;

    size_t bitShift = cellBits - (tritIndexInCell * 2) - 2;
    cell_t mask = ~((unsigned int)(0b11) << bitShift);
    cell &= mask;

    auto tritNum = static_cast<cell_t>(t);
    return cell | (tritNum << bitShift);
}

Trit TritSet::getTritInCell(cell_t cell, size_t tritIndexInCell)
{
    size_t cellBits = sizeof(cell_t) * 8;
    size_t shift = cellBits - (tritIndexInCell * 2) - 2;

    return static_cast<Trit>((unsigned int)0b11 & (cell >> shift));
}

void TritSet::setAt(size_t index, Trit trit)
{
    if ((_maxTritsCount == 0) || (index > _maxTritsCount - 1))
    {
        if (trit == Trit::Unknown)
        {
            return;
        }
        else
        {
            resize(index + 1);
        }
    }

    size_t arrIndex = getArrayIndex(index, sizeof(cell_t));

    size_t cellBits = sizeof(cell_t) * 8;

    _array[arrIndex] = insertTritInCell(_array[arrIndex], trit, index % (cellBits / 2));

    // if setting last set trit to Unknown
    if (trit == Trit::Unknown && _isLastUpdated && index == _lastSetTrit)
    {
        updateLast();
        return;
    }

    if (trit != Trit::Unknown && ((_isLastUpdated && index > _lastSetTrit) || (!_isLastUpdated)))
    {
        _lastSetTrit = index;
        _isLastUpdated = true;
    }
}

Trit TritSet::getAt(size_t index) const
{
    if (index > _maxTritsCount - 1)
    {
        return Trit::Unknown;
    }
    size_t cellBits = sizeof(cell_t) * 8;
    cell_t cell = _array[getArrayIndex(index, sizeof(cell_t))];

    return getTritInCell(cell, index % (cellBits / 2));
}

void TritSet::fillFromTo(Trit t, size_t from, size_t to)
{
    if (from > to) return;
    if (from == to)
    {
        setAt(from, t);
        return;
    }

    cell_t tritCell = getTritCell(t);
    size_t tritsInCell = sizeof(cell_t) * 8 / 2;

    while (from % tritsInCell != 0)
    {
        setAt(from, t);
        from++;
    }

    while ((to != 0) && ((to % tritsInCell) != (tritsInCell - 1)))
    {
        setAt(to, t);
        to--;
    }

    if (to == 0)
    {
        setAt(to, t);
    }

    size_t fromArrIndex = getArrayIndex(from, sizeof(cell_t));
    size_t toArrIndex = (to == 0) ? 0 : (getArrayIndex(to, sizeof(cell_t)) + 1);
    for (size_t i = fromArrIndex; i < toArrIndex; i++)
    {
        _array[i] = tritCell;
    }
}

void TritSet::fill(Trit t)
{
    if (_maxTritsCount > 0)
    {
        fillFromTo(t, 0, _maxTritsCount - 1);
    }
}

size_t TritSet::capacity() const
{
    return _capacity;
}

size_t TritSet::maxTritsCount() const
{
    return _maxTritsCount;
}

size_t TritSet::length() const
{
    return _isLastUpdated ? (_lastSetTrit + 1) : 0;
}

size_t TritSet::cardinality(Trit value) const
{
    size_t maxIndex = _isLastUpdated ? (_lastSetTrit + 1) : _maxTritsCount;
    size_t result = 0;
    for (size_t i = 0; i < maxIndex; i++)
    {
        if (getAt(i) == value)
        {
            result++;
        }
    }
    return result;
}

unordered_map<Trit, size_t, hash<Trit>> TritSet::cardinality() const
{
    unordered_map<Trit, size_t, hash<Trit>> map;

    map[Trit::False] = cardinality(Trit::False);
    map[Trit::True] = cardinality(Trit::True);
    map[Trit::Unknown] = cardinality(Trit::Unknown);

    return map;
}

void TritSet::shrink()
{
    if (_isLastUpdated)
        resize(_lastSetTrit + 1);
    else
        resize(_initialTritsCount);
}

void TritSet::trim(size_t lastIndex)
{
    if (_maxTritsCount == 0 || (lastIndex >= _maxTritsCount - 1)) return;

    fillFromTo(Trit::Unknown, lastIndex + 1, _maxTritsCount - 1);
    _maxTritsCount = lastIndex + 1;
    if (_isLastUpdated && _lastSetTrit >= lastIndex + 1)
    {
        updateLast();
    }
}

TritSet::TritProxy TritSet::operator[](size_t index)
{
    return TritProxy(*this, index);
}

Trit TritSet::operator[](size_t index) const
{
    return getAt(index);
}

TritSet TritSet::merge(const TritSet& other, Trit (* tritMergeFunc)(Trit, Trit)) const
{
    size_t setATritsCount = maxTritsCount();
    size_t setBTritsCount = other.maxTritsCount();
    size_t minTritsCount = min(setATritsCount, setBTritsCount);
    size_t maxTritsCount = max(setATritsCount, setBTritsCount);

    TritSet resultSet(maxTritsCount);

    for (size_t i = 0; i < minTritsCount; i++)
    {
        resultSet.setAt(i, tritMergeFunc(getAt(i), other.getAt(i)));
    }

    if (setATritsCount == minTritsCount)
    {
        for (size_t i = minTritsCount; i < maxTritsCount; i++)
        {
            resultSet.setAt(i, tritMergeFunc(Trit::Unknown, other.getAt(i)));
        }
    }
    else
    {
        for (size_t i = minTritsCount; i < maxTritsCount; i++)
        {
            resultSet.setAt(i, tritMergeFunc(getAt(i), Trit::Unknown));
        }
    }
    return resultSet;
}

TritSet TritSet::operator&(const TritSet& other) const
{
    return merge(other, [](Trit a, Trit b) { return a & b; });
}

TritSet TritSet::operator|(const TritSet& other) const
{
    return merge(other, [](Trit a, Trit b) { return a | b; });
}

TritSet& TritSet::operator&=(const TritSet& other)
{
    (*this) = merge(other, [](Trit a, Trit b) { return a & b; });
    return (*this);
}

TritSet& TritSet::operator|=(const TritSet& other)
{
    (*this) = merge(other, [](Trit a, Trit b) { return a | b; });
    return *this;
}

TritSet TritSet::operator~() const
{
    size_t tritsCount = maxTritsCount();
    TritSet resultSet(tritsCount);

    for (size_t i = 0; i < tritsCount; i++)
    {
        resultSet.setAt(i, ~(getAt(i)));
    }

    return resultSet;
}

TritSet::~TritSet()
{
    if (_array) free(_array);
}

TritSet& TritSet::operator=(const TritSet& other)
{
    if (this == &other)
    {
        return *this;
    }

    resize(other.maxTritsCount());

    memcpy(_array, other._array, _capacity * sizeof(cell_t));

    _capacity = other._capacity;
    _isLastUpdated = other._isLastUpdated;
    _lastSetTrit = other._lastSetTrit;
    _maxTritsCount = other._maxTritsCount;
    _initialTritsCount = other._initialTritsCount;

    return *this;
}


// ------ TritProxy ------


TritSet::TritProxy::TritProxy(TritSet& tritSet, size_t
index) : _tritSet(tritSet), _index(index) {}

TritSet::TritProxy::operator Trit()
const
{
    return static_cast<const TritSet&>(_tritSet)[_index];
}

bool TritSet::TritProxy::operator==(Trit trit) const
{
    return (Trit)(*this) == trit;
}

bool TritSet::TritProxy::operator!=(Trit trit) const
{
    return !(*this == trit);
}

TritSet::TritProxy& TritSet::TritProxy::operator=(const Trit& val)
{

    _tritSet.setAt(_index, val);
    return *this;
}

TritSet::TritProxy& TritSet::TritProxy::operator&=(const Trit& other)
{
    (*this) = (Trit)(*this) & other;
    return *this;
}

TritSet::TritProxy& TritSet::TritProxy::operator|=(const Trit& other)
{
    (*this) = (Trit)(*this) | other;
    return *this;
}

ostream& operator<<(ostream& os, const TritSet::TritProxy& tritProxy)
{
    os << (Trit)tritProxy;
    return os;
}
