//Zhou Yishen 521370910174
#include "binaryTree.h"
#include "huffmanTree.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool cmp(const Node* a, const Node* b) {
    //"smaller" node will be put behind.
    if(a->getnum() != b->getnum()) {
        return a->getnum() > b->getnum();
    }
    return a->getstr()[0] > b->getstr()[0];
}

int main(int argc, char *argv[]) {
    string filename;
    //Test which kind of "compress" will be done.
    bool have_tree = false;
    if (argc == 2) {
        filename = argv[1];
    } 
    else {
        if(string(argv[1]) == "-tree") {
            have_tree = true;
        }
        filename = argv[2];
    }
    //Read the input
    //ASCII: '\n' is 10, ' ' is 32, and 'a' to 'z' is 97 to 122.
    ifstream fin;
    fin.open(filename);
    int freq[123] = {0};
    char ch;
    while(fin.get(ch)) {
        freq[(int) ch] = freq[(int) ch] + 1;
    }
    fin.close();
    //Check how many node(s) exist.
    int node_count = 0;
    for(int i = 0; i <= 122; i++) {
        if(freq[i] != 0) {
            node_count = node_count + 1;
        }
    }
    //Build one node for each character and add all nodes to a vector.
    vector<Node*> node_set;
    string temp;
    for(int i = 10; i <= 122; i++) {
        if(freq[i] != 0) {
            temp = (char) i;
            node_set.push_back(new Node(temp, freq[i], nullptr, nullptr));
        }
    }
    //Merge those nodes if there are nore than one nodes.
    while(node_count > 1) {
        sort(node_set.begin(), node_set.end(), cmp);
        node_set[node_count - 2] = node_set[node_count - 2]->mergeNodes(node_set[node_count - 2], node_set[node_count - 1]);
        node_set.pop_back();
        node_count = node_count - 1;
    }
    //Construct a HuffmanTree
    HuffmanTree HT_114514(node_set[0]);
    //Print the result
    if(have_tree) {
        HT_114514.printTree();
    }
    else {
        fin.open(filename);
        string temp;
        while(fin.get(ch)) {
            //Search the path of each ch
            temp = ch;
            cout << HT_114514.findPath(temp) << ' ';
        }
        fin.close();
    }
    return 0;
}