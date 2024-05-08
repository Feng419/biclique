#ifndef BIDEFECTIVE
#define BIDEFECTIVE

#include "bigraphv2.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <chrono>
#include <utility>
#include <unordered_set>
#include "algorithms.h"

#define mp std::make_pair

typedef std::pair<int, int> pii;

class bidefective {
private:
    biGraph *g;
    int k;
    int piv, ord, siz, cor;
    long long tot = 0;
    int ls, rs, lrs[2];
    std::vector<CuckooHash> cuhash;
    std::vector<pii> RU, RV, CU, CV, XU, XV;
    std::vector<int> index;
    long long OUTPUT  = 0;
    int maxsize = 0;
    int timecnt = 0;
public:
    bidefective(const std::string & fPath, int k, int ls, int rs, 
const std::string & order, const std::string & core, const std::string & pivot, int size, int total) {
        g = new biGraph(fPath, core, order, size, k);
        printf("load path n1 %d n2 %d m %d k %d\n", g->n[0], g->n[1], g->m, g->k);
        fflush(stdout);
        this->ls = ls;
        this->rs = rs;
        this->siz = size;
        this->OUTPUT = total;
        lrs[0] = ls;
        lrs[1] = rs;
        if (pivot == "v1") this->piv = 1;
        else if (pivot == "v2") this->piv = 2;
        else if (pivot == "v3") this->piv = 3;
        else if (pivot == "v4") this->piv = 4;
        else if (pivot == "no") this->piv = 0;

        if (order == "deg") this->ord = 1;
        else if (order == "no") this->ord = 0;
        else if (order == "core") this->ord = 2;

        if (core == "two") this->cor = 1;
        else if (core == "no") this->cor = 0;
    }

    ~bidefective() {delete g;}
    
    void run();
    void initHash();
    bool isnbr(const int u, const int v);
    void defectiveEnum(const std::vector<pii> RU, const std::vector<pii> RV,
        std::vector<pii> CU, std::vector<pii> CV, 
        std::vector<pii> XU, std::vector<pii> XV, const int k, int t, int size);
    void printdefective(const std::vector<pii> RU, const std::vector<pii> RV);
    void branch(const std::vector<pii> RU, const std::vector<pii> RV, pii w,
        std::vector<pii> CU, std::vector<pii> CV, 
        std::vector<pii> XU, std::vector<pii> XV, int k, int t);
    std::vector<pii> seterase(const std::vector<pii> &S, const pii w);
    std::vector<pii> seterase(const std::vector<pii> &S, const std::vector<pii> &T);
    std::vector<pii> setadd(const std::vector<pii> &S, const pii w);
    std::vector<pii> intersect(const std::vector<pii> &S, const std::vector<pii> &T);
    void update(const std::vector<pii> RU, const std::vector<pii> RV, const pii u,
    std::vector<pii> &CU, std::vector<pii> &CV, const int k, int t);
    bool in(const int u, const std::vector<pii> S);
    std::vector<pii> twohopnei(const int u, const int t);
    std::vector<pii> setadd(const std::vector<pii> &S, const std::vector<pii> &T);
    std::vector<int> twohopset(const std::vector<pii> &S, const int t);
    std::vector<pii> neighbor(const std::vector<pii> S, const int t);
    int findpivot(const std::vector<pii> CU, const std::vector<pii> RV, const std::vector<pii> XU, int t);
    int findnonnei(int u, const std::vector<pii> CV, const int t);
    bool checknon(int u, const std::vector<pii> RV, const std::vector<pii> RU, const std::vector<pii> CU, const int k, const int t);
    bool checkconnect(const std::vector<pii> U, const std::vector<pii> V, const int t);
    pii findpivotv2(const std::vector<pii> CU, const std::vector<pii> RV, int k, int t);
    std::vector<pii> updateC(const std::vector<pii> CU, const std::vector<pii> RV, pii pu, int k, int t);
};

#endif