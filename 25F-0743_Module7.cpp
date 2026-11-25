//  MODULE 7 : Radar sweep (BFS with queue, DFS with stack)

class IntQueue {
    int data[100];   // stores queue elements
    int head, tail;
public:
    IntQueue() { head = tail = 0; }  
    void enqueue(int x) { data[tail++] = x; }
    int dequeue() { return data[head++]; }
    bool isEmpty() { return head == tail; }
};
class IntStack {
    int data[500];   // stores stack elements
    int top;
public:
    IntStack() { top = 0; }   // empty stack
    void push(int x) { if (top < 500) data[top++] = x; }
    int pop() { return data[--top]; }
    bool isEmpty() { return top == 0; }
};

class RadarSweep {
    SpaceGraph* g; 
public:
    int lastLevel[MAX_V];       // stores BFS level of each node 
    RadarSweep(SpaceGraph* graph) { g = graph; }  

    int bfsFrom(int source, bool print) {
        for (int i = 0; i < MAX_V; i++)
            lastLevel[i] = -1;  // -1 means unvisited node 
        IntQueue q;   // queue for BFS 
        lastLevel[source] = 0;
        q.enqueue(source);    
        int visited = 0, currentLevel = -1;
        while (!q.isEmpty()) {
            int u = q.dequeue();
            visited++;
            if (print) {
                if (lastLevel[u] != currentLevel) {
                    currentLevel = lastLevel[u];   // update level if changed 
                    cout << "\n    Level " << currentLevel << ": ";
                }
                cout << g->names[u] << "  ";
            }
            for (EdgeNode* e = g->adj[u]; e; e = e->next)  // move through adj. list
                if (lastLevel[e->to] == -1) {
                    lastLevel[e->to] = lastLevel[u] + 1; 
                    q.enqueue(e->to); 
                }
        }
        if (print) cout << "\n";
        return visited;
    }

    int dfsFrom(int source) {
        bool seen[MAX_V];  // tracks visited nodes 
        for (int i = 0; i < MAX_V; i++) seen[i] = false;
        IntStack st;
        st.push(source);
        int visited = 0;
        cout << "    ";
        while (!st.isEmpty()) {
            int u = st.pop();  // get top node 
            if (seen[u]) continue;
            seen[u] = true;   // mark node as visited 
            if (visited > 0) cout << " -> ";
            cout << g->names[u];
            visited++;
            for (EdgeNode* e = g->adj[u]; e; e = e->next)
                if (!seen[e->to]) st.push(e->to);
        }
        cout << "\n";
        return visited;
    }

    bool isReachable(int from, int to) {
        if (from < 0 || to < 0) return false;
        bfsFrom(from, false);
        return lastLevel[to] != -1;
    }

    // BFS from Control Tower to the last known sector
    bool searchMissingFlight(int lastKnownSector) {
        int tower = g->findSector("ControlTower");  
        if (tower < 0) return false;
        return isReachable(tower, lastKnownSector);  // check if a path exists 
    }
};