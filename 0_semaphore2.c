#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<time.h>
#include<unistd.h>
#define P(s) semop(s,&pop,1)
#define V(s) semop(s,&vop,1)

struct student 
{
	char first_name[20];
	char last_name[20];
	int rollno;
	float CGPA;
};

int main(int argc,char **argv) 
{
	int semid,shmid;
	FILE *fp;
	int ch;
	struct student *st;
	struct sembuf pop,vop;
	
	shmid=shmget(1110,sizeof(int),0666);
	st=(struct student *)shmat(shmid,NULL,0);
	semid=semget(5421,1,IPC_CREAT|0777);
	semctl(semid,0,SETVAL,0);
	
	int bchanged=0;
	printf("$$ Enter '0' to exit process $$\n");
	while(1)
	{
	    printf("\n\nEnter the rollno of the student:");
	    scanf("%d",&ch); 
	    if(ch==0)
	    {
	    	printf("Exiting from process Y\n");
	    	exit(EXIT_SUCCESS);
	    }
	    int i=0,flag=0;
	    while(i<100)
	    {
	    	if(st[i].rollno==ch)
	    	{
	    	   flag = 1;
				printf("\nFirst Name \t %s\n",st[i].first_name);
				printf("Last Name  \t %s\n",st[i].last_name);
				printf("Rollno     \t %d\n",st[i].rollno);
				printf("CGPA       \t %f\n",st[i].CGPA);
				break;
	    	}
	    	else
	    	{
	    		i++;
	    	}
	    }
	    if(flag==1)
	    {
	    	int chz;
			printf("\nDo you want to modify CGPA of the student(0 to exit|1 to go|2 continue):");
	    	scanf("%d",&chz);
	    	if(chz==0)
	    	{
	    		exit(0);
	    	}
	    	if(chz==2)
	    	{
	    		continue;
	    	}
	    	if(chz==1)
	    	{
	    		float cg;
	    		printf("\nCGPA:");
	    		scanf("%f",&cg);
	    		P(semid);
				st[i].CGPA =cg;
				bchanged = 1;
				V(semid);
	    	}
	    }
	    sleep(2);
	}
	
return 0;
}
