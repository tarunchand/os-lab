#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

void foo(int arr[], int l, int r)
{
	int status;
	if(r-l<9)
	{
		int max = -1;
		int i;
		for(i=l;i<=r;++i)
		{
			if(arr[i] > max)
			max = arr[i];
		}

		printf("PID = %d \t parent PID = %d \t max = %d\n",getpid(),getppid(),max);
		exit(max);
	}
	else
	{
		int cpid1, cpid2;
		if((cpid1 = fork()) == 0)
		{
			r = (l+r)/2;
			foo(arr, l, r);
		}
		else if((cpid2 = fork()) == 0)
		{
			l = (l+r)/2 + 1;
			foo(arr, l, r);
		}
		else
		{
			waitpid(cpid1, &status, 0);
			int max1 = status >> 8;

			waitpid(cpid2, &status, 0);
			int max2 = status >> 8;

			int max = max1>max2?max1:max2;
			printf("PID = %d \t parent PID = %d \t max = %d\n",getpid(),getppid(),max);
			exit(max);
		}

	}

}


int main(int argc, char * argv[])
{
	srand((unsigned int)time(NULL));
	int arr[110];
	int n,i,status;

	if(argc!=2)
	{
		printf("Invalid arguments!!\n");
		exit(0);
	}

	n = atoi(argv[1]);

	int cpid;

	for(i=0;i<n;++i)
		arr[i] = rand()%128;


	printf("Original array is : ");
	for(i=0;i<n;++i)
		printf("%d ",arr[i]);

	printf("\n\n");

	if((cpid = fork()) == 0)
	{
		foo(arr, 0, n-1);
	}
	else
	{
		waitpid(cpid, &status, 0);

		int max = status >> 8;
		printf("The maximum value in the array is : %d\n",max);
	}
	sleep(2);
	exit(0);
}
