#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
#define infinity 999

// Map of obstacle course
const int numRows = 4;
const int numCols = 4;

int envMap[numRows][numCols] = {
  {1, 1, 1, 0},
  {0, 0, 1, 0},
  {1, 1, 1, 1},
  {0, 1, 0, 1},
};

int distanceToNode[numRows * numCols];

//bool startPosition[2] = {0, 0};
//bool currentPosition[2] = {startPosition[0], startPosition[1]};
//bool goalPosition[2] = {3, 1};

/*void setup() {
  //displayMap(); 
}*/


int cost(int start, int goal){
    if (start == goal) {
        return 0;
    }
	int dist = 0;
	int x1 = start/4;
	int x2 = goal/4;
	int y1 = start%4;
	int y2 = goal%4;
	
	if (envMap[x1][y1] == 0 || envMap[x2][y2] == 0)
		return infinity;
	
	dist = abs(x2-x1)+abs(y2-y1);
	
	if (dist > 1)
		return infinity;
	else
		return 1;
}
/*int ** generateCostMatrix( bool mapE[numRows][numCols] ){
  int** costMatrix = 0;
  costMatrix = new int*[numRows * numCols];

  for(int i = 0; i < numRows * numCols; i++)
  {
    costMatrix[i] = new int[numRows * numCols];
    for(int j = 0; j < numRows * numCols; j++)
    {
      costMatrix[i][j] = infinity;
    }
  }
  for(int i = 0; i < numRows; i++)
  {
    for(int j = 0; j < numCols; j++)
    {
      if(mapE[i][j] == 1)
      {
        if(mapE[i][j+1] == 1 && j != numCols - 1)
        {
          // The position has an available neighbor spot to right
          costMatrix[j + numRows * i][j + numRows * i + 1] = 1; 
          costMatrix[j + numRows * i + 1][j + numRows * i] = 1;
        }
        if(mapE[i+1][j] == 1 && i != numRows - 1)
        {
          // The position has an available neighbor spot below
          costMatrix[j + numRows * i][j + numRows * i + numRows] = 1;           
          costMatrix[j + numRows * i + numRows][j + numRows * i] = 1;
        }
      }
    }
  }
  return costMatrix;
}*/

void dij(int n,int v,int distanceToNode[])
 {
  int u, count, w, flag[16], min;
  //int distanceToNode[n];
  
  for(int i=0;i<n;i++){
   flag[i]=0;
   distanceToNode[i]=cost(v, i);
	}
  count=1;
  while(count < n)
  {
   min= infinity;
   for(w=0;w<n;w++)
    if(distanceToNode[w]<min && !flag[w])
     min=distanceToNode[w],u=w;
   flag[u]=1;
   count++;
   for(w=0;w<n;w++)
    if((distanceToNode[u]+cost(u,w)<distanceToNode[w]) && !flag[w])
     distanceToNode[w]=distanceToNode[u]+cost(u, w);
  }

  for (int i = 0; i < n; i++) {
	  //if(distanceToNode[i] != infinity)
		printf("from %d to %d: cost=%d\n",v,i,distanceToNode[i]);
  }
  //seg fault here, result of stack overflow based on information available
}

int main() {
    //int costMatrix = cost(1, 13));
    dij(numRows * numCols, 13, distanceToNode);
    printf("done");
    return 0;
}

