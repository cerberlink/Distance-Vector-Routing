//testing Distance Vector Algorithm with double int arrays 
//written by Daniel Cardenas
//REMEMBER: Element 0 represents server id 1
//			i.e.: Element n represents server id n+1
#include <stdio.h>  
#include <string.h>   //strlen  
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>   //close  
#include <arpa/inet.h>    //close  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros  
#include <sys/select.h>
#include <unistd.h>

//function to return array element value of server id
int serverElementValue(int serverId)
{
	int temp = serverId - 1;
	return temp;
}

//function to display array matrix
void displayArray(int array[4][4])
{
	int i, j;
	printf("\n");
	printf("  |  1\t|   2\t|   3\t|   4\t|\n");
	printf("---------------------------------");
	for(i=0; i<4; i++) {
		printf("\n");
		if (i==0)
			printf("1 | ");
		else if (i==1)
			printf("2 | ");
		else if (i==2)
			printf("3 | ");
		else if (i==3)
			printf("4 | ");
		
		for(j=0; j<4; j++) {
			printf(" %d\t| ", array[i][j]);
		}
	} 
	printf("\n");
}

//function to update array matrix from a specific server using a vector (single array)
void updateArray(int fromServerId, int vector[4], int array[4][4])
{
	int i, j;
	int fromServerElement = fromServerId - 1;
	
	for (j=0; j<4; j++) {
		array[fromServerElement][j] = vector[j];
	}
	
	displayArray(array);
	
}

int main(int argc, char *argv[])   
{ 
	int myServer = 1;
	int array[4][4] = {
		{-1, -1, -1, -1},
		{-1, -1, -1, -1},
		{-1, -1, -1, -1},
		{-1, -1, -1, -1},
	};
	int *arrayPTR = &array[0][0];
	int i, j;
	int neighbor[4] = {-1, -1, -1, -1}; //initial neighbors for server 1
	
	//DISPLAYING graph
	printf("This program is for Server 1\n");
	printf("Here is a graph for the network:\n");
	printf("      (4)       \n");
	printf("     /   \\      \n");  
	printf("   5/     \\1    \n");
	printf("   /   4   \\    \n");
	printf("(1)---------(3) \n");
	printf("   \\       /    \n");
	printf("   7\\     /2    \n");
	printf("     \\   /      \n");
	printf("      (2)       \n");
	printf("\n");

	//TESTING display
	printf("Here is the server 1's initial matrix\n");
	displayArray(array);

	
	//TESTING adding server 1's neighbors
	printf("\n\nAdding link cost to server 1's neighbors\n");
	int fromServerId = 1;  //from server 1
	int fromServerElement = fromServerId - 1;  //element value of server 1
	int initCost[4]; //initial link cost of neighbors
	initCost[0] = 0;
	initCost[1] = 7;
	initCost[2] = 4;
	initCost[3] = 5;
	//update array matrix from a specific server using a vector (single array)
	for (j=0; j<4; j++) {
		array[fromServerElement][j] = initCost[j];
	}
	displayArray(array);
	
	//WHEN a link cost for a neighbor is found, update neighbor array
	neighbor[1] = 1;
	neighbor[2] = 1;
	neighbor[3] = 1;
	printf("\n\nServer 1's neighbors are: ");
	for (i=0; i<4; i++) {
		if (neighbor[i]>0) // && (i != serverElementValue(myServer)))
		{
			int temp = i+1;
			printf("Server %d, ", temp);
		}
	}
	printf("\n\n");
	
	
	//TESTING adding other neighbor server's (server 2) vectors to server 1's matrix
	printf("\n\nAdding Server 2's vector to Server 1's matrix\n");
	fromServerId = 2;  //from server 2
	fromServerElement = fromServerId - 1;  //array element value of server 2
	int server2Update[4];  //routing update from server 2
	server2Update[0] = 7;
	server2Update[1] = 0;
	server2Update[2] = 2;
	server2Update[3] = -1;
	//update array matrix for server 1
	for (j=0; j<4; j++) {
		array[fromServerElement][j] = server2Update[j];
	}
	displayArray(array);
	
	//TESTING adding another neighbor server's vectors (server 3)
	printf("\n\nAdding Server 3's vector to Server 1's matrix\n");
	fromServerId = 3;  //from server 3
	fromServerElement = fromServerId - 1;  //array element value of server 3
	int server3Update[4];  //routing update from server 3
	server3Update[0] = 4;
	server3Update[1] = 2;
	server3Update[2] = 0;
	server3Update[3] = 1;
	//update array matrix for server 1
	for (j=0; j<4; j++) {
		array[fromServerElement][j] = server3Update[j];
	}
	displayArray(array);
	
	
	//TESTING adding another neighbor server's vectors (server 4)
	printf("\n\nAdding Server 4's vector to Server 1's matrix\n");
	fromServerId = 4;  //from server 4
	fromServerElement = fromServerId - 1;  //array element value of server 4
	int server4Update[4];  //routing update from server 4
	server4Update[0] = 5;
	server4Update[1] = -1;
	server4Update[2] = 1;
	server4Update[3] = 0;
	//update array matrix for server 1
	for (j=0; j<4; j++) {
		array[fromServerElement][j] = server4Update[j];
	}
	displayArray(array);
	
	
	//TESTING Distance Vector Algorithm
	//start by updating every other server's vector
	int min;
	int neighborServer;
	for (i=0; i<4; i++) { //i represents server we're checking distance to, server i+1
		if (i != (myServer-1)) {  //every other server
			min = array[myServer-1][i];  //initialize the minimum to current value
			//cycle through every neighbor
			for (j=0; j<4; j++) {  //j represents server 1's neighbor (server (j+1))
				if (neighbor[j]>0) {  //then j is a neighbor server
					//TODO
					if (array[myServer-1][j] != -1 && array[j][i] != -1) {
						int value = array[myServer-1][j] + array[j][i];
						if (min == -1 || value < min) { //minimum currently infinite or greater than new value
							min = value;
						}
							
					}
				}
			}
			
			//min should be found for server (i+1), thus update array appropriately
			array[myServer-1][i] = min;
		}
	}
	printf("\n\nPerforming Distance Vector Algorithm");
	displayArray(array);
	
	
} //end of main