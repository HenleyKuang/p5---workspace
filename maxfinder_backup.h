// Author: Sean Davis
#ifndef maxfinderH
#define maxfinderH
#include "NetDriver.h"
#include "SplayTree.h"
#include<iostream>
#include "QuadraticProbing.h"
#include <string.h>
#include <string>
#include <stdio.h>
#include <algorithm>

class adjacencyNode
{
  public:
    //int index;
    int adjencyCount;
    string address;
    int adjenctEdges[1000];
    bool adjacentEdgesBackFlow[1000];
    adjacencyNode() { std::fill_n(adjenctEdges, 1000, 0); adjencyCount = 0; std::fill_n(adjacentEdgesBackFlow, 1000, 0); }
    adjacencyNode(string addy) : address(addy) { std::fill_n(adjenctEdges, 1000, 0); adjencyCount = 0; }
    bool operator!= (const adjacencyNode &rhs)const { return address != rhs.address; }
};


class Paths
{
  public: 
    //SplayTree<int> splay(0);
    int pathEdges[500];
    bool isBackFlow[500];
    bool pathEdgesVisited[15000];
    int edgeCount;
    int minimum;
    int maxFlow;
    Paths() : edgeCount(0), minimum(0) { std::fill_n(isBackFlow, 500, 0); }
    Paths( const Paths &copy ) : edgeCount(copy.edgeCount), minimum(copy.minimum) 
      { memcpy( &pathEdges, &copy.pathEdges, sizeof(copy.pathEdges)); 
        memcpy( &isBackFlow, &copy.isBackFlow, sizeof(copy.isBackFlow)); }
    void insertMinimum(int min) { minimum = ((minimum == 0) || min < minimum ) ? min : minimum; }
    //bool operator!= (const Paths &rhs)const { return minimum != rhs.minimum; }
    //bool operator< (const Paths &rhs)const { return minimum < rhs.minimum; }
};

class MaxFinder
{
const adjacencyNode ITEM_NOT_FOUND;
QuadraticHashTable<adjacencyNode> networkHashTable;
Computer *c;

public:
  int _numComputers;
  int _numTerminals;
  string final;
  string terminals[500];
  int flowArray[15000];
  int maxFlow;

  MaxFinder(Computer *computers, int numComputers, int numTerminals);
  int calcMaxFlow(Edge *edges, int numEdges);
  void createPath(Edge *edges, Paths thePath, adjacencyNode currentVertex);
}; // class MaxFinder


// No longer used Classes

// class DijkRow
// {
//   public:
//     bool known;
//     int dv;
//     int pv;
//     bool operator!= (const DijkRow &rhs)const { return dv < rhs.dv; }
//     bool operator< (const DijkRow &rhs)const { return dv != rhs.dv; }
// };

#endif
