// Author: Sean Davis
#ifndef maxfinderH
#define maxfinderH
#include "NetDriver.h"
#include "SplayTree.h"
#include<iostream>
#include "QuadraticProbing.h"
#include "QuadraticProbing2.h"
#include <string.h>
#include <string>
#include <stdio.h>
#include <algorithm>
#include "SplayTree.h"

class adjacencyNode
{
  public:
    int index;
    string address;
    adjacencyNode() { /* std::fill_n(adjenctEdges, 1000, 0); std::fill_n(adjacentEdgesBackFlow, 1000, 0); */  }
    adjacencyNode(string addy) : address(addy) { /* std::fill_n(adjenctEdges, 1000, 0);*/ }
    bool operator!= (const adjacencyNode &rhs)const { return address != rhs.address; }
};

struct comp2
{
  public:
    int index;
    char address[16];
    bool adjacentEdgesBackFlow[500];
    int adjenctEdges[500];
    int adjencyCount;
    int dv;
    int pv;
    int edge;
    int min;
    int searchNumber;
    bool backFlow;
    comp2() : dv(-1), pv(-1), min(-1), searchNumber(0) { }
    bool operator!= (const comp2 &rhs)const { return dv != rhs.dv; }
    bool operator< (const comp2 &rhs)const { return dv < rhs.dv; }
    
};

struct edges2
{
  public: 
    int src;
    int dest;
    short reg;
    short back;
};

class Paths
{
  public: 
    int pathEdges[500];
    bool isBackFlow[500];
    bool pathEdgesVisited[15000];
    int edgeCount;
    int minimum;
    Paths() : edgeCount(0), minimum(0) { }
    Paths( const Paths &copy ) : edgeCount(copy.edgeCount), minimum(copy.minimum) 
      { 
        memcpy( &pathEdges, &copy.pathEdges, sizeof(copy.pathEdges)); 
        memcpy( &isBackFlow, &copy.isBackFlow, sizeof(copy.isBackFlow)); 
        memcpy( &pathEdgesVisited, &copy.pathEdgesVisited, sizeof(copy.pathEdgesVisited));
      }
    void insertMinimum(int min) { minimum = ((minimum == 0) || min < minimum ) ? min : minimum; }
    //bool operator!= (const Paths &rhs)const { return minimum != rhs.minimum; }
    //bool operator< (const Paths &rhs)const { return minimum < rhs.minimum; }
};

// class DijkRow
// {
//   public:
//     bool known;
//     int dv;
//     int pv;
//     bool operator!= (const DijkRow &rhs)const { return dv != rhs.dv; }
//     bool operator< (const DijkRow &rhs)const { return dv < rhs.dv; }
// };

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
  void createPath(Paths thePath, int currentCompIndex);
}; // class MaxFinder

#endif
