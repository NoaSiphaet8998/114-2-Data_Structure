#include <iostream>
#include <queue>
#include <vector>
using namespace std;

// 樹的節點
class TreeNode {
public:
    int value;             // 節點的值
    TreeNode* left;        // 左子節點
    TreeNode* right;       // 右子節點

    // constructor: initializes node with value and sets children to nullptr
    TreeNode(int val) : value(val), left(nullptr), right(nullptr) {}
};

// 樹結構
class BinaryTree {
public:
    TreeNode* root;

    // constructor: initialize tree with empty root
    BinaryTree() : root(nullptr) {}

    // 用陣列構建二元樹
    TreeNode* buildTree(vector<int>& arr) {
        if (arr.empty()) return nullptr;

        queue<TreeNode*> q; // 儲存待處理的節點
        root = new TreeNode(arr[0]); // 建立根節點 (陣列第一個元素)
        q.push(root); // 將根節點加入queue

        size_t i = 1; // 陣列索引
        while (!q.empty()) {
            TreeNode* current = q.front(); // 取出隊列中的節點
            q.pop();

            // 添加左子節點
            if (i < arr.size()) {
                current->left = new TreeNode(arr[i]);
                q.push(current->left); // 將左子節點加入queue
                i++;
            }
            // 添加右子節點
            if (i < arr.size()) {
                current->right = new TreeNode(arr[i]);
                q.push(current->right); // 將右子節點加入queue
                i++;
            }
        }

        return root;
    }

    // 中序遍歷
    void inorderTraversal(TreeNode* node) {
        if (node == nullptr) return; // 如果節點為空，忽略

        inorderTraversal(node->left);  // 遍歷左子樹
        cout << node->value << " ";    // 訪問當前節點
        inorderTraversal(node->right); // 遍歷右子樹
    }

    // recursive to find max value in a subtree
    int findMax(TreeNode* node) {
        if (node == nullptr) return INT_MIN;
        // if node is null, return smallest int so it won't affect max comparison

        // recursive to find max in left and right subtrees
        int leftMax = findMax(node->left);
        int rightMax = findMax(node->right);

        // return the largest in current node, left subtree, and right subtree
        return max(node->value, max(leftMax, rightMax));
    }

    // print max values in left and right subtrees of root
    void printLeftRightMax() {
        if (root == nullptr) {
            cout << "Tree is empty." << endl;
            return;
        }

        // find max in left subtree
        int leftMax = findMax(root->left);

        // find max in right subtree
        int rightMax = findMax(root->right);

        cout << "Max value in left subtree: " << leftMax << endl;
        cout << "Max value in right subtree: " << rightMax << endl;
    }
};

int main() {
    BinaryTree tree; // 宣告二元樹

    // 輸入陣列用於構建樹，NULL 表示空子節點
    vector<int> arr = { 1, 2, 3, 4, 5, 6, 7 };

    tree.buildTree(arr); // 建立樹

    // 中序遍歷輸出
    cout << "Inorder Traversal: ";
    tree.inorderTraversal(tree.root);
    cout << endl;

    // output: max values in left and right subtrees
    tree.printLeftRightMax();

    return 0;
}

