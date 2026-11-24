
//  MODULE 4 : Class: BSTflight
struct TreeNode {  /// node for tree
    Flight* f;     // flight object address
    TreeNode* left;
    TreeNode* right;
    int h;               // height 
};

class BSTflight {
    TreeNode* root;

    TreeNode* insertNode(TreeNode* n, Flight* f) {  // insert 
        if (n == nullptr) {
            TreeNode* t = new TreeNode;  // new node in empty place
            t->f = f;
            t->left = t->right = nullptr;
            t->h = 1;
            return t;
        }
        if (f->callsign < n->f->callsign)  // new flight callsign is smaller 
            n->left = insertNode(n->left, f);
        else
            n->right = insertNode(n->right, f);
        return n;
    }
    TreeNode* removeNode(TreeNode* n, const string& key) {
        if (n == nullptr)
            return nullptr;
        if (key < n->f->callsign)
            n->left = removeNode(n->left, key);
        else if
            (key > n->f->callsign)
            n->right = removeNode(n->right, key);
        else {
            if (n->left == nullptr) {   // if leaf or no left child
                TreeNode* r = n->right;
                delete n;
                return r;
            }
            if (n->right == nullptr) {  // no right child
                TreeNode* l = n->left;
                delete n;
                return l;
            }
            TreeNode* temp = n->right; // both child
            while (temp->left)
                temp = temp->left;           // small in righte
            n->f = temp->f;
            n->right = removeNode(n->right, temp->f->callsign);
        }
        return n;
    }
    int heightOf(TreeNode* n) {  // height
        if (n == nullptr)
            return 0;
        int l = heightOf(n->left), r = heightOf(n->right);
        return 1 + (l > r ? l : r);
    }
    void inorder(TreeNode* n) {  // inorder triversal
        if (n == nullptr) return;
        inorder(n->left);
        cout << "    " << n->f->callsign << "  " << n->f->aircraftType << "  -> " << n->f->assignedRunway << "\n";
        inorder(n->right);
    }
    void destroy(TreeNode* n) { // destroy
        if (n == nullptr) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }
public:
    BSTflight() {  // consrtructor
        root = nullptr;
    }
    ~BSTflight() {   // destrctor
        destroyTree();
    }
    void insert(Flight* f) {// insert
        root = insertNode(root, f);
    }
    void remove(const string& callsign) { // remove a node

        root = removeNode(root, callsign);
    }
    Flight* search(const string& callsign) {// search
        TreeNode* n = root;
        while (n) {
            if (callsign == n->f->callsign)
                return n->f;
            n = (callsign < n->f->callsign) ? n->left : n->right;
        }
        return nullptr;
    }
    void inorderTraversal() {  // inorder
        if (!root) cout << "    (empty)\n";
        inorder(root);
    }
    int height() {  // height
        return heightOf(root);
    }
    void destroyTree() {  // destroy tree
        destroy(root);
        root = nullptr;
    }
};
