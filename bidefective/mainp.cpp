#include <string>
#include <chrono>
#include "bidefectivep.h"
#include "../tools/getArgs.hpp"

std::string file;
int ls, rs, s, n;

//main g++ -o main main.cpp bidefective.cpp -O3
//mainp g++ -o main1 mainp.cpp bidefectivep.cpp algorithms.cpp -O3

// mainp->main1.exe pii version. main->main.exe origin version.
// e.g. ./mainp -f ../data/test1.txt -s 0 -t 0 -c two -d core -p v3 -k 1
//-s size 0 -> no size constriant
//-t number of print 0 -> no number constriant
// -c two / no twohop or no
//-d core / no coreorder or no 
//-p no / v3 (v2 had not been added yet)
//-k k-bidefective

int main(int argc, char* argv[]) {
    freopen("output1.txt", "w", stdout);
    argsController ac(argc, argv);

    if(!ac.exist("-f")) {
        printf("file path: -f\n");
        return 0;
    }
    if(!ac.exist("-d")) {
        printf("graph order: -d(core, no, deg)\n");
        return 0;
    }

    int ls = 1, rs = 1, size = 0, total = 0;
    if(ac.exist("-l")) ls = std::stoi(ac["-l"]);
    if(ac.exist("-r")) rs = std::stoi(ac["-r"]);
    if(ac.exist("-s")) size = std::stoi(ac["-s"]);
    if(ac.exist("-t")) total = std::stoi(ac["-t"]);

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
              << "core: " << core << " "
              << "pivot: " << pivot << " "
              << "l : " << ls << " "
              << "r : " << rs << " "
              << "size: " << size << " "
              << "total: " << total << " "
              << "mode: " << graphMode << std::endl;

    bidefective bic(fPath, k, ls, rs, order, core, pivot, size, total);

    auto t1 = std::chrono::steady_clock::now();      
    bic.run();
    auto t2 = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << "time:" << duration.count() << "ms" << std::endl;
    fclose(stdout);
    return 0;
}  
