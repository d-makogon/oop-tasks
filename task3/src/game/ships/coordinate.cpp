#include <tuple>
#include "coordinate.h"

void bs::Coordinate::SetX(int newX)
{
    x = newX;
}

void bs::Coordinate::SetY(int newY)
{
    y = newY;
}

int bs::Coordinate::GetX() const
{
    return x;
}

int bs::Coordinate::GetY() const
{
    return y;
}

bool bs::Coordinate::operator==(const Coordinate& other) const
{
    return other.GetX() == this->GetX() &&
           other.GetY() == this->GetY();
}

bool bs::Coordinate::operator<(const Coordinate& other) const
{
    return std::tie(x, y) < std::tie(other.x, other.y);
}

bool bs::Coordinate::operator!=(const bs::Coordinate& other) const
{
    return !((*this) == other);
}
