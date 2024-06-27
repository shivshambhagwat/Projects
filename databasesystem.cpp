#include <iostream>
#include <algorithm>
using namespace std;

// Node structure for the Main Tree
struct MainTreeNode {
    int employeeID;
    MainTreeNode* left;
    MainTreeNode* right;

    MainTreeNode(int id) : employeeID(id), left(nullptr), right(nullptr) {}
};

// Node structure for the AVL Tree
struct AVLTreeNode {
    int employeeID;
    MainTreeNode* mainTreeAddress;
    int height;
    AVLTreeNode* left;
    AVLTreeNode* right;

    AVLTreeNode(int id, MainTreeNode* addr) 
        : employeeID(id), mainTreeAddress(addr), height(1), left(nullptr), right(nullptr) {}
};

// Utility function to get height of AVL tree
int height(AVLTreeNode* node) {
    return node ? node->height : 0;
}

// Utility function to get balance factor of AVL tree node
int getBalance(AVLTreeNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

// Right rotate utility function
AVLTreeNode* rightRotate(AVLTreeNode* y) {
    AVLTreeNode* x = y->left;
    AVLTreeNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Left rotate utility function
AVLTreeNode* leftRotate(AVLTreeNode* x) {
    AVLTreeNode* y = x->right;
    AVLTreeNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Insert function for AVL Tree
AVLTreeNode* insertAVL(AVLTreeNode* node, int id, MainTreeNode* addr) {
    if (!node)
        return new AVLTreeNode(id, addr);

    if (id < node->employeeID)
        node->left = insertAVL(node->left, id, addr);
    else if (id > node->employeeID)
        node->right = insertAVL(node->right, id, addr);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && id < node->left->employeeID)
        return rightRotate(node);

    if (balance < -1 && id > node->right->employeeID)
        return leftRotate(node);

    if (balance > 1 && id > node->left->employeeID) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && id < node->right->employeeID) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Minimum value node in AVL tree
AVLTreeNode* minValueNode(AVLTreeNode* node) {
    AVLTreeNode* current = node;
    while (current->left)
        current = current->left;
    return current;
}

// Delete function for AVL Tree
AVLTreeNode* deleteAVL(AVLTreeNode* root, int id) {
    if (!root)
        return root;

    if (id < root->employeeID)
        root->left = deleteAVL(root->left, id);
    else if (id > root->employeeID)
        root->right = deleteAVL(root->right, id);
    else {
        if (!root->left || !root->right) {
            AVLTreeNode* temp = root->left ? root->left : root->right;

            if (!temp) {
                temp = root;
                root = nullptr;
            } else
                *root = *temp;

            delete temp;
        } else {
            AVLTreeNode* temp = minValueNode(root->right);

            root->employeeID = temp->employeeID;
            root->mainTreeAddress = temp->mainTreeAddress;
            root->right = deleteAVL(root->right, temp->employeeID);
        }
    }

    if (!root)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Insert function for Main Tree
MainTreeNode* insertMain(MainTreeNode* node, int id, AVLTreeNode*& avlRoot) {
    if (!node)
        return new MainTreeNode(id);

    if (id < node->employeeID)
        node->left = insertMain(node->left, id, avlRoot);
    else if (id > node->employeeID)
        node->right = insertMain(node->right, id, avlRoot);

    avlRoot = insertAVL(avlRoot, id, node);

    return node;
}

// Delete function for Main Tree
MainTreeNode* deleteMain(MainTreeNode* root, int id, AVLTreeNode*& avlRoot) {
    if (!root)
        return root;

    if (id < root->employeeID)
        root->left = deleteMain(root->left, id, avlRoot);
    else if (id > root->employeeID)
        root->right = deleteMain(root->right, id, avlRoot);
    else {
        if (!root->left) {
            MainTreeNode* temp = root->right;
            delete root;
            avlRoot = deleteAVL(avlRoot, id);
            return temp;
        } else if (!root->right) {
            MainTreeNode* temp = root->left;
            delete root;
            avlRoot = deleteAVL(avlRoot, id);
            return temp;
        }

        MainTreeNode* temp = root->right;
        while (temp && temp->left)
            temp = temp->left;

        root->employeeID = temp->employeeID;
        root->right = deleteMain(root->right, temp->employeeID, avlRoot);
    }

    avlRoot = deleteAVL(avlRoot, id);

    return root;
}

// Utility function to print Main Tree
void inOrderMain(MainTreeNode* root) {
    if (root) {
        inOrderMain(root->left);
        cout << root->employeeID << " ";
        inOrderMain(root->right);
    }
}

// Utility function to print AVL Tree
void inOrderAVL(AVLTreeNode* root) {
    if (root) {
        inOrderAVL(root->left);
        cout << root->employeeID << " ";
        inOrderAVL(root->right);
    }
}

int main() {
    MainTreeNode* mainRoot = nullptr;
    AVLTreeNode* avlRoot = nullptr;

    mainRoot = insertMain(mainRoot, 10, avlRoot);
    mainRoot = insertMain(mainRoot, 20, avlRoot);
    mainRoot = insertMain(mainRoot, 30, avlRoot);
    mainRoot = insertMain(mainRoot, 40, avlRoot);
    mainRoot = insertMain(mainRoot, 50, avlRoot);
    mainRoot = insertMain(mainRoot, 25, avlRoot);

    cout << "Main Tree in-order traversal: ";
    inOrderMain(mainRoot);
    cout << "\n";

    cout << "AVL Tree in-order traversal: ";
    inOrderAVL(avlRoot);
    cout << "\n";

    mainRoot = deleteMain(mainRoot, 10, avlRoot);

    cout << "Main Tree in-order traversal after deletion: ";
    inOrderMain(mainRoot);
    cout << "\n";

    cout << "AVL Tree in-order traversal after deletion: ";
    inOrderAVL(avlRoot);
    cout << "\n";

    return 0;
}
