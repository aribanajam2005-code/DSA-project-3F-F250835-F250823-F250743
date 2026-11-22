// Мodule 1: Aircraft and Crew Registry
struct Flight {   // store info of one air craf
    string callsign, aircraftType, airline, origin, destination;
    //  Id , model , company , going from , to 
    int passengerCount = 0;             // no. passenger
    int fuelRemaining = 0;            // use tick unit
    string currentSector, assignedRunway;
    // geograph region 
    int arrivalTick = 0;              //
    int priorityLevel = 9;            // low to high (1 to 9)
    int status = 0;                   // 0 waiting, 1 cleared, 2 landed, 3 crashed, 4 intercepted
    int deadline = 0;                 // tick when the emergency landing 
    int travelLeft = 0;               // ticks left before landing 
    string threat;                    // name of threat  
    bool inRegistry = false;          // aircraft is present 
    Flight* next = nullptr;           // next flight in registry
    Flight* retiredNext = nullptr;    // used for the "retired" list (freed at game end)
};
struct Crew {   // people working in craft 
    string employeeID, name, role, assignedCallsign;   // member assign withcraft
    Crew* next;
};
class Registration {
public:
    Flight* head;
    Flight* tail;
    Flight* retired;      // flights that were removed 
    int flightCount;
    Crew* crewHead;
    Crew* crewTail;
    int crewCount;

    Registration() {
        head = tail = retired = nullptr;
        flightCount = 0;
        crewHead = crewTail = nullptr;
        crewCount = 0;
    }

    ~Registration() {
        Flight* f = head;  // delete active flight
        while (f) {
            Flight* n = f->next; 
            delete f; 
            f = n; 
        }
        f = retired;  // delete retired flights 
        while (f) {
            Flight* n = f->retiredNext;
            delete f; 
            f = n; 
        }
        Crew* c = crewHead;  // delete crew info
        while (c) {
            Crew* n = c->next;
            delete c;
            c = n; }
    }

    // insert flight at the tail
    void addFlight(Flight* f) {
        f->next = nullptr;
        f->inRegistry = true;
        if (head == nullptr)
            head = tail = f;
        else {
            tail->next = f;
            tail = f;
        }
        flightCount++;
    }

    // remove flight and kep in retired list
    void removeFlight(Flight* f) {
        Flight* prev = nullptr;
        Flight* cur = head;
        while (cur != nullptr && cur != f) {
            prev = cur;
            cur = cur->next;
        }
        if (cur == nullptr) 
            return;
        if (prev) 
            prev->next = cur->next; 
        else
            head = cur->next;
        if (tail == cur)
            tail = prev;
        cur->next = nullptr;
        cur->inRegistry = false;
        cur->retiredNext = retired;
        retired = cur;
        flightCount--;
    }

    // called every tick
    void updateAllFlights() {
        for (Flight* f = head; f != nullptr; f = f->next) 
            f->fuelRemaining--; // dec fuel for every tick
    }

    void displayAllFlights() {
        if (head == nullptr) {
            cout << "  No active flights.\n";
            return;
        }

        cout << left;
        cout << "  Callsign  Type              Sector        Runway    Fuel  Priority    Status\n";
        cout << "  ---------------------------------------------------------------------------\n";

        for (Flight* f = head; f != nullptr; f = f->next) {
            cout << "  "
                << setw(10) << f->callsign
                << setw(18) << f->aircraftType
                << setw(14) << f->currentSector
                << setw(10) << f->assignedRunway
                << setw(6) << f->fuelRemaining
                << setw(12) << priorityName(f->priorityLevel)
                << statusText(f) << "\n";
        }
    }

    void addCrew(const string& id, const string& name, const string& role, const string& callsign) {
        Crew* c = new Crew;
        c->employeeID = id;
        c->name = name;
        c->role = role; 
        c->assignedCallsign = callsign;
        c->next = nullptr;
        if (crewHead == nullptr)
            crewHead = crewTail = c;
        else { 
            crewTail->next = c; 
            crewTail = c; }
        crewCount++;
    }
    void displayAllCrew() {
        if (crewHead == nullptr) {
            cout << "  No crew records.\n";
            return;
        }
        cout << left;
        cout << "  ID      Name                       Role          Callsign\n";
        cout << "  ------------------------------------------------------------\n";
        for (Crew* c = crewHead; c != nullptr; c = c->next) {
            cout << "  "
                << setw(8) << c->employeeID
                << setw(27) << pretty(c->name)
                << setw(14) << pretty(c->role)
                << c->assignedCallsign << "\n";
        }
    }

    string pilotOf(const string& callsign) {
        for (Crew* c = crewHead; c != nullptr; c = c->next) {
            string role = toLower(c->role);
            if (c->assignedCallsign == callsign &&
                role.length() >= 5 &&
                role[0] == 'p' &&
                role[1] == 'i' &&
                role[2] == 'l' &&
                role[3] == 'o' &&
                role[4] == 't') {
                return pretty(c->name);
            }
        }
        return "-";
    }
};

// Hash table linear probing  find flight
class HashTable {
    Flight* table[HASH_SIZE];
    int state[HASH_SIZE];            // 0 = empty, 1 = used, 2 = deleted 
    int count, collisions, longestProbe;

    int hashFunction(const string& key) {  // convert into array
        int h = 0;
        for (int i = 0; i < (int)key.size(); i++)
            h = (h * 31 + key[i]) % HASH_SIZE;
        return h;
    }
public:
    HashTable() {
        count = collisions = longestProbe = 0;
        for (int i = 0; i < HASH_SIZE; i++) { table[i] = nullptr; state[i] = 0; }
    }
    // insert
    bool insert(const string& callsign, Flight* f) {
        if (count >= HASH_SIZE - 1) 
            return false;
        int idx = hashFunction(callsign);
        int probes = 0;
        while (state[idx] == 1) {          // slot busy next use
            idx = (idx + 1) % HASH_SIZE;
            probes++;
            collisions++;
        }
        table[idx] = f;
        state[idx] = 1;
        count++;
        if (probes > longestProbe) 
            longestProbe = probes;
        return true;
    }
    // find
    Flight* find(const string& callsign) {
        int idx = hashFunction(callsign);
        for (int steps = 0; steps < HASH_SIZE; steps++) {
            if (state[idx] == 0) return nullptr; // not used 
            if (state[idx] == 1 && table[idx]->callsign == callsign) // used and match
                return table[idx];
            idx = (idx + 1) % HASH_SIZE;
        }
        return nullptr;
    }
    // remove 
    void remove(const string& callsign) {
        int idx = hashFunction(callsign);
        for (int steps = 0; steps < HASH_SIZE; steps++) {
            if (state[idx] == 0) return;  // not used 
            if (state[idx] == 1 && table[idx]->callsign == callsign) { // used and match 
                state[idx] = 2;            //marks deleted
                table[idx] = nullptr;
                count--;
                return;
            }
            idx = (idx + 1) % HASH_SIZE;
        }
    }
    // display
    void displayStats() {
        cout << "  Hash table: size " << HASH_SIZE << ", stored " << count
            << ", load factor " << (double)count / HASH_SIZE
            << ", collisions " << collisions
            << ", longest probe chain " << longestProbe << "\n";
    }
};