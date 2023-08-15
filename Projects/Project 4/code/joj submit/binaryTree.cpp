//  @author zysssdk[odysseyzhou@126.com]
#include "binaryTree.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

/* ================================== Node =================================== */

Node::Node(const string &str, int num, Node *left, Node *right) {
    this->str = str;
    this->num = num;
    this->left = left;
    this->right = right;
}

Node* Node::leftSubtree() const{
    return this->left;  
}

void Node::setleft(Node *n) {
    this->left = n;
    return;  
}

Node* Node::rightSubtree() const{
    return this->right; 
}

void Node::setright(Node *n) {
    this->right = n;
    return;
}

string Node::getstr() const {
    return str;
}

int Node::getnum() const {
    return num;  
}

void Node::incnum() {
    num = num + 1;
    return;
}

Node *Node::mergeNodes(Node *leftNode, Node *rightNode) {
    return new Node(leftNode->str + rightNode->str ,leftNode->num + rightNode->num, leftNode, rightNode);
}

/* =============================== Binary Tree =============================== */

BinaryTree::BinaryTree(Node *rootNode) {
    this->root = rootNode;
}

void Dstr_helper(Node* root) {
    //Helper function for the destructor
    if(root != nullptr){
        Dstr_helper(root->leftSubtree());
        Dstr_helper(root->rightSubtree());
        delete root;
    }
    return;
}

BinaryTree::~BinaryTree() {
    Dstr_helper(this->root);
}

string findPath_helper(const Node *node, const string &s) {
    if(!node) {
        return "-1";
    }
    if(node->getstr() == s) {
        return "";
    }
    //This node not match
    string answer = findPath_helper(node->leftSubtree(), s);
    if(answer != "-1") {
        return "0" + answer;
    }
    answer = findPath_helper(node->rightSubtree(), s);
    if(answer != "-1") {
        return "1" + answer;
    }
    return "-1";
}

string BinaryTree::findPath(const string &s) const{
    return findPath_helper(this->root, s);
}

int sum_helper(const Node* node) {
    if(!node) {
        return 0;
    }
    return (node->getnum() + sum_helper(node->leftSubtree()) + sum_helper(node->rightSubtree()));
}

int BinaryTree::sum() const {
    return sum_helper(this->root);
}

int depth_helper(const Node* node) {
    if(!node) {
        return 0;
    }
    else {
        //Choose which one is "deeper"
        return 1 + ((depth_helper(node->leftSubtree()) > depth_helper(node->rightSubtree())) ? depth_helper(node->leftSubtree()) : depth_helper(node->rightSubtree()));
    }
}

int BinaryTree::depth() const {
    return depth_helper(this->root);
}

void prenum_helper(const Node* node) {
    if(!node) {
        return;
    }
    cout << node->getnum() << " ";
    prenum_helper(node->leftSubtree());
    prenum_helper(node->rightSubtree());
    return;
}

void BinaryTree::preorder_num() const {
    prenum_helper(this->root);
    cout << endl;
    return;
}

void instr_helper(const Node* node) {
    if(!node) {
        return;
    }
    instr_helper(node->leftSubtree());
    cout << node->getstr() << " ";
    instr_helper(node->rightSubtree());
    return;
}

void BinaryTree::inorder_str() const {
    instr_helper(this->root);
    cout << endl;
    return;
}

void posnum_helper(const Node* node) {
    if(!node) {
        return;
    }
    posnum_helper(node->leftSubtree());
    posnum_helper(node->rightSubtree());
    cout << node->getnum() << " ";
    return;
}

void BinaryTree::postorder_num() const {
    posnum_helper(this->root);
    cout << endl;
    return;
}

bool aPSG_helper(const Node* node, int sum) {
    if(!node) {
        return true;
    }
    else if(!node->leftSubtree() && !node->rightSubtree()) {
        return (node->getnum() > sum);
    }
    else {
        return (aPSG_helper(node->leftSubtree(), sum - node->getnum()) && aPSG_helper(node->rightSubtree(), sum - node->getnum()));
    }
}

bool BinaryTree::allPathSumGreater(int sum) const {
    return aPSG_helper(this->root, sum);
}

bool cov_helper(const Node* BT_a, const Node* BT_b) {
    //First check BT_a, if it's empty, it will always be contained by BT_b!
    if(!BT_a) {
        return true;
    }
    //Then check BT_b, since it has been checked that BT_a is not empty.
    if(!BT_b) {
        return false;
    }
    return ((BT_a->getnum() == BT_b->getnum()) && cov_helper(BT_a->leftSubtree(), BT_b->leftSubtree()) && cov_helper(BT_a->rightSubtree(), BT_b->rightSubtree()));
}

bool BinaryTree::covered_by(const BinaryTree &tree) const {
    return cov_helper(this->root, tree.root);
}

bool cont_helper(const Node* BT_a, const Node* BT_b) {
    if(cov_helper(BT_a, BT_b)) {
        return true;
    }
    //If BT_a is not covered by BT_b, then the following recursion process will be done to check BT_b's two subtrees.
    if(!BT_b) {
        return false;
    }
    return cont_helper(BT_a, BT_b->leftSubtree()) || cont_helper(BT_a, BT_b->rightSubtree());
}

bool BinaryTree::contained_by(const BinaryTree &tree) const {
    return cont_helper(this->root, tree.root);
}

Node* copy_helper(const Node* root) {
    //"empty" case will be checked in BinaryTree::copy().
    //Initialize
    Node* node_copy = new Node(root->getstr(), root->getnum(), nullptr, nullptr);
    Node* left_new = nullptr;
    Node* right_new = nullptr;
    if(root->leftSubtree()) {
        left_new = copy_helper(root->leftSubtree());
    }
    node_copy->setleft(left_new);
    if(root->rightSubtree()) {
        right_new = copy_helper(root->rightSubtree());
    }
    node_copy->setright(right_new);
    return node_copy;
}

BinaryTree BinaryTree::copy() const {
    if(!root) {
        return BinaryTree(nullptr);
    }
    else {
        return BinaryTree(copy_helper(this->root));
    }
}
