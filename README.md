# Google Hash Code 2018 Practice Problem

![ara](https://user-images.githubusercontent.com/79721547/125121484-ad5f0c80-e0fc-11eb-87e8-08394de3a31d.png)

## Representing the data

A pizza is a two-dimensional array where **A[i][j]** can be either **M**(mushroom) or **T**(tomato). To simplify it further, I represent it as a bool matrix, which will help with the
following point.

No matter how you'd choose to simulate the choosing of the slices, a very common query will be whether the slice satisfies the minimum number of ingredients criteria. Or, put in
a different way, whether the rectangle **[(i1, j1), (i2, j2)]** has **nr(M)** >= **min_ingr** and **nr(T) >= min_ingr**. Checking this by going over each element of the slice every time would
be too slow.

For this purpose I used dynamic programming, partial sums more specifically. In this case, **rect_sums[i][j]** will hold the number of **1**s in the rectangle **[(1, 1), (i, j)]**. So
for any rectangle **[(i1, j1), (i2, j2)]** the sum of the **1**s can be calculated as **[(1, 1), (i2, j2)]** - **[(1, 1), (i1 - 1, j2)]** - **[(1, 1), (i2, j1 - 1)]** + 
**[(1, 1), (i1 - 1, j1 - 1)]**(subtracting the rectangle above, subtracting the rectangle to the left, and adding the rectangle that's formed by the overlapping of the two previously
mentioned, since it is subtracted twice). Then, the number of **0**s is obtained by subtracting the sum of the **1**s from the total area of the slice.

Another common query is whether the new slice overlaps with any of the previously created slices. In order for this not to get exponentially more time-consuming, I used an array of vectors
**slices<Rectang>[nr_rows]**, and only the **max_slice_area** lines above are checked. The speed could be further increased by using a **BST**-like data structure for 2D objects.

## Solution 1

In this solution, we iterate through the matrix, and for every **A[i][j]** that does not belong to a slice, we attempt to create a square with the upper-left corner in **A[i][j]**. 
When the square can't be further increased in size, it is attempted to increase it in just one direction, turning it into a rectangle. This solution is faster, but less versatile, as
it can't create **n x 1** or **1 x n** rectangles for example, if the need arises. 

#### Score

* *example.in*: 12/15
* *small.in*: 28/42
* *medium.in*: 44550/50000
* *big.in*: 607500/1000000
* **total score**: 652090

## Solution 2 

Similar to solution 1, except that in the beginning all possible pairs **<l, L>** such that **l** * **L** <= **max_slice_area** are generated. Then, those are applied to each
**A[i][j]**, and the maximum size configuration is chosen. It obtains better scores, but takes slightly longer to execute.

#### Score

* *example.in*: 12/15
* *small.in*: 28/42
* *medium.in*: 48240/50000
* *big.in*: 845192/1000000
* **total score**: 893472 

## Solution 3

In addition to solution 2, for every **A[i][j]** the number of ingredients in the *general area* is determined. This will be used when choosing between two slices which have the same
size, the one using the least of the more rare ingredient being prefered. The size of the *general_area* that I used is **max_slice_area** * **max_slice_area**. Changing this size
could result in better scores for some tests. Unfortunately, the number of ingredients of each type in this *general area* does not take into account that some pieces of it are
already occupied by other slices, but checking this would reduce the program speed by a lot.

#### Score

* *example.in*: 12/15
* *small.in*: 34/42
* *medium.in*: 48077/50000
* *big.in*: 848617/1000000
* **total score**: 896740

## Solution 4

The final and best solution. For every **A[i][j]** the number of ingredients in the *general area* is determined. Based on the size of a slice, the ratio of the ingredients in the area and
the number of rare ingredients used in the slice, a score is computed by the following formula: **score = size - (ingr_wasted * ratio * 10)**. This way, sometimes a bigger slice
is penalized in the favor of a smaller slice which uses the available ingredients more efficiently. This can lead to cases where instead of a bigger slice being chosen,
two smaller slices are chosen which have a combined area greater than the bigger slice.

* *example.in*: 15/15
* *small.in*: 41/42
* *medium.in*: 48226/50000
* *big.in*: 890123/1000000
* **total score**: 938405