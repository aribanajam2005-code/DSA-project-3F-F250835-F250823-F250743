//  MODULE 6 : Airspace network graph
//  adjacency list (for traversal) and adjacency matrix (for weights)

string toLower(string s);

struct EdgeNode {
    int to;
    int weight;
    EdgeNode* next;
};
// to manage airspace network as weighted,undirected graph
class SpaceGraph {
public:
    string names[MAX_V];
    EdgeNode* adj[MAX_V]; // adj list
    int** matrix; // adj matrix
    int count;

    SpaceGraph() {
        count = 0;
        matrix = new int* [MAX_V];
        for (int i = 0; i < MAX_V; i++) {
            matrix[i] = new int[MAX_V];
            for (int j = 0; j < MAX_V; j++) matrix[i][j] = 0;
            adj[i] = nullptr;
        }
    }

    ~SpaceGraph() {
        for (int i = 0; i < MAX_V; i++) {
            EdgeNode* e = adj[i];
            while (e) { EdgeNode* n = e->next; delete e; e = n; }
            delete[] matrix[i];
        }
        delete[] matrix;
    }

    int findSector(const string& name) {
        string low = toLower(name);
        for (int i = 0; i < count; i++)
            if (toLower(names[i]) == low) return i;
        return -1;
    }

    bool isRunway(int i) {
        string prefix = "Runway";
        if (names[i].size() < 6) return false;
        for (int k = 0; k < 6; k++) {
            if (names[i][k] != prefix[k]) return false;
        }
        return true;
    }

    int findRunway(const string& s) {
        int i = findSector(s);
        if (i >= 0 && isRunway(i)) return i;
        i = findSector("Runway" + s);  //accepts 12 
        if (i >= 0 && isRunway(i)) return i;
        return -1;
    }

    int addSector(const string& name) {
        int i = findSector(name);
        if (i >= 0) return i; // save from duplicate names
        if (count >= MAX_V) return -1;
        names[count] = name;
        return count++;
    }

    void addEdgeNode(int srcIdx, int desIdx, int w) {
        EdgeNode* e = new EdgeNode;
        e->to = desIdx;
        e->weight = w; 
        e->next = adj[srcIdx];
        adj[srcIdx] = e;
    }

    void removeEdgeNode(int srcIdx, int destIdx) {
        EdgeNode* prev = nullptr;
        EdgeNode* curr = adj[srcIdx];
        while (curr != nullptr && curr->to != destIdx) { prev = curr; curr = curr->next; }
        if (curr == nullptr) return;
        if (prev!=nullptr) prev->next = curr->next; 
        else adj[srcIdx] = curr->next;
        delete curr;
    }

    void addCorridorById(int s1, int s2, int weight) {
        if (s1 < 0 || s2 < 0 || s1 == s2) return;
        removeCorridor(s1, s2);
        addEdgeNode(s1, s2, weight); 
        addEdgeNode(s2, s1, weight);
        matrix[s1][s2] = weight; 
        matrix[s2][s1] = weight;
    }

    void addCorridor(const string& from, const string& to, int weight) {
        int a = addSector(from), b = addSector(to);
        addCorridorById(a, b, weight);
    }

    void removeCorridor(int a, int b) {
        removeEdgeNode(a, b); 
        removeEdgeNode(b, a);
        matrix[a][b] = 0; matrix[b][a] = 0;
    }

    int getWeight(int a, int b) {
        if (a < 0 || b < 0 || a >= count || b >= count) return 0;
        return matrix[a][b];
    }

    int getSectorCount() { return count; }

    void displayAdjacencyList() {
        cout << "  Adjacency list (sector -> neighbour(time)):\n";
        for (int i = 0; i < count; i++) {
            cout << "    " << names[i] << " : ";
            if (adj[i] == nullptr) cout << "(no corridors)";
            for (EdgeNode* e = adj[i]; e; e = e->next)
                cout << names[e->to] << "(" << e->weight << ") ";
            cout << "\n";
        }

    }

    void displayAdjacencyMatrix() {
        cout << "  Adjacency matrix (0 = no corridor):\n";
        for (int i = 0; i < count; i++) cout << "    [" << i << "] " << names[i] << "\n";
        cout << "      ";
        for (int j = 0; j < count; j++) { cout << j; cout << (j < 10 ? "   " : "  "); }
        cout << "\n";
        for (int i = 0; i < count; i++) {
            cout << "   " << i << (i < 10 ? "  " : " ");
            for (int j = 0; j < count; j++) { cout << matrix[i][j]; cout << "   "; }
            cout << "\n";
        }
    }
};