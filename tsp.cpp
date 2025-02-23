#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <set>

#include <chrono>

using namespace std;

typedef unsigned long long ull;
int INF = 1e9;

void log(int min_s, vector <int> &argmin, vector <vector <int>> &a){
    int n = a.size();

    printf("Minimum: %i\n", min_s);
    printf("Achieved on:\n");

    for(int i = 0; i < n; ++i)
        printf("%i ", argmin[i]);
    printf("\n");

    set <pair <int, int>> opt;

    for(int i = 0; i < n; ++i)
        opt.insert({argmin[i], argmin[(i+1) % n]});

    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if(opt.find({i, j}) != opt.end())
                printf("[%3i] ", a[i][j]);
            else if(a[i][j] == INF)
                printf(" inf  ");
            else
                printf(" %3i  ", a[i][j]);
            
        }
        printf("\n");
    }
    printf("\n");
}

int tsp_naive(vector <vector <int>> &a){
    int n = a.size(), INF = 1e9;
    int min_path = INF;
    vector <int> argmin(n, -1), p(n);

    for(int i = 0; i < n; ++i){ p[i] = i; }
    
    ull cnt = 0;

    do{
        int s = 0;
        for(int i = 0; i < n; ++i)
           s += a[p[i]][p[(i+1)%n]];
        
        if (min_path > s){
            min_path = s;
            argmin = p;
        }
        ++cnt;
    }while(next_permutation(p.begin(), p.end()));

    // logs: 
    printf("%llu combinations were checked\n", cnt);
    
    log(min_path, argmin, a);
    return min_path;
}

inline int next_comb(int v) {
    int t = v | (v - 1);
    return (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctz(v) + 1));
}


// O(n^2 2^n) time and O(n 2^n) space
int tsp_dp(vector <vector <int>> &a){
    int n = a.size(), min_path = INF;    
    vector <vector<int>> dp(n);
    
    for (int k = 0; k < n; ++k)
       dp[k].resize((1 << n), INF);
    
    //dp[k][mask] - minimum path through vertices in mask from k to 0 vertice
    dp[0][0] = 0;
    
    for (int k = 1; k <= n; ++k)
        for (int mask = (1<<k)-1; mask < (1<<n); mask = next_comb(mask)) {
            for(int i = 0; i < n; ++i)
                for(int j = 0; j < n; ++j)
                    if (mask & (1 << j) && i != j)
                        dp[i][mask] = min(dp[i][mask], dp[j][mask ^ (1 << j)] + a[j][i]);

            if(mask == 0)
                break;
        }
    
   
    min_path = dp[0][(1<<n) - 1];

    vector <int> argmin(n, -1);
    int mask = (1<<n)-1, state, index = 0;

    argmin[0] = index;

    for(int k = 1; k < n; ++k)
        for(int i = 0; i < n; ++i)
            if (mask & (1 << i)){ 
                state = mask & ~(1 << i);                
                if(dp[i][state] + a[i][index] == dp[index][mask]){                     
                     index = i, mask = state;
                     argmin[k] = index;
                     break;
                }
            }

    log(min_path, argmin, a);
    return 0;
}

int main(void){
    int n = 12;
    vector <vector <int>> a(n);
    
    srand(time(NULL));

    for(int i = 0; i < n; ++i)
        a[i].resize(n, INF);

    for(int i = 0; i < n; ++i)
        for(int j = i+1; j < n; ++j)
            a[j][i] = a[i][j] = 1 + rand() % 50;
   
    auto s1 = chrono::high_resolution_clock::now();
    tsp_naive(a);
    auto s2 = chrono::high_resolution_clock::now();
    
    printf("Ready in %lf s.\n", chrono::duration<double, milli>(s2-s1).count()/1000);     

    auto s3 = chrono::high_resolution_clock::now();
    tsp_dp(a);
    auto s4 = chrono::high_resolution_clock::now();
    
    printf("Ready in %lf s.\n", chrono::duration<double, milli>(s4-s3).count()/1000);  

    return 0;
}
