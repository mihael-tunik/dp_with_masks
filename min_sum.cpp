#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include "profile.h"

using namespace std;

typedef unsigned long long ull;

void log(int min_s, vector <int> &argmin, vector <vector <int>> &a){
    int n = a.size();

    printf("Minimum: %i\n", min_s);
    printf("Achieved on:\n");

    for(int i = 0; i < n; ++i)
        printf("%i ", argmin[i]);
    printf("\n");

    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if(j == argmin[i]){
                printf("[%3i] ", a[i][j]);
            }else{
                printf(" %3i  ", a[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

int min_sum_2d_naive(vector <vector <int>> &a){
    int n = a.size(), INF = 1e9;
    int min_s = INF;
    vector <int> argmin(n, -1), p(n);

    for(int i = 0; i < n; ++i){ p[i] = i; }
    
    ull cnt = 0;

    do{
        int s = 0;
        for(int i = 0; i < n; ++i)
           s += a[i][p[i]];
        
        if (min_s > s){
            min_s = s;
            argmin = p;
        }
        ++cnt;
    }while(next_permutation(p.begin(), p.end()));

    // logs: 
    printf("%llu combinations were checked\n", cnt);
    
    log(min_s, argmin, a);
    return min_s;
}

inline int next_comb(int v) {
    int t = v | (v - 1);
    return (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctz(v) + 1));
}

// O(n^2 2^n) time and O(2^n) space
int min_sum_2d_dp(vector <vector <int>> &a){
    int n = a.size(), INF = 1e9;
    vector <int> dp(1 << n, 0);
    vector <int> choice(1 << n, 0);

    for (int k = 0; k <= n; ++k) {
        for (int mask = (1<<k)-1; mask < (1<<n); mask = next_comb(mask)) {
            for(int i = 0; i < n; ++i){
                if ((mask & (1 << i)) == 0){
                    int min_in_col = INF, l = -1, mask_i = mask + (1 << i);
                    
                    for (int j = 0; j < n; ++j)
                        if ((choice[mask] & (1 << j)) == 0 && (a[i][j] < min_in_col))
                            l = j, min_in_col = a[i][j];
                            
                    if (choice[mask_i] == 0 || dp[mask] + min_in_col < dp[mask_i] ){
                        dp[mask_i] = dp[mask] + min_in_col;
                        choice[mask_i] = choice[mask] + (1 << l);
                    }    
                }
            }            
            if(mask == 0)
               break;
        }
    }

    vector <int> argmin(n,-1);
    int mask = (1<<n)-1, state, index;

    for(int k = 0; k < n; ++k){
        for(int i = 0; i < n; ++i){
            if (mask & (1<<i)){ 
                state = mask & ~(1<<i);
                index = 8 * sizeof(int) - __builtin_clz(choice[mask] - choice[state]) - 1; 
                
                if(dp[state] + a[i][index] == dp[mask]){
                     mask = state;
                     argmin[i] = index;
                     break;    
                }
            }
        }
    }

    log(dp[(1<<n) - 1], argmin, a);
    return 0;
}

// O(n 2^n) time and O(2^n) space
int min_sum_2d_dp_fast(vector <vector <int>> &a){
    int n = a.size(), INF = 1e9;
    
    vector <int> dp(1<<n, INF);
    dp[0] = 0;

    for (int k = 1; k <= n; ++k)
        for (int mask = (1<<k)-1; mask < (1<<n); mask = next_comb(mask))
            for(int i = 0; i < n; ++i)
                if (mask & (1 << i))
                    dp[mask] = min(dp[mask], dp[mask ^ (1<<i)] + a[k-1][i]);

    printf("Minimum: %i\n", dp[(1 << n) - 1]);
    return dp[(1 << n) - 1];
}


int main(void){
    int n = 23;
    vector <vector <int>> a(n);
    
    srand(time(NULL));

    for(int i = 0; i < n; ++i){
        a[i].resize(n);
        for(int j = 0; j < n; ++j)
            a[i][j] = 1 + rand() % 50;
    }
   
    //profile(min_sum_2d_naive, a);
    profile(min_sum_2d_dp, a);
    profile(min_sum_2d_dp_fast, a);

    return 0;
}
