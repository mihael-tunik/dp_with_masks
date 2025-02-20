#include <iostream>
#include <algorithm>
#include <vector>

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

int min_sum_2d_dp(vector <vector <int>> &a){
    int n = a.size(), INF = 1e9;

    vector <int> dp(1 << n, 0);
    vector <int> choice(1 << n, 0);
    vector <int> init(1 << n, 0);
    
    //printf(":: %llu ", dp.size());

    init[0] = 1;

    for (int k = 0; k <= n; ++k) {
        for (int mask = (1<<k)-1; mask < (1<<n); mask = next_comb(mask)) {

            //for(int i = 0; i < n; ++i)
            //    printf("%i ", (mask >> i)%2);
            //printf("\n");
            

            for(int i = 0; i < n; ++i){

                if ((mask & (1 << i)) == 0){
                    int min_in_col = INF, l = -1;
                    
                    for (int j = 0; j < n; ++j){
                        if ((choice[mask] & (1 << j)) == 0 && (a[i][j] < min_in_col)){
                            l = j;
                            min_in_col = a[i][j];
                        }
                    }
                            
                    int mask_i = mask + (1 << i); 

                    if (init[mask_i] == 0){
                        dp[mask_i] = dp[mask] + min_in_col;
                        choice[mask_i] = choice[mask] + (1 << l);
                        init[mask_i] = 1;
                    }
                    else{                    
                        if (dp[mask] + min_in_col < dp[mask_i]){
                            dp[mask_i] = dp[mask] + min_in_col;
                            choice[mask_i] = choice[mask] + (1 << l);
                        }
                    }        
                }
            }
            
            if(k == 0)
               break;
        }
    }

    printf("\nDP: %i\n", dp[(1<<n) - 1]);
    return 0;
}

int main(void){
    int n = 12;
    vector <vector <int>> a(n);
    
    printf("a: \n");

    for(int i = 0; i < n; ++i){
        a[i].resize(n);
        for(int j = 0; j < n; ++j){
            a[i][j] = 1 + rand() % 10;
            //printf("  %3i  ", a[i][j]);
        }
        //printf("\n");
    }

    min_sum_2d_naive(a);
    min_sum_2d_dp(a);

    return 0;
}
