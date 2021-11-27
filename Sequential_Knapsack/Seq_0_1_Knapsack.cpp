#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
using namespace std;

#define CAPACITY_MAX 10000
#define PROFIT_MAX 100

int n; // number of items
int knapsackCapacity; // total weight
vector<int> weight;
vector<int> profit;

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

vector<vector<int>> sequentialKnapsack()
{
    vector<vector<int>> knapsack(n+1);

    for(int i=0;i<=n;i++)
    {
        knapsack[i] = vector<int> (knapsackCapacity+1); 
        for(int j=0;j<=knapsackCapacity;j++)
        {
            if(i==0 || j==0)
                knapsack[i][j] = 0;
            else if(j<weight[i-1])
            {
                knapsack[i][j] = knapsack[i-1][j];
            }
            else
            {
                //cout<<"Else: "<<knapsack[i-1][j]<<" "<<knapsack[i-1][j-weight[i-1]]+profit[i-1]<<endl;
                knapsack[i][j] = max(knapsack[i-1][j],knapsack[i-1][j-weight[i-1]]+profit[i-1]);
            }                
        }
    }

    return knapsack;
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

    cout<<"Executing Sequential 0/1 Knapsack using DP: "<<endl;

    auto start = chrono::steady_clock::now();
 
    vector<vector<int>> result = sequentialKnapsack();
 
    auto end = chrono::steady_clock::now();

    cout <<"Elapsed time in microseconds: "<< chrono::duration_cast<chrono::milliseconds>(end - start).count()<<endl;

    cout <<"Maximum profit that can be obtained = "<<result[n][knapsackCapacity]<<endl;
    
    //outputItemsTaken(result);

    return 0;
}