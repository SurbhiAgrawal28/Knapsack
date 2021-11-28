
Steps to run the parallel programs (using OpenMP)

1. Compile the program using: 
  g++ -o output.exe -fopenmp filename

2. Run the object file
  output.exe

-----------------------------------------------------------------------------------

Fractional Knapsack

Algorithm

parallelMergeSort()
if(l<r)
  m = (l+r)/2
  #pragma omp parallel sections
  {
    #pragma omp section
    {
    mergeSort(l,m)
    }
    #pragma omp section
    {
    mergeSort(m+1,r)
    }
  }
  merge(l,m,r)
  
//Item is struct containing weight(w) and profit(p) as members
Greedy-Fractional-Knapsack (vector<Item> items, knapsackCapacity) 
n = items.size()
parallel merge sort will sort items based on items[i].p/items[i].w ratio in non-increasing order
finalValue = 0
currWeight = 0 
for i = 1 to n 
   if currWeight + Item[i].w ≤ knapsackCapacity then  
      currWeight = currWeight + Item[i].w
      finalValue = finalValue + Item[i].p
   else 
      remain = (knapsackCapacity - currWeight)
      finalValue = finalValue +  remain*(items[i].p/items[i].w)
      break 
return finalValue

Analysis:

  1. Sorting Items will require O(logn^3) (T(n) = T(n/2)+T(n))
  2. For loop to add items to sack takes O(n)
  3. Total time = O(logn^3) + O(n)

------------------------------------------------------------------------------------------------

0/1 Knapsack

Algorithm

//Weights -> w[]; Profits -> p[]; knapsackCapacity -> W; Items -> n
DP-0-1-Knapsack()
for i=0 to 1
  for j=0 to W
    m[i,j]=0
currentItem = 1;
for i=1 to n
  #pragma omp parallel for num_threads(noOfThreads)
  for j=1 to W
    if w[i] > j then
      m[currentItem,j] = m[!currentItem,j]
    else
      m[currentItem,j] = max(m[!currentItem,j],m[!currentItem,j-w[i]] + p[i])
  currentItem = !currentItem
return m[!currentItem,W]

Analysis

  1. Initialising the 2 rows takes O(W)
  2. If we have t threads, each thread will be assigned W/t columns. Therefore, for loop      will get executed in O(W/t) unlike O(W) for serial one.
    Now we need to iterate through all the items and execute this for loop everytime.
    Therefore, O(n*(W/t))
