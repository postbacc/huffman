// huffman.cpp
//

#include "huffman.h"

using namespace std;

freq_info* create_leaf(char symbol, int count) {
  // TODO
  return NULL;
}

freq_info* combine_nodes(freq_info* left, freq_info* right) {
  // TODO
  return NULL;
}

void increment_lookup(map<char, int>& lookup, char symbol) {
  // TODO
}

void load_queue(const map<char, int>& lookup, tree_queue& q) {
  // TODO
}

freq_info* build_tree(tree_queue& q) {
  // TODO
  return NULL;
}

map<char, string> build_encoding_table(freq_info* root) {
  // TODO
  return map<char, string>();
}

string encode(map<char, string> enc_table, string input) {
  // TODO
  return "";
}

string decode(freq_info* root, string input) {
  // TODO
  return "";
}
