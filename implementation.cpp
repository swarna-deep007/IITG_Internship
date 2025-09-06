#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <random>

using namespace std;

class Graph {
public:
    set<int> V;
    set<pair<int, int>> E;
    unordered_map<int, unordered_set<int>> adj;

    Graph(const set<int>& vertices, const set<pair<int, int>>& edges) {
        V = vertices;
        E = edges;
        for (const auto& edge : edges) {
            int u = edge.first;
            int v = edge.second;
            adj[u].insert(v);
            adj[v].insert(u);
        }
    }

    Graph inducedSubgraph(const set<int>& vertices) {
        set<pair<int, int>> newE;
        for (const auto& edge : E) {
            int u = edge.first;
            int v = edge.second;
            if (vertices.count(u) && vertices.count(v)) {
                newE.insert({min(u, v), max(u, v)});
            }
        }
        return Graph(vertices, newE);
        
    }
};

map<int, int> galeShapley(set<int>& X, set<int>& Y, Graph& graph) {
    map<int, vector<int>> proposals;
    map<int, int> match;
    set<int> free(X.begin(), X.end());

    while (!free.empty()) {
        int p = *free.begin();
        free.erase(p);

        vector<int> candidates;
        for (int v : graph.adj[p]) {
            if (Y.count(v) && find(proposals[p].begin(), proposals[p].end(), v) == proposals[p].end()) {
                candidates.push_back(v);
            }
        }

        if (candidates.empty()) continue;

        int c = candidates[0];
        proposals[p].push_back(c);

        if (!match.count(c)) {
            match[c] = p;
        } else {
            int current = match[c];
            if (p < current) {
                match[c] = p;
                free.insert(current);
            } else {
                free.insert(p);
            }
        }
    }

    map<int, int> result;
    for (const auto& entry : match) {
        int v = entry.first;
        int u = entry.second;
        result[u] = v;
        result[v] = u;
    }
    return result;
    
}

vector<vector<int>> stablePartition(const vector<int>& V) {
    vector<int> shuffled = V;
    random_device rd;
    mt19937 g(rd());
    shuffle(shuffled.begin(), shuffled.end(), g);

    vector<vector<int>> parts;
    for (size_t i = 0; i < shuffled.size(); i += 4) {
        vector<int> group;
        for (size_t j = i; j < i + 4 && j < shuffled.size(); ++j)
            group.push_back(shuffled[j]);
        parts.push_back(group);
    }
    return parts;
}

set<pair<int, int>> unpopularityMatching(Graph& graph) {
    if (graph.V.empty()) return {};

    vector<vector<int>> P = stablePartition(vector<int>(graph.V.begin(), graph.V.end()));
    set<int> X;
    for (const auto& Ai : P) {
        for (size_t t = 0; t < Ai.size() / 2; ++t) {
            X.insert(Ai[2 * t]);
        }
    }

    set<int> Y;
    for (int v : graph.V) {
        if (!X.count(v)) Y.insert(v);
    }

    map<int, int> M = galeShapley(X, Y, graph);

    set<int> matchedY;
    for (const auto& pair : M) {
        int u = pair.first;
        int v = pair.second;
        if (X.count(u)) matchedY.insert(v);
    }

    set<int> V1;
    for (int v : graph.V) {
        if (!X.count(v) && !matchedY.count(v))
            V1.insert(v);
    }

    bool isIndependent = true;
    for (int u : V1) {
        for (int v : graph.adj[u]) {
            if (V1.count(v)) {
                isIndependent = false;
                break;
            }
        }
        if (!isIndependent) break;
    }

    set<pair<int, int>> S;
    for (const auto& pair : M) {
        int u = pair.first;
        int v = pair.second;
        if (u < v)
            S.insert({u, v});
        else
            S.insert({v, u});
    }

    if (V1.empty() || isIndependent) return S;

    Graph G1 = graph.inducedSubgraph(V1);
    set<pair<int, int>> S1 = unpopularityMatching(G1);

    S.insert(S1.begin(), S1.end());
    return S;
}

// ----------------------------

int main() {
    set<int> V;
    for (int i = 0; i < 10; ++i) V.insert(i);

    set<pair<int, int>> E;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);

    for (int i : V) {
        for (int j : V) {
            if (i < j && dis(gen) < 0.3) {
                E.insert({i, j});
            }
        }
    }

    Graph G(V, E);
    set<pair<int, int>> result = unpopularityMatching(G);

    cout << "Matching Result:\n";
    for (const auto& pair : result) {
        cout << pair.first << " - " << pair.second << endl;
    }

    return 0;
}
