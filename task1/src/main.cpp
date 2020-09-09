#include "TritSet.h"
#include <iostream>
#include <cassert>

using namespace std;

int main(int argc, char* argv[])
{
    TritSet set(1000);

    size_t allocLength = set.capacity();
    assert(allocLength >= 1000 * 2 / 8 / sizeof(unsigned int));

    set[1000'000'000] = Unknown;
    assert(allocLength == set.capacity());

    if (set[2000'000'000] == True)
    {
        assert(false);
    }
    assert(allocLength == set.capacity());

    set[1000'000'000] = True;
    assert(allocLength < set.capacity());

    allocLength = set.capacity();
    set[1000'000'000] = Unknown;
    set[1000'000] = False;
    assert(allocLength == set.capacity());

    set.shrink();
    assert(allocLength > set.capacity());

    return 0;
}
