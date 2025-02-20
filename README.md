DP on masks or Subset DP is a technique to decrease O(n!) complexity of algorithms.
Here's collection of nice examples. 

Problem list:

- max_sum 
Given n x n chess board with numbers. 
Place n rooks so sum on positions with maximal sum.

Solution:

```
Store two arrays: dp[2^n - 1], chosen[2^n - 1]


for k in 1..n
    for mask in combinations(k)
        choose index i not in mask (mask & (1<<i) is zero)
            choose minimum a[i][j] among columns not chosen (chosen[mask])
               update dp, chosen when necessary
```

Complexity: O(n^2 2^n) time, O(2^n) space.

Tested:
```
n  T (dp) T (n!)
12 0.6ms  3s
20 0.3s   >>
30 500s   >>
```
