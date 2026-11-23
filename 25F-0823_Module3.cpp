#include iostream
#include string
using namespace std;

struct Flight; 

struct Snapshot {
    Flight flight;
    string runway, sector;
    int priority, status, deadline, travelLeft;
    int credDelta;
    int cyberIndex;
    Snapshot() {
        flight = nullptr; priority = 9; status = 0; deadline = 0;
        travelLeft = 0; credDelta = 0; cyberIndex = -1;
    }
};

struct LogNode {
    string action, callsign;
    int tick;
    LogNode prev;
    LogNode next;
};

struct StackNode {
    Snapshot snap;
    StackNode next;
};

class CommandHistory {
    LogNode head;
    LogNode tail;
    StackNode top;
public
    CommandHistory() { head = tail = nullptr; top = nullptr; }
    ~CommandHistory() {
        while (head) { LogNode n = head-next; delete head; head = n; }
        while (top) { StackNode n = top-next; delete top; top = n; }
    }

    void logCommand(const string& action, const string& callsign, int tick) {
        LogNode n = new LogNode;
        n-action = action; n-callsign = callsign; n-tick = tick;
        n-prev = tail; n-next = nullptr;
        if (tail) tail-next = n; else head = n;
        tail = n;
    }

    void pushSnapshot(const Snapshot& s) {
        StackNode n = new StackNode;
        n-snap = s; n-next = top; top = n;
    }

    bool undo(Snapshot& out) {
        if (top == nullptr) return false;
        StackNode n = top;
        out = n-snap;
        top = n-next;
        delete n;
        return true;
    }

    void displayHistory(int count) {
        LogNode cur = tail;
        int shown = 0;
        if (cur == nullptr) cout    (no commands yet)n;
        while (cur && shown  count) {
            cout    Tick   cur-tick     cur-action;
            if (cur-callsign != ) cout   [  cur-callsign  ];
            cout  n;
            cur = cur-prev; shown++;
        }
    }

    void displayLastCommand() {
        if (tail) cout    Last command   tail-action   [  tail-callsign  ]n;
        else cout    No commands yet.n;
    }
};