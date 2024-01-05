#include <string>
#include <chrono>
#include "bidefective.h"
#include "getArgs.hpp"

std::string file;
int ls, rs, s, n;

int main(int argc, char* argv[]) {
    argsController ac(argc, argv);

    if(!ac.exist("-f")) {
        printf("file path: -f\n");
        return 0;
    }
    if(!ac.exist("-d")) {
        printf("graph order: -d(core, two)\n");
        return 0;
    }

    uint32_t ls = 1, rs = 1;
    if(ac.exist("-l")) ls = std::stoi(ac["-l"]);
    if(ac.exist("-r")) rs = std::stoi(ac["-r"]);

    std::string fPath = ac["-f"];
    std::string order = ac["-d"];
    std::string core = ac["-c"];
    std::string pivot = ac["-p"];

    int graphMode = (ac.exist("noUVM") == false);

    int k = 2;
    if(ac.exist("-k")) k = atoi(ac["-k"].c_str());

    std::cout << "file path: " << fPath << " "
              << "k: " << k << " "
              << "order: " << order << " "
              << "core" << core << " "
              << "pivot: " << pivot << " "
              << "l : " << ls << " "
              << "r : " << rs << " "
              << "mode: " << graphMode << std::endl;

    bidefective bic(fPath, k, ls, rs, order, core, pivot);

    auto t1 = std::chrono::steady_clock::now();      
    bic.run();
    auto t2 = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << "time:" << duration.count() << "ms" << std::endl;
    
    return 0;
}  