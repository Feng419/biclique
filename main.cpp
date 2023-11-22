#include <string>
#include "bidefective.h"

std::string file;
int ls, rs, s;

int main() {
    std::cin >> file >> ls >> rs >> s;
    bidefective bic(file, s, ls, rs);
    bic.run();
    return 0;
} 
