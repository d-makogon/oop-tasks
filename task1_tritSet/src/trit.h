#pragma once
#include <iostream>

enum class Trit : int
{
    False = 0, Unknown = 1, True = 2
};

Trit operator&(Trit left, Trit right);

Trit operator|(Trit left, Trit right);

Trit operator~(Trit t);

std::ostream& operator<<(std::ostream& os, const Trit& t);
