#include <stdio.h>
#include <limits.h>
#define MAX 50

typedef struct graph{
    int n;
    int a[MAX][MAX];
}GRAPH;

void create_graph(GRAPH *g){
    int i , j, weight;
    for(i = 0; i < g->n; i++){
        for(j = 0; j < g->n; j++)
           g->a[i][j] = 0;
    }
    while(1){
        printf("Enter the source, destination and the distance between the two: ");
        scanf("%d%d%d", &i, &j, &weight);
        if(i < 0 || j < 0 || i >= g->n || j >= g->n)
            break;
        g->a[i][j] = g->a[j][i] = weight;
    }
}


int minDistance(int dist[], int sptSet[], int n){
    int min = INT_MAX;
    int minIndex;

    for(int i = 0; i < n; i++){
        if(sptSet[i] == 0 && dist[i] <= min){
            min = dist[i];
            minIndex = i;
        }
    }

    return minIndex;
}


void fastestPath(GRAPH* g, int src, int dest){
    int dist[MAX];    
    int sptSet[MAX];  

    for(int i = 0; i < MAX; i++){
        dist[i] = INT_MAX;
        sptSet[i] = 0;
    }
    dist[src] = 0;

    for(int i = 0; i < MAX - 1; i++){
        int u = minDistance(dist, sptSet, g->n);
        sptSet[u] = 1;
        for(int v = 0; v < MAX; v++){
            if(!sptSet[v] && g->a[u][v] && dist[u] != INT_MAX && dist[u] + g->a[u][v] < dist[v]){
                dist[v] = dist[u] + g->a[u][v];
            }
        }
    }
    printf("Shortest distance from %d to %d is %d\n", src, dest, dist[dest]);
}

int main(){
    GRAPH g;
    int source, destination;
    
    printf("Please enter the number of vertices: ");
    scanf("%d", &g.n);
    create_graph(&g);

    printf("Please enter the source and destination: ");
    scanf("%d%d", &source, &destination);
    fastestPath(&g, source, destination);

    return 0;
}

