//
// Created by argem on 01.12.2021.
//

#ifndef OOP_LAB4_CSVPARSER_H
#define OOP_LAB4_CSVPARSER_H

#include <tuple>
#include <stdexcept>
#include <optional>
#include <algorithm>

#include "VectorToTuple.h"
#include "Ifstream.h"


template<typename ...Types>
class CSVParser {

private:
    detail::Ifstream input_file_;
    std::size_t current_line_number_;
    char line_delimiter_;
    char cell_delimiter_;
    char escaping_char_;

    std::vector<std::string> getCells(const std::string&);

public:

    explicit CSVParser(const std::string&, size_t = 0, char='\n', char='"', char=',');

    std::optional<std::tuple<Types...>> getRecord();

    class Iterator{
    private:
        CSVParser* csv_parser_;
        std::optional<std::tuple<Types...>> record_;
        size_t line_number_;
    public:
        Iterator();
        explicit Iterator(CSVParser&);
        std::tuple<Types...> operator *();
        Iterator& operator++();
        bool operator !=(const Iterator&);
    };

    Iterator begin();
    Iterator end();

    void setLineDelimiter(char);
    void setCellDelimiter(char);
    void setEscapingChar(char);
};


template<typename... Types>
CSVParser<Types...>::Iterator::Iterator(CSVParser& csv_parser): csv_parser_(&csv_parser),
                                                                record_(csv_parser.getRecord()),
                                                                line_number_(csv_parser.current_line_number_){
    if(!record_){
        csv_parser_ = nullptr;
        line_number_ = 0ull;
    }
}


template<typename... Types>
std::tuple<Types...> CSVParser<Types...>::Iterator::operator*() {
    return *record_;
}


template<typename... Types>
CSVParser<Types...>::Iterator::Iterator(): csv_parser_(nullptr),
                                           record_({}),
                                           line_number_(0ull){}


template<typename... Types>
typename CSVParser<Types...>::Iterator& CSVParser<Types...>::Iterator::operator++(){

    record_ = csv_parser_->getRecord();
    if(!record_){
        csv_parser_ = nullptr;
        line_number_ = 0ull;
    }

    return *this;
}


template<typename... Types>
bool CSVParser<Types...>::Iterator::operator != (const CSVParser::Iterator & other){

    return other.csv_parser_ != csv_parser_ ||
           other.line_number_ != line_number_ ||
           other.record_.has_value() != record_.has_value();
}


template<typename... Types>
CSVParser<Types...>::CSVParser(const std::string &file_name,
                               size_t skip_lines,
                               char line_delimiter,
                               char escaping_char,
                               char cell_delimiter):
                                    input_file_(file_name),
                                    current_line_number_(skip_lines),
                                    line_delimiter_(line_delimiter),
                                    cell_delimiter_(cell_delimiter),
                                    escaping_char_(escaping_char) {

    if (!input_file_.is_open()) {
        throw std::runtime_error("Can`t open file: " + file_name);
    }

    std::string unused_line;
    for (size_t i = 0; i < skip_lines; ++i) {
        input_file_.getline(unused_line, line_delimiter_, line_delimiter_);
    }
}


template<typename... Types>
std::optional<std::tuple<Types...>> CSVParser<Types...>::getRecord() {

    std::string line;
    if(!input_file_.getline(line, line_delimiter_, line_delimiter_)){
        return {};
    }
    ++current_line_number_;

    auto cells = getCells(line);
    if(cells.size() != sizeof...(Types)){
        throw std::runtime_error("In line " + std::to_string(current_line_number_) +
                                 " incorrect number of cells: " + std::to_string(cells.size()));
    }

    return {detail::VectorStringToTuple<Types...>::make_tuple(cells)};
}


template<typename... Types>
std::vector<std::string> CSVParser<Types...>::getCells(const std::string & line) {

    std::vector<std::string> res;

    for (auto it = line.cbegin(); it != line.cend();) {

        std::string cell;
        it = detail::Ifstream::getline(it, line.cend(), cell, cell_delimiter_, escaping_char_);
        cell.erase(std::remove(cell.begin(), cell.end(), escaping_char_), cell.end());
        res.push_back(std::move(cell));
    }

    return res;
}


template<typename... Types>
typename CSVParser<Types...>::Iterator CSVParser<Types...>::begin(){
    return Iterator(*this);
}


template<typename... Types>
typename CSVParser<Types...>::Iterator CSVParser<Types...>::end(){
    return Iterator();
}


template<typename... Types>
void CSVParser<Types...>::setLineDelimiter(char delimiter) {
    line_delimiter_ = delimiter;
}


template<typename... Types>
void CSVParser<Types...>::setCellDelimiter(char delimiter) {
    cell_delimiter_ = delimiter;
}


template<typename... Types>
void CSVParser<Types...>::setEscapingChar(char esc_char) {
    escaping_char_ = esc_char;
}


#endif //OOP_LAB4_CSVPARSER_H
