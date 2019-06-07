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
	int shmid,semid;
	struct student *st;
	struct sembuf pop,vop;
	FILE *fp;
	size_t sl=0;
	char *line;
	if(argc!=2)
	{
		printf("File name not given\n");
		exit(-1);
	}
	shmid=shmget(1110,sizeof(int),IPC_CREAT|0666);
	st=(struct student *)shmat(shmid,NULL,0);
	fp=fopen(argv[1],"r");
	if(fp==NULL)
	{
		perror("File does not exists\n");
		exit(1);
	}
	char *bf[4];
	int ck=0;
	printf("\nThe record in the file is\n"); 
	while(getline(&line,&sl,fp)!=(size_t)-1)
	{
		int szline=strlen(line)/2;
		char *str;
		int i=0;
		while(szline-->1)
		{
		str=strsep(&line," ");
		bf[i]=str;
		 i++;
		}
	printf("Record is:%s %s %d %f\n",bf[0],bf[1],atoi(bf[2]),atof(bf[3]));
	strcpy(st[ck].first_name,bf[0]);
	strcpy(st[ck].last_name,bf[1]);
	st[ck].rollno=atoi(bf[2]);
	st[ck].CGPA=atof(bf[3]);
	ck++;
	}
	fclose(fp);
	int bchanged=1;
	int b=1;
	semid=semget(5421,1,IPC_CREAT|0777);
	semctl(semid,0,SETVAL,0);
	printf("\nRecord copied into shared memory\n");
	while(1)
	{
		if(bchanged==1)
		{
			P(semid);
			fp=fopen(argv[1],"w");
			int l=0;
			while(l<ck)
			{
				fprintf(fp,"%s %s %d %f\n",st[l].first_name,st[l].last_name,
													st[l].rollno,st[l].CGPA);
				l++;
			}
			fclose(fp);
			if(b==1)
			{
				b==0;
			}
			else
			{
				printf("$$  You are rewriting the file  $$\n");
			}
			bchanged=0;
			V(semid);
		}
		sleep(5);
	}
return 0;
}
