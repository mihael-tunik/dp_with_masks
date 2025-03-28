#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <cstring>
#include <set>

#include "profile.h"

using namespace std;

const int INF = 1e9;
vector <vector <int>> cost, cap;

int path(int s, int t, vector <int> &pi, vector <int> &par) {
    int n = pi.size();
    vector <int> dist(n, INF);
    set <pair <int, int> > Q;

    Q.insert({0, s}), dist[s] = 0;

    while (!Q.empty()) {
        auto it = Q.begin();
        int v = (*it).second;
        Q.erase(it);

        for (int u = 0; u < n; u++) {
            int w = cost[v][u] + pi[v] - pi[u];

            if (cap[v][u] > 0 && dist[u] > dist[v] + w) {
                Q.erase({dist[u], u});
                dist[u] = dist[v] + w, par[u] = v;
                Q.insert({dist[u], u});
            }
        }
    }
    pi = dist;
    return (dist[t] < INF);
}

int mincost_maxflow_matching(vector <vector <int>> &a, vector <int> &chosen){
    int n_dim = a.size();
    int n = 2 * n_dim + 2, sum = 0, flow;

    vector <int> pi(n), par(n);
    cost.resize(n), cap.resize(n), chosen.resize(n_dim);

    for(int i = 0; i < n; ++i)
        cost[i].resize(n, 0), cap[i].resize(n, 0);

    for(int i = 0; i < n_dim; ++i){
        for(int j = 0; j < n_dim; ++j){
            int r = 1 + i, c = 1 + n_dim + j;
            cost[r][c] = a[i][j], cost[c][r] = -a[i][j], cap[r][c] = 1;
        }
        cap[1 + n_dim + i][n-1] = cap[0][1 + i] = 1;
    }

    int s = 0, t = n - 1;

    while(path(s, t, pi, par)){
        flow = INF;
        for (int v = t; v != s; v = par[v])
            flow = min(flow, cap[par[v]][v]);

        for (int v = t; v != s; v = par[v]){
            cap[par[v]][v] -= flow;
            cap[v][par[v]] += flow;
            sum += cost[par[v]][v] * flow;
        }
    }

    for(int i = 0; i < n_dim; ++i)
        for(int j = 0; j < n_dim; ++j)
            if(!cap[1 + i][1 + n_dim + j])
                chosen[i] = j;

    return sum;
}

inline int next_comb(int v) {
    int t = v | (v - 1);
    return (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctz(v) + 1));
}

int min_sum_2d_dp_fast(vector <vector <int>> &a, vector <int> &chosen){
    int n = a.size(), INF = 1e9;
    
    vector <int> dp(1<<n, INF);
    dp[0] = 0;

    for (int k = 1; k <= n; ++k)
        for (int mask = (1<<k)-1; mask < (1<<n); mask = next_comb(mask))
            for(int i = 0; i < n; ++i)
                if (mask & (1 << i))
                    dp[mask] = min(dp[mask], dp[mask ^ (1<<i)] + a[k-1][i]);

    int mask = (1 << n) - 1;

    chosen.resize(n);

    for(int k = n; k > 0; --k)
        for(int i = 0; i < n; ++i){
            if((mask & (1 << i)) && (dp[mask] == dp[mask ^ (1<<i)] + a[k-1][i])){
                mask = mask ^ (1<<i);
                chosen[k - 1] = i;
                break;
            } 
        }
    return dp[(1 << n) - 1];
}

void print(vector <int> &a){
    for(int i = 0; i < a.size(); ++i)
        cout << a[i] << " ";
    cout << endl;
}

int main(void){
    int n_dim = 1000;
    srand(time(NULL));

    vector <vector <int>> a(n_dim);
    for(int i = 0; i < n_dim; ++i)
        a[i].resize(n_dim);

    for(int i = 0; i < n_dim; ++i)
        for(int j = 0; j < n_dim; ++j)
            a[i][j] = 1 + rand() % 1000;
    
    vector <int> chosen;

    cout << profile(mincost_maxflow_matching, a, chosen) << endl;
    //print(chosen);

    //n_dim < 30
    //cout <<  profile(min_sum_2d_dp_fast, a, chosen) << endl;
    //print(chosen);
}
