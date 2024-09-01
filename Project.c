#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<limits.h>
#define MAX 50

int visited[MAX] = {0};

int path[MAX] = {0};

typedef struct graph{
    int n;
    int a[MAX][MAX];
}GRAPH;

void create_graph(GRAPH *g){
    int i , j;
    for(i = 0; i < g->n; i++){
        for(j = 0; j < g->n; j++)
           g->a[i][j] = 0;
    }
    FILE* fp = fopen("Hosa.csv", "r");
    if(fp == NULL){
        printf("Can't open file\n");
        exit(0);
    }
    else{
        char line[15];
        char *so;
        char *des;
        while(fgets(line, 15, fp)){ 
                so = strtok(line, ",");
                des = strtok(NULL, ",");
                i = atoi(so)-1; 
                j = atoi(des)-1;
                g->a[i][j] = g->a[j][i] = 1;
        }
    }
    fclose(fp);
}

void display(GRAPH *g){
    FILE* fptr = fopen("Test.txt", "w");
    for(int i = 0; i < g->n; i++){
        for(int j = 0; j < g->n; j++)
            fprintf(fptr, "%d\t", g->a[i][j]);
        fprintf(fptr, "\n");
    }
    fclose(fptr);
}

void degree(GRAPH *g, int v){
    if(v > g->n || v <= 0){
        printf("Please choose a valid node!\n");
        return;
    }

    int count = 0;
    for(int i = 0; i < g->n; i++){
        if(g->a[i][v-1] == 1)
            count++;
    }

    printf("The number of connections of the junction %d = %d\n", v, count);
}

void max_degree(GRAPH* g){
    int max = 0;
    int count = 0;
    int node;
    for(int i = 0; i < g->n; i++){
        count = 0;
        for(int j = 0; j < g->n; j++){
            if(g->a[i][j] == 1)
                count++;
        }
        if(max < count){
            max = count;
            node = i + 1;
        }
    }
    printf("Junction: %d\tDegree: %d\n", node, max);
}

void min_degree(GRAPH* g){
    int min = g->n;
    int count = 0;
    int node;
    for(int i = 0; i < g->n; i++){
        count = 0;
        for(int j = 0; j < g->n; j++){
            if(g->a[i][j] == 1)
                count++;
        }
        if(min > count){
            min = count;
            node = i + 1;
        }
    }
    printf("Junction: %d\tDegree: %d\n", node, min);
}

int isSafe(GRAPH* g, int v, int path[], int pos){
    if(g->a[path[pos - 1]][v] == 0)
        return 0;
 
    for(int i = 0; i < pos; i++){
        if(path[i] == v)
            return 0;
    }
    return 1;
}

int hamiltonianCycleUtil(GRAPH* g, int path[], int pos){
    if(pos == g->n){
        if(g->a[path[pos - 1]][path[0]] == 1)
            return 1;
        else
            return 0;
    }

    for(int v = 1; v < g->n; v++){
        if(isSafe(g, v, path, pos)){
            path[pos] = v;

            if(hamiltonianCycleUtil(g, path, pos + 1) == 1)
                return 1;

            path[pos] = -1;
        }
    }
    return 0;
}

void findHamiltonianCycle(GRAPH* g){
    int path[g->n];
    path[0] = 0;
    for(int i = 1; i < g->n; i++)
        path[i] = -1;

    if(hamiltonianCycleUtil(g, path, 1) == 0){
        printf("Hamiltonian Cycle does not exist\n");
        return;
    }

    printf("Hamiltonian Cycle Exists, Following is one Hamiltonian Cycle:\n");
    for(int i = 0; i < g->n; i++)
        printf(" %d ", path[i] + 1);

    printf(" %d ", path[0] + 1);
    printf("\n");
}

int minDistance(int dist[], int visited[], int n){
    int min_index, min = INT_MAX;
    for(int v = 0; v < n; v++){
        if(!visited[v] && dist[v] < min){
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void printPath(int parent[], int destination){
    if(parent[destination] == -1){
        printf("%d", destination + 1);
        return;
    }
    printPath(parent, parent[destination]);
    printf(" -> %d", destination + 1);
}

void findShortestPath(GRAPH* g, int source, int destination){
    int dist[g->n];
    int visited[g->n];
    int parent[g->n];
    for(int i = 0; i < g->n; i++){
        dist[i] = INT_MAX;
        visited[i] = 0;
        parent[i] = -1;
    }

    dist[source] = 0;
    for(int count = 0; count < (g->n) - 1; count++){
        int u = minDistance(dist, visited, g->n);
        visited[u] = 1;
        for(int v = 0; v < g->n; v++){
            if (!visited[v] && g->a[u][v] && dist[u] != INT_MAX && dist[u] + g->a[u][v] < dist[v]){
                dist[v] = dist[u] + g->a[u][v];
                parent[v] = u;
            }
        }
    }

    if(dist[destination] == INT_MAX)
        printf("Shortest path from %d to %d does not exist.\n", source + 1, destination + 1);
    else{
        printf("Shortest path from %d to %d: ", source + 1, destination + 1);
        printPath(parent, destination);
        printf("\nShortest path length: %d\n", dist[destination]);
    }
}

void findPathVia(GRAPH* g, int source, int via, int destination, int check){
    int dist[g->n];
    int visited[g->n];
    int parent[g->n];
    for(int i = 0; i < g->n; i++){
        dist[i] = INT_MAX;
        visited[i] = 0;
        parent[i] = -1;
    }

    if(check == 0)
        dist[source] = 0;
    else
        dist[via] = 0;

    for(int count = 0; count < (g->n) - 1; count++){
        int u = minDistance(dist, visited, g->n);
        visited[u] = 1;
        for(int v = 0; v < g->n; v++){
            if (!visited[v] && g->a[u][v] && dist[u] != INT_MAX && dist[u] + g->a[u][v] < dist[v]){
                dist[v] = dist[u] + g->a[u][v];
                parent[v] = u;
            }
        }
    }

    if(check == 0){
        if(dist[via-1] == INT_MAX){
            printf("Shortest path from %d to %d does not exist.\n", source + 1, via);
            return;
        }
        else{
            {
                printf("Shortest path from %d to %d via %d: ", source + 1, destination + 1, via + 1);
                printPath(parent, via-1);
                printf(" -> ");
            }
        }
    }
    else{
        if(dist[destination] == INT_MAX){
            printf("Shortest path from %d to %d does not exist.\n", via + 1, destination + 1);
            return;
        }
        else{
            printPath(parent, destination);
            printf("\n");
        }
    }
}

void findShortestPathVia(GRAPH* g, int source, int via, int destination){
    findPathVia(g, source, via, destination, 0);
    findPathVia(g, source, via, destination, 1);
}

void findEulerianPath(GRAPH* g){
    for(int i = 0; i < g->n; i++)
        visited[i] = 0;

    int numAdj[g->n];

    for(int i = 0; i < g->n; i++){
        numAdj[i] = 0;
        for(int j = 0; j < g->n; j++)
            numAdj[i] += g->a[i][j];
    }

    int start = 0, numOdd = 0;
    for(int i = (g->n) - 1; i >= 0; i--){
        if(numAdj[i] % 2 == 1){
            numOdd++;
            start = i;
        }
    }

    if(numOdd > 2){
        printf("Eulerian Path doesn't exist\n");
        return;
    }

    int stack[g->n];
    int stackSize = 0;
    int path[g->n];
    int pathSize = 0;
    int cur = start;

    while(stackSize > 0 || numAdj[cur] != 0){
        if(numAdj[cur] == 0){
            path[pathSize++] = cur;
            cur = stack[--stackSize];
        }
        else{
            for(int i = 0; i < g->n; i++){
                if(g->a[cur][i] == 1){
                    stack[stackSize++] = cur;
                    numAdj[cur]--;
                    numAdj[i]--;
                    g->a[cur][i] = 0;
                    g->a[i][cur] = 0;
                    cur = i;
                    break;
                }
            }
        }
    }

    printf("Eulerian Path Exists, Following is one Eulerian Path:\n");
    for (int i = 0; i < pathSize; i++)
        printf("%d -> ", path[i] + 1);
    printf("%d\n", cur + 1);
}

int countPaths = 0;
void printAllPathsUtil(GRAPH* g, int src, int dest, int pathIndex){
    visited[src] = 1;
    path[pathIndex] = src;
    pathIndex++;
    if(src == dest){
        for(int i = 0; i < pathIndex; i++){
            printf("%d", path[i] + 1);
            if(i < pathIndex - 1)
                printf(" -> ");
        }
        printf("\n");
        countPaths++;
    } 
    else{
        for(int i = 0; i < g->n; i++){
            if(g->a[src][i] && !visited[i]) 
                printAllPathsUtil(g, i, dest, pathIndex);
        }
    }
    visited[src] = 0;
}

int printAllPaths(GRAPH* g, int src, int dest){
    if(src >= g->n || dest >= g->n || src < 0 || dest < 0){
        printf("There are no paths from Junctions %d to %d\n", src + 1, dest + 1);
        return 0;
    }
    for(int i = 0; i < g->n; i++)
        path[i] = 0;

    int pathIndex = 0;
    printf("All paths from node %d to node %d:\n", src + 1, dest + 1);
    printAllPathsUtil(g, src, dest, pathIndex);
    return 1;
}

void DFS(GRAPH* g, int v, int k){
    visited[v] = 1;
    if(k == 0){
        printf("%d ", v + 1);
        return;
    }

    for(int u = 0; u < g->n; u++){
        if(g->a[v][u] && !visited[u])
            DFS(g, u, k - 1);
    }
}

void printNthNeighbors(GRAPH* g, int source, int k){
    if(source < 1 || source > g->n){
        printf("Invalid source node.\n");
        return;
    }

    for(int i = 0; i < MAX; i++)
        visited[i] = 0;

    printf("Junctions at a distance of %d from node %d: ", k, source);
    DFS(g, source - 1, k);
    printf("\n");
}

int main(){
    GRAPH g;
    g.n = 6;
    create_graph(&g);
    display(&g);
    while(1){
        int choice, v, src, via, dest;
        printf("1.All the Paths\n2.Shortest Path\n3.Shortest Path via a Junction\n4.Eulerian Path\n5.Hamiltonian Cycle\n");
        printf("6.No.of Connections of a Junction\n7.Most Congested Junction\n8.Least Congested Junction\n");
        printf("9.Nth Neighbor Junctions\n10.Exit\n");
        printf("-> What action would you like to perform? ");
        scanf("%d", &choice);
        switch(choice){
            case 1: printf("-> Please enter the Source and Destination: ");
                    scanf("%d%d", &src, &dest);
                    choice = printAllPaths(&g, src - 1, dest - 1);
                    if(choice == 1)
                        printf("The total number of paths from Junctions %d to %d are %d\n", src, dest, countPaths);
                    break;
            case 2: printf("-> Please enter the Source and Destination: ");
                    scanf("%d%d", &src, &dest);
                    findShortestPath(&g, src - 1, dest - 1);
                    break;
            case 3: printf("-> Please enter the Source, Destination and the Junction you have to go via: ");
                    scanf("%d%d%d", &src, &dest, &via);
                    findShortestPathVia(&g, src - 1, via - 1, dest - 1);
                    break;
            case 4: findEulerianPath(&g);
                    break;
            case 5: findHamiltonianCycle(&g);
                    break;
            case 6: printf("-> Please enter the node number: ");
                    scanf("%d", &v);
                    degree(&g, v);
                    break;
            case 7: max_degree(&g);
                    break;
            case 8: min_degree(&g);
                    break;
            case 9: printf("-> Please enter the source node and distance (v): ");
                    scanf("%d%d", &src, &v);
                    printNthNeighbors(&g, src, v);
                    break;
            case 10: exit(0);
            default: printf("Invalid choice.");
                     break;
        }
        printf("\n");
    }
    return 0;
}
