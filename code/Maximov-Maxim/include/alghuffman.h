// Copyright 2014 Maxim Maximov

#ifndef CODE_MAXIMOV_MAXIM_INCLUDE_ALGHUFFMAN_H_
#define CODE_MAXIMOV_MAXIM_INCLUDE_ALGHUFFMAN_H_
#include <string>
#include <map>
#include <vector>
#include <list>

#pragma pack(push, 1)
class Node {
    friend class HuffmanAlgorithm;
    friend void BuildTable(Node *root,
    std::map<char, std::vector<bool> > *table, std::vector<bool> *key);
 private:
    Node *_left, *_right;
    void copyTree(const Node* copy);
 public:
    Node();
    Node(Node *left, Node *right);
    ~Node();
    explicit Node(Node const& copy);
    Node& operator=(const Node& a);
    bool operator<(const Node& a) const;
    int _value;
    char _c;
};
#pragma pack(pop)

class HuffmanAlgorithm {
 private:
    std::list<Node> _nodelist;
 public:
    HuffmanAlgorithm();
    ~HuffmanAlgorithm();
    explicit HuffmanAlgorithm(HuffmanAlgorithm const& copy);
    HuffmanAlgorithm& operator=(const HuffmanAlgorithm& a);
    std::string code(std::string source);
    std::string decode(std::string source);
    std::string getFrequencies();
    void setFrequencies(std::string freqString);
};

#endif  // CODE_MAXIMOV_MAXIM_INCLUDE_ALGHUFFMAN_H_
