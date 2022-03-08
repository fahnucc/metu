#include "the7.h"


void calcAdjMatrix(int** result, int n, std::vector<Road> roads){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            result[i][j] = 0;
        }
    }
    for(unsigned int i = 0; i < roads.size(); i++){
        result[roads[i].endpoints.first][roads[i].endpoints.second] = roads[i].time;
        result[roads[i].endpoints.second][roads[i].endpoints.first] = roads[i].time;
    }
}

void dijkstra(int* dist, int** adj, int src, int V, std::vector<int>* p)
{
    bool visited[V];

    for (int i = 0; i < V; i++)
    {
        dist[i] = INT_MAX;
        visited[i] = false;
    }
        
    dist[src] = 0;

    for (int i = 1; i < V; i++) {
        int current;
        int min = INT_MAX;

        for (int v = 0; v < V; v++){
            if (visited[v] == false && dist[v] <= min)
            {
                min = dist[v];
                current = v;
            }
        }
        visited[current] = true;
        
        for (int v = 0; v < V; v++){
            if (!visited[v] && adj[current][v] > 0 && dist[current] + adj[current][v] < dist[v]){
                dist[v] = dist[current] + adj[current][v];

                p[v].insert(p[v].begin(), p[current].begin(), p[current].end());
                p[v].push_back(current);
            }
        }
    }
}

void FindRoute(int n, std::vector<Road> roads, int s, int d, int x, int y) {
    std::vector<int> path;
    int cost = INT_MAX;
    
    /*
    std::cout << "n: " << n << std::endl;
    std::cout << "s: " << s << std::endl;
    std::cout << "d: " << d << std::endl;
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    
    for (int i = 0; i < roads.size(); ++i)
    {
        std::cout << roads[i].endpoints.first << " " << roads[i].endpoints.second << " " << roads[i].time << std::endl;
    }
    */
    
    int **adjMatrix = new int*[n];
  
    for (int i = 0; i < n; i++)
    {
        adjMatrix[i] = new int[n];
    }
    calcAdjMatrix(adjMatrix, n, roads);
  
    std::vector<int> ps[n];
    int* distS = new int[n];
    dijkstra(distS, adjMatrix, s, n, ps);
    
    std::vector<int> px[n];
    int* distX = new int[n];
    dijkstra(distX, adjMatrix, x, n, px);
    
    std::vector<int> py[n];
    int* distY = new int[n];
    dijkstra(distY, adjMatrix, y, n, py);
    
    
    int sxyd = distS[x] + distX[y] + distY[d];
    int syxd = distS[y] + distY[x] + distX[d];
    
    if(sxyd <= syxd) {
        cost = sxyd;
        
        //std::cout << "psx size: " << ps[x].size() << std::endl;
        path.insert(path.end(), ps[x].begin(), ps[x].end());
        
        //std::cout << "pxy size: " << px[y].size() << std::endl;
        path.insert(path.end(), px[y].begin(), px[y].end());
        
        //std::cout << "pyd size: " << py[d].size() << std::endl;
        path.insert(path.end(), py[d].begin(), py[d].end());
    }
    
    else{
        cost = syxd;
        
        //std::cout << "psy size: " << ps[y].size() << std::endl;
        path.insert(path.end(), ps[y].begin(), ps[y].end());

        //std::cout << "pyx size: " << py[x].size() << std::endl;
        path.insert(path.end(), py[x].begin(), py[x].end());

        //std::cout << "pxd size: " << px[d].size() << std::endl;
        path.insert(path.end(), px[d].begin(), px[d].end());
    } 
    path.push_back(d);


    // Your output should be like this. You can change this as long as you keep
    // the output format. PrintRange function helps you print elements of
    // containers with iteratos (e.g., std::vector).
    std::cout << cost << " ";
    PrintRange(path.begin(), path.end());
    std::cout << std::endl;
}


    /*

    n=5;
    s=3;
    d=2;
    x=1;
    y=4;
    std::vector<Road> roads2;
    roads2.push_back({0, 1, 13});
    roads2.push_back({0, 2, 13});
    roads2.push_back({0, 3, 18});
    roads2.push_back({1, 2, 19});
    roads2.push_back({1, 3, 10});
    roads2.push_back({1, 4, 18});
    roads2.push_back({2, 3, 13});
    roads2.push_back({2, 4, 20});
    roads2.push_back({3, 4, 18});
    
    */









