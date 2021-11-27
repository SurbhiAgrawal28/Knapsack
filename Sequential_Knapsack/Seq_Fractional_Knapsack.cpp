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

vector<Item> ItemList;

double sequentialKnapsack()
{
    // sorting Items based on value/weight ratio    
    sort(ItemList.begin(), ItemList.end(), [](const Item& lhs, const Item& rhs) {
        double r1 = (double)lhs.value / (double)lhs.weight;
        double r2 = (double)rhs.value / (double)rhs.weight;
        return r1 > r2;
    });
    
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

    cout<<"Executing Sequential Fractional Knapsack using Greedy Method: "<<endl;

    auto start = chrono::steady_clock::now();
 
    double result = sequentialKnapsack();
 
    auto end = chrono::steady_clock::now();

    cout << "Elapsed time in microseconds: "<< chrono::duration_cast<chrono::microseconds>(end - start).count()<<endl;
    cout << "Maximum value we can obtain = "<<result<<endl;
    
    return 0;
}