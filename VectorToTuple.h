//
// Created by argem on 01.12.2021.
//

#ifndef OOP_LAB4_VECTORTOTUPLE_H
#define OOP_LAB4_VECTORTOTUPLE_H

#include <sstream>
#include <stdexcept>
#include <vector>


namespace detail{

    std::stringstream& operator >> (std::stringstream& ss, std::string& s){
        std::getline(ss, s);
        return ss;
    }

    template<typename ...Types>
    class VectorStringToTuple{

    private:

        template<typename T>
        static T str2T(const std::string&);

        template <std::size_t... Indices>
        static std::tuple<Types...> helper(const std::vector<std::string>&,std::index_sequence<Indices...>);

    public:

        static std::tuple<Types...> make_tuple(const std::vector<std::string>&);
    };


    template<typename... Types>
    template<typename T>
    T VectorStringToTuple<Types...>::str2T(const std::string &s){

        T res;
        std::stringstream converting_stream;
        converting_stream.str(s);
        converting_stream >> res;
        return res;
    }


    template<typename... Types>
    template<size_t... Indices>
    std::tuple<Types...> VectorStringToTuple<Types...>::helper(const std::vector<std::string>& v,
                                                               std::index_sequence<Indices...>) {

        return std::make_tuple(str2T<Types>(v[Indices])...);
    }


    template<typename... Types>
    std::tuple<Types...> VectorStringToTuple<Types...>::make_tuple(const std::vector<std::string>& v){

        if(v.size() != sizeof...(Types)){
            throw std::runtime_error("The number of input strings does`t match "
                                     "the number of types: " + std::to_string(v.size()) + " vs "
                                     + std::to_string(sizeof...(Types)));
        }

        return helper(v, std::make_index_sequence<sizeof...(Types)>());
    }
}


#endif //OOP_LAB4_VECTORTOTUPLE_H
