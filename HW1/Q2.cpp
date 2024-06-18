#include <bits/stdc++.h>

using namespace std;

int n;
vector<int> adj[500];

int visited[500], match[500];
int sides[500]; // 0 for A, 1 for B

void recursive_sides( int u, int s )
{
    int i,j,k;

    visited[u] = 1;
    sides[u] = s;

    for(j=0;j<adj[u].size();j++)
    {
        if( !visited[adj[u][j]] )
            recursive_sides(adj[u][j],!s);
    }
}

int recursive_aug_path( int u )
{
    int i,j,k;

    visited[u] = 1;
    if( match[u] != -1 )    // in this case, the procedure is called from match[u]
        visited[match[u]] = 1;

    for(j=0;j<adj[u].size();j++)
    {
        if( match[adj[u][j]] == -1 || ( !visited[match[adj[u][j]]] && recursive_aug_path(match[adj[u][j]]) ) )
        {
            match[u] = adj[u][j];
            match[adj[u][j]] = u;

            return 1;
        }
    }

    return 0;
}

int aug_path()
{
    int i,j,k;

    for(i=0;i<n;i++)
        visited[i] = 0;

    for(i=0;i<n;i++)
        if( !sides[i] && match[i] == -1 && recursive_aug_path(i) )
            return 1;

    return 0;
}

int main()
{
    int i,j,k;

    scanf("%d%d", &n, &k);
    while(k--)
    {
        scanf("%d%d", &i, &j);
        adj[i].push_back(j);
        adj[j].push_back(i);
    }

    for(i=0;i<n;i++)
        if( !visited[i] )
            recursive_sides(i,0);

    for(i=0;i<n;i++)
        match[i] = -1;

    while(aug_path())
        ;

    for(i=0,k=0;i<n;i++)
        if( match[i] != -1 )
            k++;

    printf("%d\n", k/2);

    for(i=0;i<n;i++)
        if( match[i] != -1 && i < match[i] )
            printf("%d %d\n", i, match[i]);

    printf("%d\n", k/2);

    for(i=0;i<n;i++)    // ( visited && in B (i.e., in T) ) || ( unvisited && in A (i.e., in A-S) )
        if( match[i] != -1 && !( sides[i] ^ visited[i] ) )
                printf("%d ", i);

    printf("\n");

    return 0;
}
