#include <iostream>
#include <limits.h>
#include <queue>
#include <string.h>
using namespace std;

#define V 100 // Maximum number of vertices

const long long INF = LLONG_MAX; // Infinity

// Returns true if there is a path from source 's' to sink
// 't' in residual graph. Also fills parent[] to store the
// path
bool bfs(long long rGraph[V][V], int s, int t, int parent[], int n)
{
    bool visited[V];
    memset(visited, 0, sizeof(visited));

    queue<int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; v++) {
            if (visited[v] == false && rGraph[u][v] > 0) {
                if (v == t) {
                    parent[v] = u;
                    return true;
                }
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }

    return false;
}

// Returns the maximum flow from s to t in the given graph
long long fordFulkerson(long long graph[V][V], int s, int t, int n)
{
    int u, v;
    long long rGraph[V][V];

    for (u = 0; u < n; u++)
        for (v = 0; v < n; v++)
            rGraph[u][v] = graph[u][v];

    int parent[V];
    long long max_flow = 0;

    while (bfs(rGraph, s, t, parent, n)) {
        long long path_flow = INF;
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }

        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }

        max_flow += path_flow;
    }

    return max_flow;
}

int main()
{
    int n, m;
    cin >> n >> m;
    int s, t;
    cin >> s >> t;

    long long graph[V][V];
    memset(graph, 0, sizeof(graph));

    for (int i = 0; i < m; i++) {
        int u, v, c;
        cin >> u >> v >> c;
        graph[u][v] += c;
        graph[v][u] += c; // For undirected graph
    }

    cout << fordFulkerson(graph, s, t, n);

    return 0;
}

