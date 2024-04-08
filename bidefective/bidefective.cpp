#include "bidefective.h"

void bidefective::run() {
    // printf("\n");
    // for (int x : CV) printf("%d ", x);
    // for (int x : twohopnei(0, 0)) printf("%d ", x);
    // printf("\n");
    // for (int x : neighbour(0, 0)) printf("%d ", x);
    if (cor == 1) {
        std::vector<int> index;
        index.clear();
        for (int i = 0; i < g->n1; ++i) index.push_back(i);
        if (ord <= 1 || ord == 3) {
            std::sort(index.begin(), index.end(), [this](int u, int v) -> bool{
                return g->deg1(u) > g->deg1(v);
            });
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
                defectiveEnum(RU, std::vector<int>(), CU, CV, XU, std::vector<int>(), g->k, 0, siz);
                //printf("total: %d", total);
            }
        } else if (ord == 2) {
            std::vector<int> after(g->n1 + g->n2), before;
            for (int i = 0; i < g->n1 + g->n2; ++i)
                after[i] = i;
            for (int i = 0; i < g->n1 + g->n2; ++i) {
                after = seterase(after, i);
                if (g->inv[i] < g->n1) {    
                    std::vector<int> CU(intersect(seterase(twohopnei(i, 0), i), after));
                    std::vector<int> CV(neighbor(setadd(CU, i), 0));
                    std::vector<int> XU(intersect(seterase(twohopnei(i, 0), i), before));
                    // std::vector<int> CU, XU;
                    // for (int i = 0; i < g->n1 + g->n2; ++i) {
                    //     if (g->inv[i] < g->n1) CU[i] = i, XU[i] = i;
                    // }
                    // CU = intersect(CU, after);
                    // XU = intersect(XU, before);
                    // std::vector<int> CV;
                    // for (int i = 0; i < g->n1 + g->n2; ++i) {
                    //     if (g->inv[i] >= g->n1) CV[i] = i;
                    // }
                    // printf("i: %d, CU: %d, CV: %d, XU: %d\n", i, CU.size(), CV.size(), XU.size()); 
                    // for (int i : CU) printf("%d ", i);
                    // printf("\n");
                    // for (int i : CV) printf("%d ", i);
                    // printf("\n");
                    defectiveEnum(std::vector<int>{i}, std::vector<int>(), CU, CV,
                        XU, std::vector<int>(), g->k, 0, siz);
                }
                before.emplace_back(i);
                if (OUTPUT != 0 && tot >= OUTPUT) {
                    break;
                }
            }
            printf("total: %lld\n", tot);
            // printf("maxsize: %d\n", maxsize);
        }
    } else {
        std::vector<int> after(g->n1 + g->n2), before;
        for (int i = 0; i < g->n1 + g->n2; ++i)
            after[i] = i;
        for (int i = 0; i < g->n1 + g->n2; ++i) {
            after = seterase(after, i);
            if (g->inv[i] < g->n1) {
                std::vector<int> CU(g->n1), CV(g->n2), XU(g->n1);
                for (int i = 0; i < g->n1; ++i)
                    CU[i] = g->ord[i], XU[i] = g->ord[i];
                for (int i = 0; i < g->n2; ++i)
                    CV[i] = g->ord[i + g->n1];
                CU = intersect(CU, after);
                XU = intersect(XU, before);
                defectiveEnum(std::vector<int>{i}, std::vector<int>(), CU, CV,
                    XU, std::vector<int>(), g->k, 0, siz);
            }
            before.emplace_back(i);
        }
        printf("total: %lld", tot);
    }

    
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

std::vector<int> bidefective::neighbor(const std::vector<int> &S, int t) {
    std::unordered_set<int> set;
    for (int s : S)
        set.insert(g->nei[t][s].begin(), g->nei[t][s].end());
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
    std::unordered_set<int> s(g->nei[t][u].begin(), g->nei[t][u].end());
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
    // std::vector<int> n(g->nei[t][u]);
    // std::vector<int> ans(g->nei[1 - t][g->nei[t][u][0]]);
    // for (int i = 1; i < g->nei[t][u].size(); ++i) {
    //     int v = g->nei[t][u][i];
    //     ans = intersect(ans, g->nei[1 - t][v]);
    // }
    // return ans;
    std::vector<int> set;
    for (int v : g->nei[t][u]) {
        set = setadd(set, g->nei[1 - t][v]);
    }
    return set;
}

std::vector<int> bidefective::twohopset(const std::vector<int> &S, const int t) {
    std::vector<int> set;
    for (int u : S) {
        set = setadd(set, twohopnei(u, t));
    }
    return set;
}

void bidefective::printdefective(const std::vector<int> &RU, const std::vector<int> &RV) {
    printf("RU:");
    for(int u : RU) printf("%d ", u);printf("\n");
    printf("RV:");
    for(int u : RV) printf("%d ", u);printf("\n");
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
    int pivot = -1;
    int min_loss = k;
    for (int u : S) {
        if (nondeg(u, RV, t) == 0) return u;
        if (nondeg(u, RV, t) <= min_loss) min_loss =  nondeg(u, RV, t), pivot = u;
    }
    return pivot;
}

int bidefective::findpivotv4(const std::vector<int> CU, const std::vector<int> CV, int t) {
    std::vector<int> S(CU);
    int minn = g->n1 + g->n2;
    int pivot = -1;
    for (int u : S) {
        if (nondeg(u, CV, t) <= minn) {
            minn = nondeg(u, CV, t);
            pivot = u;
        }
    }   
    return pivot;
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

int bidefective::findnonnei(int u, const std::vector<int> CV, const int t) {
    std::unordered_set<int> s(g->nei[t][u].begin(), g->nei[t][u].end());
    for (int v : CV) {
        if (s.find(v) == s.end()) return v;
    }
    return -1;
}

bool bidefective::checknon(int u, const std::vector<int> RV, const std::vector<int> RU, const std::vector<int> CU, const int k, const int t) {
    std::vector<int> S(seterase(RV, g->nei[t][u]));
    for (int v : S) {
        if (nondeg(v, setadd(RU, CU), 1 - t) >= k) return false;
    }
    return true;
}

bool bidefective::checkpi(const std::vector<int> RU, const std::vector<int> RV, const std::vector<int> CV, const int t, const int k) {
    std::vector<int> S(RU);
    int tot = 0;
    for (int u : S) {
        tot += nondeg(u, setadd(RV, CV), t);
    }
    return tot > k;
}

void bidefective::defectiveEnum(const std::vector<int> RU, const std::vector<int> RV,
    std::vector<int> CU, std::vector<int> CV, 
    std::vector<int> XU, std::vector<int> XV, const int k, int t, int size) {
    if (tot >= OUTPUT && OUTPUT != 0) return ;
    if (piv == 4) {
        if (RU.size() + CU.size() <= size || RV.size() + CV.size() <= size) return ;
        if (CU.size() == 0 && CV.size() == 0 && k >= 0) {
            if (XU.size() == 0 && XV.size() == 0 && RU.size() >= 2 * g->k + 1 && RV.size() >= 2 * g->k + 1) {
                printdefective(RU, RV);
                tot++;
            }
            return;  
        }
        //printf("here.\n");
        //getchar();
        printf("-----------------\n");
        printf("RU:");
        for (int x : RU) printf("%d ", x);
        printf("\nRV:");
        for (int x : RV) printf("%d ", x);
        printf("\nk:%d\nXU:", k);
        for (int x : XU) printf("%d ", x);
        printf("\nXV:");
        for (int x : XV) printf("%d ", x);
        printf("\nCU:");
        for (int x : CU) printf("%d ", x);
        printf("\nCV:");
        for (int x : CV) printf("%d ", x);
        printf("\n");
        
        if (checkpi(RU, RV, CV, t, k)) {
            //printf("here\n");
            branchb(RV, RU, CV[0], CV, CU, XV, XU, k, 1 - t);
        } else if (checkpi(RV, RU, CU, 1 - t, k)) {
            branchb(RU, RV, CU[0], CU, CV, XU, XV, k, t);
        } else {
            int puu = findpivotv4(CU, CV, t);
            int pvv = findpivotv4(CV, CU, 1 - t);
            if (puu != -1) {
                for (int u : XU) {
                    if (nondeg(u, CV, t) <= nondeg(puu, CV, t) && checknon(u, RV, RU, CU, k, t)) puu = u;
                }
            }
            if (pvv != -1) {
                for (int v : XV) {
                    if (nondeg(v, CU, 1 - t) <= nondeg(pvv, CU, 1 - t) && checknon(v, RU, RV, CV, k, 1 - t)) pvv = v;
                }
            }
            
            printf("puu: %d pvv: %d\n", puu, pvv);
            if (puu == -1 && pvv != -1) {
                branchp(RV, RU, pvv, CV, CU, XV, XU, k, 1 - t);
            }  
            if (puu != -1 && pvv == -1) {
                branchp(RU, RV, puu, CU, CV, XU, XV, k, t);
            }
            if (puu != -1 && pvv != -1 && nondeg(puu, CV, t) <= nondeg(pvv, CU, 1 - t)) {
                branchp(RU, RV, puu, CU, CV, XU, XV, k, t);
            } else if (puu != -1 && pvv != -1 && nondeg(puu, CV, t) > nondeg(pvv, CU, 1 - t)) {
                branchp(RV, RU, pvv, CV, CU, XV, XU, k, 1 - t);
            }
        }
    } else {
        if (RU.size() + CU.size() <= size || RV.size() + CV.size() <= size) return ;
        if (CU.size() == 0 && CV.size() == 0 && k >= 0) {
            if (XU.size() == 0 && XV.size() == 0 && RU.size() >= 2 * g->k + 1 && RV.size() >= 2 * g->k + 1) {
                //putchar('1'); 
                printdefective(RU, RV);
                tot++;
                //std::cout << tot << std::endl;
                // int sizen = RU.size() + RV.size();
                // maxsize = std::max(maxsize, sizen);
                // if (size != 0) {
                //     if (RU.size() > size && RV.size() > size) total++, printdefective(RU, RV);
                // } else total++;
            } 
            return;  
        }
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
            if (checkpi(RU, RV, CV, t, k)) {
                std::vector<int> CU_(CU);
                for (int w : CU_) {
                    if (k - nondeg(w, RV, t) >= 0 && in(w, CU)) {
                        branch(RU, RV, w, seterase(CU, w), CV, XU, XV, k - nondeg(w, RV, t), t);
                        CU = seterase(CU, w);
                        XU = setadd(XU, w);
                    }
                }
            } else {
                int pu = findpivotv4(CU, CV, t);
                if (pu != -1) {
                    if (k - nondeg(pu, RV, t) >= 0 && in(pu, CU)) {
                        branch(RU, RV, pu, seterase(CU, pu), CV, XU, XV, k - nondeg(pu, RV, t), t);
                        CU = seterase(CU, pu);
                        XU = setadd(XU, pu);
                    }
                }
            }
            if (checkpi(RV, RU, CU, 1 - t, k)) {
                std::vector<int> CV_(CV);
                for (int w : CV_) {
                    if (k - nondeg(w, RU, 1 - t) >= 0 && in(w, CV)) {
                        branch(RV, RU, w, seterase(CV, w), CU, XV, XU, k - nondeg(w, RU, 1 - t), 1 - t);
                        CV = seterase(CV, w);
                        XV = setadd(XV, w);
                    }
                }
            } else {
                int pv = findpivotv4(CV, CU, 1 - t);
                if (pv != -1) {
                    if (k - nondeg(pv, RU, 1 - t) >= 0 && in(pv, CV)) {
                        branch(RV, RU, pv, seterase(CV, pv), CU, XV, XU, k - nondeg(pv, RU, 1 - t), 1 - t);
                        CV = seterase(CV, pv);
                        XV = setadd(XV, pv);
                    }
                }
            }
        } else if (piv == 3) {
            pu = findpivotv2(CU, RV, t);
            int delk = pu == -1 ? -1 : nondeg(pu, RV, t);
            if (pu != -1) {
                if (delk == 0) {
                    if (k - nondeg(pu, RV, t) >= 0 && in(pu, CU)) {
                        branch(RU, RV, pu, seterase(CU, pu), CV, XU, XV, k - nondeg(pu, RV, t), t);
                        CU = seterase(CU, pu);
                        XU = setadd(XU, pu);
                    }
                } else {         
                    std::vector<int> delCU = updateC(CU, RV, pu, k - delk, t);
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
            if (pu != -1 && !delk) CV_ = seterase(CV_, g->nei[t][pu]);
            for (int w : CV_) {
                if (k - nondeg(w, RU, 1 - t) >= 0 && in(w, CV)) {
                    branch(RV, RU, w, seterase(CV, w), CU, XV, XU, k - nondeg(w, RU, 1 - t), 1 - t);
                    CV = seterase(CV, w);
                    XV = setadd(XV, w);
                }
            }
        }
    }
    
}

void bidefective::branch(const std::vector<int> RU, const std::vector<int> RV, int u,
    std::vector<int> CU, std::vector<int> CV, 
    std::vector<int> XU, std::vector<int> XV, int k, int t) {
    update(RU, RV, u, CU, CV, k, 0, t);
    update(RU, RV, u, XU, XV, k, 1, t);
    defectiveEnum(setadd(RU, u), RV, CU, CV, XU, XV, k, t, siz);
}

void bidefective::branchb(const std::vector<int> RU, const std::vector<int> RV, int u,
    std::vector<int> CU, std::vector<int> CV, 
    std::vector<int> XU, std::vector<int> XV, int k, int t) {
    update(RU, RV, u, CU, CV, k, 0, t);
    update(RU, RV, u, XU, XV, k, 1, t);
    defectiveEnum(setadd(RU, u), RV, CU, CV, XU, XV, k - nondeg(u, RV, t), t, siz);
    defectiveEnum(RU, RV, seterase(CU, u), CV, setadd(XU, u), XV, k, t, siz);
}

void bidefective::branchp(const std::vector<int> RU, const std::vector<int> RV, int u,
    std::vector<int> CU, std::vector<int> CV, 
    std::vector<int> XU, std::vector<int> XV, int k, int t) {
    std::vector<int> S(seterase(CV, g->nei[t][u]));
    for (int v : S) {
        update(RV, RU, v, CV, CU, k, 0, 1 - t);
        update(RV, RU, v, XV, XU, k, 1, 1 - t);
        defectiveEnum(RU, setadd(RV, v), CU, CV, XU, XV, k - nondeg(v, RU, 1 - t), t, siz);
        CV = seterase(CV, v);
        XV = setadd(XV, v);
    }
    if (in(u, XU)) {
        update(RU, RV, u, CU, CV, k, 0, t);
        update(RU, RV, u, XU, XV, k, 1, t);
        defectiveEnum(setadd(RU, u), RV, CU, CV, XU, XV, k - nondeg(u, RV, t), t, siz);
    }
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
    std::vector<int> CV_;
    for (int v : CV)
        if (nondeg(v, setadd(RU, u), 1 - t) <= k)
            CV_ = setadd(CV_, v);
    for (int v : seterase(CU, u))
        if (nondeg(v, RV, t) <= k) {
            CU_ = setadd(CU_, v);
        }
    CU = CU_;
    CV = CV_;
}
