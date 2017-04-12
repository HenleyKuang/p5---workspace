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
  //cout << endl;
  
  
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
    newEdges[e].back = 0;
    
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
  
  //Cout our adjacency list for testing purposes
  // for (int i = 0; i < _numComputers; i++ )
  // {
  //   cout << c[i].address << "(" << i << ")";
  //   for( int x = 0; x < c[i].adjencyCount; x++)
  //   {
  //     if( c[i].adjacentEdgesBackFlow[x] == false )
  //     {
  //       cout << " -> " << c[newEdges[c[i].adjenctEdges[x]].dest].address;
  //       if( c[newEdges[c[i].adjenctEdges[x]].dest].address == c[i].address)
  //         cout << "FOUND ERROR" << endl;
  //     }
  //     else if( c[i].adjacentEdgesBackFlow[x] == true )
  //     {
  //       cout << " -> " << c[newEdges[c[i].adjenctEdges[x]].src].address;
  //       if( c[newEdges[c[i].adjenctEdges[x]].src].address == c[i].address)
  //         cout << "FOUND ERROR" << endl;
  //     }
  //   }
  //   cout << " Adjecents: " << c[i].adjencyCount << endl;
  // } 
  
  // cout << endl; 
  
  comp2* ITEM_NOT_FOUND; 
  SplayTree<comp2* > splay(ITEM_NOT_FOUND);
  Paths newPath;
  
  int ITEM_NOT_FOUND2;
  QuadraticHashTable2<int> visitedVertexes(ITEM_NOT_FOUND2);
  
  int searchNum = 0;

    for ( int i = 0; i < _numTerminals; i++ )
    {
      visitedVertexes.insert(i);
      splay.insert( &c[i] );
    }
    while ( !splay.isEmpty() )
    {
      int compIndex = splay.findMax()->index;
      splay.remove(splay.findMax());
      
      //cout << "Splayed out: " << c[compIndex].address << endl;
      
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
          
          //cout << "compSrc: " << c[compSrc].address << " compDest: " << c[compDest].address << " flow: " << flow << endl;
          //cout << "dv: " << c[compDest].dv << " pv: " << c[c[compDest].pv].address << endl;
          
          // Refresh pv & dv if new dijkstras table
          if( searchNum != c[compDest].searchNumber)
          {
            c[compDest].dv = -1;
            c[compDest].min = -1;
            c[compDest].searchNumber = searchNum;
          }

          if( flow != 0  && flow >= c[compDest].dv && visitedVertexes.findInt(compDest) == ITEM_NOT_FOUND2 )
          {
            c[compDest].pv = compSrc;
            c[compDest].dv = flow;
            c[compDest].min = (c[compSrc].min == -1 || flow < c[compSrc].min) ? flow : c[compSrc].min;
            c[compDest].edge = edgeNum;
            visitedVertexes.insertInt(compSrc);
            c[compDest].backFlow = backFlow;
            splay.insert( &c[compDest] );
            //cout << "Inserted: " << c[compDest].address << " dv: " << c[compDest].dv << " pv: " << c[c[compDest].pv].address <<"(" << c[compDest].pv << ")" << " min: " << c[compDest].min << endl;
          }
        }
      }
      else //if we are at final
      {
        //cout << endl << c[compIndex].address;
        int edgeNum = c[compIndex].edge;
        int flowMin = c[compIndex].min;
        
        while( true )
        {
          edgeNum = c[compIndex].edge;
          
          bool backFlow = c[compIndex].backFlow;
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
          c[compIndex].min = -1;//= ( c[compIndex].dv < c[compDest].min) ? c[compIndex].dv : c[compDest].min;
          
          // cout << " <- ";
          // cout << c[compSrc].address; //" pv: " << c[compIndex].pv << " CompIndex: " << compIndex << " compSrc " << compSrc << endl ;
          
          if ( c[compSrc].pv != -1 )
          {
            c[compIndex].dv = -1;
            c[compIndex].pv = -1;
            compIndex = compSrc;
          }
          else 
          {
            c[compIndex].dv -= flowMin;
            break;
          }
        }
        //cout << " Flow: " << flowMin << endl << endl;
        
       // cout << c[5].pv << endl;
        maxFlow += flowMin;
        
        searchNum++;
        visitedVertexes.makeEmpty();
        splay.makeEmpty();
        for ( int i = 0; i < _numTerminals; i++ )
        {
          visitedVertexes.insert(i);
          splay.insert( &c[i] );
        }
      }
  }
    
    return maxFlow;
} // calcMaxFlow()

