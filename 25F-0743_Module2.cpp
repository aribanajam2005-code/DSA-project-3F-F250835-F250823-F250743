// ============================================================
//  MODULE 2 : Landing queue (normal) + min-heap (emergency)
// ============================================================
struct QNode { Flight* f; QNode* next; };

class FlightQueue {
    QNode* front;
    QNode* rear;
public:
    FlightQueue() { front = rear = nullptr; }
    ~FlightQueue() { while (front) { QNode* n = front->next; delete front; front = n; } }

    bool contains(Flight* f) {
        for (QNode* q = front; q; q = q->next) if (q->f == f) return true; // traverse queue and check for same flight
        return false;
    }
    void enqueueNormal(Flight* f) {
        if (contains(f)) return;    // avoid duplicate flights 
        QNode* n = new QNode; n->f = f; n->next = nullptr;
        if (rear == nullptr) front = rear = n;
        else { rear->next = n; rear = n; }  // link new node and update rear
    }
    Flight* dequeueNormal() {
        if (front == nullptr) return nullptr;
        QNode* n = front;
        Flight* f = n->f;
        front = front->next;
        if (front == nullptr) rear = nullptr;
        delete n;
        return f;
    }
    Flight* frontNormal() { return front ? front->f : nullptr; }  // return first flight
    bool isEmptyNormal() { return front == nullptr; }
    bool valid(Flight* f) { return f->status == 0 && f->inRegistry && f->priorityLevel == 9; }

    // throw away flights that are no longer waiting (already landed, cleared, ...)
    void cleanFront() { while (front && !valid(front->f)) dequeueNormal(); }

    void displayNormalQueue() {
        bool any = false;
        for (QNode* q = front; q; q = q->next)
            if (valid(q->f)) {
                cout << "    " << q->f->callsign << " -> " << q->f->assignedRunway
                    << " (fuel " << q->f->fuelRemaining << ")\n"; any = true;
            }
        if (!any) cout << "    (empty)\n"; // display empty queue
    }
};

struct HeapItem { int priority; int arrival; Flight* f; };

class MinHeap {
    HeapItem items[HEAP_SIZE];
    int size;

    // two-field comparison : priority first, then arrival tick
    bool isBefore(const HeapItem& a, const HeapItem& b) {
        if (a.priority != b.priority) return a.priority < b.priority;
        return a.arrival < b.arrival;
    }
    bool isValid(const HeapItem& it) {
        return it.f->status == 0 && it.f->inRegistry && it.f->priorityLevel == it.priority;
    }
    void popTop() {
        size--;
        items[0] = items[size]; // move last item to root
        int i = 0;
        while (true) {
            int l = 2 * i + 1, r = 2 * i + 2, small = i;
            if (l < size && isBefore(items[l], items[small])) small = l;
            if (r < size && isBefore(items[r], items[small])) small = r;
            if (small == i) break;
            HeapItem t = items[i]; items[i] = items[small]; items[small] = t;
            i = small;   // continue new position
        }
    }
    void cleanTop() { while (size > 0 && !isValid(items[0])) popTop(); }
public:
    MinHeap() { size = 0; }

    bool insertEmergency(Flight* f) {
        if (size >= HEAP_SIZE) return false;
        int i = size++;
        items[i].priority = f->priorityLevel;
        items[i].arrival = f->arrivalTick;
        items[i].f = f;
        while (i > 0) {        // move up
            int p = (i - 1) / 2;
            if (!isBefore(items[i], items[p])) break;
            HeapItem t = items[i]; items[i] = items[p]; items[p] = t;
            i = p;
        }
        return true;
    }
    Flight* peekMin() { cleanTop(); return size > 0 ? items[0].f : nullptr; }
    Flight* extractMin() {
        cleanTop();
        if (size == 0) return nullptr;
        Flight* f = items[0].f;
        popTop();    // remove root and fix heap
        return f;
    }
    bool isEmptyHeap() { cleanTop(); return size == 0; }

    void displayHeap() {     // prints in priority order (works on a copy)
        MinHeap copy = *this;
        Flight* last = nullptr;
        bool any = false;
        while (true) {
            Flight* f = copy.extractMin();  // get next highest priority flight
            if (f == nullptr) break;
            if (f == last) continue;     // skip duplicates
            last = f;
            any = true;
            cout << "    [" << priorityName(f->priorityLevel) << "] " << f->callsign
                << " -> " << f->assignedRunway << " (fuel " << f->fuelRemaining
                << ", deadline tick " << f->deadline << ")\n";
        }
        if (!any) cout << "    (empty)\n";
    }
};

class EmergencyLanding {
public:
    FlightQueue normalQ;
    MinHeap heap;

    void addFlight(Flight* f) {
        if (f->priorityLevel == 9) normalQ.enqueueNormal(f);
        else heap.insertEmergency(f);
    }
    // rule from the manual: normal queue first, otherwise the heap
    Flight* peekNext() {
        normalQ.cleanFront();
        if (!normalQ.isEmptyNormal()) return normalQ.frontNormal();
        return heap.peekMin();
    }
    Flight* takeNext() {
        normalQ.cleanFront();   // remove invalid flights 
        if (!normalQ.isEmptyNormal()) return normalQ.dequeueNormal();
        return heap.extractMin();
    }
};
