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
    // for (int x : twohopnei(0, 0)) printf("%d ", x);
    // printf("\n");
    // for (int x : neighbour(0, 0)) printf("%d ", x);
    std::vector<int> index;
    index.clear();
    for (int i = 0; i < g->n1; ++i) index.push_back(i);
    if (ord == 1) {
        std::sort(index.begin(), index.end(), [this](int u, int v) -> bool{
            return g->deg1(u) > g->deg1(v);
        });
    }
    int indexsiz = index.size();
    for (int i = 0; i < g->n1; ++i) {
        int u = index[i];
        std::vector<int> RU;
        std::vector<int> CU, CV, XU;
        std::vector<int> orderpre(index.begin() + i + 1, index.end()), orderpost(index.begin(), index.begin() + i);
        RU.push_back(u);
        CU = seterase(twohopnei(u, 0), u);
        CV = twohopset(g->nei[0][u], 1);
        if (ord == 1) {
            CU = intersect(CU, orderpre);
            XU = intersect(seterase(twohopnei(u, 0), u), orderpost);
        }
        //XU = intersect(seterase(twohopnei(u, 0), u), orderpost);
        defectiveEnum(RU, std::vector<int>(), CU, CV, XU, std::vector<int>(), g->k, 0);
    }
    // defectiveEnum(std::vector<int>(), std::vector<int>(), CU, CV,
    //     std::vector<int>(), std::vector<int>(), g->k, 0);
    //printf("over\n");
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

std::vector<int> bidefective::setadd(const std::vector<int> &S, const std::vector<int> &T) {
    std::unordered_set<int> set(S.begin(), S.end());
    for (int t : T) 
        set.insert(t);
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

// std::vector<int> bidefective::neighbour(const int u, const int t) {
//     std::vector<int> s(g->e[t].begin() + g->p[t][u], g->e[t].begin() + g->p[t][u + 1]);
//     return s;
// }

std::vector<int> bidefective::twohopnei(const int u, const int t) {
    std::vector<int> n(g->nei[t][u]);
    std::vector<int> s; 
    for (int v : n) {
        s = setadd(s, g->nei[1 - t][v]);
    }
    return s;
}

std::vector<int> bidefective::twohopset(const std::vector<int> &S, const int t) {
    std::vector<int> set;
    for (int u : S) {
        set = setadd(set, twohopnei(u, t));
    }
    return set;
}

void bidefective::printdefective(const std::vector<int> &RU, const std::vector<int> &RV) {
    printf("\nRU: ");
    for (int ru : RU)
        printf("%d, ", ru + g->n1);
    printf("\n");

    printf("RV: ");
    for (int rv : RV)
        printf("%d, ", rv);
    printf("\n");
}

int bidefective::findpivot(const std::vector<int> CU, const std::vector<int> RV, const std::vector<int> XU, int t) {
    std::vector<int> S(CU);
    for (int u : S) {
        if (nondeg(u, RV, t) == 0) return u;
    }
    return -1;
}

int bidefective::findpivotv2(const std::vector<int> CU, const std::vector<int> RV, int t) {
    std::vector<int> S(CU);
    for (int u : S) {
        if (nondeg(u, RV, t) == 0) return u;
    }
    for (int u : S) {
        if (nondeg(u, RV, t) == 1) return u;
    }
    return -1;
}

std::vector<int> bidefective::updateC(const std::vector<int> CU, const std::vector<int> RV, int pu, int k, int t) {
    std::vector<int> delCU;
    int ans = 0;
    for (int u : CU) {
        if (ans + nondeg(u, RV, t) <= k && u != pu) {
            delCU = setadd(delCU, u);
            ans += nondeg(u, RV, t);
        } 
    }
    return delCU;
}

bool bidefective::checkconnect(const std::vector<int> U, const std::vector<int> V, const int t) {
    for (int u : U) {
        if (intersect(V, g->nei[t][u]).empty()) return false;
    }
    return true;
}

void bidefective::defectiveEnum(const std::vector<int> RU, const std::vector<int> RV,
    std::vector<int> CU, std::vector<int> CV, 
    std::vector<int> XU, std::vector<int> XV, const int k, int t) {
    if (CU.size() == 0 && CV.size() == 0 && k >= 0) {
        if (XU.size() == 0 && XV.size() == 0 && RU.size() > g->k && RV.size() > g->k) {
            //putchar('1');
            printdefective(RU, RV);
            return;
        }   
    }
    // getchar();
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
    int pu, pv;
    if (piv == 0) {
        std::vector<int> CU_(CU);
        for (int w : CU_) {
            if (k - nondeg(w, RV, t) >= 0 && in(w, CU)) {
                branch(RU, RV, w, seterase(CU, w), CV, XU, XV, k - nondeg(w, RV, t), t);
                CU = seterase(CU, w);
                XU = setadd(XU, w);
            }
        }
        std::vector<int> CV_(CV);
        for (int w : CV_) {
            if (k - nondeg(w, RU, 1 - t) >= 0 && in(w, CV)) {
                branch(RV, RU, w, seterase(CV, w), CU, XV, XU, k - nondeg(w, RU, 1 - t), 1 - t);
                CV = seterase(CV, w);
                XV = setadd(XV, w);
            }
        }
    } else if (piv == 1) {
        pu = findpivot(CU, RV, XU, t);
        pv = findpivot(CV, RU, XV, 1 - t);
        std::vector<int> CU_(CU);
        if (pu != -1) {
            if (k - nondeg(pu, RV, t) >= 0 && in(pu, CU)) {
                branch(RU, RV, pu, seterase(CU, pu), CV, XU, XV, k - nondeg(pu, RV, t), t);
                CU = seterase(CU, pu);
                XU = setadd(XU, pu);
            }
        } else {
            for (int w : CU_) {
                if (k - nondeg(w, RV, t) >= 0 && in(w, CU)) {
                    branch(RU, RV, w, seterase(CU, w), CV, XU, XV, k - nondeg(w, RV, t), t);
                    CU = seterase(CU, w);
                    XU = setadd(XU, w);
                }
            }
        }
        std::vector<int> CV_(CV);
        if (pu != -1) CV_ = seterase(CV_, g->nei[t][pu]);
        for (int w : CV_) {
            if (k - nondeg(w, RU, 1 - t) >= 0 && in(w, CV)) {
                branch(RV, RU, w, seterase(CV, w), CU, XV, XU, k - nondeg(w, RU, 1 - t), 1 - t);
                CV = seterase(CV, w);
                XV = setadd(XV, w);
            }
        }
    } else if (piv == 2) {
        std::vector<int> cudeg(CU);
        // printf("here.\n");
        if (!cudeg.empty()) {
            std::sort(cudeg.begin(), cudeg.end(), [=](int u, int v) -> bool {
                return nondeg(u, RV, t) == nondeg(v, RV, t) ? g->deg1(u) > g->deg1(v) : nondeg(u, RV, t) < nondeg(v, RV, t);
            });
        }
        // printf("pivotCU: ");
        // for (int u : cudeg) printf("%d ", u);
        // printf("\n");
        // printf("here.\n");
        // std::sort(cvdeg.begin(), cvdeg.end(), [=](int u, int v) -> bool {
        //     return nondeg(u, RU, 1 - t) < nondeg(v, RU, 1 - t);
        // });

        // printf("cudeg[0]: %d\n", cudeg[0]);

        int pu = -1;
        if (!cudeg.empty()) pu = nondeg(cudeg[0], RV, t) > k ? -1 : cudeg[0];
        // printf("pu: %d \n", pu);

        if (pu == -1) {
            std::vector<int> CU_(CU);
            for (int w : CU_) {
                if (k - nondeg(w, RV, t) >= 0 && in(w, CU)) {
                    branch(RU, RV, w, seterase(CU, w), CV, XU, XV, k - nondeg(w, RV, t), t);
                    CU = seterase(CU, w);
                    XU = setadd(XU, w);
                }
            }
            std::vector<int> CV_(CV);
            for (int w : CV_) {
                if (k - nondeg(w, RU, 1 - t) >= 0 && in(w, CV)) {
                    branch(RV, RU, w, seterase(CV, w), CU, XV, XU, k - nondeg(w, RU, 1 - t), 1 - t);
                    CV = seterase(CV, w);
                    XV = setadd(XV, w);
                }
            }
        } else {
            int delk = k - nondeg(pu, RV, t);
            std::vector<int> delCU(updateC(cudeg, RV, pu, delk, t));
            std::vector<int> pivotCU(seterase(CU, delCU));

            // getchar();
            // printf("cudeg: ");
            // for (int u : cudeg) printf("%d ", u);
            // printf("\nRV: ");
            // for (int u : RV) printf("%d ", u);
            // printf("\ndelk: %d\npu: %d\nt: %d\n", delk, pu, t); 
            // printf("nondeg(pu, RV, t): %d\n", nondeg(pu, RV, t));
            // printf("delCU: ");
            // for (int u : delCU) printf("%d ", u);
            // printf("\n");
            // printf("delk: %d k: %d\n", delk, k);
            // printf("updateCU: ");
            // for (int u : pivotCU) printf("%d ", u);
            // printf("\n");
            // printf("CU: ");
            // for (int u : CU) printf("%d ", u);
            // printf("\n");

            std::vector<int> CU_(pivotCU);
            for (int w : CU_) {
                if (k - nondeg(w, RV, t) >= 0 && in(w, CU)) {
                    branch(RU, RV, w, seterase(CU, w), CV, XU, XV, k - nondeg(w, RV, t), t);
                    CU = seterase(CU, w);
                    XU = setadd(XU, w);
                }
            }
            std::vector<int> CV_(CV);
            for (int w : CV_) {
                if (k - nondeg(w, RU, 1 - t) >= 0 && in(w, CV)) {
                    branch(RV, RU, w, seterase(CV, w), CU, XV, XU, k - nondeg(w, RU, 1 - t), 1 - t);
                    CV = seterase(CV, w);
                    XV = setadd(XV, w);
                }
            }
        }
    } else if (piv == 3) {
        pu = findpivotv2(CU, RV, t);
        //pv = findpivotv2(CV, RU, 1 - t);
        if (pu != -1) {
            int delk = nondeg(pu, RV, t);
            // printf("delk: %d\n", delk);
            if (delk == 0) {
                if (k - nondeg(pu, RV, t) >= 0 && in(pu, CU)) {
                    branch(RU, RV, pu, seterase(CU, pu), CV, XU, XV, k - nondeg(pu, RV, t), t);
                    CU = seterase(CU, pu);
                    XU = setadd(XU, pu);
                }
            } else {
                if (!CU.empty()) {
                    std::sort(CU.begin(), CU.end(), [=](int u, int v) -> bool {
                        return nondeg(u, RV, t) == nondeg(v, RV, t) ? g->deg1(u) > g->deg1(v) : nondeg(u, RV, t) < nondeg(v, RV, t);
                    });
                }                
                std::vector<int> delCU = updateC(CU, RV, pu, k - delk, t);
                //if (!delCU.empty()) printf("@@ %d\n", delCU.size());
                std::vector<int> CU_(seterase(CU, delCU));
                for (int w : CU_) {
                    if (k - nondeg(w, RV, t) >= 0 && in(w, CU)) {
                        branch(RU, RV, w, seterase(CU, w), CV, XU, XV, k - nondeg(w, RV, t), t);
                        CU = seterase(CU, w);
                        XU = setadd(XU, w);
                    }
                }
            }
        } else {
            std::vector<int> CU_(CU);
            for (int w : CU_) {
                if (k - nondeg(w, RV, t) >= 0 && in(w, CU)) {
                    branch(RU, RV, w, seterase(CU, w), CV, XU, XV, k - nondeg(w, RV, t), t);
                    CU = seterase(CU, w);
                    XU = setadd(XU, w);
                }
            }
        }
        std::vector<int> CV_(CV);
        //if (pu != -1) CV_ = seterase(CV_, g->nei[t][pu]);
        for (int w : CV_) {
            if (k - nondeg(w, RU, 1 - t) >= 0 && in(w, CV)) {
                branch(RV, RU, w, seterase(CV, w), CU, XV, XU, k - nondeg(w, RU, 1 - t), 1 - t);
                CV = seterase(CV, w);
                XV = setadd(XV, w);
            }
        }
    }
}

void bidefective::branch(const std::vector<int> RU, const std::vector<int> RV, int u,
    std::vector<int> CU, std::vector<int> CV, 
    std::vector<int> XU, std::vector<int> XV, int k, int t) {
    update(RU, RV, u, CU, CV, k, 0, t);
    update(RU, RV, u, XU, XV, k, 1, t);
    defectiveEnum(setadd(RU, u), RV, CU, CV, XU, XV, k, t);
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
    std::vector<int> CU_;
    CU_.clear();
    std::vector<int> CV_;
    CV_.clear();
    std::vector<int> range0(seterase(CV, g->nei[t][u]));
    for (int v : CV)
        if (nondeg(v, setadd(RU, u), 1 - t) <= k)
            CV_ = setadd(CV_, v);
    std::vector<int> range1(seterase(RV, g->nei[t][u]));
    for (int v : seterase(CU, u))
        if (nondeg(v, RV, t) <= k) {
            // if (fl == 0) CU_ = intersect(CU_, neighbour(v, 1 - t));
            // if (fl == 1) {
            //     std::vector<int> r(intersect(RU, neighbour(v, 1 - t)));
            //     CU_ = intersect(CU_, r);
            // }
            CU_ = setadd(CU_, v);
        }
    CU = CU_;
    CV = CV_;
}
