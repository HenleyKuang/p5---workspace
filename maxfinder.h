// Author: Sean Davis
#ifndef maxfinderH
#define maxfinderH
#include "NetDriver.h"
#include <iostream>
#include "QuadraticProbing.h"
#include "BinaryHeap.h"

class adjacencyNode
{
  public:
    int index;
    string address;
    adjacencyNode() { }
    adjacencyNode(string addy) : address(addy) { }
    bool operator!= (const adjacencyNode &rhs)const { return address != rhs.address; }
};

struct comp2
{
  public:
    int index;
    char address[16];
    bool adjacentEdgesBackFlow[25];
    int adjenctEdges[25];
    int adjencyCount;
    int dv;
    int pv;
    int edge;
    int min;
    int searchNumber;
    bool backFlow;
    comp2() : dv(-1), pv(-1), edge(-1), min(-1), searchNumber(0) { }
    //bool operator!= (const comp2 &rhs)const { return dv != rhs.dv; }
    //bool operator< (const comp2 &rhs)const { return dv < rhs.dv; }
    //bool operator> (const comp2 &rhs)const { return dv > rhs.dv; }
};

struct edges2
{
  public: 
    int src;
    int dest;
    short reg;
    short back;
};

class MaxFinder
{
const adjacencyNode ITEM_NOT_FOUND;
QuadraticHashTable<adjacencyNode> networkHashTable;
comp2 *c;
edges2* newEdges;

public:
  int _numComputers;
  int _numTerminals;
  string final;
  string terminals[500];
  int maxFlow;

  MaxFinder(Computer *computers, int numComputers, int numTerminals);
  int calcMaxFlow(Edge *edges, int numEdges);
}; // class MaxFinder

#endif
