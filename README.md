# Huffman Codes

We often work with storing or transporting data. If we can compress
this data in a lossless way, we save money and time on disk space and
network traffic. _Huffman Encoding_ is a neat process where we reduce
the number of bits required to store individual components from some
alphabet, when the frequency those components appear is not evenly
distributed.

In English, there are common letters (e, m, s, t) and less common
letters (x, z, q). Huffman codes exploit this by letting us use short
bit patterns for common letters and longer ones for uncommon letters.

The examples you'll read about unanimously talk about encoding written
human languages, but it could just as easily be applied to _any_
system of communication where there is some alphabet of symbols (such
as the instructions used in a program).

The Wikipedia page on Huffman Encoding is good. So is
[this thing by Alex Allain](http://www.cprogramming.com/tutorial/computersciencetheory/huffman.html). There's
really no shortage of good resources.


## A Note About Bits

Rather that encode strings as proper bits (low-level zero and one),
we're going to fake it using the strings "." and "^". This is mostly
because going all low-level requires a bit of explaination that is
really besides the point, and would just needlessly complicate things.

But do be aware that a proper implementation of any compression scheme
would likely go all the way down to a bit-level representation.

## What To Do For This Assignment

The header file documents everything for you to do. There aren't any
extra credit functions this time.

You'll likely need to create your own helper functions. In my solution
I made a recursive one to build the encoding table and just adding
`LEFT_CHAR` or `RIGHT_CHAR` to a string whenever I recursed.

## Tests

The tests are fairly well decoupled so you should be able to implement
your functions in whatever order you want, except for [build tree]
which depends on `load_queue`.

There is a unit test called [full] that you can run manually from the
command line:

```
$ make && ./huffman_test "[full]"
```

If you've done everything correctly you'll see some gratifying output
there, like this:

```
Using corpus from 'pkd.txt'
Original: These pretzels are making me thirsty
Encoded:  ^.^.^^.^^^^.....^^^.^...^^..^.^..^^^^^...^..^^.^^^^^..^....^...^^^^.^^^..^^.^^^^^...^^.^.^.^..^^.^........^^.^..^.^^..^^.^.^.^....^^.^..^^^^....^^^^^^^^^^.^^..^.^.^.^
Decoded:  These pretzels are making me thirsty
Compression ratio (higher is better): 1.73494
```

## Typedef for Priority Queue

There is a line that might look unfamiliar to you:

```
typedef priority_queue<freq_info*, vector<freq_info*>, freq_info> tree_queue;
```

This is called a typedef, and it is used to say "the thing on the left
can be written with the shorthand on the right". So everywhere you see
`tree_queue`, imagine it is expanded with that horrible, awful long
thing, the `priority_queue< ... blah ...>`.

Not only does this improve legibility, it also makes the coding job
easier. Remember Larry Wall's first virtue of great programmers...

## Maps

There are a few function signatures that call for maps. A `map` in C++
is a standard data structure that implements the Map abstract data
type, though (in my opinion) in a really awkward way.

Example usage:

```c++
m['c'] = 0; // map value 0 to key 'c'
cout << "value of c is: " << m['c]] << endl; // // read value associated with 'c'

// does m contain key 'c'?
bool present = m.find('c') != m.end();

// iterate over the key/value pairs. the 'auto' thing is a recent C++ keyword.
// the cbegin and cend are 'constant iterator' values.
for (auto it = m.cbegin(); it != m.cend(); ++it) {
  char k = (*it).first;
  int v =  (*it).second;
  // now do something with key k and value v
}
```

## Streams

It will also help you in a couple places to use string streams.

```c++
// be sure to include the header
#include <sstream>

std::stringstream fake; // make a new string stream for a fake file
fake << "this is fake file data!"; 
char ch; // make a landing spot for reading characters
while (in >> noskipws >> ch) { // read a character into ch, and don't skip whitespace.
  // do something with ch
}
cout << fake.str(); // this is how you get a string from a stringstream
```
