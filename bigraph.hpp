#ifndef BIGRAPH_HPP
#define BIGRAPH_HPP

#include <vector>
#include <algorithm>
#include <queue>
#include <iostream>
#include "fastIO.hpp"
#include "filesystem.hpp"

const int N = 10000;

struct biGraph {
    int n1, n2, m, maxDu, maxDv, k;
    int n[2];
    struct Edge {
        int u, v;
    };
    std::vector<Edge> edges;
    std::vector<int> pU, pV, e1, e2;
    std::vector<int> p[2], e[2];

    biGraph() {}

    biGraph(const std::string & filepath) {
        read(filepath);
    }

    void read(const std::string & filepath) {
        fastIO in(filepath, "r");

        n1 = in.getUInt();
        n2 = in.getUInt();
        m = in.getUInt();
        k = in.getUInt();

        edges.resize(N);
        e1.resize(N);
        e2.resize(N);
        pU.resize(N);
        pV.resize(N);

        for(int i = 0; i < m; i++) {
            edges[i].u = in.getUInt();
            edges[i].v = in.getUInt();
        }
        init();
        n[0] = n1;
        n[1] = n2;
        p[0] = std::move(pU);
        p[1] = std::move(pV);
        e[0] = std::move(e1);
        e[1] = std::move(e2);
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
        printf("nei:");
        int t = 1, u = 2;
        std::vector<int> set(e[t].begin() + p[t][u], e[t].begin() + p[t][u + 1]);
        for (int x : set) printf("%d ", x);
    }
    
    void init() {
        std::vector<int> d1, d2;

        d1.resize(N);
        d2.resize(N);

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
};

#endif