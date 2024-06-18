#include <iostream>
#include <deque>
#include <cstring>
using namespace std;

const int V = 500; // Number of vertices
bool adj[V][V];    // adjacency matrix
deque<int> p[V];   // p[x] stores the alternating path from the root to vertex x.
int m[V];          // Stores the matched vertex for each vertex. -1 means unmatched.
int d[V];          // -1 for unvisited, 0 for even, 1 for odd.
int q[V], *qf, *qb; // queue, only even points are pushed.

// Set the alternating path from the root to each odd vertex in the blossom, and make odd vertices even.
// Only process one side of the blossom.
// Edge xy is a cross edge. b is the blossom base.
void label_one_side(int x, int y, int b)
{
    for (int i = b + 1; i < p[x].size(); ++i)
    {
        int z = p[x][i];
        if (d[z] == 1)
        {
            // Set the alternating path from the root to each odd vertex in the blossom.
            // Will pass through cross edges.
            p[z] = p[y];
            p[z].insert(p[z].end(), p[x].rbegin(), p[x].rend() - i);

            d[z] = 0; // Make odd vertices even in the blossom.
            *qb++ = z; // Can extend the alternating path in the future.
        }
    }
}

// Given an unmatched vertex r, build an alternating tree.
bool BFS(int r)
{
    for (int i = 0; i < V; ++i)
        p[i].clear();
    p[r].push_back(r); // Alternating tree root

    memset(d, -1, sizeof(d));
    d[r] = 0; // Root is even

    qf = qb = q;
    *qb++ = r; // Push root into the queue

    while (qf < qb)
        for (int x = *qf++, y = 0; y < V; ++y)
            if (adj[x][y] && m[y] != y) // Has neighbor, point exists.
                if (d[y] == -1)         // Unvisited point
                    if (m[y] == -1)     // Found augmenting path
                    {
                        for (int i = 0; i + 1 < p[x].size(); i += 2)
                        {
                            m[p[x][i]] = p[x][i + 1];
                            m[p[x][i + 1]] = p[x][i];
                        }
                        m[x] = y;
                        m[y] = x;
                        return true;
                    }
                    else // Extend alternating tree
                    {
                        int z = m[y];

                        p[z] = p[x];
                        p[z].push_back(y);
                        p[z].push_back(z);

                        d[y] = 1;
                        d[z] = 0;
                        *qb++ = z;
                    }
                else if (d[y] == 0) // Form a blossom
                {
                    // Find the blossom base from the two alternating paths.
                    int b = 0;
                    while (b < p[x].size() && b < p[y].size() && p[x][b] == p[y][b])
                        b++;

                    b--;

                    // Process two paths separately
                    label_one_side(x, y, b);
                    label_one_side(y, x, b);
                }
                else // Keep only one path
                    ;
    return false;
}

int matching()
{
    memset(m, -1, sizeof(m));

    int c = 0;
    for (int i = 0; i < V; ++i)
        if (m[i] == -1)
            if (BFS(i))
                c++; // Found augmenting path, increase matching count
            else
                m[i] = i; // Remove this vertex from the graph
    return c;
}

void printMatching(int n)
{
    cout << matching() << endl;
    for (int i = 0; i < n; ++i) // Print all matching edges
        if (i < m[i])
            cout << i << ' ' << m[i] << endl;
}

int main()
{
    int n, m;
    cin >> n >> m;

    for (int i = 0; i < m; ++i)
    {
        int u, v;
        cin >> u >> v;
        adj[u][v] = adj[v][u] = true;
    }

    printMatching(n);

    return 0;
}

