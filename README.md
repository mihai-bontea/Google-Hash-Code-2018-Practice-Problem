# Google Hash Code 2018 Practice Problem

## Representing the data

A pizza is a two-dimensional array where **A[i][j]** can be either **M**(mushroom) or **T**(tomato). To simplify it further, I represent it as a bool matrix, which will help with the
following point.

No matter how you'd choose to simulate the choosing of the slices, a very common query will be whether the slice satisfies the minimum number of ingredients criteria. Or, put in
a different way, whether the rectangle **[(i1, j1), (i2, j2)]** has **nr(M)** >= **min_ingr** and **nr(T) >= min_ingr**. Checking this by going over each element of the slice every time would
be too slow.

For this purpose I used dynamic programming, partial sums more specifically. In this case, **rect_sums[i][j]** will hold the number of **1**s in the rectangle **[(1, 1), (i, j)]**. So
for any rectangle **[(i1, j1), (i2, j2)]** the sum of the **1**s can be calculated as **[(1, 1), (i2, j2)]** - **[(1, 1), (i1 - 1, j2)]** - **[(1, 1), (i2, j1 - 1)]** + 
**[(1, 1), (i1 - 1, j1 - 1)]**(subtracting the rectangle above, subtracting the rectangle to the left, and adding the rectangle that's formed by the overlapping of the two previously
mentioned, since it is subtracted twice).

## Solution 1



## Solution 2 


