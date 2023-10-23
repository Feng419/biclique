#include <bits/stdc++.h>

struct Edge {
    int u, v;
};

std::vector<Edge> e;
std::vector<int> leftnodes, rightnodes;

int n1, n2, m, s;

bool hasEdge(int u, int v) {
    for (const auto& edge : e) {
        if ((edge.u == u && edge.v == v) || (edge.u == v && edge.v == u)) {
            return true;
        }
    }
    return false;
}

int num_of_neighbours(std::vector<int> s, int v) {
    int ans = 0;
    for (auto u : s) {
        if (hasEdge(u, v)) ans++;
    }
    return ans;
}

void print(std::vector<int> A, std::vector<int> B, int rest) {
    std::cout << "rest:" << rest << std::endl;
    std::unordered_set<int> set;
    std::cout << "L:";
    for (auto a : A) {
        if (set.insert(a).second) {
            std::cout << a << " ";
        }
    }
    std::cout << std::endl << "R:";
    for (auto b : B) {
        if (set.insert(b).second) {
            std::cout << b << " "; 
        }
    }
    std::cout << std::endl;
}

int get_rest(std::vector<int> L, std::vector<int> R) {
    int ans = 0;
    std::unordered_set<int> Ls(L.begin(), L.end());
    std::unordered_set<int> Rs(R.begin(), R.end());
    for (auto l : Ls) {
        for (auto r : Rs) {
            if (!hasEdge(l, r)) ans++;
        }
    }
    return ans;
}

std::vector<int> num_ver(std::vector<int> A) {
    std::vector<int> _A;
    std::unordered_set<int> As(A.begin(), A.end());
    for (auto x : As) _A.push_back(x);
    return _A;
}

void bic(std::vector<int> _L, std::vector<int> _R, std::vector<int> _P, std::vector<int> _Q, int rest) {
    std::vector<int> L = _L;
    std::vector<int> R = _R;
    std::vector<int> P = _P;
    std::vector<int> Q = _Q;
    int restx = 0;
    for (auto x : P) {
        std::vector<int> Rx, Lx;
        Rx.push_back(x);
        for (int u : L) {
            if (hasEdge(u, x)) {
                Lx.push_back(u);
            }
        }
        std::vector<int> Px, Qx;
        bool is_maximal = true;
        for (auto v : Q) {
            int num = num_of_neighbours(num_ver(Lx), v);
            if (rest + num_ver(Lx).size() - num <= s) {
                is_maximal = false;
                break;
            } else if (num > 0) {
                Qx.push_back(v);
            }
        }
        if (is_maximal) {
            for (auto v : P) {
                if (v == x) continue;
                int num = num_of_neighbours(num_ver(Lx), v);
                if (rest + num_ver(Lx).size() - num <= s) {
                    Rx.push_back(v);
                    rest = get_rest(Lx, Rx);
                } else if (num > 0) {
                    Px.push_back(v);
                }
            }
            print(Lx, Rx, rest);
            if (!Px.empty()) {
                bic(Lx, Rx, Px, Qx, rest);
            }
        }
        P.erase(std::remove(P.begin(), P.end(), x), P.end());
        rest -= (num_ver(Lx).size() - num_of_neighbours(num_ver(Lx), x));
        if (rest < 0) rest = 0;
        Q.push_back(x);
    }
}
    

int main() {
    std::cin >> n1 >> n2 >> m >> s;
    for (int i = 1; i <= m; ++i) {
        int u, v;
        std::cin >> u >> v;
        leftnodes.push_back(u);
        rightnodes.push_back(v);
        e.push_back((Edge){u, v});
    }
    std::vector<int> L, R, P, Q;
    L = leftnodes;
    P = rightnodes;
    bic(L, R, P, Q, 0);
}