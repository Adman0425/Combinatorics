#include <iostream>
#include <algorithm>
using namespace std;
#define fo(i,n) for(int i=0,_n=(n);i<_n;i++)

int n,delta,w[300][300],l[300],r[300],rm[300];
bool ul[300], ur[300], m[300][300];
const int oo = 2000000;

void relax() {
    int delta = oo;
    fo(i,n) if(ul[i]) fo(j,n) if(!ur[j]) delta = min(delta, l[i]+r[j]-w[i][j]);
    fo(i,n) if(ul[i]) l[i] -= delta;
    fo(i,n) if(ur[i]) r[i] += delta;
}

#define UNMATCHED -1
bool DFS(int node) {
    ul[node] = true;
    fo(i,n) if(!ur[i] && node != rm[i]) if(w[node][i] == l[node]+r[i]) {
        if(rm[i]==UNMATCHED) {
            m[node][i] = true;
            rm[i] = node;
            return true;
        } else {
            ur[i] = true;
            if(DFS(rm[i])) {
                m[node][i] = true;
                m[rm[i]][i] = false;
                if(m[node][i]) rm[i] = node;
                return true;
            }
        }
    }
    return false;
}


// Input:
// N
// (N lines with N integers, specifiying the weights.)
//
// Output: One integer, the value of the maximum weight matching.

int main() {
    cin >> n;
    fo(i,n) fo(j,n) cin >> w[i][j];
    fo(i,n) l[i] = r[i] = oo, rm[i] = UNMATCHED;
  
    fo(i,n) {
        fo(j,n) ul[j] = ur[j] = false;
        while(!DFS(i)) {
            relax();
            fo(j,n) ul[j] = ur[j] = false;
        }
    }
  
    int tot = 0;
    fo(i,n) fo(j,n) tot += m[i][j] ? w[i][j] : 0;
    cout << tot;
}
