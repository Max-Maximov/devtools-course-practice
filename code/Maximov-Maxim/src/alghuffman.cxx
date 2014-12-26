// Copyright 2014 Maxim Maximov
#include "include/alghuffman.h"
#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include <list>

using std::map;
using std::vector;
using std::list;

Node::Node(Node *left, Node *right)
    : _left(left),
      _right(right),
      _value(_left->_value + _right->_value),
      _c('\0') {
}

Node::Node()
    : _left(NULL),
      _right(NULL),
      _value(0),
      _c('\0') {
}

Node::~Node() {
    if (_left != NULL) delete _left;
    if (_right != NULL) delete _right;
}

Node::Node(Node const& copy)
    : _left(NULL),
      _right(NULL),
      _value(copy._value),
      _c(copy._c) {
        this->copyTree(&copy);
    }

Node& Node::operator=(const Node& a) {
    this->copyTree(&a);
    return *this;
    }

bool Node::operator<(const Node& a) const {
    return this->_value < a._value;
    }

void Node::copyTree(const Node* tempRoot) {
    _value = tempRoot->_value;
    _c = tempRoot->_c;
    if (tempRoot->_right != NULL) {
        _right = new Node();
        _right->copyTree(tempRoot->_right);
    }
    if (tempRoot->_left != NULL) {
        _left = new Node();
        _left->copyTree(tempRoot->_left);
    }
}


void BuildTable(Node *root, map<char, vector<bool> > *table,
    vector<bool> *key);
void BuildTable(Node *root, map<char, vector<bool> > *table,
    vector<bool> *key) {
    if (root->_left != NULL) {
        key->push_back(0);
        BuildTable(root->_left, table, key);
    }

    if (root->_right != NULL) {
        key->push_back(1);
        BuildTable(root->_right, table, key);
    }

    if (root->_c != '\0') (*table)[root->_c] = *key;

    key->pop_back();
}


HuffmanAlgorithm::HuffmanAlgorithm():_nodelist(0) {}
HuffmanAlgorithm::~HuffmanAlgorithm() {}
HuffmanAlgorithm& HuffmanAlgorithm::operator=(const HuffmanAlgorithm& a) {
    _nodelist = a._nodelist;
    return *this;
    }

HuffmanAlgorithm::HuffmanAlgorithm(HuffmanAlgorithm const& copy)
  : _nodelist(copy._nodelist) {}

std::string HuffmanAlgorithm::code(std::string source) {
    map<char, int> charfreq;
    for (unsigned int i = 0; i < source.length(); i++) {
        charfreq[source[i]]++;
    }

    // Make list of nodes

    _nodelist.clear();
    list<Node> tempNodelist;

    for (map<char, int>::iterator itr = charfreq.begin(); itr != charfreq.end();
        ++itr) {
        Node p;
        p._c = itr->first;
        p._value = itr->second;
        _nodelist.push_back(p);
    }

    tempNodelist = _nodelist;

    // Build tree

    Node* root;
    if (tempNodelist.size() == 1) {
        root = new Node();
        root->_left = new Node(tempNodelist.front());
    } else {
        while (tempNodelist.size() != 1) {
            tempNodelist.sort();
            Node *sonL = new Node(tempNodelist.front());
            tempNodelist.pop_front();
            Node *sonR = new Node(tempNodelist.front());
            tempNodelist.pop_front();
            Node parent(sonL, sonR);
            tempNodelist.push_back(parent);
        }
        root = new Node(tempNodelist.front());
    }

    // Build table with pairs "character - code"

    map<char, vector<bool> > table;
    vector<bool> key;

    BuildTable(root, &table, &key);
    delete root;

    // Build final string

    std::string result;
    for (unsigned int i = 0; i < source.length(); i++) {
        vector<bool> x = table[source[i]];
        for (unsigned int n = 0; n < x.size(); n++) {
            result += (x[n]) ? "1" : "0";
        }
    }

    return(result);
}

std::string HuffmanAlgorithm::decode(std::string source) {
    Node* root;
    list<Node> tempNodelist(_nodelist);
    if (tempNodelist.size() == 0) {
            throw std::string("Have not coded yet");
    } else {
        if (tempNodelist.size() == 1) {
            root = new Node();
            root->_left = new Node(tempNodelist.front());
        } else {
            while (tempNodelist.size() != 1) {
                tempNodelist.sort();
                Node *sonL = new Node(tempNodelist.front());
                tempNodelist.pop_front();
                Node *sonR = new Node(tempNodelist.front());
                tempNodelist.pop_front();
                Node parent(sonL, sonR);
                tempNodelist.push_back(parent);
            }
        root = new Node(tempNodelist.front());
        }
    }

    Node* p = root;
    std::string result;
    for (unsigned int i = 0; i < source.length(); i++) {
        if (source[i] != '1' && source[i] != '0')
            throw std::string("Wrong string");
        if (source[i] == '1')
            p = p->_right;
        else
            p = p->_left;
        if (p->_left == NULL && p->_right == NULL) {
            result += p->_c;
            p = root;
        }
    }
    delete root;
    return (result);
}

std::string HuffmanAlgorithm::getFrequencies() {
    std::string result = "$";
    for (list<Node>::iterator itr = _nodelist.begin(); itr != _nodelist.end();
        ++itr) {
        result += itr->_c + std::to_string(itr->_value) + "$";
    }
    result += "~";
    return(result);
}

void HuffmanAlgorithm::setFrequencies(std::string freqString) {
    list<Node> tempnodelist;
    unsigned int itr = 1;
    while (itr < freqString.length() - 1) {
        Node newNode;
        newNode._c = freqString[itr];
        itr++;
        std::string valuebuf = "";
        while (freqString[itr] != '$') {
            valuebuf += freqString[itr];
            itr++;
            if (itr == freqString.length())
                throw std::string("Wrong string of frequencies");
        }
        newNode._value = atoi(valuebuf.c_str());
        if (newNode._value == 0)
            throw std::string("Wrong string of frequencies");
        tempnodelist.push_back(newNode);
        itr++;
    }
    _nodelist = tempnodelist;
}
