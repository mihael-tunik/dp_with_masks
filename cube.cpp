#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <cstring>

using namespace std;

typedef unsigned long long ull;

int min_sum_3d_naive(vector <vector <vector <int>>> &c){
    int n = c.size(), INF = 1e9;
    int min_s = INF;
    ull cnt = 0;
        
    vector <int> argmin_y(n, -1), argmin_z(n, -1), y(n), z(n);
    for(int i = 0; i < n; ++i){ y[i] = i; }
    
    do{
        for(int i = 0; i < n; ++i){ z[i] = i; }
        do{
            int s = 0;
            for(int i = 0; i < n; ++i)
                s += c[i][y[i]][z[i]];        
            if (min_s > s){
                min_s = s;
                argmin_y = y;
                argmin_z = z;
            }
            ++cnt;
        }while(next_permutation(z.begin(), z.end()));
    }while(next_permutation(y.begin(), y.end()));

    printf("%llu combinations were checked\n", cnt);
    
    for(int i = 0; i < n; ++i)
        printf("(%i %i %i) = %i\n", i, argmin_y[i], argmin_z[i], c[i][argmin_y[i]][argmin_z[i]]);
    
    printf("Minimum: %i\n", min_s);
    return min_s;
}

inline int next_comb(int v) {
    int t = v | (v - 1);
    return (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctz(v) + 1));
}

int min_sum_3d_dp(vector <vector <vector <int>>> &c){
    int n = c.size(), INF = 1e9;
    vector <vector <int>> dp(1<<n);
    for(int i = 0; i < (1<<n); ++i)
        dp[i].resize(1<<n, INF);
    
    dp[0][0] = 0;
    
    for (int k = 1; k <= n; ++k)
        for (int mask_y = (1<<k)-1; mask_y < (1<<n); mask_y = next_comb(mask_y))
            for (int mask_z = (1<<k)-1; mask_z < (1<<n); mask_z = next_comb(mask_z))
                for(int i = 0; i < n; ++i)
                    for(int j = 0; j < n; ++j)
                        if ( (mask_y & (1<<i)) && (mask_z & (1<<j)) )
                            dp[mask_y][mask_z] = min(dp[mask_y][mask_z], dp[mask_y ^ (1 << i)][mask_z ^ (1 << j)] + c[k-1][i][j]);

    printf("Minimum: %i\n", dp[(1 << n) - 1][(1 << n) - 1]);
    return dp[(1 << n) - 1][(1 << n) - 1];
}

#define _min(a, b) ((a)<(b))?(a):(b)
#define _next_comb(v)  (((v) | ((v) - 1)) + 1) | (((~((v) | ((v) - 1)) & -~((v) | ((v) - 1))) - 1) >> (__builtin_ctz((v)) + 1))

int min_sum_3d_dp_opt(vector <vector <vector <int>>> &c){
    int n = c.size(), INF = 1e9, minimum_sum;    
    // use C-arrays instead of vectors
    int **dp = (int **)malloc(sizeof(int *) * (1<<n));
    for(int i = 0; i < (1<<n); ++i){
        dp[i] = (int *)malloc(sizeof(int) * (1<<n));
        memset(dp[i], 127, sizeof(int) * (1<<n)); // 0x7f7f7f7f > INF
    }
    
    dp[0][0] = 0;
    
    for (int k = 1; k <= n; ++k)
        for (int mask_y = (1<<k)-1; mask_y < (1<<n); mask_y = _next_comb(mask_y))
            for (int mask_z = (1<<k)-1; mask_z < (1<<n); mask_z = _next_comb(mask_z))// use C macros
                for(int y = mask_y, i = 0; (i = __builtin_ctz(y)) < n; y ^= 1<<i)    // iterate only bits equal 1
                     for(int z = mask_z, j = 0; (j = __builtin_ctz(z)) < n; z ^= 1<<j)
                         dp[mask_y][mask_z] = _min(dp[mask_y][mask_z], dp[mask_y ^ (1<<i)][mask_z ^ (1<<j)] + c[k-1][i][j]);

    minimum_sum = dp[(1 << n) - 1][(1 << n) - 1];
    
    for(int i = 0; i < (1<<n); ++i)
        free(dp[i]);
    free(dp);
    
    printf("Minimum: %i\n", minimum_sum);
    return minimum_sum;
}
    
int main(void){
    int n = 15;
    vector <vector <vector <int>>> c(n);
    
    srand(time(NULL));

    for(int i = 0; i < n; ++i){
        c[i].resize(n);
        for(int j = 0; j < n; ++j){
             c[i][j].resize(n);
             for(int k = 0; k < n; ++k)
                 c[i][j][k] = 1 + rand() % 50;
        }
    }
   
    auto s1 = chrono::high_resolution_clock::now();
    //min_sum_3d_naive(c);
    auto s2 = chrono::high_resolution_clock::now();
    
    printf("Ready in %lf s.\n", chrono::duration<double, milli>(s2-s1).count()/1000);

    auto s3 = chrono::high_resolution_clock::now();
    //min_sum_3d_dp(c);
    min_sum_3d_dp_opt(c);
    auto s4 = chrono::high_resolution_clock::now();
    
    printf("Ready in %lf s.\n", chrono::duration<double, milli>(s4-s3).count()/1000);
    return 0;
}
