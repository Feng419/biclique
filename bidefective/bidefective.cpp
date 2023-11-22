#include "bidefective.h"

void bidefective::run() {
    int t = 0;
    std::vector<int> CU(g->n1), CV(g->n2);
    for (int i = 0; i < g->n1; ++i)
        CU[i] = i;
    for (int i = 0; i < g->n2; ++i)
        CV[i] = i;
    // for (int x : CU) printf("%d ", x);
    // printf("\n");
    // for (int x : CV) printf("%d ", x);
    if (std::min(ls, rs) >= 2 * s + 1) {
        defectiveEnum(std::vector<int>(), std::vector<int>(), CU, CV,
            std::vector<int>(), std::vector<int>(), g->k, 0);
    }
    // printf("over\n");
}

std::vector<int> bidefective::seterase(const std::vector<int> &S, const int w) {
    std::unordered_set<int> set(S.begin(), S.end());
    set.erase(w);
    return std::vector<int>(set.begin(), set.end());
}

std::vector<int> bidefective::seterase(const std::vector<int> &S, const std::vector<int> &T) {
    std::unordered_set<int> set(S.begin(), S.end());
    for (int t : T)
        set.erase(t);
    return std::vector<int>(set.begin(), set.end());
}

std::vector<int> bidefective::setadd(const std::vector<int> &S, const int w) {
    std::unordered_set<int> set(S.begin(), S.end());
    set.insert(w);
    return std::vector<int>(set.begin(), set.end());
}

std::vector<int> bidefective::intersect(const std::vector<int> &S, const std::vector<int> &T) {
    std::unordered_set<int> set(S.begin(), S.end());
    std::unordered_set<int> res{};
    for (int t : T)
        if (set.find(t) != set.end())
            res.insert(t);
    return std::vector<int>(res.begin(), res.end());
}

bool bidefective::in(const int u, const std::vector<int> S) {
    std::unordered_set<int> set(S.begin(), S.end());
    return set.find(u) != set.end();
}

int bidefective::nondeg(const int u, const std::vector<int> S, const int t) {
    int ans = 0;
    std::unordered_set<int> s(g->e[t].begin() + g->p[t][u], g->e[t].begin() + g->p[t][u + 1]);
    for (int v : S) {
        if (s.find(v) == s.end()) ans++;
    }
    return ans;
}

std::vector<int> bidefective::neighbour(const int u, const int t) {
    std::vector<int> s(g->e[t].begin() + g->p[t][u], g->e[t].begin() + g->p[t][u + 1]);
    return s;
}

void bidefective::printdefective(const std::vector<int> &RU, const std::vector<int> &RV) {
    printf("\nRU: ");
    for (int ru : RU)
        printf("%d, ", ru);
    printf("\n");

    printf("RV: ");
    for (int rv : RV)
        printf("%d, ", rv);
    printf("\n");
}

void bidefective::defectiveEnum(const std::vector<int> RU, const std::vector<int> RV,
    std::vector<int> CU, std::vector<int> CV, 
    std::vector<int> XU, std::vector<int> XV, const int k, int t) {
    if (CU.size() == 0 && CV.size() == 0) {
        if (XU.size() == 0 && XV.size() == 0) {
            printdefective(RU, RV);
            return;
        }   
    }
    // printf("-----------------\n");
    // printf("RU:");
    // for (int x : RU) printf("%d ", x);
    // printf("\nRV:");
    // for (int x : RV) printf("%d ", x);
    // printf("\nk:%d\nXU:", k);
    // for (int x : XU) printf("%d ", x);
    // printf("\nXV:");
    // for (int x : XV) printf("%d ", x);
    // printf("\nCU:");
    // for (int x : CU) printf("%d ", x);
    // printf("\nCV:");
    // for (int x : CV) printf("%d ", x);
    // printf("\n");
    std::vector<int> CU_(CU);
    for (int w : CU_) {
        if (in(w, CU) && branch(RU, RV, w, seterase(CU, w), CV, XU, XV, k, t)) {
            CU = seterase(CU, w);
            XU = setadd(XU, w);
        }
    }
    std::vector<int> CV_(CV);
    for (int w : CV_) {
        if (in(w, CV) && branch(RV, RU, w, seterase(CV, w), CU, XV, XU, k, 1 - t)) {
            CV = seterase(CV, w);
            XV = setadd(XV, w);
        }
    }
}

bool bidefective::branch(const std::vector<int> RU, const std::vector<int> RV, int u,
    std::vector<int> CU, std::vector<int> CV, 
    std::vector<int> XU, std::vector<int> XV, int k, int t) {
    if (k < 0)
        return false;
    // printf("u:%d t:%d\n", u, t);
    update(RU, RV, u, CU, CV, k, 0, t);
    update(RU, RV, u, XU, XV, k, 1, t);
    k -= nondeg(u, RV, t);
    defectiveEnum(setadd(RU, u), RV, CU, CV, XU, XV, k, t);
    return true;
}

void bidefective::update(const std::vector<int> RU, const std::vector<int> RV, const int u,
    std::vector<int> &CU, std::vector<int> &CV, const int k, int fl, int t)
{
    // printf("u:%d t:%d\nCU:", u, t);
    // for (int x : CU) printf("%d ", x);
    // printf("\nCV:");
    // for (int x : CV) printf("%d ", x);
    // printf("\nRU:");
    // for (int x : RU) printf("%d ", x);
    // printf("\nRV:");
    // for (int x : RV) printf("%d ", x);
    // printf("\nnei:");
    // for (int x : neighbour(u, t)) printf("%d ", x);
    // printf("\n");
    std::vector<int> CU_(seterase(CU, u));
    std::vector<int> CV_(intersect(CV, neighbour(u, t)));
    std::vector<int> range0(seterase(CV, neighbour(u, t)));
    for (int v : range0)
        if (nondeg(v, setadd(RU, u), 1 - t) <= k && nondeg(u, setadd(RV, v), t) <= k)
            CV_ = setadd(CV_, v);
    std::vector<int> range1(seterase(RV, neighbour(u, t)));
    for (int v : range1)
        if (nondeg(v, setadd(neighbour(v, 1 - t), u), 1 - t) == k) {
            if (fl == 0) CU_ = intersect(CU_, neighbour(v, 1 - t));
            if (fl == 1) {
                std::vector<int> r(intersect(RU, neighbour(v, 1 - t)));
                CU_ = intersect(CU_, r);
            }
        }
    CU = CU_;
    CV = CV_;
}
