#include <iostream>
#include "assembler.h"

using namespace std;

int main(int argc, char* argv[]) {

    if(argc != 2) {
        cerr << "Usage: " << argv[0] << " <input file> " << endl;
        return -1;
    }

    Assembler assembler;
    assembler.translate_file(argv[1]);
}
