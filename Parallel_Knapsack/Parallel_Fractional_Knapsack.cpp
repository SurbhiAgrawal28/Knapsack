#include <iostream> 
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <omp.h>
using namespace std;

#define VALUE_MAX 100

int n; //No of items
int knapsackCapacity; //Capacity of knapsack

struct Item {
    int value, weight;
    Item() {}
    Item(int value, int weight)
    {
       this->value = value;
       this->weight = weight;
    }
};

vector<Item> ItemList, ItemListParallel;

void merge_parallel(int const left, int const mid, int const right)
{
	auto const subArrayOne = mid - left + 1;
	auto const subArrayTwo = right - mid;

	vector<Item> leftArray(subArrayOne);
	vector<Item> rightArray(subArrayTwo);

	// Copy data to temp arrays leftArray[] and rightArray[]
	#pragma omp parallel for
	for (auto i = 0; i < subArrayOne; i++)
		leftArray[i] = ItemListParallel[left + i];
	#pragma omp parallel for
	for (auto j = 0; j < subArrayTwo; j++)
		rightArray[j] = ItemListParallel[mid + 1 + j];

	auto indexOfSubArrayOne = 0, // Initial index of first sub-array
		indexOfSubArrayTwo = 0; // Initial index of second sub-array
	int indexOfMergedArray = left; // Initial index of merged array

	// Merge the temp arrays back into array[left..right]
	while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
		double l = (double)leftArray[indexOfSubArrayOne].value/(double)leftArray[indexOfSubArrayOne].weight;
		double r = (double)rightArray[indexOfSubArrayTwo].value/(double)rightArray[indexOfSubArrayTwo].weight;		
		if (l > r) {
			ItemListParallel[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
			indexOfSubArrayOne++;
		}
		else {
			ItemListParallel[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
			indexOfSubArrayTwo++;
		}
		indexOfMergedArray++;
	}
	// Copy the remaining elements of
	// left[], if there are any
	while (indexOfSubArrayOne < subArrayOne) {
		ItemListParallel[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
		indexOfSubArrayOne++;
		indexOfMergedArray++;
	}
	// Copy the remaining elements of
	// right[], if there are any
	while (indexOfSubArrayTwo < subArrayTwo) {
		ItemListParallel[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
		indexOfSubArrayTwo++;
		indexOfMergedArray++;
	}
}

void mergeSort_parallel(int const l, int const r)
{
	if (l >= r)
		return;

	auto mid = l + (r - l) / 2;
	#pragma omp parallel sections num_threads(2)
	{
		#pragma omp section
		{
			mergeSort_parallel(l, mid);
		}
		#pragma omp section
		{
			mergeSort_parallel(mid + 1, r);
		}
	}
	
	merge_parallel(l, mid, r);
}

void merge_seq(int const left, int const mid, int const right)
{
	auto const subArrayOne = mid - left + 1;
	auto const subArrayTwo = right - mid;

	vector<Item> leftArray(subArrayOne);
	vector<Item> rightArray(subArrayTwo);

	// Copy data to temp arrays leftArray[] and rightArray[]
	#pragma omp parallel for
	for (auto i = 0; i < subArrayOne; i++)
		leftArray[i] = ItemList[left + i];
	#pragma omp parallel for
	for (auto j = 0; j < subArrayTwo; j++)
		rightArray[j] = ItemList[mid + 1 + j];

	auto indexOfSubArrayOne = 0, // Initial index of first sub-array
		indexOfSubArrayTwo = 0; // Initial index of second sub-array
	int indexOfMergedArray = left; // Initial index of merged array

	// Merge the temp arrays back into array[left..right]
	while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
		double l = (double)leftArray[indexOfSubArrayOne].value/(double)leftArray[indexOfSubArrayOne].weight;
		double r = (double)rightArray[indexOfSubArrayTwo].value/(double)rightArray[indexOfSubArrayTwo].weight;		
		if (l > r) {
			ItemList[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
			indexOfSubArrayOne++;
		}
		else {
			ItemList[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
			indexOfSubArrayTwo++;
		}
		indexOfMergedArray++;
	}
	// Copy the remaining elements of
	// left[], if there are any
	while (indexOfSubArrayOne < subArrayOne) {
		ItemList[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
		indexOfSubArrayOne++;
		indexOfMergedArray++;
	}
	// Copy the remaining elements of
	// right[], if there are any
	while (indexOfSubArrayTwo < subArrayTwo) {
		ItemList[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
		indexOfSubArrayTwo++;
		indexOfMergedArray++;
	}
}

void mergeSort_seq(int const l, int const r)
{
	if (l >= r)
		return;

	auto mid = l + (r - l) / 2;
    mergeSort_seq(l, mid);
	mergeSort_seq(mid + 1, r);
	merge_seq(l, mid, r);
}


double sequentialKnapsack()
{
    // sorting Items based on value/weight ratio    
    // sort(ItemList.begin(), ItemList.end(), [](const Item& lhs, const Item& rhs) {
    //     double r1 = (double)lhs.value / (double)lhs.weight;
    //     double r2 = (double)rhs.value / (double)rhs.weight;
    //     return r1 > r2;
    // });
    mergeSort_seq(0,n-1);
    //Uncomment to see the Items after sorting
    
    // for (int i = 0; i < n; i++)
    // {
    //     cout <<ItemList[i].value <<"  "<<ItemList[i].weight<<" :"<<((double)ItemList[i].value/ItemList[i].weight)<<endl;
    // }
    
  
    int curWeight = 0; // Current weight in knapsack
    double finalvalue = 0.0; // Result (value in Knapsack)
    
    for (int i = 0; i < n; i++) 
    {
        // If adding Item won't overflow, add it completely
        if (curWeight + ItemList[i].weight <= knapsackCapacity) 
        {
            curWeight += ItemList[i].weight;
            finalvalue += ItemList[i].value;
        }
        else 
        {
            int remain = knapsackCapacity - curWeight;
            finalvalue += ItemList[i].value*((double)remain/(double)ItemList[i].weight);
            break;
        }
    }
    return finalvalue;
}


double parallelKnapsack()
{   
    
    mergeSort_parallel(0,n-1);
    
    //Uncomment to see the Items after sorting
    
    // for (int i = 0; i < n; i++)
    // {
    //     cout <<ItemListParallel[i].value <<"  "<<ItemListParallel[i].weight<<" :"<<((double)ItemListParallel[i].value/ItemListParallel[i].weight)<<endl;
    // }
    
    int curWeight = 0; // Current weight in knapsack
    double finalvalue = 0.0; // Result (value in Knapsack)
    
    for (int i = 0; i < n; i++) 
    {
        // If adding Item won't overflow, add it completely
        if (curWeight + ItemListParallel[i].weight <= knapsackCapacity) 
        {
            curWeight += ItemListParallel[i].weight;
            finalvalue += ItemListParallel[i].value;
        }
        else 
        {
            int remain = knapsackCapacity - curWeight;
            finalvalue += ItemListParallel[i].value*((double)remain/(double)ItemListParallel[i].weight);
            break;
        }
    }
    return finalvalue;
}

void input()
{
    cout<<"Enter capacity of knapsack: ";
    cin>>knapsackCapacity;
    cout<<"Enter number of items: ";
    cin>>n;
    for(int i=0;i<n;i++)
    {
        //cout<<"Enter value and weight for item "<<i+1<<endl;
        int weight,value;
        //cin>>value>>weight;
        value=rand()%VALUE_MAX;
		weight=rand()%(knapsackCapacity-1)+1;
        Item d = {value,weight};
        ItemList.push_back(d);
    }   
}  

// Driver code
int main()
{
    input();

    ItemListParallel = ItemList;

    cout<<"Executing Sequential Fractional Knapsack using Greedy Method: "<<endl;

    auto start = chrono::steady_clock::now();
 
    double result = sequentialKnapsack();
 
    auto end = chrono::steady_clock::now();

    cout << "Elapsed time in microseconds: "<< chrono::duration_cast<chrono::microseconds>(end - start).count()<<endl;
    cout << "Maximum value we can obtain = "<<result<<endl;

    cout<<"Executing Parallel Fractional Knapsack using Greedy Method: "<<endl;

    auto start_p = chrono::steady_clock::now();
 
    double result_p = parallelKnapsack();
 
    auto end_p = chrono::steady_clock::now();

    cout << "Elapsed time in microseconds: "<< chrono::duration_cast<chrono::microseconds>(end_p - start_p).count()<<endl;
    cout << "Maximum value we can obtain = "<<result_p<<endl;
    
    return 0;
}