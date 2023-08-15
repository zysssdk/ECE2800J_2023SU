//  @author zysssdk[odysseyzhou@126.com]
#include "binaryTree.h"
#include "huffmanTree.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

char code_to_char(const string &temp, Node* root) {
    //Find the char according to the path given by temp.
    Node* node_temp = root;
    for(unsigned int i = 0; i < temp.size(); i++) {
        if(temp[i] == '0') {
            node_temp = node_temp->leftSubtree();
        }
        else {
            node_temp = node_temp->rightSubtree();
        }
    }
    return node_temp->getstr()[0];
}

int main(int argc, char *argv[]) {
    string treefile_name = argv[1];
    string compressedfile_name = argv[2];
    //Constructs a huffman tree from the treefile.
    HuffmanTree HT(treefile_name);
    //Decode the compressed file.
    ifstream fin;
    string temp;
    fin.open(compressedfile_name);
    while(fin >> temp) {
        cout << code_to_char(temp, HT.root);
    }
    fin.close();
    return 0;
}