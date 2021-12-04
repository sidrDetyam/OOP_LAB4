//
// Created by argem on 01.12.2021.
//

#ifndef OOP_LAB4_TUPLEPRINTER_H
#define OOP_LAB4_TUPLEPRINTER_H

#include <tuple>

namespace detail {

    template<size_t Iter>
    struct TuplePrinter {

        template<typename Ostream, typename Tuple>
        static void print(Ostream& os, Tuple& t) {
            os << std::get< std::tuple_size_v<Tuple>-Iter >(t) << " ";
            TuplePrinter<Iter - 1>::print(os, t);
        }
    };


    template<>
    struct TuplePrinter<0ull> {

        template<typename Ostream, typename Tuple>
        static void print(Ostream&, Tuple&) {}
    };
}


template<typename Ch, typename Tr, typename ...Args>
auto& operator << (std::basic_ostream<Ch, Tr>& os, const std::tuple<Args...>& t){

    detail::TuplePrinter<sizeof...(Args)>::print(os, t);
    return os;
}


#endif //OOP_LAB4_TUPLEPRINTER_H
