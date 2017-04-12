#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstdio>
using namespace std;

struct Computer
{
  unsigned address;
};

void printAddress(unsigned address, ofstream &outf)
{
  outf << ((address & 0xFF000000) >> 24) << '.' << ((address & 0xFF0000) >> 16)
    << '.' << ((address & 0xFF00) >> 8) << '.' << (address & 0xFF);
} // printAddress()


void createComputers(ofstream &outf, unsigned *addresses, unsigned numComputers)
{
  unsigned address, j, i;

  for(i = 0; i < numComputers; i++)
  {
    do{
      address = rand() * rand();
      for(j = 0; j < i; j++)
        if(addresses[j] == address)
          break;
    } while(addresses[j] == address); // guarantee unique address;

    addresses[i] = address;
    printAddress(addresses[i], outf);
    outf << endl;
  } // for each computer

} // createAddresses()


void createNet(ofstream &outf, unsigned *addresses, unsigned numComputers,
  unsigned edges, unsigned numTerminals)
{
  unsigned i, srcIndex, destIndex, sinkIndex = numComputers - 1,
    unVisitedCount, pos;
  bool **connected = new bool*[numComputers];
  int length, *depth = new int[numComputers];

  unVisitedCount = numComputers - numTerminals - 1;  // don't include sink
  int *unvisited = new int[unVisitedCount];

  for(i = 0; i < unVisitedCount; i++)
    unvisited[i] = i + numTerminals;

  for(i = 0; i < numComputers; i++)
  {
    depth[i] = 0;
    connected[i] = new bool[numComputers];
    for(unsigned j = 0; j < numComputers; j++)
      connected[i][j] = false;

    connected[i][i] = true;
  }


  for(i = 0; i < numTerminals; i++)
  {
    srcIndex = i;
    length = unVisitedCount / (numTerminals - i);
    for(int j = 0; j <= length  && unVisitedCount > 0; j++)
    {
      pos = rand() % unVisitedCount;
      destIndex = unvisited[pos];
      unvisited[pos] = unvisited[--unVisitedCount];
      printAddress(addresses[srcIndex], outf);
      outf << ' ';
      printAddress(addresses[destIndex], outf);
      outf << ' ' << rand() % 20 + 5 << endl;
      connected[srcIndex][destIndex] = true;
      connected[destIndex][srcIndex] = true;
      edges--;
      srcIndex = destIndex;
      depth[srcIndex] = j + 1;
    } // for the length

    printAddress(addresses[srcIndex], outf);
    outf << ' ';
    printAddress(addresses[sinkIndex], outf);
    outf << ' ' << rand() % 20 + 5 << endl;
    connected[srcIndex][sinkIndex] = true;
    connected[destIndex][srcIndex] = true;
    edges--;
  }  // for each terminal

  depth[sinkIndex] = length + 1;
  for(; edges > 0; --edges)
  {
    do
    {
      srcIndex = rand() % sinkIndex;  // sink not ever source
      destIndex = rand() % numComputers;
    } while(connected[srcIndex][destIndex]
      || abs(depth[destIndex] - depth[srcIndex]) > 1);  // no leapfrogging

    printAddress(addresses[srcIndex], outf);
    outf << ' ';
    printAddress(addresses[destIndex], outf);  // sink
    outf << ' ' << rand() % 20 + 5 << endl;
    connected[srcIndex][destIndex] = true;
    connected[destIndex][srcIndex] = true;
  } // for remaining edges

  for(unsigned i = 0; i < numComputers; i++)
    delete [] connected[i];

  delete [] connected;
  delete [] depth;
  delete [] unvisited;

} // createNet()


int main()
{
  char filename[80];
  int seed;
  unsigned numComputers, terminals, edges, *addresses;
  cout << "Computers (2 - 5000): ";
  cin >> numComputers;
  do
  {
    cout << "Terminals (< computers): ";
    cin >> terminals;
  } while(terminals >= numComputers);

  do
  {
    cout << "Edges (computers  <= # << computers ^ 2): ";
    cin >> edges;
  } while(edges < numComputers);

  cout << "Seed: ";
  cin >> seed;
  srand(seed);
  sprintf(filename, "net-%d-%d-%d-%d.txt", numComputers, terminals, edges, seed);
  ofstream outf(filename);
  addresses = new unsigned[numComputers];

  outf << numComputers << ' ' << terminals << ' ' << edges << endl;
  createComputers(outf, addresses, numComputers);
  createNet(outf, addresses, numComputers, edges, terminals);
  outf.close();
  return 0;
}  // main()

