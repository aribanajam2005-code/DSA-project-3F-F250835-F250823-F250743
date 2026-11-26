
// MODULE 9: Minimum Radar Coverage using Prim's Algorithm
class RadarMST {
    SpaceGraph* g;
    int mstFrom[MAX_V], mstTo[MAX_V], mstWeight[MAX_V]; // stores selected MST corridors
    int mstCount;
    int totalCost;
    int parentSet[MAX_V]; // used in verifyConnectivity() to check graph components

    // helper functions for connectivity verification
    int findSet(int x) {
        while (parentSet[x] != x) x = parentSet[x];
        return x;
    }

    bool unionSets(int a, int b) {
        int rootA = findSet(a), rootB = findSet(b);
        if (rootA == rootB) return false;
        parentSet[rootA] = rootB;
        return true;
    }

    void resetSets() {
        for (int i = 0; i < MAX_V; i++) parentSet[i] = i;
    }

public:
    RadarMST(SpaceGraph* graph) {
        g = graph;
        mstCount = 0;
        totalCost = 0;
    }

    // builds Minimum Spanning Tree using Prim's algorithm
    void computeMST() {
        mstCount = 0;
        totalCost = 0;
        int n = g->count;
        if (n <= 0) return;

        const int INF = 1000000;
        int minCost[MAX_V];   // minimum edge weight to connect each sector
        int parent[MAX_V];    // stores which sector connected this vertex
        bool visited[MAX_V];  // tracks sectors already included in MST

        for (int i = 0; i < n; i++) {
            minCost[i] = INF;
            parent[i] = -1;
            visited[i] = false;
        }

        // outer loop handles disconnected components (in case cyber attack cuts a corridor)
        for (int start = 0; start < n; start++) {
            if (visited[start]) continue;

            minCost[start] = 0;

            for (int step = 0; step < n; step++) {
                // pick the unvisited sector with the smallest connection cost
                int u = -1;
                for (int v = 0; v < n; v++) {
                    if (!visited[v] && minCost[v] < INF) {
                        if (u == -1 || minCost[v] < minCost[u]) {
                            u = v;
                        }
                    }
                }

                // no more reachable sectors in this component
                if (u == -1) break;

                visited[u] = true;

                // if u is not the starting node of this component, save the edge in MST
                if (parent[u] != -1) {
                    mstFrom[mstCount] = parent[u];
                    mstTo[mstCount] = u;
                    mstWeight[mstCount] = minCost[u];
                    totalCost += minCost[u];
                    mstCount++;
                }

                // update minimum costs for all unvisited neighbors of u
                for (int v = 0; v < n; v++) {
                    int w = g->getWeight(u, v);
                    if (w > 0 && !visited[v] && w < minCost[v]) {
                        minCost[v] = w;
                        parent[v] = u;
                    }
                }
            }
        }
    }

    // prints all selected MST corridors and total radar coverage cost
    void displayMST() {
        cout << "  Selected corridors (Prim):\n";
        for (int i = 0; i < mstCount; i++) {
            cout << "    " << g->names[mstFrom[i]] << " <-> " << g->names[mstTo[i]]
                << "  (" << mstWeight[i] << ")\n";
        }
        cout << "  Total cost: " << totalCost << "\n";
        if (mstCount != g->count - 1) {
            cout << "  WARNING: network is not fully connected right now.\n";
        }
    }

    // removes each MST edge one by one to prove that removing any edge disconnects the network
    void verifyConnectivity() {
        cout << "  Removing each MST edge to prove the tree is minimal:\n";
        for (int skip = 0; skip < mstCount; skip++) {
            resetSets();
            int components = g->count;
            for (int i = 0; i < mstCount; i++) {
                if (i != skip && unionSets(mstFrom[i], mstTo[i])) {
                    components--;
                }
            }
            cout << "    Without " << g->names[mstFrom[skip]] << " <-> " << g->names[mstTo[skip]] << " : "
                << (components > 1 ? "DISCONNECTED (good)" : "still connected (bad)") << "\n";
        }
    }
};