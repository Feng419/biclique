#ifndef BIDEFECTIVE
#define BIDEFECTIVE

#include "bigraph.hpp"
#include <string>
#include <vector>
#include <set>
#include <unordered_set>

class bidefective {
private:
    biGraph *g;
    int s;
    long long total = 0;
    int ls, rs, lrs[2];
    std::vector<int> RU, RV, CU, CV, XU, XV;
    long long OUTPUT  = 0;
public:
    bidefective(const std::string & fPath, int s, int ls, int rs) {
        g = new biGraph(fPath);
        printf("load path n1 %d n2 %d m %d\n", g->n[0], g->n[1], g->m);
        fflush(stdout);
        this->ls = ls;
        this->rs = rs;
        lrs[0] = ls;
        lrs[1] = rs;
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
    std::vector<int> neighbour(const int u, const int t);
    bool in(const int u, const std::vector<int> S);
};

#endif
