#include <iostream>
using namespace std;

class SpaceGraph;
const int MAX_V = 20;

class RadarMST {
    SpaceGraph* g;
    int ea[200], eb[200], ew[200], edgeCount;
    int ma[MAX_V], mb[MAX_V], mw[MAX_V], mstCount;
    int total;
    int parentSet[MAX_V];

    int findSet(int x) {
        while (parentSet[x] != x) x = parentSet[x];
        return x;
    }

    bool unionSets(int a, int b) {
        int ra = findSet(a), rb = findSet(b);
        if (ra == rb) return false;
        parentSet[ra] = rb;
        return true;
    }

    void resetSets() {
        for (int i = 0; i < MAX_V; i++) parentSet[i] = i;
    }
public:
    RadarMST(SpaceGraph* graph) { g = graph; edgeCount = mstCount = total = 0; }

    void computeMST() {
        edgeCount = 0; mstCount = 0; total = 0;
        for (int i = 0; i < g->count; i++)
            for (int j = i + 1; j < g->count; j++)
                if (g->getWeight(i, j) > 0) {
                    ea[edgeCount] = i; eb[edgeCount] = j; ew[edgeCount] = g->getWeight(i, j);
                    edgeCount++;
                }

        for (int i = 1; i < edgeCount; i++) {
            int a = ea[i], b = eb[i], w = ew[i], j = i - 1;
            while (j >= 0 && ew[j] > w) {
                ea[j + 1] = ea[j]; eb[j + 1] = eb[j]; ew[j + 1] = ew[j];
                j--;
            }
            ea[j + 1] = a; eb[j + 1] = b; ew[j + 1] = w;
        }

        resetSets();
        for (int i = 0; i < edgeCount; i++)
            if (unionSets(ea[i], eb[i])) {
                ma[mstCount] = ea[i]; mb[mstCount] = eb[i]; mw[mstCount] = ew[i];
                total += ew[i];
                mstCount++;
            }
    }

    void displayMST() {
        cout << "  Selected corridors (Kruskal):\n";
        for (int i = 0; i < mstCount; i++)
            cout << "    " << g->names[ma[i]] << " <-> " << g->names[mb[i]] << "  (" << mw[i] << ")\n";
        cout << "  Total cost: " << total << "\n";
        if (mstCount != g->count - 1)
            cout << "  WARNING: network is not fully connected right now.\n";
    }

    void verifyConnectivity() {
        cout << "  Removing each MST edge to prove the tree is minimal:\n";
        for (int skip = 0; skip < mstCount; skip++) {
            resetSets();
            int components = g->count;
            for (int i = 0; i < mstCount; i++)
                if (i != skip && unionSets(ma[i], mb[i])) components--;
            cout << "    Without " << g->names[ma[skip]] << " <-> " << g->names[mb[skip]] << " : "
                << (components > 1 ? "DISCONNECTED (good)" : "still connected (bad)") << "\n";
        }
    }
};