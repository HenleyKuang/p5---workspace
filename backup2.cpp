// Author: Sean Davis

#include "NetDriver.h"
#include "maxfinder.h"

using namespace std;

int convertToInt(string& src)
{
  //src.erase (std::remove(src.begin(), src.end(), '.'), src.end());
  return atoi(src.c_str());
  // int hashVal = 0;
  // for( int i = 0; i < src.length( ); i++ )
  //     hashVal = src[ i ];
  //   return hashVal;
}

void MaxFinder::createPath(SplayTree<Paths> &splay, Edge *edges, Paths thePath, adjacencyNode currentVertex)
{
  string currentSrc = currentVertex.address;

  if( currentSrc != final ) 
  {
    int index = convertToInt(currentSrc); 
    adjacencyNode resultNode = networkHashTable.find( index ); 
  
    for(int i = 0; i < resultNode.adjencyCount; i++) 
    {
      int nextEdgeIndex = resultNode.adjenctEdges[i];

      bool visited = false;
      for( int x = 0; x < thePath.edgeCount; x++)
      {
        if( thePath.pathEdges[x] == nextEdgeIndex )
          visited = true;
      }
      
      if( !visited )
      {
        if( resultNode.adjacentEdgesBackFlow[i] == false ) //FALSE = Not a backflow
        {
          string nextDest = edges[nextEdgeIndex].dest;
          int nextDestIndex = convertToInt(nextDest); 
          
          adjacencyNode nextVertex = networkHashTable.find(nextDestIndex); 
          
          Paths copyPath(thePath);
          
          copyPath.pathEdges[copyPath.edgeCount] = nextEdgeIndex;
          copyPath.edgeCount++;
          
          int edgeCapacity = edges[nextEdgeIndex].capacity - flowArray[nextEdgeIndex];
          if( edgeCapacity > 0 )
          {
            copyPath.insertMinimum(edgeCapacity);
            
            createPath(splay, edges, copyPath, nextVertex);
          }
        }
        else if( resultNode.adjacentEdgesBackFlow[i] == true  )//TRUE = This is a backflow
        {
          string backFlow_nextDest = edges[nextEdgeIndex].src;
          int backFlow_nextDestIndex = convertToInt(backFlow_nextDest); 
          
          adjacencyNode backFlow_nextVertex = networkHashTable.find(backFlow_nextDestIndex); 
          
          Paths backFlow_copyPath(thePath);
          
          backFlow_copyPath.pathEdges[backFlow_copyPath.edgeCount] = nextEdgeIndex;
          backFlow_copyPath.edgeCount++;
          
          //cout << "edgeCount: " << backFlow_copyPath.edgeCount << endl;
          
          int backFlow_edgeCapacity = flowArray[backFlow_nextDestIndex];
          if( backFlow_edgeCapacity > 0 )
          {
            backFlow_copyPath.insertMinimum(backFlow_edgeCapacity);
            
            createPath(splay, edges, backFlow_copyPath, backFlow_nextVertex);
          }
       }
      }
    }
  }
  else if( currentSrc == final )
  {
    cout << edges[thePath.pathEdges[0]].src;
    for( int i = 0; i < thePath.edgeCount; i++)
    {
      int edgeIndex = thePath.pathEdges[i];
      string edgeDest = edges[edgeIndex].dest;
      cout << "->" << edgeDest;
    }
    cout << " Flow: " << thePath.minimum << endl;
    splay.insert(thePath);
  }
}

MaxFinder::MaxFinder(Computer *computers, int numComputers, int numTerminals) : networkHashTable(ITEM_NOT_FOUND)
{
  //store numComputers and numTerminals into our MaxFinder Class so we can use them in our calcMaxFlow function
  _numComputers = numComputers;
  _numTerminals = numTerminals;
  c = computers;
  
  //Store all NETWORKS in a hashtables
  for ( int i = 0; i < numComputers; i++ )
  {
    string addr = computers[i].address;
    //Parse the NETWORK source address to be used for hashing in our hash table
    int index = convertToInt(addr); //NEED TO CHANGE PARSING FUNCTION LATER
    
    //Create the NETWORK Vertex to be added into our Hash Table
    adjacencyNode newComputerAddress(index);
    newComputerAddress.address = addr;
    
    //Insert into Hash Table
    networkHashTable.insert(newComputerAddress);
  }
  
  //Store our terminals into a string array
  for ( int i = 0; i < numTerminals; i++ )
    terminals[i] = computers[i].address;
  
  //store final = final destination node in our MaxFinder class to be used in calcMaxFlow
  final = computers[_numComputers-1].address;
} // MaxFinder()

int MaxFinder::calcMaxFlow(Edge *edges, int numEdges)
{
  int maxFlow = 0;
  std::fill_n(flowArray, 15000, 0);
  
  // Go through all the Edges and create an adjacency list for our NETWORK Vertexes
  for (int e = 0; e < numEdges; e++ )
  {
    ////////////////////////////////////
    //        REGULAR FLOW            //
    ///////////////////////////////////
    //current edge's NETWORK source and NETWORK destination
    string currentSrc = edges[e].src;
    //string currentDest = edges[e].dest;
    
    //Parse the NETWORK source address to be used for hashing in our hash table
    int index = convertToInt( currentSrc); //NEED TO CHANGE PARSING FUNCTION LATER
    
    //Find for the NETWORK from our Hash Table
    adjacencyNode resultNode = networkHashTable.find( index ) ;
    
    //Remove the NETWORK from our Hash Table so we can update it
    networkHashTable.remove(resultNode);
    
    // Add the edge into the NETWORK we found
    resultNode.adjenctEdges[resultNode.adjencyCount] = e;
    resultNode.adjencyCount++;
    
    //re-insert the NETWORK back into our Hash Table
    networkHashTable.insert(resultNode);
    
    ////////////////////////////////////
    //        BACK FLOW              //
    ///////////////////////////////////
    //current edge's NETWORK source and NETWORK destination
    if(e > _numTerminals && e < _numComputers)
    {
    string backFlow_currentSrc = edges[e].dest;
    // //string currentDest = edges[e].dest;
    
    // //Parse the NETWORK source address to be used for hashing in our hash table
    int backFlow_index = convertToInt( backFlow_currentSrc ); //NEED TO CHANGE PARSING FUNCTION LATER
    
    // //Find for the NETWORK from our Hash Table
    adjacencyNode backFlow_resultNode = networkHashTable.find( backFlow_index );
    
    // //Remove the NETWORK from our Hash Table so we can update it
    networkHashTable.remove(backFlow_resultNode);
    
    // // Add the edge into the NETWORK we found
    backFlow_resultNode.adjenctEdges[backFlow_resultNode.adjencyCount] = e;
    backFlow_resultNode.adjacentEdgesBackFlow[backFlow_resultNode.adjencyCount] = true; 
    backFlow_resultNode.adjencyCount++;
    
    // //re-insert the NETWORK back into our Hash Table
    networkHashTable.insert(backFlow_resultNode);
    }
  }
  
  //Create SplayTree to hold our Paths
  Paths emptyPath;
  SplayTree<Paths> splay(emptyPath);
  
  while(true)
  {
    // Loop through our terminal NETWORKS and find all the paths to final as well as the max flow
    for ( int i = 0; i < _numTerminals; i++ )
    {
      //current terminal's NETWORK address
      string currentSrc = terminals[i];
      int index = convertToInt(currentSrc); //For Hashing
      
      //Find the terminal NETWORK in our Hash Table
      adjacencyNode resultNode = networkHashTable.find( index );
  
      Paths newPath;
      createPath(splay, edges, newPath, resultNode);
    }
    
    //cout << "Max of the SplayTree: " << flowPath.minimum << endl;
    
    if( !splay.isEmpty() )
    {
      Paths flowPath = splay.findMax();
      for( int i = 0; i < flowPath.edgeCount; i++ )
      {
        int currentEdgeIndex = flowPath.pathEdges[i];
        flowArray[currentEdgeIndex] = flowPath.minimum;
      }
      
      maxFlow += flowPath.minimum;
      splay.makeEmpty();
    }
    else break;
  }

//Cout our adjacency list for testing purposes
  
  for (int i = 0; i < _numComputers; i++ )
  {
    string addr = c[i].address;
    int index = convertToInt(addr);
    
    //Search for our Node in our HashTable
    adjacencyNode resultNode = networkHashTable.find(index);
    
    cout << resultNode.address;
    for( int x = 0; x < resultNode.adjencyCount; x++)
      cout << " -> " << edges[resultNode.adjenctEdges[x]].dest;
    cout << " Adjecents: " << resultNode.adjencyCount << endl;
  } 
  cout << endl; 
  
    return maxFlow;
} // calcMaxFlow()
   
   /*
    for(int k = 0; computers[i].address[k] != '\0'; k++)
    {
      if(computers[i].address[k] != '.')
      {
        c = computers[i].address[k];
        //hash = ((hash << 5) + hash) + c;
        hash +=  37 * hash + computers[i].address[k];
       //hash += 37 * hash + c;
       c = k;
      }
     cout << "Array at " << i << ": " << hash << endl;
    }
  hash %= _numComputers;
    //if( hash < 0 )
      //hash += _numComputers;
  hash = networkHashTable.hash(addr, _numComputers);
  
  */
