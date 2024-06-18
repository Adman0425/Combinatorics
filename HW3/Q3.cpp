#include <iostream>
#include <limits.h>
#include <string.h>
#include <queue>
#include <vector>
using namespace std;

#define V 100 // Maximum number of vertices

const long long INF = LLONG_MAX; // Infinity

// Returns true if there is a path from source 's' to sink 't' in
// residual graph. Also fills parent[] to store the path
bool bfs(long long rGraph[V][V], int s, int t, int parent[], int n)
{
    // Create a visited array and mark all vertices as not visited
    bool visited[V];
    memset(visited, 0, sizeof(visited));

    // Create a queue, enqueue source vertex and mark source vertex
    // as visited
    queue<int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    // Standard BFS Loop
    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; v++)
        {
            if (visited[v] == false && rGraph[u][v] > 0)
            {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }

    // If we reached sink in BFS starting from source, then return
    // true, else false
    return (visited[t] == true);
}

// Computes and prints the minimum s-t cut
void minCut(long long graph[V][V], int s, int t, int n)
{
    // Create a residual graph and fill the residual graph with
    // given capacities in the original graph as residual capacities
    // in residual graph
    long long rGraph[V][V]; // rGraph[i][j] indicates residual capacity of edge i-j
    for (int u = 0; u < n; u++)
        for (int v = 0; v < n; v++)
            rGraph[u][v] = graph[u][v];

    int parent[V]; // This array is filled by BFS and to store path

    // Initialize max_flow to 0
    long long max_flow = 0;

    // Augment the flow while there is a path from source to sink
    while (bfs(rGraph, s, t, parent, n))
    {
        // Find minimum residual capacity of the edges along the
        // path filled by BFS. Or we can say find the maximum flow
        // through the path found.
        long long path_flow = INF;
        for (int v = t; v != s; v = parent[v])
        {
            int u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }

        // update residual capacities of the edges and reverse edges
        // along the path
        for (int v = t; v != s; v = parent[v])
        {
            int u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }

        // Add path flow to overall flow
        max_flow += path_flow;
    }

    // Now max_flow contains the maximum flow
    cout << max_flow << endl;

    // Find vertices reachable from source (s) in residual graph
    vector<bool> reachable(n, false);
    bfs(rGraph, s, t, parent, n);
    for (int i = 0; i < n; i++)
    {
        if (parent[i] != -1)
            reachable[i] = true;
    }

    // Print the minimum s-t cut
    for (int u = 0; u < n; u++)
    {
        for (int v = 0; v < n; v++)
        {
            if (reachable[u] && !reachable[v] && graph[u][v] > 0)
            {
                cout << v << " " << u << endl;
            }
        }
    }
}

int main()
{
    int n, m;
    cin >> n >> m;
    int s, t;
    cin >> s >> t;

    // Initialize graph with 0 capacity
    long long graph[V][V] = {{0}};

    // Input edge capacities
    for (int i = 0; i < m; i++)
    {
        int u, v, c;
        cin >> u >> v >> c;
        graph[u][v] += c;
        graph[v][u] += c; // For undirected graph
    }

    minCut(graph, s, t, n);

    return 0;
}

