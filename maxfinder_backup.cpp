// Author: Sean Davis

#include "NetDriver.h"
#include "maxfinder.h"

using namespace std;

// int convertToInt(string& src)
// {
//   //src.erase (std::remove(src.begin(), src.end(), '.'), src.end());
//   /*
//   src.erase(std::remove(src.begin(), src.end(), '.'), src.end()); //removing the dot
//   std::stringstream ss(src);
//   int value;
//   ss >> value;
  
//   cout << value << endl;
  
//   return value;
//   */
  
//   // char * pEnd = new char;
//   // long int value = strtol(src.c_str(), &pEnd, src.length());
  
//   // return value;
  
//   return atoi(src.c_str());
//   //int hash2 = atoi(src.c_str());
//   /*
//   string hash3temp;
//   for (int count = 0; count < strlen(src); count++)
//   {
//   	//	hash3temp += string((int)src[count]); //interprets the characters as integer type
//   }
//   */
//   //cout << hash3temp << endl;
//   //int hash = 0;
//   //long int hash = strtol (hash3temp);
//   //cout << "converted value: " << hash << " " << hash2 << endl;
//   //return hash;
  
//   // Counldnt we just convert the addresses into a hash table value 
//   // Then we wouldnt need to worry about converting the whole address
//   // int hashVal = 0;
//   // for( int i = 0; i < src.length( ); i++ )
//   //     hashVal = src[ i ];
//   //   return hashVal;
// }

void MaxFinder::createPath(Edge *edges, Paths thePath, adjacencyNode currentVertex)
{
  string currentSrc = currentVertex.address;

  if( currentSrc != final ) 
  {
    //int index = convertToInt(currentSrc); 
    adjacencyNode searchNode(currentSrc);
    
    adjacencyNode resultNode = networkHashTable.find( searchNode ); 
  
    for(int i = 0; i < resultNode.adjencyCount; i++) 
    {
      int nextEdgeIndex = resultNode.adjenctEdges[i];

      bool visited = thePath.pathEdgesVisited[nextEdgeIndex];
      
      if( !visited )
      {
        if( resultNode.adjacentEdgesBackFlow[i] == false ) //FALSE = Not a backflow
        {
          string nextDest = edges[nextEdgeIndex].dest;
          //int nextDestIndex = convertToInt(nextDest); 
          adjacencyNode searchNode(nextDest);
          
          adjacencyNode nextVertex = networkHashTable.find(searchNode); 
          
          Paths copyPath(thePath);
          
          copyPath.pathEdges[copyPath.edgeCount] = nextEdgeIndex;
          copyPath.pathEdgesVisited[nextEdgeIndex] = true;
          thePath.pathEdgesVisited[nextEdgeIndex] = true;
          copyPath.edgeCount++;
          
          int edgeCapacity = edges[nextEdgeIndex].capacity - flowArray[nextEdgeIndex];
          if( edgeCapacity > 0 )
            createPath(edges, copyPath, nextVertex);
        }
        else if( resultNode.adjacentEdgesBackFlow[i] == true  )//TRUE = This is a backflow
        {
          string backFlow_nextDest = edges[nextEdgeIndex].src;
          
          int backFlow_edgeCapacity = flowArray[nextEdgeIndex];
          if( backFlow_edgeCapacity > 0 )
          {
            //int backFlow_nextDestIndex = convertToInt(backFlow_nextDest); 
            adjacencyNode searchNode(backFlow_nextDest);
            
            adjacencyNode backFlow_nextVertex = networkHashTable.find(searchNode); 
            
            Paths backFlow_copyPath(thePath);
            
            backFlow_copyPath.pathEdges[backFlow_copyPath.edgeCount] = nextEdgeIndex;
            backFlow_copyPath.isBackFlow[backFlow_copyPath.edgeCount] = true;
            backFlow_copyPath.pathEdgesVisited[nextEdgeIndex] = true;
            thePath.pathEdgesVisited[nextEdgeIndex] = true;
            backFlow_copyPath.edgeCount++;
            
            //cout << "edgeCount: " << backFlow_copyPath.edgeCount << endl;
            
              //backFlow_copyPath.insertMinimum(backFlow_edgeCapacity);
              
              createPath(edges, backFlow_copyPath, backFlow_nextVertex);
          }
        }
      }
    }
  }
  else if( currentSrc == final )
  {
    bool zeroFound = false;
    
    //INSTEAD OF ADDING INTO SPLAY, WE'LL SUBTRACT RIGHT AWAY
    cout << "edge count "<< thePath.edgeCount << endl;
    for( int i = 0; i < thePath.edgeCount; i++ )
    {
      int currentEdgeIndex = thePath.pathEdges[i];
      
      int regFlow = edges[currentEdgeIndex].capacity - flowArray[currentEdgeIndex];
      int backFlow = flowArray[currentEdgeIndex];
      
      cout << "Reg Flow1: " << regFlow;
      cout << " BackFlow1: " << backFlow << endl;
      
      if ( !thePath.isBackFlow[i] ) //NOT A BACKFLOW
      {
        if( regFlow != 0 )
        {
          thePath.insertMinimum(regFlow);
          
          flowArray[currentEdgeIndex] += thePath.minimum;
        }
        else
        {
          zeroFound = true;
          break;
        }
      }
      else if ( backFlow != 0 ) //IS A BACKFLOW
      {
        thePath.insertMinimum(backFlow);
        flowArray[currentEdgeIndex] -= thePath.minimum;
      }
      else
      {
        zeroFound = true;
        break;
      }
    }
        
    // Print our flow
    cout << edges[thePath.pathEdges[0]].src;
    for( int i = 0; i < thePath.edgeCount; i++)
    {
      int edgeIndex = thePath.pathEdges[i];
      string edgeDest = edges[edgeIndex].dest;
      cout << "->" << edgeDest;
    }
    cout << " Flow: " << thePath.minimum << endl;
    
    if ( !zeroFound )
    {
      maxFlow += thePath.minimum; 
      cout << "New maxFlow: " << maxFlow << endl;
    }
  }
}

MaxFinder::MaxFinder(Computer *computers, int numComputers, int numTerminals) : networkHashTable(ITEM_NOT_FOUND)
{
  //store numComputers and numTerminals into our MaxFinder Class so we can use them in our calcMaxFlow function
  _numComputers = numComputers;
  _numTerminals = numTerminals;
  c = computers;
  // 
  //Store all NETWORKS in a hashtables
  for ( int i = 0; i < numComputers; i++)
  {
    // char * pEnd;
    // long int value = strtol(computers[i].address, &pEnd, strlen(computers[i].address));
    // int x = value;
    
    // std::cout << computers[i].address << " -> " <<  x << " " << atoi(computers[i].address) << endl;
    
    string addr = computers[i].address;
    //cout << addr << endl;
    
    //Parse the NETWORK source address to be used for hashing in our hash table
    //int index = convertToInt(addr); //NEED TO CHANGE PARSING FUNCTION LATER
    
    //Create the NETWORK Vertex to be added into our Hash Table
    adjacencyNode newComputerAddress(addr);
    //newComputerAddress.address = addr;
    
    //Insert into Hash Table
    networkHashTable.insert(newComputerAddress);
  }
  cout << endl;
  
  //Store our terminals into a string array
  for ( int i = 0; i < numTerminals; i++ )
    terminals[i] = computers[i].address;
  
  //store final = final destination node in our MaxFinder class to be used in calcMaxFlow
  final = computers[_numComputers-1].address;
} // MaxFinder()

int MaxFinder::calcMaxFlow(Edge *edges, int numEdges)
{
  maxFlow = 0;
  std::fill_n(flowArray, 15000, 0);
  
  // Go through all the Edges and create an adjacency list for our NETWORK Vertexes
  for (int e = 0; e < numEdges; e++ )
  {
    ////////////////////////////////////
    //        REGULAR FLOW            //
    ///////////////////////////////////
    //current edge's NETWORK source and NETWORK destination
    string currentSrc = edges[e].src;
    
    //Parse the NETWORK source address to be used for hashing in our hash table
    //int index = convertToInt( currentSrc); //NEED TO CHANGE PARSING FUNCTION LATER
    adjacencyNode searchNode(currentSrc);
    
    //Find for the NETWORK from our Hash Table
    adjacencyNode resultNode = networkHashTable.find( searchNode ) ;
    
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
    string backFlow_currentSrc = edges[e].dest;
    
    bool isTerminal = false;
    for( int i = 0; i < _numTerminals; i++)
    {
      if( terminals[i] == currentSrc )
      {

        isTerminal = true;
        break;
      }
    }
    
    if( backFlow_currentSrc != final && !isTerminal )
    {
      //Parse the NETWORK source address to be used for hashing in our hash table
      //int backFlow_index = convertToInt( backFlow_currentSrc ); //NEED TO CHANGE PARSING FUNCTION LATER
      adjacencyNode searchNode(backFlow_currentSrc);
      
      // //Find for the NETWORK from our Hash Table
      adjacencyNode backFlow_resultNode = networkHashTable.find( searchNode );
      
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
  
  // Loop through our terminal NETWORKS and find all the paths to final as well as the max flow
  for ( int i = 0; i < _numTerminals; i++ )
  {
    //current terminal's NETWORK address
    string currentSrc = terminals[i];
    //int index = convertToInt(currentSrc); //For Hashing
    adjacencyNode searchNode(currentSrc);
    
    //Find the terminal NETWORK in our Hash Table
    adjacencyNode resultNode = networkHashTable.find( searchNode );

    Paths newPath;
    createPath(edges, newPath, resultNode);
  }

//Cout our adjacency list for testing purposes
  
  for (int i = 0; i < _numComputers; i++ )
  {
    string addr = c[i].address;
    //int index = convertToInt(addr);
    adjacencyNode searchNode(addr);
    
    //Search for our Node in our HashTable
    adjacencyNode resultNode = networkHashTable.find(searchNode);
    
    cout << resultNode.address;
    for( int x = 0; x < resultNode.adjencyCount; x++)
    {
      if( resultNode.adjacentEdgesBackFlow[x] == false )
        cout << " -> " << edges[resultNode.adjenctEdges[x]].dest;
      else if( resultNode.adjacentEdgesBackFlow[x] == true )
        cout << " -> " << edges[resultNode.adjenctEdges[x]].src;
    }
    cout << " Adjecents: " << resultNode.adjencyCount << endl;
  } 
  cout << endl; 
  
    return maxFlow;
} // calcMaxFlow()

