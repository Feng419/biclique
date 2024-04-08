#ifndef BIGRAPH_HPP
#define BIGRAPH_HPP

#include <vector>
#include <algorithm>
#include <queue>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include "../tools/fastIO.hpp"
#include "../tools/filesystem.hpp"
#include "../tools/listlinearheap.hpp"

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
    // std::vector<CuckooHash> cuhash[2];
    std::vector<int> old_lables[2];

    std::vector<std::vector<int> > nei[2], tnei[2], onei[2]; 
    std::vector< std::unordered_set<int> > edge, edge0;
    std::vector<int> ord, inv;
    int lrs[2];
    biGraph() {}

    biGraph(const std::string & filepath, const std::string & core, const std::string & order, int size, int k) {
        read(filepath, core, order, size, k);
    }

    void read(const std::string & filepath, const std::string & core, const std::string & order, int size, int k) {
        fastIO in(filepath, "r");
        
        //printf("overread.\n");

        n1 = in.getUInt();
        n2 = in.getUInt();
        m = in.getUInt();

        this->k = k;
        //printf("%d %d %d %d\n", n1, n2, m, k);
        edges.resize(m);
        edge.resize(n1 + n2);
        onei[0].resize(n1 + n2);
        onei[1].resize(n1 + n2);
        tnei[0].resize(n1 + n2);
        tnei[1].resize(n1 + n2);
        //printf("222\n");
        for(int i = 0; i < m; i++) {
            int uu = 0, vv = 0;
            uu = in.getUInt(), vv = in.getUInt();
            edges[i].u = uu;
            edges[i].v = vv;
            vv = vv + n1;
            edge[uu].insert(vv);
            edge[vv].insert(uu);
        }

        // for (int i = 0; i < n1 + n2; ++i) {
        //     std::cout << "tnei[" << i << "]:" << std::endl;
        //     for (int u : tnei[i < n1 ? 0 : 1][i]) std::cout << u << " ";
        //     std::cout << std::endl;
        // }

        //printf("overhere.\n");
        if (size != 0) { 
            corereduction(edge, size - k);
            int newm = 0;
            for (int i = 0; i < n1; ++i) {
                newm += edge[i].size();
            }
            m = newm;
        }
        std::cout << "n1: " << n1 << ", n2: " << n2 << ", m: " << m << std::endl;
        // int num = 0;
        // for (const auto& set : edge) {
        //     std::cout << num++ << ": ";
        //     for (int num : set) {
        //         std::cout << num << " ";
        //     }
        //     std::cout << std::endl;
        // }
        e1.resize(m);
        e2.resize(m);
        pU.resize(n1 + 5);
        pV.resize(n2 + 5);
        // if (core == "no") init();
        // else if (core == "core") changeToCoreOrder();
        // printf("111");
        if (order == "no" || order == "deg") {
            init();
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
        } else if (order == "core") {
            // printf("333\n");
            coreorder();
            // printf("111\n");
            n[0] = n1;
            n[1] = n2;
            // printf("222\n");
            nei[0].resize(n1 + n2);
            nei[1].resize(n1 + n2);
            for (int i = 0; i < n1 + n2; ++i) {
                if (inv[i] < n1) nei[0][i] = neighbor(i);
                else nei[1][i] = neighbor(i);
            }
            // for (int i = 0; i < n1 + n2; ++i) {
            //     if (inv[i] < n1) printf("%d ", i);
            // } 
            // printf("\n");
            // for (int i = 0; i < n1 + n2; ++i) {
            //     printf("nei: ");
            //     for (int u : neighbor(i)) printf("%d ", u);
            //     printf("\n");
            // }
        }
        // std::cout << "n1: " << n1 << ", n2: " << n2 << ", m: " << m << std::endl;
        // int num = 0;
        // for (const auto& set : edge) {
        //     std::cout << num++ << ": ";
        //     for (int num : set) {
        //         std::cout << num << " ";
        //     }
        //     std::cout << std::endl;
        // }
        // printf("over\n");
    }

    void corereduction(std::vector< std::unordered_set<int> > & edge, int p) { 
        std::unordered_set<int> toRemove;
        std::unordered_map<int, int> indexMapU, indexMapV; 

        for (int i = 0; i < n1; ++i) {
            if (edge[i].size() < p) {
                toRemove.insert(i);
            } else {
                indexMapU[i] = indexMapU.size();
            }
        }

        for (int i = n1; i < edge.size(); ++i) {
            if (edge[i].size() < p) {
                toRemove.insert(i);
            } else {
                indexMapV[i] = indexMapV.size() + n1;
            }
        }

        std::vector<std::unordered_set<int>> newEdge;
        for (int i = 0; i < edge.size(); ++i) {
            if (toRemove.find(i) == toRemove.end()) { 
                std::unordered_set<int> newSet;
                for (int elem : edge[i]) {
                    if (toRemove.find(elem) == toRemove.end()) { 
                        newSet.insert(i < n1 ? indexMapV[elem] : indexMapU[elem]); 
                    }
                }
                newEdge.push_back(newSet);
            }
        }

        for (int i = 0; i < indexMapU.size(); ++i) {
            std::unordered_set<int> s;
            for (int u : newEdge[i]) {
                s.insert(u - n1 + indexMapU.size());
            }
            newEdge[i] = std::move(s);
        }
        n1 = indexMapU.size();
        n2 = indexMapV.size();

        edge = std::move(newEdge);
    }

    std::vector<int> neighbor(const int u) {
        std::vector<int> S(edge[u].begin(), edge[u].end());
        return S;
    }

    std::vector<int> neighbor(const std::vector<int> &S) {
        std::unordered_set<int> set;
        for (int s : S)
            set.insert(edge[s].begin(), edge[s].end());
        return std::vector<int>(set.begin(), set.end());
    }

    std::vector<int> twoHopNeighbor(const int w) {
        return neighbor(neighbor(w));
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

    void coreorder() {
        ord = std::vector<int>(n1 + n2, -1);
        inv.resize(n1 + n2);
        edge0.resize(n1 + n2);
        std::priority_queue< std::pair<int, int> > pq;
        for (int i = 0; i < n1; ++i)
            pq.push(std::make_pair(-int(edge[i].size()), i));
        for (int i = 0; i < n2; ++i)
            pq.push(std::make_pair(-int(edge[i + n1].size()), i + n1));

        int idx = 0;
        while (!pq.empty()) {
            int p = pq.top().second;
            pq.pop();
            if (ord[p] != -1)
                continue;
            ord[p] = idx++;
            for (int q : edge[p]) {
                edge[q].erase(p);
                edge0[q].insert(p);
                pq.push(std::make_pair(-int(edge[q].size()), q));
            }
        }

        for (int i = 0; i < n1 + n2; ++i) {
            inv[ord[i]] = i;
            edge0[i].insert(edge[i].begin(), edge[i].end());
            edge[i].clear();
        }
        for (int i = 0; i < n1 + n2; ++i) {
            for (int u : edge0[i]) {
                edge[ord[i]].insert(ord[u]);
                edge[ord[u]].insert(ord[i]);
            }
        }

        // d1.resize(n1 + 5);
        // d2.resize(n2 + 5);

        // for (int i = 0; i < n1; ++i) {
        //     d1[i] = edge[i].size();
        // }
        // for (int i = n1; i < n1 + n2; ++i) {
        //     d2[i] = edge[i].size();
        // }

        // // printf("d1\n");
        // // for (int x : d1) if (x != 0) printf("%d ", x);
        // // printf("\n");
        // maxDu = 0;
        // for (int i = 0; i < n1; ++i) {
        //     maxDu = std::max(maxDu, d1[i]);
        // }
        // maxDv = 0;
        // for (int i = 0; i < n2; ++i) {
        //     maxDv = std::max(maxDu, d2[i]);
        // }
        // pU[0] = 0;
        // for (int u = 0; u < n1; ++u) {
        //     pU[u + 1] = d1[u] + pU[u];
        // }
        // for (int i = 0; i < m; ++i) {
        //     e1[pU[edges[i].u]++] = edges[i].v;
        // }
        // pU[0] = 0;
        // for(int u = 0; u < n1; u++) {
        //     pU[u + 1] = d1[u] + pU[u];
        // } 
        
        // pV[0] = 0;
        // for(int v = 0; v < n2; v++) {
        //     pV[v + 1] = d2[v] + pV[v];
        // }
        // for(int i = 0; i < m; i++) {
        //     e2[pV[edges[i].v]++] = edges[i].u; 
        // }
        // pV[0] = 0;
        // for(int v = 0; v < n2; v++) {
        //     pV[v + 1] = d2[v] + pV[v];
        // }
    }
    // void degorder() {
    //     std::sort(index.begin(), index.end(), cmp1);
    // }

    // bool cmp1(int u, int v) {
    //     int degu = deg1(u), degv = deg1(v);
    //     return degu > degv;
    // }

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

    // bool connect(int u, int v, int t) {
    //     return cuhash[t][u].find(v);
    // }
};

#endif
