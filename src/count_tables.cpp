#include <stdio.h>
#include <vector>
#include <set>
#include <string>
#include <map>
#include <math.h>
#include <algorithm>
#include <iostream>

#include "profile.h"

using namespace std;

typedef vector <vector <int>> vvi;

template <typename... Ts>
void print(Ts... args){ ((cout << args << " "), ...); cout << endl; }

template <typename T>
void print(vector <vector<T>> &a){ 
    for(int i = 0; i < a.size(); ++i){
        for(int j = 0; j < a[i].size(); ++j)
            //cout << a[i][j] << " ";
            printf(" %3i ", a[i][j]);
        cout << endl;
    }
}

template <typename S, typename T>
void print(map <S, T> &m){ for(auto &[k, v] : m) cout << "{" << k << " : " << v << "} "; cout << endl; }

template <typename S, typename T>
bool in(map <S, T> &m, S k){ return m.find(k) != m.end(); }

void table_count_v1(vvi &a, int &target){
    int sol = 0, n = a.size(), m = a[0].size();

    for(int mx = 1; mx < 1<<n; ++mx){
        for(int my = 1; my < 1<<m; ++my){
            int s = 0;

            for(int i = 0; i < n; ++i){
                for(int j = 0; j < n; ++j){
                    if((mx & (1 << i)) && (my & (1 << j)) )
                       s += a[i][j];
                }
            }
            
            if(s == target)
                sol++;
        }
    }

    print(sol);
}

void table_count_v2(vvi &a, int &target){
    int sol = 0, n = a.size(), m = a[0].size();

    for(int mx = 1; mx < 1<<n; ++mx){
        for(int my = 1; my < 1<<m; ++my){
            int s = 0;

            for(int x = mx, i = 0; (i = __builtin_ctz(x)) < n; x ^= 1<<i)
                for(int y = my, j = 0; (j = __builtin_ctz(y)) < m; y ^= 1<<j)
                    s += a[i][j];
            
            if(s == target)
                sol++;
        }
    }

    print(sol);
}

inline int next_comb(int v) {
    int t = v | (v - 1);
    return (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctz(v) + 1));
}

void table_count_v3(vvi &a, int &target){
    int sol = 0, n = a.size(), m = a[0].size();
    vvi dp;

    dp.resize((1<<n));
    for(int i = 0; i < (1<<n); ++i)
        dp[i].resize((1<<m), 0);

    dp[0][0] = 0;

    for(int i = 0; i < n; ++i)
        for(int j = 0; j < m; ++j)
            dp[1<<i][1<<j] = a[i][j];

    for (int k = 1; k <= n; ++k)
        for (int mask_x = (1<<k)-1; mask_x < (1<<n); mask_x = next_comb(mask_x))
            for (int l = 1; l <= m; ++l)
                for (int mask_y = (1<<l)-1; mask_y < (1<<m); mask_y = next_comb(mask_y)){
                    int ii = 1 << (__builtin_ffs(mask_x)-1), jj = 1 << (__builtin_ffs(mask_y)-1);
                    dp[mask_x][mask_y] = dp[mask_x ^ ii][mask_y ^ jj] + dp[ii][mask_y ^ jj] + 
                                         dp[mask_x ^ ii][jj] + dp[ii][jj]; 
                    // y v v v v
                    // u x x x x
                    // u x x x x
                    // u x x x x
                    if(dp[mask_x][mask_y] == target)
                        ++sol;
                }
    
    print(sol);
}

struct count_t{
    int value = 0;
};

void compute_dp(vvi &a, vvi &dp, vector<map <int, count_t>> &count){
    int n = a.size(), m = a[0].size();

    count[0][0].value = 1<<m;
    for(int mask_x = 1; mask_x < (1<<n); ++mask_x)
        count[mask_x][0].value = 1;

    for (int k = 1; k <= n; ++k)
        for (int mask_x = (1<<k)-1; mask_x < (1<<n); mask_x = next_comb(mask_x))
            for (int l = 1; l <= m; ++l)
                for (int mask_y = (1<<l)-1; mask_y < (1<<m); mask_y = next_comb(mask_y)){
                    int i = (__builtin_ffs(mask_x)-1), j = (__builtin_ffs(mask_y)-1);
                    int ii = 1 << i, jj = 1 << j;
                    dp[mask_x][mask_y] = dp[mask_x ^ ii][mask_y ^ jj] + dp[ii][mask_y ^ jj] + 
                                         dp[mask_x ^ ii][jj] + a[i][j];
                    int t = dp[mask_x][mask_y];
                    count[mask_x][t].value = count[mask_x][t].value + 1;
                }
}

void table_count_meet_in_the_middle(vvi &a, int &target){
    int sol = 0, n = a.size(), m = a[0].size();
    vvi dp_l, dp_r, a_l, a_r;

    dp_l.resize((1<<n)), dp_r.resize((1<<n));

    int ml = (m+1)/2, mr = m/2;

    for(int i = 0; i < (1<<n); ++i)
        dp_l[i].resize((1<<ml), 0), dp_r[i].resize((1<<mr), 0);
    a_l.resize(n), a_r.resize(n);

    for(int i = 0; i < n; ++i)
        for(int j = 0; j < m; ++j)
            if(j < (m+1)/2)
                a_l[i].push_back(a[i][j]);
            else
                a_r[i].push_back(a[i][j]);


    vector <map <int, count_t>> count_1, count_2; // sum -> count

    count_1.resize(1 << n);
    count_2.resize(1 << n);

    compute_dp(a_l, dp_l, count_1);
    compute_dp(a_r, dp_r, count_2);
    
    int ans = 0;

    for(int mask_x = 0; mask_x < (1 << n); ++mask_x)
        for(auto &[sum, cnt] : count_1[mask_x])
            if(in(count_2[mask_x], target - sum))
                ans += count_1[mask_x][sum].value * count_2[mask_x][target - sum].value;

    print(ans);
}

int main(void){
    //read();
    vvi a;
    int n = 15, m = 15, target = 20;

    a.resize(n);

    for(int i = 0; i < n; ++i){
        a[i].resize(m);
        for(int j = 0; j < n; ++j)
           a[i][j] = rand() % 3 + 1;
    }

    print(a);
    
    //profile(table_count_v1, a, target); // 299s
    //profile(table_count_v2, a, target); // 37s
    profile(table_count_v3, a, target); // 5s
    profile(table_count_meet_in_the_middle, a, target); // < 1s

    return 0;
}
