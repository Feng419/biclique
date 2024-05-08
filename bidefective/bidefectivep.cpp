#include "bidefectivep.h"

void bidefective::initHash() {
    cuhash.clear();
    if (cuhash.empty()) cuhash.resize(g->n1 + g->n2);
    for (int i = 0; i < g->n1 + g->n2; ++i) {
        if (g->inv[i] < g->n1) {
            int d = g->nei[0][i].size();
            cuhash[i].reserve(d);
            for (int j = 0; j < d; ++j) {
                int u = g->nei[0][i][j].first;
                cuhash[i].insert(u);
                if (i > u && !isnbr(u, i)) {
                    printf("Can not find the nbr %d:%d\n",u, i);
                    abort();
                } 
            }
        } else {
            int d = g->nei[1][i].size();
            cuhash[i].reserve(d);
            for (int j = 0; j < d; ++j) {
                int u = g->nei[1][i][j].first;
                cuhash[i].insert(u);
                if (i > u && !isnbr(u, i)) {
                    printf("Can not find the nbr %d:%d\n",u, i);
                    abort();
                } 
            }
        } 
    }
}

void bidefective::run() {
    timecnt = 0;
    initHash();
    if (cor == 0 && ord == 0) {
        std::vector<pii> CU, CV;
        for (int i = 0; i < g->n1; ++i) CU.emplace_back(mp(i, 0));
        for (int i = g->n1; i < g->n2 + g->n1; ++i) CV.emplace_back(mp(i, 0));
        defectiveEnum(std::vector<pii>(), std::vector<pii>(), CU, CV, std::vector<pii>(), std::vector<pii>(), g->k, 0, siz);
    }
    if (cor == 1 && ord == 2) {
        std::vector<pii> after(g->n1 + g->n2, mp(0, 0)), before;
        for (int i = 0; i < g->n1 + g->n2; i++) {
            after[i] = mp(i, 0);
        }
        for (int i = 0; i < g->n1 + g->n2; i++) {
            //std::cout << "here\n" << std::endl;
            after = seterase(after, mp(i, 0));
            if (g->inv[i] < g->n1) {
                std::vector<pii> CU(intersect(seterase(twohopnei(i, 0), mp(i, 0)), after));
                std::vector<pii> CV(neighbor(setadd(CU, mp(i, 0)), 0));
                std::vector<pii> XU(intersect(seterase(twohopnei(i, 0), mp(i, 0)), before));
                for (int j = 0; j < CV.size(); ++j) {
                    if (!in(CV[j].first, g->nei[0][i])) CV[j].second++;
                }
                // std::vector<pii> CU(g->n1), XU(g->n1);
                // for (int i = 0; i < g->n1 + g->n2; ++i) {
                //     if (g->inv[i] < g->n1) CU[i] = mp(i, 0), XU[i] = mp(i, 0);
                // }
                // CU = intersect(CU, after);
                // XU = intersect(XU, before);
                // std::vector<pii> CV(g->n2);
                // for (int i = 0; i < g->n1 + g->n2; ++i) {
                //     if (g->inv[i] >= g->n1) CV[i] = mp(i, 0);
                // }
                // printf("i: %d, CU: %d, CV: %d, XU: %d\n", i, CU.size(), CV.size(), XU.size()); 
                // for (auto i : CU) printf("%d ", i.first);
                // printf("\n");
                // for (auto i : CV) printf("%d ", i.first);
                // printf("\n");
                defectiveEnum(std::vector<pii>{mp(i, 0)}, std::vector<pii>(), CU, CV, XU, std::vector<pii>(), g->k, 0, siz);
            }
            before = setadd(before, mp(i, 0));
            if (OUTPUT != 0 && tot >= OUTPUT) break;
        }
    }
    printf("total: %lld\n", tot);
    printf("timecnt: %d\n", timecnt);  
}

std::vector<pii> bidefective::seterase(const std::vector<pii> &S, const pii w) {
    std::vector<pii> res;
    for (int i = 0; i < S.size(); i++) {
        if (S[i].first != w.first) {
            res.emplace_back(S[i]);
        }
    }
    return res;
}

std::vector<pii> bidefective::seterase(const std::vector<pii> &S, const std::vector<pii> &T) {
    std::vector<pii> res;
    for (int i = 0; i < S.size(); i++) {
        if (std::find(T.begin(), T.end(), S[i]) == T.end()) {
            res.emplace_back(S[i]);
        }
    }
    return res;
}

std::vector<pii> bidefective::setadd(const std::vector<pii> &S, const pii w) {
    std::vector<pii> res = S;
    if (std::find(S.begin(), S.end(),  w) == S.end()) res.emplace_back(w);
    return res;
}

std::vector<pii> bidefective::setadd(const std::vector<pii> &S, const std::vector<pii> &T) {
    std::vector<pii> res = S;
    for (int i = 0; i < T.size(); i++) {
        if (std::find(S.begin(), S.end(), T[i]) == S.end()) res.emplace_back(T[i]);
    }
    return res;
}

std::vector<pii> bidefective::intersect(const std::vector<pii> &S, const std::vector<pii> &T) {
    std::vector<pii> res;
    for (int i = 0; i < S.size(); i++) {
        if (std::find(T.begin(), T.end(), S[i]) != T.end()) {
            res.emplace_back(S[i]);
        }
    }
    return res;
}

std::vector<pii> bidefective::neighbor(const std::vector<pii> S, const int t) {
    std::vector<pii> res;
    for (int i = 0; i < S.size(); i++) {
        res = setadd(res, g->nei[t][S[i].first]);
    }
    return res;
}

std::vector<pii> bidefective::twohopnei(const int u, const int t) {
    std::vector<pii> res;
    for (auto v : g->nei[t][u]) {
        res = setadd(res, g->nei[1 - t][v.first]);
    }
    return res;
}
 
void bidefective::printdefective(const std::vector<pii> RU, const std::vector<pii> RV) {
    printf("RU:");
    for (auto u : RU) {
        if (ord == 2) printf("%d ", g->inv[u.first]);
        else printf("%d ", u.first);
    }
    printf("\nRV:");
    for (auto u : RV) {
        if (ord == 2) printf("%d ", g->inv[u.first]);
        else printf("%d ", u.first);    
    }
    printf("\n\n");
}

bool bidefective::isnbr(const int u, const int v) {
    if (cuhash[u].getsize() <= 0) return false;
    return cuhash[u].find(v);
}

bool bidefective::in(const int u, const std::vector<pii> S) {
    for (auto v : S) {
        if (v.first == u) return true;
    }
    return false;
}

pii bidefective::findpivotv2(const std::vector<pii> CU, const std::vector<pii> RV, int k, int t) {
    int min_loss = k;
    pii res = mp(-1, 0);
    for (auto u : CU) {
        if (u.second == 0) return u;
        if (u.second <= min_loss) {
            min_loss = u.second;
            res = u;
        }
    }
    return res;
}

std::vector<pii> bidefective::updateC(const std::vector<pii> CU, const std::vector<pii> RV, pii pu, int k, int t) {
    std::vector<pii> res;
    int ans = 0;
    for (auto u : CU) {
        if (ans + u.second <= k && u != pu) {
            res = setadd(res, u);
            ans += u.second;
        }
    }
    return res;
}

void bidefective::defectiveEnum(const std::vector<pii> RU, const std::vector<pii> RV,
    std::vector<pii> CU, std::vector<pii> CV, 
    std::vector<pii> XU, std::vector<pii> XV, const int k, int t, int size) {
    // printf("-----------------\n");
    // printf("RU:");
    // for (auto x : RU) printf("%d ", x.first);
    // printf("\nRV:");
    // for (auto x : RV) printf("%d ", x.first);
    // printf("\nk:%d\nXU:", k);
    // for (auto x : XU) printf("%d ", x.first);
    // printf("\nXV:");
    // for (auto x : XV) printf("%d ", x.first);
    // printf("\nCU:");
    // for (auto x : CU) printf("%d ", x.first);
    // printf("\nCU2:");
    // for (auto x : CU) printf("%d ", x.second);
    // printf("\nCV:");
    // for (auto x : CV) printf("%d ", x.first);
    // printf("\nCV2:");
    // for (auto x : CV) printf("%d ", x.second);
    // printf("\n");
    if (CU.size() + CV.size() + XU.size() + XV.size() == 0) {
        // if (RU.size() >= 2 * g->k + 1 && RV.size() >= 2 * g->k + 1) {
            printdefective(RU, RV);
            tot++;
        // }
        return;
    }
    pii pu, pv;
    if (piv == 0) {
        std::vector<pii> CU_(CU);
        for (auto w : CU_) {
            //printf("w.second: %d\n", w.second);
            if (k - w.second >= 0 && in(w.first, CU)) {
                branch(setadd(RU, w), RV, w, seterase(CU, w), CV, XU, XV, k - w.second, t);
                CU = seterase(CU, w);
                XU = setadd(XU, w);
            }
        }
        std::vector<pii> CV_(CV);
        for (auto w : CV_) {
            if (k - w.second >= 0 && in(w.first, CV)) {
                branch(setadd(RV, w), RU, w, seterase(CV, w), CU, XV, XU, k - w.second, 1 - t);
                CV = seterase(CV, w);
                XV = setadd(XV, w);
            }
        }
    }
    if (piv == 3) {
        pu = findpivotv2(CU, RV, k, t);
        int delk = pu.first == -1 ? -1 : pu.second;
        if (pu.first == -1) {
            if (delk == 0) {
                if (k - pu.second >= 0 && in(pu.first, CU)) {
                    branch(setadd(RU, pu), RV, pu, seterase(CU, pu), CV, XU, XV, k - pu.second, t);
                    CU = seterase(CU, pu);
                    XU = setadd(XU, pu);
                }
            } else {
                std::vector<pii> delCU = updateC(CU, RV, pu, k - delk, t);
                std::vector<pii> CU_(seterase(CU, delCU));
                for (auto w : CU_) {
                    if (k - w.second >= 0 &&in(w.first, CU)) {
                        branch(setadd(RU, w), RV, w, seterase(CU, w), CV, XU, XV, k - w.second, t);
                        CU = seterase(CU, w);
                        XU = setadd(XU, w);
                    }
                }
            }
        } else {
            std::vector<pii> CU_(CU);
            for (auto w : CU_) {
                if (k - w.second >= 0 && in(w.first, CU)) {
                    branch(setadd(RU, w), RV, w, seterase(CU, w), CV, XU, XV, k - w.second, t);
                    CU = seterase(CU, w);
                    XU = setadd(XU, w);
                }
            }
        }
        std::vector<pii> CV_(CV);
        if (pu.first != -1 && !delk) CV_ = seterase(CV_, g->nei[t][pu.first]);
        for (auto w : CV_) {
            if (k - w.second >= 0 && in(w.first, CV)) {
                branch(setadd(RV, w), RU, w, seterase(CV, w), CU, XV, XU, k - w.second, 1 - t);
                CV = seterase(CV, w);
                XV = setadd(XV, w);
            }
        }
    }
}

void bidefective::branch(const std::vector<pii> RU, const std::vector<pii> RV, pii u,
    std::vector<pii> CU, std::vector<pii> CV, 
    std::vector<pii> XU, std::vector<pii> XV, int k, int t) {
    update(RU, RV, u, CU, CV, k, t);
    update(RU, RV, u, XU, XV, k, t);
    defectiveEnum(RU, RV, CU, CV, XU, XV, k, t, siz);
}

void bidefective::update(const std::vector<pii> RU, const std::vector<pii> RV, pii u,
    std::vector<pii> &CU, std::vector<pii> &CV, int k, int t) {
    std::vector<pii> CU_;
    std::vector<pii> CV_;
    for (auto &v : CV) {
        if (!isnbr(u.first, v.first)) v.second++;
        //if (!in(v.first, g->nei[t][u.first])) v.second++;
        if (v.second <= k) CV_.emplace_back(v);
    }
    for (auto v : seterase(CU, u)) {
        if (v.second <= k) CU_.emplace_back(v);
    }
    CU = CU_;
    CV = CV_;
}