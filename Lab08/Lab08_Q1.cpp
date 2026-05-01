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

    // postorder traversal
    void postorderTraversal(TreeNode* node) {
        if (node == nullptr) return; // stop at null node

        postorderTraversal(node->left);   // first left subtree
        postorderTraversal(node->right);  // then right subtree
        cout << node->value << " ";       // last the node itself
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

    // output: postorder traversal
    cout << "Postorder Traversal: ";
    tree.postorderTraversal(tree.root);

    return 0;
}
