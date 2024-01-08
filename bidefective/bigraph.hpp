#ifndef BIGRAPH_HPP
#define BIGRAPH_HPP

#include <vector>
#include <algorithm>
#include <queue>
#include <iostream>
#include "fastIO.hpp"
#include "filesystem.hpp"
#include "listlinearheap.hpp"
#include "../tools/hash.hpp"

const int N = 10000;

struct biGraph {
    int n1, n2, m, maxDu, maxDv, k;
    int n[2], core[2];
    struct Edge {
        int u, v;
    };
    std::vector<Edge> edges;
    std::vector<int> pU, pV, e1, e2;
    std::vector<int> p[2], e[2];
    std::vector<int> cores[2];
    std::vector<CuckooHash> cuhash[2];
    std::vector<int> old_lables[2];

    std::vector<std::vector<int> > nei[2]; 
    biGraph() {}

    biGraph(const std::string & filepath, const std::string & core) {
        read(filepath, core);

        cuhash[0].resize(n[0]);
        cuhash[1].resize(n[1]);
        for(uint32_t t = 0; t <= 1; t++) {
            for(uint32_t i = 0; i < n[t]; i++) {
                uint32_t d = p[t][i + 1] - p[t][i];
                cuhash[t][i].reserve(d+1);
                for (uint32_t j = p[t][i]; j < p[t][i + 1];++j)
                    cuhash[t][i].insert(e[t][j]);
            }
        }
    }

    void read(const std::string & filepath, const std::string & core) {
        fastIO in(filepath, "r");

        n1 = in.getUInt();
        n2 = in.getUInt();
        m = in.getUInt();
        k = in.getUInt();

        edges.resize(m);
        e1.resize(m);
        e2.resize(m);
        pU.resize(n1 + 5);
        pV.resize(n2 + 5);

        // for (int i = 0; i < n1; ++i) {
        //     index.push_back(i);
        // }
        // for (int i = 0; i < n2; ++i) {
        //     index1.push_back(i);
        // }

        for(int i = 0; i < m; i++) {
            edges[i].u = in.getUInt();
            edges[i].v = in.getUInt();
        }

        cores[0].resize(n1);
        cores[1].resize(n2);

        if (core == "no") init();
        else if (core == "core") changeToCoreOrder();
        //

        n[0] = n1;
        n[1] = n2;
        p[0] = std::move(pU);
        p[1] = std::move(pV);
        e[0] = std::move(e1);
        e[1] = std::move(e2);

        // if (order == "deg") {
        //     std::sort(index.begin(), index.end(), cmp1);
        // }
        
        for (int i = 0; i < n1; ++i) {
            nei[0].push_back(returnnei(i, 0));
        }
        for (int i = 0; i < n2; ++i) {
            nei[1].push_back(returnnei(i, 1));
        }
        // printf("edges\n");
        // for (auto x : edges) if (x.u != 0 && x.v != 0) printf("%d %d\n", x.u, x.v);
        // printf("p[0]\n");
        // for (int i = 0; i <= 10; ++i) {
        //     int x = p[0][i];
        //     printf("%d ", x);
        // }
        // printf("p[1]\n");
        // for (int i = 0; i <= 10; ++i) {
        //     int x = p[1][i];
        //     printf("%d ", x);
        // }
        // printf("\ne[1]\n");
        // for (int i = 0; i <= 10; ++i) {
        //     int x = e[1][i];
        //     printf("%d ", x);
        // }   
        // printf("nei:");
        // int t = 1, u = 2;
        // std::vector<int> set(e[t].begin() + p[t][u], e[t].begin() + p[t][u + 1]);
        // for (int x : set) printf("%d ", x);
    }

    void init() {
        std::vector<int> d1, d2;

        d1.resize(n1 + 5);
        d2.resize(n2 + 5);

        for (int i = 0; i < m; ++i) {
            ++d1[edges[i].u];
            ++d2[edges[i].v];
        }

        // printf("d1\n");
        // for (int x : d1) if (x != 0) printf("%d ", x);
        // printf("\n");
        maxDu = 0;
        for (int i = 0; i < n1; ++i) {
            maxDu = std::max(maxDu, d1[i]);
        }
        maxDv = 0;
        for (int i = 0; i < n2; ++i) {
            maxDv = std::max(maxDu, d2[i]);
        }
        pU[0] = 0;
        for (int u = 0; u < n1; ++u) {
            pU[u + 1] = d1[u] + pU[u];
        }
        for (int i = 0; i < m; ++i) {
            e1[pU[edges[i].u]++] = edges[i].v;
        }
        pU[0] = 0;
        for(int u = 0; u < n1; u++) {
            pU[u + 1] = d1[u] + pU[u];
        } 
        
        pV[0] = 0;
        for(int v = 0; v < n2; v++) {
            pV[v + 1] = d2[v] + pV[v];
        }
        for(int i = 0; i < m; i++) {
            e2[pV[edges[i].v]++] = edges[i].u; 
        }
        pV[0] = 0;
        for(int v = 0; v < n2; v++) {
            pV[v + 1] = d2[v] + pV[v];
        }
    }

    // void degorder() {
    //     std::sort(index.begin(), index.end(), cmp1);
    // }

    // bool cmp1(int u, int v) {
    //     int degu = deg1(u), degv = deg1(v);
    //     return degu > degv;
    // }

    void degreeOrder() {
        std::vector<int> d1, d2;
        std::vector<int> label1, label2;

        d1.resize(std::max(n1, n2) + 5);
        d2.resize(std::max(n1, n2) + 5);
        label1.resize(n1);
        label2.resize(n2);

        for (int i = 0; i < m; ++i) {
            ++d1[edges[i].u];
            ++d2[edges[i].v];
        }

        maxDu = 0;
        for(int i = 0; i < n1; i++) {
            maxDu = std::max(maxDu, d1[i]);
        }
        maxDv = 0;
        for(int i = 0; i < n2; i++) {
            maxDv = std::max(maxDv, d2[i]);
        }

        for(int i = 0; i < n1; i++) {
            pV[d1[i] + 1]++;
        }
        for(int i = 0; i < maxDu; i++) {
            pV[i + 1] += pV[i];
        }

        for(int i = 0; i < n1; i++) {
            label1[i] = pV[d1[i]]++;
        }

        for(int i = 0; i < n2; i++) {
            pU[d2[i] + 1]++;
        }
        for(int i = 0; i < maxDv; i++) {
            pU[i + 1] += pU[i];
        }
        
        for(int i = 0; i < n2; i++) {
            label2[i] = pU[d2[i]]++;
        }

        for(int i = 0; i < m; i++) {
            edges[i].u = label1[edges[i].u];
            edges[i].v = label2[edges[i].v];
        }

        std::fill(d1.begin(), d1.begin() + std::max(n1, n2) + 1, 0);
        std::fill(d2.begin(), d2.begin() + std::max(n1, n2) + 1, 0);
        std::fill(pU.begin(), pU.begin() + n1 + 1, 0);
        std::fill(pV.begin(), pV.begin() + n2 + 1, 0);

        for(int i = 0; i < m; i++) {
            ++d1[edges[i].u];
            ++d2[edges[i].v];
        }

        for(int i = 0; i < n1; i++) {
            pU[i + 1] = pU[i] + d1[i];
        }
        for(int i = 0; i < n2; i++) {
            pV[i + 1] = pV[i] + d2[i];
        }

        for(int i = 0; i < m; i++) {
            e1[ pU[edges[i].u]++ ] = edges[i].v;
        }
        for(int i = 0; i < m; i++) {
            e2[ pV[edges[i].v]++ ] = edges[i].u;
        }

        pU[0] = pV[0] = 0;
        for(int i = 0; i < n1; i++) {
            pU[i + 1] = pU[i] + d1[i];
        }
        for(int i = 0; i < n2; i++) {
            pV[i + 1] = pV[i] + d2[i];
        }

        for(int i = 0; i < n1; i++) {
            std::sort(e1.begin() + pU[i], e1.begin() + pU[i + 1]);
        }
        for(int i = 0; i < n2; i++) {
            std::sort(e2.begin() + pV[i], e2.begin() + pV[i + 1]);
        }
    }

    void coreReduction(int p, int q) {
        std::queue<int> qL, qR;
        std::vector<int> d1(n1 + 1), d2(n2 + 1);
        std::vector<int> labelsL(n1 + 1), labelsR(n2 + 1);
        std::vector<bool> visL(n1 + 1), visR(n2 + 1);
                
        for(int i = 0; i < n1; i++) {
            d1[i] = deg1(i);
            if(deg1(i) < q) {
                qL.push(i);
                visL[i] = true;
            }
        }
        for(int i = 0; i < n2; i++) {
            d2[i] = deg2(i);
            if(deg2(i) < p) {
                qR.push(i);
                visR[i] = true;
            }
        }

        while(!qL.empty() || !qR.empty()) {
            while(!qL.empty()) {
                int u = qL.front(); qL.pop();

                for(int i = 0; i < d1[u]; i++) {
                    int v = e1[pU[u] + i];
                    // if(d2[v] < q) continue;

                    for(int j = pV[v]; j < pV[v] + d2[v]; j++) {
                        if(e2[j] == u) {
                            --d2[v];
                            std::swap(e2[j], e2[pV[v] + d2[v]]);

                            if(d2[v] == p - 1 && !visR[v]) {
                                qR.push(v);
                                visR[v] = true;
                            }
                            break;
                        }
                    }
                }
            }

            while(!qR.empty()) {
                int v = qR.front(); qR.pop();

                for(int i = 0; i < d2[v]; i++) {
                    int u = e2[pV[v] + i];
                    // if(d1[u] < p) continue;

                    for(int j = pU[u]; j < pU[u] + d1[u]; j++) {
                        if(e1[j] == v) {
                            --d1[u];
                            std::swap(e1[j], e1[pU[u] + d1[u]]);

                            if(d1[u] == q - 1 && !visL[u]) {
                                qL.push(u);
                                visL[u] = true;
                            }
                            break;
                        }
                    }
                }
            }
        }

        int pL = 1, pR = 1;
        for(int u = 0; u < n1; u++) {
            if(!visL[u]) labelsL[u] = pL++;
        }
        for(int v = 0; v < n2; v++) {
            if(!visR[v]) labelsR[v] = pR++;
        }
        
        int pm = 0;
        for(int u = 0; u < n1; u++) {
            if(visL[u]) continue;
            for(int i = pU[u]; i < pU[u + 1]; i++) {
                int v = e1[i];
                if(!visR[v]) {
                    edges[pm].u = labelsL[u] - 1;
                    edges[pm].v = labelsR[v] - 1;
                    ++pm;
                } 
            }
        }
        m = pm;

        n1 = pL - 1;
        n2 = pR - 1;
        // printf("n1 %u, n2 %u, m %u\n", n1, n2, m);
        
        std::fill(pU.begin(), pU.begin() + n1 + 1, 0);
        std::fill(pV.begin(), pV.begin() + n2 + 1, 0);

        std::fill(d1.begin(), d1.begin() + n1 + 1, 0);
        std::fill(d2.begin(), d2.begin() + n2 + 1, 0);
        degreeOrder();

    }

    void changeToCoreOrder() {
        std::vector<int> d1, d2;
        
        d1.resize(n1);
        d2.resize(n2);

        for(int i = 0; i < m; i++) {
            ++d1[edges[i].u];
            ++d2[edges[i].v];
        }

        maxDu = 0;
        for(int i = 0; i < n1; i++) {
            maxDu = std::max(maxDu, d1[i]);
        }
        maxDv = 0;
        for(int i = 0; i < n2; i++) {
            maxDv = std::max(maxDv, d2[i]);
        }

        pU[0] = 0;
        for(int u = 0; u < n1; u++) {
            pU[u + 1] = d1[u] + pU[u];
        }
        for(int i = 0; i < m; i++) {
            e1[pU[edges[i].u]++] = edges[i].v; 
        }
        pU[0] = 0;
        for(int u = 0; u < n1; u++) {
            pU[u + 1] = d1[u] + pU[u];
        } 
        
        pV[0] = 0;
        for(int v = 0; v < n2; v++) {
            pV[v + 1] = d2[v] + pV[v];
        }
        for(int i = 0; i < m; i++) {
            e2[pV[edges[i].v]++] = edges[i].u; 
        }
        pV[0] = 0;
        for(int v = 0; v < n2; v++) {
            pV[v + 1] = d2[v] + pV[v];
        }

        // printf("firsttest:\n");
        // printf("PU:\n");
        // for (int u : pU) printf("%d ", u);
        // printf("\nPV:\n");
        // for (int u : pV) printf("%d ", u);
        // printf("\n"); 

        ListLinearHeap lheap(n1, maxDu + 1), rheap(n2, maxDv + 1);
        int n = std::max(n1, n2);
        std::vector<int> ids(n);
        std::vector<int> keys(n);
        std::vector<int> labelsL(n1);
        std::vector<int> labelsR(n2);
        int l1 = 0, l2 = 0;

        for(int i = 0; i < n1; i++) {
            ids[i] = i;
            keys[i] = d1[i] + 1;
        }
        lheap.init(n1, maxDu + 1, ids.data(), keys.data());
        for(int i = 0; i < n2; i++) {
            ids[i] = i;
            keys[i] = d2[i] + 1;
        }
        // printf("1\n");
        rheap.init(n2, maxDv + 1, ids.data(), keys.data());
        core[0] = 0;
        core[1] = 0;
        int t = 0;
        for(int i = 0; i < n1 + n2; i++) {
            int u, degU = n2 + t;
            int v, degV = n1 + t;
            if(!lheap.empty() && !lheap.pop_min(u, degU)) printf("errorLheap\n");
            if(!rheap.empty() && !rheap.pop_min(v, degV)) printf("errorRheap\n");
            printf("u:%d v:%d\n", u, v);
            if(degU <= degV) {
                if(degV != n1 + t)
                    rheap.insert(v, degV); 
                for(int j = pU[u]; j < pU[u + 1]; j++) {
                    rheap.decrement(e1[j]);
                }
                old_lables[0][l1] = u;
                labelsL[u] = l1++;
                core[0] = std::max(core[0], degU);
                cores[0][l1-1] = core[0];
            }
            else {
                if(degU != n2 + t)
                    lheap.insert(u, degU);

                for(int j = pV[v]; j < pV[v + 1]; j++) {
                    lheap.decrement(e2[j]);
                }
                old_lables[1][l2] = v;
                labelsR[v] = l2++;
                core[1] = std::max(core[1], degV);
                cores[1][l2-1] = core[1];
            }
        }

        for(int i = 0; i < m; i++) {
            edges[i].u = labelsL[edges[i].u];
            edges[i].v = labelsR[edges[i].v];
        }
        // printf("labelsL:\n");
        // for (int i : labelsL) printf("%d ", i);
        // printf("\nlabelsR:\n");
        // for (int i : labelsR) printf("%d ", i);
        // printf("\n");

        std::fill(d1.begin(), d1.begin() + n1, 0);
        std::fill(d2.begin(), d2.begin() + n2, 0);
        std::fill(pU.begin(), pU.begin() + n1 + 1, 0);
        std::fill(pV.begin(), pV.begin() + n2 + 1, 0);

        for(int i = 0; i < m; i++) {
            ++d1[edges[i].u];
            ++d2[edges[i].v];
        }

        for(int i = 0; i < n1; i++) {
            pU[i + 1] = pU[i] + d1[i];
        }
        for(int i = 0; i < n2; i++) {
            pV[i + 1] = pV[i] + d2[i];
        }

        for(int i = 0; i < m; i++) {
            e1[ pU[edges[i].u]++ ] = edges[i].v;
        }
        for(int i = 0; i < m; i++) {
            e2[ pV[edges[i].v]++ ] = edges[i].u;
        }

        pU[0] = pV[0] = 0;
        for(int i = 0; i < n1; i++) {
            pU[i + 1] = pU[i] + d1[i];
        }
        for(int i = 0; i < n2; i++) {
            pV[i + 1] = pV[i] + d2[i];
        }

        for(int i = 0; i < n1; i++) {
            std::sort(e1.begin() + pU[i], e1.begin() + pU[i + 1]);
        }
        for(int i = 0; i < n2; i++) {
            std::sort(e2.begin() + pV[i], e2.begin() + pV[i + 1]);
        }
        printf("last test:\n");
        printf("PU:\n");
        for (int u : pU) printf("%d ", u);
        printf("\nPV:\n");
        for (int u : pV) printf("%d ", u);
        printf("\n"); 
    }

    int deg1(int u) {
        return p[0][u + 1] - p[0][u];
    }

    int deg2(int v) {
        return p[1][v + 1] - p[1][v];
    }
    
    std::vector<int> returnnei(int u, int t) {
        std::vector<int> s(e[t].begin() + p[t][u], e[t].begin() + p[t][u + 1]);
        return s;
    }

    bool connect(uint32_t u, uint32_t v, uint32_t t) {
        return cuhash[t][u].find(v);
    }
};

#endif
