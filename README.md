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
> Given cube array $c_{ijk}$ of $n^3$ elements, choose $n$ elements each from distinct (xy, yz, xz)-planes so their sum would be minimal.

```
Store array: dp[2^n - 1][2^n - 1]

for k in 1..n
    for mask_y in combinations(k)     # C_{n}^{k}
        for mask_z in combinations(k) # ...
            for bits in mask_y:       # O(k^2) or O(n^2)
                for bits in mask_z:
                    update dp
```

Solution complexity: depending on the code it can be
$O(\sum_{k=1}^n (n C_{n}^{k})^2)$ or $O(\sum_{k=1}^n (k C_{n}^{k})^2)$.
The first one is $n^2 \cdot ( C_{2n}^{n}-1)$ and it can be proved that second one is $n^2 \cdot C_{2n-2}^{n-1}$.
Both behave like central binomial $\sim 2^{2n}/\sqrt{n}$, however second version performs ~3.9 times less operations.

Result: $O(n \sqrt{n} \cdot 2^{2n})$ time, $O(2^{2n})$ space.

Update: try optimized version of dp for comparison with following command (x5 faster on my laptop)
```
g++ -O3 -march=native -ffast-math -fno-tree-vectorize -fno-exceptions cube.cpp -o cube
```

### More details:
In every solution, I prefer to iterate k-subsets in following pattern:
```
for (int k = 0; k <= n; ++k) {
    for (int mask = (1<<k)-1; mask < (1<<n); mask = next_comb(mask)) {
        // do something
    }
}
```
where next\_comb() is a following bit trick
```
inline int next_comb(int v) {
    int t = v | (v - 1);
    return (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctz(v) + 1));
}
```
Of course for practical reasons, one can prefer any convenient way to iterate over k-subsets.
The main thing is to follow order from subsets of size k to k+1.
