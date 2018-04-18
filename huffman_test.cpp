//
// huffman_test.cpp
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "huffman.h"

using namespace std;

// forward declaration of helpers
map<char, int> monaMap();
map<char, string> monaEncTbl();
freq_info* monaTree();
string monaEncoded();
string moonmanEncoded();
void debugTree(freq_info* n, string spaces);
void fullCircuitFile(string fn, string msg, int expected_size_encoded);

// Unit Tests

TEST_CASE("Huffman: sanity check", "[sanity]") {
  REQUIRE(true);
}

TEST_CASE("Huffman: create leaf frequency node", "[create leaf]") {
  freq_info* n = create_leaf('e', 99);
  REQUIRE(n != NULL);
  REQUIRE(n->symbol == 'e');
  REQUIRE(n->count == 99);
  REQUIRE(n->left == NULL);
  REQUIRE(n->right == NULL);
  REQUIRE(n->is_leaf);
}

TEST_CASE("Huffman: create internal frequency node", "[create internal]") {
  freq_info l = freq_info { 'e', 10, NULL, NULL, true };
  freq_info r = freq_info { 'x', 2, NULL, NULL, true };
  freq_info* comb = combine_nodes(&l, &r);
  REQUIRE(comb != NULL);
  REQUIRE(comb->count == 12);
  REQUIRE(comb->left == &l);
  REQUIRE(comb->right == &r);
  REQUIRE(comb->is_leaf == false);
}

TEST_CASE("Huffman: count symbols", "[count symbols]") {
  // simulate a file with a stringstream, put text in it
  std::stringstream fake_file;
  fake_file << "fake file text fabulous fred";

  map<char, int> lookup;
  char ch;
  while (fake_file >> noskipws >> ch) {
    increment_lookup(lookup, ch);
  }
  REQUIRE(lookup['f'] == 4);
  REQUIRE(lookup['s'] == 1);
  REQUIRE(lookup['!'] == 0);
  REQUIRE(lookup['a'] == 2);
}

TEST_CASE("Huffman: load queue", "[load queue]") {
  map<char, int> lookup = monaMap();
  tree_queue q;
  load_queue(lookup, q);

  // q should now have the same number of elements as the lookup
  // table.
  REQUIRE(q.size() == lookup.size());

  // the items in the queue should be in the order: n, o, a, m
  freq_info* n;
  n = q.top(); q.pop();
  REQUIRE(n != NULL);
  REQUIRE(n->symbol == 'n');
  REQUIRE(n->count == 1);

  n = q.top(); q.pop();
  REQUIRE(n != NULL);
  REQUIRE(n->symbol == 'o');
  REQUIRE(n->count == 8);

  n = q.top(); q.pop();
  REQUIRE(n != NULL);
  REQUIRE(n->symbol == 'a');
  REQUIRE(n->count == 10);

  n = q.top(); q.pop();
  REQUIRE(n != NULL);
  REQUIRE(n->symbol == 'm');
  REQUIRE(n->count == 15);
}

TEST_CASE("Huffman: build tree", "[build tree]") {
  // requires load_queue to work
  map<char, int> lookup = monaMap();
  tree_queue q;
  load_queue(lookup, q);
  freq_info* r = build_tree(q);
  REQUIRE(r != NULL);
  REQUIRE(r->count == (1 + 8 + 10 + 15));
  REQUIRE(r->left != NULL);
  REQUIRE(r->left->count == 15);
  REQUIRE(r->left->symbol == 'm');
  REQUIRE(r->right != NULL);
  REQUIRE(r->right->count == 19);
  REQUIRE(r->right->left != NULL);
  REQUIRE(r->right->left->count == 9);
  REQUIRE(r->right->right != NULL);
  REQUIRE(r->right->right->symbol == 'a');
  REQUIRE(r->right->left->left != NULL);
  REQUIRE(r->right->left->left->symbol == 'n');
  REQUIRE(r->right->left->right != NULL);
  REQUIRE(r->right->left->right->symbol == 'o');
}

TEST_CASE("Huffman: build encoding table", "[encode table]") {
  freq_info* r = monaTree();
  
  map<char, string> enc_tbl = build_encoding_table(r);
  map<char, string> key = monaEncTbl();
  REQUIRE(key['m'] == enc_tbl['m']);
  REQUIRE(key['a'] == enc_tbl['a']);
  REQUIRE(key['o'] == enc_tbl['o']);
  REQUIRE(key['n'] == enc_tbl['n']);
}

TEST_CASE("Huffman: encode string", "[encode]") {
  map<char, string> tbl = monaEncTbl();
  string orig = "mona"; 
  string encoded = encode(tbl, orig);
  string expected = monaEncoded();
  REQUIRE(encoded == expected);

  orig = "moonman";
  encoded = encode(tbl, orig);
  expected = moonmanEncoded();
  REQUIRE(encoded == expected);
}

TEST_CASE("Huffman: decode string", "[decode]") {
  freq_info* r = monaTree();
  
  string encoded = monaEncoded();
  string decoded = decode(r, encoded);
  REQUIRE(decoded == "mona");

  encoded = moonmanEncoded();
  decoded = decode(r, encoded);
  REQUIRE(decoded == "moonman");
}

TEST_CASE("Huffman: full circuit micro file", "[full]") {
  fullCircuitFile("micro.txt", "tax does good, all to lists!!", 115);
  fullCircuitFile("small.txt", "a minute phrase", 67);
  fullCircuitFile("pkd.txt", "These pretzels are making me thirsty", 166);
}

// Helper functions down here. Take a look.

map<char, int> monaMap() {
  map<char, int> lookup;
  lookup['m'] = 15;
  lookup['n'] = 1;
  lookup['o'] = 8;
  lookup['a'] = 10;
  return lookup;
}

map<char, string> monaEncTbl() {
  //    [root]
  //   m    [19]
  //      [9]   a
  //     n   o
  map<char, string> tbl;
  tbl['n'] = RIGHT_CHAR + LEFT_CHAR + LEFT_CHAR;
  tbl['m'] = LEFT_CHAR;
  tbl['a'] = RIGHT_CHAR + RIGHT_CHAR;
  tbl['o'] = RIGHT_CHAR + LEFT_CHAR + RIGHT_CHAR;
  return tbl;
}

freq_info* monaTree() {
  freq_info* nodeM  = create_leaf('m', 15);
  freq_info* nodeA  = create_leaf('a', 10);
  freq_info* nodeO  = create_leaf('o',  8);
  freq_info* nodeN  = create_leaf('n',  1);
  freq_info* node9  = combine_nodes(nodeN, nodeO);
  freq_info* node19 = combine_nodes(node9, nodeA);
  freq_info* root   = combine_nodes(nodeM, node19);
  return root;
}

string monaEncoded() {
  return LEFT_CHAR + RIGHT_CHAR + LEFT_CHAR + RIGHT_CHAR + RIGHT_CHAR + LEFT_CHAR + LEFT_CHAR + RIGHT_CHAR + RIGHT_CHAR;
}

string moonmanEncoded() {
  return LEFT_CHAR + RIGHT_CHAR + LEFT_CHAR + RIGHT_CHAR + RIGHT_CHAR + LEFT_CHAR + RIGHT_CHAR + RIGHT_CHAR + LEFT_CHAR + LEFT_CHAR + LEFT_CHAR + RIGHT_CHAR + RIGHT_CHAR + RIGHT_CHAR + LEFT_CHAR + LEFT_CHAR;
}

void debugTree(freq_info* n, string spaces) {
  if (n->is_leaf) {
    cout << spaces << "sym: " << n->symbol << ", count: " << n->count << endl;
  } else {
    cout << spaces << "internal count: " << n->count << endl;
    debugTree(n->left, spaces + "  ");
    debugTree(n->right, spaces + "  ");
  }
}

void fullCircuitFile(string fn, string msg, int expected_size_encoded) {
  // open a file for reading
  fstream fin(fn, fstream::in);

  // create a table that maps symbols with counts
  map<char, int> lookup;
  char ch;
  while (fin >> noskipws >> ch) {
    increment_lookup(lookup, ch);
  }

  // create a priority queue based on this table
  tree_queue q;
  load_queue(lookup, q);

  // create a Huffman codec tree using the priority queue
  freq_info* hufftree = build_tree(q);
  // if you're hung up on this step, consider uncommenting these debugging lines:
  // 
  // cout << "root node has total symbol count: " << hufftree->count << endl;
  // debugTree(hufftree, "");

  // create an encoding table based on the Huffman codec tree
  map<char, string> enc_table = build_encoding_table(hufftree);


  // encode the text 'tax does good, all to lists!!'
  string encoded = encode(enc_table, msg);

  cout << "Using corpus from '" << fn << "'" << endl;
  cout << "Original: " << msg << endl;
  cout << "Encoded:  " << encoded << endl;
    
  REQUIRE(encoded.size() == expected_size_encoded);

  // finally decode the encoded string, should get the original back.
  string decoded = decode(hufftree, encoded);
  cout << "Decoded:  " << decoded << endl;
  REQUIRE(decoded == msg);

  float uncompressed_size = (float) msg.size() * 8.0; // mult by 8 because we need to compare bit-length
  float compressed_size = (float) encoded.size();
  cout << "Compression ratio (higher is better): " << (uncompressed_size / compressed_size) << endl;

  cout << endl; // blank line for legibilityness.
}
