#ifndef BIDEFECTIVE
#define BIDEFECTIVE

#include "bigraph.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_set>

class bidefective {
private:
    biGraph *g;
    int k;
    int piv, ord;
    long long total = 0;
    int ls, rs, lrs[2];
    std::vector<int> RU, RV, CU, CV, XU, XV;
    std::vector<int> index;
    long long OUTPUT  = 0;
public:
    bidefective(const std::string & fPath, int k, int ls, int rs, const std::string & order, const std::string & core, const std::string & pivot) {
        g = new biGraph(fPath, core);
        printf("load path n1 %d n2 %d m %d\n", g->n[0], g->n[1], g->m);
        fflush(stdout);
        this->ls = ls;
        this->rs = rs;
        lrs[0] = ls;
        lrs[1] = rs;
        if (pivot == "v1") this->piv = 1;
        else if (pivot == "v2") this->piv = 2;
        else if (pivot == "v3") this->piv = 3;
        else if (pivot == "no") this->piv = 0;

        if (order == "deg") this->ord = 1;
        else if (order == "no") this->ord = 0;
    }

    ~bidefective() {delete g;}
    
    void run();
    
    void defectiveEnum(const std::vector<int> RU, const std::vector<int> RV,
        std::vector<int> CU, std::vector<int> CV, 
        std::vector<int> XU, std::vector<int> XV, const int k, int t);
    void printdefective(const std::vector<int> &RU, const std::vector<int> &RV);
    void branch(const std::vector<int> RU, const std::vector<int> RV, int w,
        std::vector<int> CU, std::vector<int> CV, 
        std::vector<int> XU, std::vector<int> XV, int k, int t);
    std::vector<int> seterase(const std::vector<int> &S, const int w);
    std::vector<int> seterase(const std::vector<int> &S, const std::vector<int> &T);
    std::vector<int> setadd(const std::vector<int> &S, const int w);
    std::vector<int> intersect(const std::vector<int> &S, const std::vector<int> &T);
    void update(const std::vector<int> RU, const std::vector<int> RV, const int u,
    std::vector<int> &CU, std::vector<int> &CV, const int k, int fl, int t);
    int nondeg(const int u, const std::vector<int> S, const int t);
    //std::vector<int> neighbour(const int u, const int t);
    bool in(const int u, const std::vector<int> S);
    std::vector<int> twohopnei(const int u, const int t);
    std::vector<int> setadd(const std::vector<int> &S, const std::vector<int> &T);
    std::vector<int> twohopset(const std::vector<int> &S, const int t);
    int findpivot(const std::vector<int> CU, const std::vector<int> RV, const std::vector<int> XU, int t);
    bool checkconnect(const std::vector<int> U, const std::vector<int> V, const int t);
    int findpivotv2(const std::vector<int> CU, const std::vector<int> RV, int t);
    std::vector<int> updateC(const std::vector<int> CU, const std::vector<int> RV, int pu, int k, int t);
};

#endif