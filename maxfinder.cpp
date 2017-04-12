// Author: Sean Davis

#include "NetDriver.h"
#include "maxfinder.h"

using namespace std;

MaxFinder::MaxFinder(Computer *computers, int numComputers, int numTerminals) : networkHashTable(ITEM_NOT_FOUND)
{
  //store numComputers and numTerminals into our MaxFinder Class so we can use them in our calcMaxFlow function
  _numComputers = numComputers;
  _numTerminals = numTerminals;
  c = new comp2[numComputers];
  
  //Store all NETWORKS in a hashtables
  for ( int i = 0; i < numComputers; i++)
  {
    if(i < numTerminals)
      terminals[i] = computers[i].address;
      
    string addr = computers[i].address;
    
    //Create the NETWORK Vertex to be added into our Hash Table
    adjacencyNode newComputerAddress(addr);
    newComputerAddress.index = i;
    
    //Insert into Hash Table
    networkHashTable.insert(newComputerAddress);
    
    strcpy(c[i].address, computers[i].address);
    c[i].index = i;
  }
  
  //store final = final destination node in our MaxFinder class to be used in calcMaxFlow
  final = computers[_numComputers-1].address;
} // MaxFinder()

int MaxFinder::calcMaxFlow(Edge *edges, int numEdges)
{
  
  maxFlow = 0;
  
  newEdges = new edges2[numEdges];
  
  // Go through all the Edges and create an adjacency list for our NETWORK Vertexes
  for (int e = 0; e < numEdges; e++ )
  {
    ////////////////////////////////////
    //        REGULAR FLOW            //
    ///////////////////////////////////
    //current edge's NETWORK source and NETWORK destination
    string currentSrc = edges[e].src;
    string currentDest = edges[e].dest;
    
    //Find for the NETWORK's src Index from our Hash Table
    adjacencyNode searchNodeSrc(currentSrc);
    adjacencyNode resultNodeSrc = networkHashTable.find( searchNodeSrc ) ;
    int currentSrcIndex = resultNodeSrc.index;
    
    //Find for the NETWORK's dest Index from our Hash Table
    adjacencyNode searchNodeDest(currentDest);
    adjacencyNode resultNodeDest = networkHashTable.find( searchNodeDest ) ;
    int currentDestIndex = resultNodeDest.index;
    
    newEdges[e].src = currentSrcIndex;
    newEdges[e].dest = currentDestIndex;
    newEdges[e].reg = edges[e].capacity;
    //newEdges[e].back = 0;
    
    if( currentDestIndex > _numTerminals-1 )
    {
      c[currentSrcIndex].adjenctEdges[c[currentSrcIndex].adjencyCount] = e;
      c[currentSrcIndex].adjencyCount++;
    }
    
    ////////////////////////////////////
    //        BACK FLOW              //
    ///////////////////////////////////
    
    if( currentDest != final && currentSrcIndex > _numTerminals-1 )
    {
      // // Add the edge into the NETWORK we found
      c[currentDestIndex].adjenctEdges[c[currentDestIndex].adjencyCount] = e;
      c[currentDestIndex].adjacentEdgesBackFlow[c[currentDestIndex].adjencyCount] = true; 
      c[currentDestIndex].adjencyCount++;
    }
  }
  
  BinaryHeap<comp2* > splay(10000);

	 bool visitedVertexes[5000];
	 
    int searchNum = 0;

    int times = 0;
    
     visitedVertexes[0] = true;
	  splay.insert( &c[0] );
	 
    while ( !splay.isEmpty() && times < _numTerminals )
    {
      int compIndex = splay.findMax()->index;
      splay.deleteMin();
      
      if( compIndex != _numComputers - 1) // If we're not at final
      {
      // Loop through our terminal NETWORKS and find all the paths to final as well as the max flow
        for( int a = 0; a < c[compIndex].adjencyCount; a++ )
        {
          int edgeNum = c[compIndex].adjenctEdges[a];
          bool backFlow = c[compIndex].adjacentEdgesBackFlow[a];
          
          int compSrc = backFlow ? newEdges[edgeNum].dest : newEdges[edgeNum].src;
          int compDest = backFlow ? newEdges[edgeNum].src : newEdges[edgeNum].dest;
          
          int flow = c[compIndex].adjacentEdgesBackFlow[a] ? newEdges[edgeNum].back : newEdges[edgeNum].reg;

          // Refresh dv if new dijkstras table
          if( searchNum != c[compDest].searchNumber)
          {
            c[compDest].dv = -1;
            c[compDest].min = -1;
            c[compDest].searchNumber = searchNum;
          }

          if( flow != 0  && flow > c[compDest].dv && !visitedVertexes[compDest] )
          {
            c[compDest].pv = compSrc;
            c[compDest].dv = flow;
            c[compDest].min = (c[compSrc].min == -1 || flow < c[compSrc].min) ? flow : c[compSrc].min;
            c[compDest].edge = edgeNum;
            visitedVertexes[compDest] = true;
            c[compDest].backFlow = backFlow;
            splay.insert( &c[compDest] );
          }
        }
        if( splay.isEmpty() )
        {
          times++;
           visitedVertexes[times] = true;
          //visitedVertexes.insert(times);
          splay.insert( &c[times] );
        }
      }
      else //if we are at final
      {
        int edgeNum = c[compIndex].edge;
        bool backFlow = c[compIndex].backFlow;
        int flowMin = c[compIndex].min;
        
        while( flowMin > 0 && c[c[compIndex].pv].pv != -1 )
        {
          edgeNum = c[compIndex].edge;
          
          backFlow = c[compIndex].backFlow;
          int compSrc = c[compIndex].pv;
          
          if( backFlow )
          {
            newEdges[edgeNum].reg += flowMin;
            newEdges[edgeNum].back -= flowMin;
          }
          else
          {
            newEdges[edgeNum].reg -= flowMin;
            newEdges[edgeNum].back += flowMin;
          }
            c[compIndex].dv = -1;
            compIndex = compSrc;
        }
        maxFlow += flowMin;
        
        searchNum++;
        
        if( splay.isEmpty() )
          times++;
        else splay.makeEmpty();
        
        for( int o = 0; o < 5000; o++)
          visitedVertexes[o] = false;

        visitedVertexes[times] = true;
          splay.insert( &c[times] );
     }
  }
    
    return maxFlow;
} // calcMaxFlow()

