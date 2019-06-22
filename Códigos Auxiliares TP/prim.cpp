/*
 * Prim's Algorithm for
 * Undirected Weighted Graph
 * Code using C++ STL
 * 
 * Authored by,
 * Vamsi Sangam.
 *
 */
  
#include <cstdio>
#include <vector>
#include <list>
//#include <utility>
  
using namespace std;
  
struct edge
{
    int u, v;
    int weight;
};
  
void Enqueue(struct edge heap[], int size, struct edge value)
{
    heap[size] = value;
      
    int i = size;
    struct edge temp;
      
    while (i >= 1) {
        if (heap[i / 2].weight > heap[i].weight) {
            //Parent node is greater than Child Node
            //Heap Property violated
            //So, swap
            temp = heap[i / 2];
            heap[i / 2] = heap[i];
            heap[i] = temp;
              
            i = i / 2;
        } else {
            //Parent is less or equal to the child
            //Heap property not violated
            //So, Insertion is done, break
            break;
        }
    }
}
  
void Heapify(struct edge heap[], int size, int index)
{
    int i = index;
    struct edge temp;
      
    while ((2 * i) < size) {
        //Left Child exists
          
        if ((2 * i) + 1 >= size) {
            //Right child does not exist, but left does
            if (heap[i].weight > heap[2 * i].weight) {
                //Left child is smaller than parent
                temp = heap[i];
                heap[i] = heap[2 * i];
                heap[2 * i] = temp;
                break;
                //Once you reach this level where it does not
                //have a right child, the heap ends here
                //taking it to the next iteration is pointless
            }
        }
          
        //Both Children exist
        if (heap[i].weight > heap[2 * i].weight || heap[i].weight > heap[2 * i + 1].weight) {
            //One of the other child is lesser than parent
            //Now find the least amoung 2 children
              
            if (heap[2 * i].weight <= heap[(2 * i) + 1].weight) {
                //Left Child is lesser, so, swap
                temp = heap[2 * i];
                heap[2 * i] = heap[i];
                heap[i] = temp;
                //And go down the heap
                i = 2 * i;
            } else if (heap[2 * i].weight > heap[(2 * i) + 1].weight) {
                //Left Child is lesser, so, swap
                temp = heap[(2 * i) + 1];
                heap[(2 * i) + 1] = heap[i];
                heap[i] = temp;
                //And go down the heap
                i = (2 * i) + 1;
            }
        } else {
            //Parent is lesser than its children
            break;
        }
    }
}
  
void DeleteNode(struct edge heap[], int size, int index)
{
    //Swap the indexed element with the last
    struct edge temp = heap[index];
    heap[index] = heap[size - 1];
    heap[size - 1] = temp;
      
    int i = index;
    --size;
      
    Heapify(heap, size, i);
}
  
// Returns the element with
// Minimum Priority and deletes it
struct edge ExtractMin(struct edge heap[], int size)
{
    struct edge min = heap[0];
   
    DeleteNode(heap, size, 0);
   
    return min;
}
  
// Prim's Algorithm function
void Prim(
            vector< list< pair<int, int> > > & adjacencyList, 
            int vertices, 
            int edges, 
            int startVertex, 
            vector< list< pair<int, int> > > & MST
         )
{
    int current = startVertex, newVertex;
    bool visited[vertices + 1];
    struct edge var;
    struct edge PriorityQueue[2 * edges];
    int QueueSize = 0;
   
    int i;
   
    for (i = 0; i <= vertices; ++i) {        // Initializing
        visited[i] = false;
    }
   
    i = 0;
   
    while (i < vertices) {
        if (!visited[current]) {            // If current node is not visited
            visited[current] = true;        // Mark it visited
              
            list< pair<int, int> >::iterator itr = adjacencyList[current].begin();
              
            while (itr != adjacencyList[current].end()) {                
                if (!visited[(*itr).first]) {
                    // If the edge leads to an un-visited
                    // vertex only then enqueue it
                    var.u = current;
                    var.v = (*itr).first;
                    var.weight = (*itr).second;
                  
                    Enqueue(PriorityQueue, QueueSize, var);
                    ++QueueSize;
                }
                  
                ++itr;
            }
   
            var = ExtractMin(PriorityQueue, QueueSize);     // The greedy choice
            --QueueSize;
   
            newVertex = var.v;
            current = var.u;
   
            if (!visited[newVertex]) {
                MST[current].push_back(make_pair(newVertex, var.weight));
                MST[newVertex].push_back(make_pair(current, var.weight));
            }
   
            current = newVertex;
            ++i;
        } else {
   
            var = ExtractMin(PriorityQueue, QueueSize);
            --QueueSize;
   
            newVertex = var.v;
            current = var.u;
   
            if (!visited[newVertex]) {
                MST[current].push_back(make_pair(newVertex, var.weight));
                MST[newVertex].push_back(make_pair(current, var.weight));
            }
   
            current = newVertex;
        }
    }
}
  
int main()
{
    int vertices, edges, v1, v2, weight;
      
    printf("Enter the Number of Vertices -\n");
    scanf("%d", &vertices);
      
    printf("Enter the Number of Edges -\n");
    scanf("%d", &edges);
      
    // Adjacency List is a vector of list.
    // Where each element is a pair<int, int>
    // pair.first -> the edge's destination
    // pair.second -> edge's weight
    vector< list< pair<int, int> > > adjacencyList(vertices + 1);
    vector< list< pair<int, int> > > MST(vertices + 1);
      
    printf("Enter the Edges V1 -> V2, of weight W\n");
      
    for (int i = 1; i <= edges; ++i) {
        scanf("%d%d%d", &v1, &v2, &weight);
          
        // Adding Edge to the Directed Graph
        adjacencyList[v1].push_back(make_pair(v2, weight));
        adjacencyList[v2].push_back(make_pair(v1, weight));
    }
      
    printf("\nThe Adjacency List-\n");
    // Printing Adjacency List
    for (int i = 1; i < adjacencyList.size(); ++i) {
        printf("adjacencyList[%d] ", i);
          
        list< pair<int, int> >::iterator itr = adjacencyList[i].begin();
          
        while (itr != adjacencyList[i].end()) {
            printf(" -> %d(%d)", (*itr).first, (*itr).second);
            ++itr;
        }
        printf("\n");
    }
   
    int startVertex;
   
    printf("\nEnter a Start Vertex - ");
    scanf("%d", &startVertex);
    Prim(adjacencyList, vertices, edges, startVertex, MST);
   
    printf("\nThe Minimum Spanning Tree-\n");
    // Printing Adjacency List
    for (int i = 1; i < MST.size(); ++i) {
        printf("MST[%d] ", i);
          
        list< pair<int, int> >::iterator itr = MST[i].begin();
          
        while (itr != MST[i].end()) {
            printf(" -> %d(%d)", (*itr).first, (*itr).second);
            ++itr;
        }
        printf("\n");
    }
      
    return 0;
}