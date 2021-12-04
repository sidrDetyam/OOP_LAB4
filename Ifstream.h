//
// Created by argem on 03.12.2021.
//

#ifndef OOP_LAB4_IFSTREAM_H
#define OOP_LAB4_IFSTREAM_H

#include <string>
#include <fstream>


namespace detail {

    class Ifstream : public std::ifstream {
    public:

        template<typename InputIter>
        static InputIter getline(InputIter, InputIter, std::string&, char, char);

        basic_istream &getline(std::string&, char, char);

        explicit Ifstream(const std::string &);
        ~Ifstream() override = default;
    };


    template<typename InputIter>
    InputIter Ifstream::getline(InputIter begin, InputIter end, std::string &res,
                                char delimiter_char, char escaping_char){

        res.clear();

        auto it = begin;
        bool is_escaped = false;

        while (it != end && (*it != delimiter_char || is_escaped)) {

            if (*it == escaping_char) {
                is_escaped = !is_escaped;
            }

            res.push_back(*it);
            ++it;
        }

        if(it!=end){
            ++it;
        }

        return it;
    }


    std::basic_istream<char,std::char_traits<char>> &Ifstream::getline(std::string & res,
                                                                       char delimiter_char,
                                                                       char escaping_char){

        auto begin = std::istreambuf_iterator<char>(*this);
        auto end = std::istreambuf_iterator<char>();

        if (begin == end) {
            return std::getline(*this, res);
        }

        getline(begin, end, res, delimiter_char, escaping_char);

        return *this;
    }


    Ifstream::Ifstream(const std::string &file_name) : std::ifstream(file_name) {}
}

#endif //OOP_LAB4_IFSTREAM_H
