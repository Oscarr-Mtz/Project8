/*
Oscar Martinez, Xinduo Fan
CS 271
Dr. Lall
Project 8
*/

//#include <bits/stdc++.h>
#include <sstream>
#include <string>
#include <iostream>
#include <typeinfo>
#include <fstream>
#include <map>
#include <vector>
#include <stack>
#include <queue>

using namespace std;

// ============================== Constructor ==================================
graph::graph(std::string filename)
//Preconditions:  N/A
//Postcondition:  N/A
{
  constructAdjMatrix(filename);   //does error check
  constructAdjList();
}


// ============================= Copy Constructor ==============================
graph::graph(const graph& g)
//Preconditions:  N/A
//Postcondition:  N/A
{
  this->adjList = g.adjList;
  this->adjMatrix = g.adjMatrix;
  this->colors = g.colors;
}


// ============================= Destructor ====================================
graph::~graph()
//Preconditions:  N/A
//Postcondition:  N/A
{
  //bc we are using libaries they have Destructors implemented already
}


// ============================ Depth First Search Method ======================
void graph::dfs()
//Preconditions:  N/A
//Postcondition:  N/A
{
  for(int i = 0; i < numVertices; i++)
  {
    if (colors[i] == WHITE)
    {
      dfsNonRecursive(i);
    }
  }
}


// ============================ Topological Sort Method ========================
void graph::topologicalsort()
//Preconditions:  N/A
//Postcondition:  N/A
{
  std::vector<int> inDegree = countInDegree();
  queue<int> processQueue;
  int vertex = 0;
  while( (inDegree[vertex] != 0) && (vertex < inDegree.size()) )vertex++;     //find an vertex having in-degree 0
  if (vertex >= inDegree.size())
  {
    cout<< "cycle detected"<<endl;
    exit(1);
  }
  processQueue.push(vertex);
  cout << "The topological order of this graph is:";
  while ( !processQueue.empty() )
  {
    vertex = processQueue.front();
    processQueue.pop();
    cout << vertex << " ";
    for (int i = 0; i < adjList.at(vertex).size(); i++ )
    {
      int document_vertex = adjList.at(vertex)[i];
      inDegree[document_vertex] --;     //= adjMatrix[vertex][document_vertex];
      if ( inDegree[document_vertex] == 0 ) processQueue.push(document_vertex);
    }
  }
  cout <<"\n";  
}


// ============================= Count In Degree Method ========================
std::vector<int> graph::countInDegree()
//Preconditions:  N/A
//Postcondition:  N/A
{
  vector<int> countInD;
  for(int i =0; i < numVertices; i++) countInD.push_back(0);
  for(int i =0; i < numVertices; i++)
  {
    vector<int> keyVec = adjList.at(i);
    for(int j = 0; j < keyVec.size(); j++)
    {
      countInD[keyVec[j]]++;
    }
  }
  return countInD;
  
}


// ============================ Display Colors  Method =========================
void graph::display_colors()
//Preconditions:  N/A
//Postcondition:  N/A
{
  for (std::vector<int>::const_iterator i = colors.begin(); i != colors.end(); ++i)
      std::cout << *i << ' ';

  cout << "\n" << endl;
}


// =========================== Display Adjaceny List Method ====================
void graph::display_adjList()
//Preconditions:  N/A
//Postcondition:  N/A
{
  for(auto it = adjList.begin(); it != adjList.end(); ++it)   //https://stackoverflow.com/questions/18362896/printing-the-vector-in-a-map
  {
    cout << "\n" << it->first << " : ";
    for(auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
      cout << *it2 << " ";
  }
    cout << "\n" << endl;
}



// =========================== Display Adjaceny Matrix Method ==================
void graph::display_adjMatrix()
//Preconditions:  N/A
//Postcondition:  N/A
{
  for (int i = 0; i < adjMatrix.size(); i++)
  {
    for (int j = 0; j < adjMatrix[i].size(); j++)
    {
      std::cout << adjMatrix[i][j] << " ";
    }

    std::cout << std::endl;
  }
}


// =========================== Assignment Opertor Method =======================
graph& graph::operator=(const graph& g)
//Preconditions:  N/A
//Postcondition:  N/A
{
  this->adjList = g.adjList;
  this->adjMatrix = g.adjMatrix;
  this->colors = g.colors;
}


// ========================== Adjaceny Matrix Constructor ======================
void graph::constructAdjMatrix(std::string filename)
//Preconditions:  N/A
//Postcondition:  N/A
{
  std::string vertices;
  std::string line;
  std::ifstream inFile;

  inFile.open(filename);

  if (!inFile)    //file opening failed
  {
    std::cerr << "Unable to open file"; //FIX, always outputs
    exit(1);
  }

  getline(inFile, vertices);    //we get the num of vertices this way
  numVertices = stoi(vertices);
  adjMatrix.resize(numVertices);    //resize the vector so we can push to the subvec

  for(int i = 0; i < numVertices; i++)   //populate the adjList
  {
    getline(inFile, line);
    line = removeWhiteSpaces(line);

    for(int j = 0; j < line.length(); j++)
    {
      int intResult = line[j] - '0';    //turn a char to an int
      adjMatrix[i].push_back(intResult);
    }
  }

  inFile.close();
}


// =========================== Adjaceny List Constructor =======================
void graph::constructAdjList()
//Preconditions:  N/A
//Postcondition:  N/A
{

  for (int i = 0; i < adjMatrix.size(); i++)    //can build list from matrix
  {
    adjList[i];   //populate keys
    std::vector<int> result;
    for (int j = 0; j <(adjMatrix[i].size()) ; j++)
    {
      if(adjMatrix[i][j] != 0)    //we dont care about 0 weight bc no connection
      {
       result.push_back(j);   //pushes vertex not weight
      }
    }
    adjList[i] = result;
    colors.push_back(WHITE);
  }
}


// ======================== Remove White Spaces Method =========================
string graph::removeWhiteSpaces(std::string line)
//Preconditions:  N/A
//Postcondition:  N/A
{
  string result = "";
  for(char c: line)   //for every char in line
  {
    if (c != '\n' && c != ' ' && c != '\t')   //strips white characters
    {
      result += c;
    }
  }

  return result;
}


// ========================== DFS NonRecursive Method =========================
void graph::dfsNonRecursive(int vertex)
//Preconditions:  N/A
//Postcondition:  N/A
{
  stack<int> processStack;
  colors[vertex] = GRAY;
  processStack.push(vertex);
  //int j = 0;
  cout << "Order of Processed Vertices: ";
  while(processStack.empty() != true)   //while stack is not empty
  {
    int u = processStack.top();       //returns top element
    processStack.pop();   //pops top element
    int vecLen = adjList.at(u).size();    //gets size of vector at key j
    //cout << "\nLength of the vector:" <<vecLen << endl;
    for(int i = 0; i < vecLen; i++)
    {
      int v = adjList.at(u)[i];   //get the vertex at u
      //cout << "Vertex: " << v << endl;
      if(colors[v] == WHITE)    //if vertex is white
      {
        //cout << "Test" << endl;
        processStack.push(v);   //add vertex to stack
        colors[v] = GRAY;   //color vertex gray
      }
    }

    colors[u] = BLACK;    //once no more neighbors we color black
    // j++;    //iterator variable increment
    cout <<u << " ";   //cout the order vertices are processed
  }
  cout << "\n" << endl;
}
