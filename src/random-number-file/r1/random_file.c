#include<stdio.h>
#include<time.h>

//Receive an unsigned integer N as input. Generate N random numbers between [0-100] and store them in a text file. 
//Put newline character between each generated number. 

int main(){
	
	time_t t;
	srand((unsigned) time(&t));
	FILE *f=fopen("integers.txt","w");
	
	unsigned int N;
	printf("How many random numbers would you like?:");
	scanf("%d",&N);
	int array[N];
	int i;
	for(i=0;i<N;i++){
		array[i]=(rand()%100);
	}
	//fprintf(f,"%s","Array:\n");
	for(i=0;i<N;i++){
		fprintf(f,"%d",array[i]);
		if(i<N-1){
			fprintf(f,"\n");
		}
	}

	fclose(f);
	
	return 0;
}