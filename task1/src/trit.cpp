#include "trit.h"

Trit operator&(Trit left, Trit right)
{
    if (left == Trit::False || right == Trit::False)
    {
        return Trit::False;
    }
    if (left == Trit::True && right == Trit::True)
    {
        return Trit::True;
    }
    return Trit::Unknown;
}

Trit operator|(Trit left, Trit right)
{
    if (left == Trit::True || right == Trit::True)
    {
        return Trit::True;
    }
    if (left == Trit::False && right == Trit::False)
    {
        return Trit::False;
    }
    return Trit::Unknown;
}

Trit operator~(Trit t)
{
    if (t == Trit::False) return Trit::True;
    if (t == Trit::True) return Trit::False;
    return Trit::Unknown;
}

std::ostream& operator<<(std::ostream& os, const Trit& t)
{
    char c = 'T';
    if (t == Trit::Unknown)
    {
        c = 'U';
    }
    else if (t == Trit::False)
    {
        c = 'F';
    }
    os << c;
    return os;
}