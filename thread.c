#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


void* timeCheck(void* arg){
	while(1){
		sleep(30);
		printf("30second!\n");
	}

	return NULL;
}

void secondTimeCheck(){
	while(1){
		sleep(60);
		printf("60second!\n");
	}

}

int main(){
	pthread_t newthread;
	//pthread_attr_t attr;
	//pthread_attr_init(&attr);

	pthread_create(&newthread, NULL, timeCheck, NULL);
	
	secondTimeCheck();
	
	//return 0;
}
