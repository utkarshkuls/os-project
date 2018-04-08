#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#define REQUESTING 0
#define QUERYING 1
#define IDLE -1
#define STUDENT 0
#define FACULTY 1
char type[][10]={"STUDENT","FACULTY"};
int nof=0,nos=0;
struct time{
	int hh,mm;
};

struct Student{
	int id;
	char name[20];
	int q_time;
	int state;
	struct time arrival; 
};
struct Faculty{
	int id;
	char name[20];
	int q_time;
	int state;
	struct time arrival;
};
struct Instructor{
	int r_id;//id of the querying student
	char r_name[20];//name of the querying student
	int rem_time;
	struct time clock;
	int q_turn;//Strudent, Faculty or IDLE
}sudesh;

struct queue{
	int id;
	char name[20];
	int qt;
	struct queue* next;
}*f_front=NULL,*f_rear=NULL,*s_front=NULL,*s_rear=NULL;

void ins_q(struct queue **front,struct queue **rear,int id,char* name,int qt){
	struct queue* ptr=(struct queue*)malloc(sizeof(struct queue));
	ptr->id=id;
	strcpy(ptr->name,name);
	ptr->qt=qt;
	ptr->next=NULL;
	if(*front==NULL){
		*front=ptr;
		*rear=ptr;
	}
	else{
		(*rear)->next=ptr;
		*rear=ptr;
	}

}

struct queue* del_q(struct queue **front,struct queue **rear){
	struct queue* temp=*front;
	if(*front==NULL){
	temp=(struct queue*)malloc(sizeof(struct queue));
	temp->id=-1;
	strcpy(temp->name,"\0");
	temp->qt=0;
	temp->next=NULL;
	return temp;
	}
	if(*front==*rear){
		*front=NULL;
		*rear=NULL;
	}
	else{
		*front=(*front)->next;
	}
	return temp;
}
int tick(struct Instructor* s){
	s->clock.mm++;
	if(s->clock.mm>=60){
		s->clock.hh++;
		s->clock.mm=0;
	}
	if(s->clock.hh==12&&s->clock.mm>30)
		return 0;
	else return 1;
}

int switch_process(struct queue ** ptr_f,struct queue** ptr_r){
	//insert element from SUDESH to apt queue
	struct queue* temp=NULL;
	if(sudesh.q_turn!=IDLE)
		ins_q(ptr_f,ptr_r,sudesh.r_id,sudesh.r_name,sudesh.rem_time);
	temp=del_q(ptr_f,ptr_r);
		/*if(sudesh.q_turn==FACULTY){
			f_front=ptr_f;
			f_rear=ptr_r;
		}
		else if(sudesh.q_turn==STUDENT){
			s_front=ptr_f;
			s_rear=ptr_r;
		}*/
	sudesh.r_id=temp->id;
	strcpy(sudesh.r_name,temp->name);
	sudesh.rem_time=temp->qt;
	free(temp);
}

int switch_queue(struct queue*** front,struct queue*** rear){
	sudesh.q_turn=1-sudesh.q_turn;
	if(sudesh.q_turn==FACULTY){
	*front=&s_front;
	*rear=&s_rear;
	}
	else if(sudesh.q_turn==STUDENT){
	*front=&f_front;
	*rear=&f_rear;
	}
}

void recorder(struct Faculty *F,struct Student* S){
	printf("Enter no of Faculties\n");
	scanf(" %d",&nof);
	printf("Enter no of Students\n");
	scanf(" %d",&nos);
	for(int i=0;i<nof;i++){
		printf("Enter id of Faculty %d\n",i+1);
		scanf(" %d",&F[i].id);
		printf("Enter name of Faculty %d\n",i+1);
		scanf("%s",F[i].name);
		printf("Enter total querrying time of Faculty %d\n",i+1);
		scanf(" %d",&F[i].q_time);
		printf("Time of arrival of Faculty %d\nEnter hh\n",i+1);
		scanf("%d",&F[i].arrival.hh);
		printf("Enter mm\n");
		scanf("%d",&F[i].arrival.mm);
		F[i].state=IDLE;
	}
	for(int i=0;i<nos;i++){
		printf("Enter id of Student %d\n",i+1);
		scanf(" %d",&S[i].id);
		printf("Enter name of Student %d\n",i+1);
		scanf("%s",S[i].name);
		printf("Enter total querrying time of Student %d\n",i+1);
		scanf(" %d",&S[i].q_time);
		printf("Time of arrival of Student %d\nEnter hh\n",i+1);
		scanf("%d",&S[i].arrival.hh);
		printf("Enter mm\n");
		scanf("%d",&S[i].arrival.mm);
		S[i].state=IDLE;
	}
}

void initialise_sudesh(){
	sudesh.r_id=IDLE;
	strcpy(sudesh.r_name,"\0");
	//sudesh.rem_time.hh=2;
	//sudesh.rem_time.mm=30;
	sudesh.clock.hh=10;
	sudesh.clock.mm=30;
	sudesh.q_turn=IDLE;

}


int main(){
/*
	ins_q(&f_front,&f_rear,1,"utkarsh",2);
	ins_q(&s_front,&s_rear,2,"utkarsh",2);
	del_q(&s_front,&s_rear);
	if(s_front==NULL)
		printf("NULL\n");
	else
	printf("%d * %d\n",s_front->id,f_front->id);
*/
	struct Student* S=(struct Student*)malloc(nos*sizeof(struct Student));
	struct Faculty* F=(struct Faculty*)malloc(nof*sizeof(struct Faculty));
	int TQQ=2,TQP=2;
	struct queue **front=&s_front,**rear=&s_rear;
	sudesh.q_turn=STUDENT;
	recorder(F,S);//initialising the pointers
	initialise_sudesh();
	do{
		printf("T - %d:%d   TQP - %d  TQQ - %d\n",sudesh.clock.hh,sudesh.clock.mm,TQP,TQQ);
		for(int i=0;i<nos;i++){
			if(S[i].arrival.hh==sudesh.clock.hh && S[i].arrival.mm==sudesh.clock.mm&& S[i].state==IDLE){
				ins_q(&s_front,&s_rear,S[i].id,S[i].name,S[i].q_time);
				S[i].state=REQUESTING;
			}
		}
		for(int j=0;j<nof;j++){
			if(F[j].arrival.hh==sudesh.clock.hh && F[j].arrival.mm==sudesh.clock.mm && F[j].state==IDLE){
				ins_q(&f_front,&f_rear,F[j].id,F[j].name,F[j].q_time);
			
				F[j].state=REQUESTING;
			}
		}
		if(f_front==NULL&&s_front==NULL){TQQ++;}
		//else if(f_front!=NULL&&s_front!=NULL){
		else{
			if(TQQ==0||front==NULL){
				switch_queue(&front,&rear);
				TQQ=2;TQP=0;
			}
			if(TQP==0||sudesh.rem_time<=0){
				switch_process(front,rear);
				TQP=2;
			}
		}
		/*}
		else{
			if(f_front==NULL){
				front=&s_front;
				rear=&s_rear;
			}
			else if(s_front==NULL){
				front=&f_front;
				rear=&f_rear;
			}
		}
		if(sudesh.rem_time==0){
			switch_process(front,rear);
		}*/
		if(sudesh.r_id==-1){
			TQP=2;
			printf("SUDESH IS FREE\n\n");
		}
		else{
			printf("%s %s Querrying\n\n",type[sudesh.q_turn],sudesh.r_name);
			sudesh.rem_time--;
		}
		sleep(1);
		TQQ--;
		TQP--;

	}while(tick(&sudesh));
	free(S);
	free(F);
}


