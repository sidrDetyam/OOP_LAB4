#include <iostream>
#include "TuplePrinter.h"
#include "CSVParser.h"

using namespace std;


int main() {

    CSVParser<int, string, string, string, double> parser("input.txt");

    for(auto t : parser){
        cout << t << endl;
    }

    return 0;
}
