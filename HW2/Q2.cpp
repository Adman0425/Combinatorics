#include<bits/stdc++.h>
using namespace std;

class Solution {
public:

    int cost[505][505]; // Maximum size as per constraints
    int n, max_match; // n workers and n jobs
    int lx[505], ly[505]; // Labels of X and Y parts
    int xy[505]; // xy[x] - vertex that is matched with x,
    int yx[505]; // yx[y] - vertex that is matched with y
    bool S[505], T[505]; // Sets S and T in algorithm
    int slack[505]; // As in the algorithm description
    int slackx[505]; // slackx[y] such a vertex, that
    int prev_ious[505]; // Array for memorizing alternating p

    void init_labels()
    {
        memset(lx, 0, sizeof(lx));
        memset(ly, 0, sizeof(ly));
        for (int x = 0; x < n; x++)
            for (int y = 0; y < n; y++)
                lx[x] = max(lx[x], cost[x][y]);
    }


    void update_labels()
    {
        int x, y;
        int delta = INT_MAX; // Initialize delta as maximum integer value
        for (y = 0; y < n; y++) // Calculate delta using slack
            if (!T[y])
                delta = min(delta, slack[y]);
        for (x = 0; x < n; x++) // Update X labels
            if (S[x])
                lx[x] -= delta;
        for (y = 0; y < n; y++) // Update Y labels
            if (T[y])
                ly[y] += delta;
        for (y = 0; y < n; y++) // Update slack array
            if (!T[y])
                slack[y] -= delta;
    }


    void add_to_tree(int x, int prev_iousx)
    {
        S[x] = true; // Add x to S
        prev_ious[x] = prev_iousx; // We need this when augmenting
        for (int y = 0; y < n; y++) // Update slacks, because we add new vertex to S
            if (lx[x] + ly[y] - cost[x][y] < slack[y])
            {
                slack[y] = lx[x] + ly[y] - cost[x][y];
                slackx[y] = x;
            }
    }




    void augment() // Main function of the algorithm
    {
        if (max_match == n) return; // Check whether matching is already perfect
        int x, y, root; // Just counters and root vertex
        int q[505], wr = 0, rd = 0; // q - queue for bfs, wr, rd - write and read
        // Pos in queue
        memset(S, false, sizeof(S)); // Init set S
        memset(T, false, sizeof(T)); // Init set T
        memset(prev_ious, -1, sizeof(prev_ious)); // Init set prev_ious - for the alternating tree

        for (x = 0; x < n; x++) // Finding root of the tree
        {
            if (xy[x] == -1)
            {
                q[wr++] = root = x;
                prev_ious[x] = -2;
                S[x] = true;
                break;
            }
        }

        for (y = 0; y < n; y++) // Initializing slack array
        {
            slack[y] = lx[root] + ly[y] - cost[root][y];
            slackx[y] = root;
        }

        // Second part of augment() function
        while (true) // Main cycle
        {
            while (rd < wr) // Building tree with bfs cycle
            {
                x = q[rd++]; // Current vertex from X part
                for (y = 0; y < n; y++) // Iterate through all edges in equality graph
                    if (cost[x][y] == lx[x] + ly[y] && !T[y])
                    {
                        if (yx[y] == -1) break; // An exposed vertex in Y found, so
                        // Augmenting path exists!
                        T[y] = true; // Else just add y to T,
                        q[wr++] = yx[y]; // Add vertex yx[y], which is matched
                        // With y, to the queue
                        add_to_tree(yx[y], x); // Add edges (x,y) and (y,yx[y]) to the tree
                    }
                if (y < n)
                    break; // Augmenting path found!
            }
            if (y < n)
                break; // Augmenting path found!

            update_labels(); // Augmenting path not found, so improve labeling

            wr = rd = 0;
            for (y = 0; y < n; y++)
                // In this cycle we add edges that were added to the equality graph as a
                // Result of improving the labeling, we add edge (slackx[y], y) to the tree if
                // And only if !T[y] && slack[y] == 0, also with this edge we add another one
                // (y, yx[y]) or augment the matching, if y was exposed
                if (!T[y] && slack[y] == 0)
                {
                    if (yx[y] == -1) // Exposed vertex in Y found - augmenting path exists!
                    {
                        x = slackx[y];
                        break;
                    }
                    else
                    {
                        T[y] = true; // Else just add y to T,
                        if (!S[yx[y]])
                        {
                            q[wr++] = yx[y]; // Add vertex yx[y], which is matched with
                            // y, to the queue
                            add_to_tree(yx[y], slackx[y]); // And add edges (x,y) and (y,
                            // yx[y]) to the tree
                        }
                    }
                }
            if (y < n) break; // Augmenting path found!
        }

        if (y < n) // We found augmenting path!
        {
            max_match++; // Increment matching
            // In this cycle we inverse edges along augmenting path
            for (int cx = x, cy = y, ty; cx != -2; cx = prev_ious[cx], cy = ty)
            {
                ty = xy[cx];
                yx[cy] = cx;
                xy[cx] = cy;
            }
            augment(); // Recall function, go to step 1 of the algorithm
        }
    } // End of augment() function

    int hungarian()
    {
        int ret = 0; // Weight of the optimal matching
        max_match = 0; // Number of vertices in current matching
        memset(xy, -1, sizeof(xy));
        memset(yx, -1, sizeof(yx));
        init_labels(); // Step 0
        augment(); // Steps 1-3

        for (int x = 0; x < n; x++) // Forming answer there
            ret += cost[x][xy[x]];

        return ret;
    }

    void printMatching()
    {
        for (int x = 0; x < n; x++)
        {
            if (xy[x] != -1) {
                cout << x << " " << xy[x] << endl;
            }
        }
    }

    void assignmentProblem(int Arr[][505], int N) {

        n = N;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                cost[i][j] = Arr[i][j];

        int ans = hungarian();

        cout << ans << endl;
        printMatching();
    }
};

int main()
{
    int n;
    cin >> n;
    int Arr[505][505];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> Arr[i][j];

    Solution ob;
    ob.assignmentProblem(Arr, n);

    return 0;
}

