#pragma once

#include <tuple>
#include <ostream>

template<typename Ch, typename Tr, typename TupleType, size_t... I>
void printTuple(std::basic_ostream<Ch, Tr>& os, const TupleType& tuple, std::index_sequence<I...>)
{
    os << "(";

    // C++17 fold expression
    (..., (os << (I == 0 ? "" : ", ") << std::get<I>(tuple)));
    /*
     * expands into:
     * (
     * (std::cout << "" << std::get<0>(tuple)),
     * (std::cout << ", " << std::get<1>(tuple)),
     * (std::cout << ", " << std::get<2>(tuple)),
     * ...
     * (std::cout << ", " << std::get<I>(tuple))
     * );
     */

    os << ")";
}

template<typename Ch, typename Tr, typename ... Args>
auto operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t)
-> std::basic_ostream<Ch, Tr>&
{
    printTuple(os, t, std::make_index_sequence<sizeof...(Args)>());
    return os;
}
