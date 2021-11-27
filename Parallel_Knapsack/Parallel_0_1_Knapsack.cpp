#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <omp.h>
using namespace std;

#define CAPACITY_MAX 10000
#define PROFIT_MAX 100

int n; // number of items
int knapsackCapacity; // total weight
vector<int> weight;
vector<int> profit;

int noOfThreads;

void inputs()
{
    cout<<"Enter capacity of knapsack: ";
    cin>>knapsackCapacity;
    cout<<"Enter number of items: ";
    cin>>n;
    for(int i=0;i<n;i++)
    {
        profit.push_back(rand()%PROFIT_MAX);
        weight.push_back(rand()%(knapsackCapacity-1)+1);
        // int p,w;
        // cin>>p>>w;
        // profit.push_back(p);
        // weight.push_back(w);
    }
}

int parallelKnapsack()
{
    //As the value in current row depends only on the previous row
    vector<vector<int>> knapsack(2);
    knapsack[0] = vector<int> (knapsackCapacity+1, 0);
    knapsack[1] = vector<int> (knapsackCapacity+1, 0); 
    
    int currentItem = 1;
    for(int i=1;i<=n;i++)
    {
        #pragma omp parallel for num_threads(noOfThreads)
        for (int j=1 ; j<=knapsackCapacity ; j++)
        {
            if(j<weight[i-1])
            {
                knapsack[currentItem][j] = knapsack[!currentItem][j];
            }
            else
            {
                //cout<<"Else: "<<knapsack[i-1][j]<<" "<<knapsack[i-1][j-weight[i-1]]+profit[i-1]<<endl;
                knapsack[currentItem][j] = max(knapsack[!currentItem][j],knapsack[!currentItem][j-weight[i-1]]+profit[i-1]);
            }       
        }
        currentItem = !currentItem;
    }

    return knapsack[!currentItem][knapsackCapacity];
} 

int sequentialKnapsack()
{
    //As the value in current row depends only on the previous row
    vector<vector<int>> knapsack(2);
    knapsack[0] = vector<int> (knapsackCapacity+1, 0);
    knapsack[1] = vector<int> (knapsackCapacity+1, 0); 
    
    int currentItem = 1;
    for(int i=1;i<=n;i++)
    {
        for (int j=1 ; j<=knapsackCapacity ; j++)
        {
            if(j<weight[i-1])
            {
                knapsack[currentItem][j] = knapsack[!currentItem][j];
            }
            else
            {
                //cout<<"Else: "<<knapsack[i-1][j]<<" "<<knapsack[i-1][j-weight[i-1]]+profit[i-1]<<endl;
                knapsack[currentItem][j] = max(knapsack[!currentItem][j],knapsack[!currentItem][j-weight[i-1]]+profit[i-1]);
            }       
        }
        currentItem = !currentItem;
    }

    return knapsack[!currentItem][knapsackCapacity];
}

void outputItemsTaken(vector<vector<int>> knapsack)
{   
    cout<<"Items taken: ";
	int i = n;
	int j = knapsackCapacity;
	while(i > 0 && j>0)
    {
		if( knapsack[i][j] == knapsack[i-1][j] ) 
        {
			i--;
		} 
        else 
        {
			cout<<i-1<<" ";
			j-=weight[i-1];
			i--;
		}
	}
    cout<<endl;
    for(int i=0;i<=n;i++)
    {
        for(int j=0;j<=knapsackCapacity;j++)
        {
            cout<<knapsack[i][j]<<" ";
        }
        cout<<endl;
    }
}

int main()
{
    inputs();

    cout<<"Executing Parallel 0/1 Knapsack using DP: "<<endl;

    noOfThreads = 2;
    auto start = chrono::steady_clock::now();
 
    int result = parallelKnapsack();
 
    auto end = chrono::steady_clock::now();

    cout <<"Elapsed time in microseconds: "<< chrono::duration_cast<chrono::milliseconds>(end - start).count()<<endl;

    cout <<"Maximum profit that can be obtained = "<<result<<endl<<endl;
    
    cout<<"Executing Sequential 0/1 Knapsack using DP: "<<endl;

    auto start_seq = chrono::steady_clock::now();
 
    result = sequentialKnapsack();
 
    auto end_seq = chrono::steady_clock::now();

    cout <<"Elapsed time in microseconds: "<< chrono::duration_cast<chrono::milliseconds>(end_seq - start_seq).count()<<endl;

    cout <<"Maximum profit that can be obtained = "<<result<<endl;

    cout<<"--------------------------------------------------------\n";
    
    //outputItemsTaken(result);

    return 0;
}