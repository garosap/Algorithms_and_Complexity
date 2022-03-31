// Sources:
// https://web.stanford.edu/class/archive/cs/cs166/cs166.1146/lectures/04/Small04.pdf
// https://www.geeksforgeeks.org/kruskals-minimum-spanning-tree-using-stl-in-c/
// https://www.topcoder.com/community/data-science/data-science-tutorials/range-minimum-query-and-lowest-common-ancestor/

#include <iostream>
#include <bits/stdc++.h>

using namespace std;

// Cartesian tree struct
typedef struct
{
    int id;
    int value;
    int parent;
    int left = 0;
    int right = 0;
} Node;

// Graph edge struct
typedef struct
{
    int u, v;
    int w;
} edge;

// Returns true if edge e1 has smaller weight that e2
bool compare_edge(edge e1, edge e2)
{
    return e1.w < e2.w;
}

// Union find struct
struct UnionFind
{
    int n;
    int *parent, *rankArray;

    UnionFind()
    {
        this->n = 0;
    }

    UnionFind(int n)
    {
        this->n = n;
        parent = new int[n + 1];
        rankArray = new int[n + 1];

        // Initialize nodes with rank 0 and parent the node itself
        for (int i = 0; i <= n; i++)
        {
            rankArray[i] = 0;
            parent[i] = i;
        }
    }

    // find with path compression
    int find(int u)
    {
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }

    // union
    void unionByRank(int a, int b)
    {
        a = find(a);
        b = find(b);

        // union by rank
        if (rankArray[a] > rankArray[b])
            parent[b] = a;
        else
            parent[a] = b;

        if (rankArray[a] == rankArray[b])
            rankArray[b]++;
    }
};

// Graph class
class Graph
{
    // adjacency list
    vector<vector<int>> adjList;

public:
    int V, E;
    vector<edge> edges;

    // constructors
    Graph() {}
    Graph(int V, int E);

    void initialize(int E);
    void addEdge(int u, int v, int d);
    void kruskalMST(vector<edge> edgeList);
};

// Graph constructor
Graph::Graph(int V, int E)
{
    this->V = V;
    this->E = E;

    adjList = vector<vector<int>>(V + 1, vector<int>());
}

// Add edge to graph
void Graph::addEdge(int u, int v, int d)
{
    adjList[u].push_back(v);
    adjList[v].push_back(u);

    edges.push_back({u, v, d});
}

// Initialize graph from input
void Graph::initialize(int E)
{
    int u, v, d;

    for (int i = 0; i < E; i++)
    {
        scanf("%d %d %d", &u, &v, &d);

        addEdge(u, v, d);
    }
}

// Find MST using Kruskal's Algorithm
void Graph::kruskalMST(vector<edge> edgeList)
{
    // Sort edges
    sort(edgeList.begin(), edgeList.end(), compare_edge);

    // Create union find
    UnionFind unionFind(V);

    vector<edge>::iterator it;

    for (it = edgeList.begin(); it != edgeList.end(); it++)
    {
        int u = it->u;
        int v = it->v;

        int set_u = unionFind.find(u);
        int set_v = unionFind.find(v);

        // Check if the selected edge is creating
        // a cycle or not (Cycle is created if u
        // and v belong to same set)
        if (set_u != set_v)
        {
            // Current edge will be in the MST
            // so add it
            addEdge(u, v, it->w);

            // unionByRank the two sets
            unionFind.unionByRank(set_u, set_v);
        }
    }
}

// Class to hold info about the parent of every node of union-find
class Parent
{
public:
    vector<int> parent;
    UnionFind unionFind;

    Parent(int V)
    {
        parent = vector<int>(V + 1);
        for (int i = 1; i <= V; i++)
        {
            parent[i] = i;
        }

        UnionFind unionFind(V);
        this->unionFind = unionFind;
    }

    void set_parent(int u, int v, int p)
    {
        // When merging make sure the new parent if both
        // nodes will be p
        unionFind.unionByRank(u, v);
        parent[unionFind.find(u)] = p;
    }

    int get(int u)
    {
        // The parent of the Node will be the
        // parent of the parent Node of u
        return parent[unionFind.find(u)];
    }
};

// class that inputs questions and outputs answers
class QuestionSolver
{
    int V, E;
    Graph mst;
    Node *cartesianTree;
    int *Euler;
    int *L;
    int *H;
    int **M;

public:
    QuestionSolver(Graph mst);
    void mstToCartesian();
    void eulerTour(int node, int level, int *cnt);
    void preprocessQueries(int *A, int N);
    int solveRMQ(int *L, int i, int j);
    int query(int i, int j);
};

// constructor from graph
QuestionSolver::QuestionSolver(Graph mst)
{
    this->mst = mst;
    this->V = mst.V;
    this->E = mst.E;

    this->cartesianTree = (Node *)malloc((V + E + 1) * sizeof(Node));

    // Convert MST to cartesian
    mstToCartesian();

    // Prepare the arrays for the Euler Tour
    this->Euler = (int *)malloc(2 * (V + E) * sizeof(int));
    this->L = (int *)malloc(2 * (V + E) * sizeof(int));
    this->H = (int *)malloc(2 * (V + E) * sizeof(int));

    // initialize everything to 0
    for (int i = 0; i < 2 * (V + E); i++)
    {
        Euler[i] = L[i] = H[i] = 0;
    }

    // Euler Tour
    int cnt = 0;
    eulerTour(V + E, 0, &cnt);

    int maxVal = 2 * (V + E);

    this->M = (int **)malloc(maxVal * sizeof(int *));

    for (int i = 0; i < maxVal; i++)
    {
        M[i] = (int *)malloc(ceil(log2(maxVal)) * sizeof(int));
    }

    // Sparce Matrix
    preprocessQueries(L, 2 * (E + V));
}

// Convert MST to Cartesian tree
void QuestionSolver::mstToCartesian()
{
    sort(mst.edges.begin(), mst.edges.end(), compare_edge);

    // Initialize the vector containing the nodes
    for (int i = 1; i <= V + E; i++)
    {
        Node temp;

        temp.id = i;
        temp.value = i;
        temp.left = 0;
        temp.right = 0;
        temp.parent = 0;

        cartesianTree[i] = temp;
    }

    // initialize parents
    Parent parents(V);

    for (int i = V + 1; i <= V + E; i++)
    {
        edge minEdge = mst.edges[i - V - 1];
        int u = minEdge.u, v = minEdge.v;

        cartesianTree[i].left = parents.get(u);
        cartesianTree[i].right = parents.get(v);
        cartesianTree[i].value = minEdge.w;

        cartesianTree[cartesianTree[i].left].parent = i;
        cartesianTree[cartesianTree[i].right].parent = i;

        parents.set_parent(u, v, i);
    }
}

// Euler tour
void QuestionSolver::eulerTour(int p, int level, int *cnt)
{
    (*cnt)++;

    // Update values
    Euler[*cnt] = p;
    L[*cnt] = level;
    if (H[p] == 0)
    {
        H[p] = *cnt;
    }

    // Continue DFS
    if (cartesianTree[p].left != 0)
    {
        eulerTour(cartesianTree[p].left, level + 1, cnt);

        (*cnt)++;
        Euler[*cnt] = p;
        L[*cnt] = level;
    }

    if (cartesianTree[p].right != 0)
    {
        eulerTour(cartesianTree[p].right, level + 1, cnt);

        (*cnt)++;
        Euler[*cnt] = p;
        L[*cnt] = level;
    }
}

// Prepare solveRMQ solution sparse matrix
void QuestionSolver::preprocessQueries(int *A, int N)
{
    int i, j;

    for (i = 0; i < N; i++)
        M[i][0] = i;

    for (j = 1; 1 << j <= N; j++)
        for (i = 0; i + (1 << j) - 1 < N; i++)
            if (A[M[i][j - 1]] < A[M[i + (1 << (j - 1))][j - 1]])
                M[i][j] = M[i][j - 1];
            else
                M[i][j] = M[i + (1 << (j - 1))][j - 1];
}

// Return bottleneck of path between u, v
int QuestionSolver::solveRMQ(int *A, int u, int v)
{

    int k = log2(v - u + 1);
    if (A[M[u][k]] <= A[M[v - (int)pow(2, k) + 1][k]])
        return M[u][k];
    else
        return M[v - (int)pow(2, k) + 1][k];
}

int QuestionSolver::query(int u, int v)
{
    return cartesianTree[Euler[solveRMQ(L, min(H[u], H[v]), max(H[u], H[v]))]].value;
}

int main()
{
    int V, E, Q, u, v, query;
    scanf("%d", &V);
    scanf("%d", &E);


    // initialize the Input Graph
    Graph g(V, E), mst(V, V - 1);
    g.initialize(E);

    // Calculate MST
    mst.kruskalMST(g.edges);

    QuestionSolver qSolver(mst);

    scanf("%d", &Q);
    for (int i = 0; i < Q; i++)
    {
        scanf("%d", &u);
        scanf("%d", &v);
        query = qSolver.query(u, v);
        cout << query << endl;
    }

    return 0;
}