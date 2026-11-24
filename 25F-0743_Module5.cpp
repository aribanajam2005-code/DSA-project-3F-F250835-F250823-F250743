
//  MODULE 5 : Class: AVLflight

class AVLflight {
    TreeNode* root;

    int hgt(TreeNode* n) { return n ? n->h : 0; } // return height , 0 for NULL
    void update(TreeNode* n) {
        int l = hgt(n->left), r = hgt(n->right);
        n->h = 1 + (l > r ? l : r);    // calculate balance factor
    }
    int balanceOf(TreeNode* n) { return n ? hgt(n->left) - hgt(n->right) : 0; }

    TreeNode* rotateRight(TreeNode* y) {
        TreeNode* x = y->left;   // left child becomes new root
        y->left = x->right;
        x->right = y;   
        update(y); update(x);
        return x;
    }
    TreeNode* rotateLeft(TreeNode* x) {
        TreeNode* y = x->right;   // right child becomes new root
        x->right = y->left;
        y->left = x;
        update(x); update(y);
        return y;
    }
    TreeNode* rotateLeftRight(TreeNode* n) { n->left = rotateLeft(n->left); return rotateRight(n); }
    TreeNode* rotateRightLeft(TreeNode* n) { n->right = rotateRight(n->right); return rotateLeft(n); }

    TreeNode* balance(TreeNode* n) {
        update(n);
        int b = balanceOf(n);
        if (b > 1) {  // left subtree is heavier 
            if (balanceOf(n->left) >= 0) return rotateRight(n);  
            return rotateLeftRight(n);
        } 
        if (b < -1) {   // right subtree is heavier
            if (balanceOf(n->right) <= 0) return rotateLeft(n);
            return rotateRightLeft(n);
        }
        return n; // already balanced 
    }
    TreeNode* insertNode(TreeNode* n, Flight* f) {
        if (n == nullptr) {
            TreeNode* t = new TreeNode;
            t->f = f; t->left = t->right = nullptr; 
            t->h = 1; // height of new node : 1
            return t;
        }
        if (f->callsign < n->f->callsign) n->left = insertNode(n->left, f);
        else n->right = insertNode(n->right, f);
        return balance(n);   // restore AVL balance
    }
    TreeNode* removeNode(TreeNode* n, const string& key) {
        if (n == nullptr) return nullptr;
        if (key < n->f->callsign) n->left = removeNode(n->left, key);
        else if (key > n->f->callsign) n->right = removeNode(n->right, key);
        else {
            if (n->left == nullptr || n->right == nullptr) {
                TreeNode* child = n->left ? n->left : n->right;  // get existing child
                delete n;
                return child;
            }
            TreeNode* s = n->right;
            while (s->left) s = s->left;
            n->f = s->f;    // replace flight with successor's flight
            n->right = removeNode(n->right, s->f->callsign);
        }
        return balance(n);
    }
    void inorder(TreeNode* n) {
        if (n == nullptr) return;
        inorder(n->left);
        cout << "    " << n->f->callsign << "\n";
        inorder(n->right);
    }
    void destroy(TreeNode* n) {
        if (n == nullptr) return;
        destroy(n->left); destroy(n->right);
        delete n;  // delete current node
    }
public:
    AVLflight() { root = nullptr; }
    ~AVLflight() { destroyTree(); }
    void insert(Flight* f) { root = insertNode(root, f); }  // update root after insertion
    void remove(const string& callsign) { root = removeNode(root, callsign); }
    Flight* search(const string& callsign) {
        TreeNode* n = root;
        while (n) {
            if (callsign == n->f->callsign) return n->f; // flight found 
            n = (callsign < n->f->callsign) ? n->left : n->right;
        }
        return nullptr;  // flight not found 
    }
    void inorderTraversal() { inorder(root); }  // display flights in sorted order
    int height() { return hgt(root); }
    void destroyTree() { destroy(root);  
    root = nullptr; }  // reset root 
};
