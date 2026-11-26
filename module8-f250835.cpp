//  4.3.3 Module 8: Fastest Intercept Route
struct Route {
    int sectors[MAX_V];  // store sector in route
    int length;          // number of sectors 
    int cost;          // total travel time
};

class PairHeap {      // tiny min-heap 
    int dist[500];     // store distance 
    int node[500];     // store sctor number 
    int size;
public:
    PairHeap() { // constructor
        size = 0;
    }
    bool empty() { // is empty
        return size == 0;
    }
    void push(int d, int n) { // push
        int i = size++;     // inset new pair into heap
        dist[i] = d;
        node[i] = n;
        while (i > 0) {      // find parent 
            int p = (i - 1) / 2;
            if (dist[p] <= dist[i])
                break;
            int t = dist[p];   // swap  dist
            dist[p] = dist[i];
            dist[i] = t;
            t = node[p];    // dist node
            node[p] = node[i];
            node[i] = t;
            i = p;
        }
    }
    void pop(int& d, int& n) {   // pop
        d = dist[0];    // remoove top element 
        n = node[0];
        size--;         // dec size and move last to root 
        dist[0] = dist[size];
        node[0] = node[size];
        int i = 0;
        while (true) {
            int l = 2 * i + 1, r = 2 * i + 2, s = i;
            if (l < size && dist[l] < dist[s])   // left or right child has small dist
                s = l;
            if (r < size && dist[r] < dist[s])
                s = r;
            if (s == i)
                break;
            int t = dist[s];  // swap dist
            dist[s] = dist[i];
            dist[i] = t;
            t = node[s];   // swap node
            node[s] = node[i];
            node[i] = t;
            i = s;
        }
    }
};

class RouteFinder {   // dijkstra algo 
    SpaceGraph* g;
public:
    RouteFinder(SpaceGraph* graph) {   // constructor 
        g = graph;
    }
    Route findFastestRoute(int src, int dest) {  // fasted route
        Route r;
        r.length = 0;
        r.cost = -1;
        if (src < 0 || dest < 0) // no oute
            return r;
        const int INF = 1000000;
        int n = g->count;
        int dist[MAX_V], parent[MAX_V];
        bool done[MAX_V];
        for (int i = 0; i < n; i++) {   // initialize 
            dist[i] = INF;
            parent[i] = -1;
            done[i] = false;
        }
        PairHeap h; // heap
        dist[src] = 0;
        h.push(0, src);
        while (!h.empty()) {
            int d, u;
            h.pop(d, u);
            if (done[u]) continue;
            done[u] = true;
            for (int v = 0; v < n; v++) {  // check all neighbours 
                int w = g->getWeight(u, v);
                if (w > 0 && !done[v] && d + w < dist[v]) {
                    dist[v] = d + w;
                    parent[v] = u;
                    h.push(dist[v], v);
                }
            }
        }
        if (dist[dest] >= INF)
            return r;  // route exists
        int temp[MAX_V], cnt = 0;     // trace back using parent pointers
        for (int cur = dest; cur != -1;cur = parent[cur])
            temp[cnt++] = cur;
        for (int i = 0; i < cnt; i++)
            r.sectors[i] = temp[cnt - 1 - i];
        r.length = cnt;
        r.cost = dist[dest];
        return r;
    }

    void printRoute(const Route& r) {  // print 
        if (r.cost < 0) { cout << "  No route available.\n"; return; }
        cout << "  Route: ";
        for (int i = 0; i < r.length; i++) {
            cout << g->names[r.sectors[i]];
            if (i < r.length - 1) cout << " -> ";
        }
        cout << "\n  Total travel time: " << r.cost << " tick(s)\n";
    }
};
