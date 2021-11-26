Steps to run the sequential programs

1. Compile the program using: 
  g++ -o output.exe filename

2. Run the object file
  output.exe

-----------------------------------------------------------------------------------

Fractional Knapsack

Algorithm

//Item is struct containing weight(w) and profit(p) as members
Greedy-Fractional-Knapsack (vector<Item> items, knapsackCapacity) 
n = items.size()
sort items based on items[i].p/items[i].w ratio in non-increasing order
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

  1. Sorting Items will require O(nlogn)
  2. For loop to add items to sack takes O(n)
  3. Total time = O(nlogn) + O(n)

------------------------------------------------------------------------------------------------

0/1 Knapsack

Algorithm

//Weights -> w[]; Profits -> p[]; knapsackCapacity -> W; Items -> n
DP-0-1-Knapsack()
for i = 0 to W
  m[0,i] = 0
for i = 1 to n
  for j = 0 to W
    if w[i] > j then
      m[i,j] = m[i-1,j]
    else
      m[i,j] = max(m[i-1,j],m[i-1,j-w[i]] + p[i])
return m[n,W]

Analysis

  1. Initialising the 0th row takes O(W)
  2. for each item iterate through all possible values of finalValue of knapsack will take O(n*W)
  3. Total time = O(n*W) + O(W)
