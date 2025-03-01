# About
DP on masks or subset DP is a technique to decrease complexity of algorithms with permutational structure.

Here's my collection of nice examples. 

Problem list:

### **tsp**:
> Given distances between cities find closed tour through all of them with minimal length.

This is NP-hard problem and its DP solution is based on masks.

Tested (_tsp.cpp_):
```
n  T (dp) T (n!)
12 1ms    7s
20 0.7s   >>
```

Solution complexity: $O(n^2 \cdot 2^n)$ time, $O(n \cdot 2^n)$ space.

### **min_sum**:

> Given $n$ x $n$ chess board with $n^2$ integer numbers. 
Place $n$ rooks so they don't attack each other and sum of their positions would be minimal.

Actually it can be solved in polynomial time by one of the matching algorithms. It is surprizing because **min_sum** and **tsp** look so similar (given some matrix, need to find certain permutation of indexes).

Anyway, DP solution:

```
Store two arrays: dp[2^n - 1], chosen[2^n - 1]

for k in 1..n
    for mask in combinations(k)
        choose index i not in mask (mask & (1<<i) is zero)
            choose minimum a[i][j] among columns not chosen (chosen[mask])
               update (dp, chosen) when necessary
```

Solution complexity: $O(n^2 \cdot 2^n)$ time, $O(2^n)$ space.

Tested (_min\_sum.cpp_):
```
n  T (dp) T (n!)
12 0.6ms  3s
20 0.3s   >>
30 500s   >>
```

### **min_sum_3d**:
> Given $a_{ijk}$, choose $n$ elements each on distinct xy, yz and xz planes so their sum would be minimal.

