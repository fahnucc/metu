#include "the6.h"
// Don't add any additional includes

/*
  N: Number of racers
  updates: Updates you have heard
*/




void topological(int v, bool visited[], std::vector<int>& Stack, std::vector<int> adjList[], std::vector<int>& recStack, bool rec[], bool &flag)
{
    if(flag)
    {
        
        visited[v] = true;
        rec[v] = true;
        recStack.push_back(v);
    
        std::vector<int>::iterator i;
        for (i = adjList[v].begin(); i != adjList[v].end(); ++i)
        {
            if (!visited[*i]) 
            {
                topological(*i, visited, Stack, adjList, recStack, rec, flag);
            }
            if(rec[*i])
            {
                //recStack.push_back(*i);
                flag = false;
                return;
            }
        }
        
        rec[v] = false;
        recStack.pop_back();
        Stack.insert(Stack.begin(), v);
    }
}

std::pair<bool, std::vector<int>> RaceResult(int N, std::vector<std::pair<int, int>> updates) {
    
    std::vector<int> Stack;
    std::vector<int> recStack;
    std::pair<bool, std::vector<int>> result;
    

    //create adj list
    std::vector<int> adjList[N];

    for(unsigned int i = 0; i < updates.size(); i++){
        adjList[updates[i].first].push_back(updates[i].second);
    }
    //adj list ok
    
    
    //init visited array
    bool* visited = new bool[N];
    for (int i = 0; i < N; i++){
        visited[i] = false;
    }
    
    //init rec array
    bool* rec = new bool[N];
    for (int i = 0; i < N; i++){
        rec[i] = false;
    }
    
    bool flag = true;
    //dfs search
    for (int i = 0; i < N; i++)
    {
        if (visited[i] == false){ 
            topological(i, visited, Stack, adjList, recStack, rec, flag);
        }
        if(!flag){
            
            /*
            std::vector<int>::iterator it;
            for (it = recStack.begin(); it != recStack.end(); ++it) 
            {
                std::cout << *it << " ";
            }*/
    
            recStack.push_back(*(recStack.begin()));
            result.first = false;
            result.second = recStack;
            return result;
        }
    }
    
    //result
    result.first = true;
    result.second = Stack;
    
    
    
    
    return result;
}