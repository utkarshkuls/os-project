#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

#define REQUESTING 0
#define QUERYING 1
#define IDLE -1
#define STUDENT 0
#define FACULTY 1

struct time{
	int hh,mm;
};

struct Student{
	int id;
	char* name;
	int q_time;
	int state;
	struct time arrival; 
};
struct Faculty{
	int id;
	char* name;
	int q_time;
	int state;
	struct time arrival;
};
struct Instructor{
	int r_id;//id of the querying student
	char* r_name;//name of the querying student
	int rem_time;
	struct time clock;
	int q_turn;//Strudent, Faculty or IDLE
}sudesh;

struct queue{
	int id;
	char* name;
	struct queue* next;
}*f_front=NULL,*f_rear=NULL,*s_front=NULL,*s_rear=NULL;

void ins_q(struct queue **front,struct queue **rear,int id,char* name){
	struct queue* ptr=(struct queue*)malloc(sizeof(struct queue));
	ptr->id=id;
	strcpy(ptr->name,name);
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
	if(*front==NULL)return NULL;
	if(*front==*rear){
		*front=NULL;
		*rear=NULL;
	}
	else{
		*front=(*front)->next;
	}
	return temp;
}
void clock_tick(struct Instructor* s){
	s->clock.mm++;
	if(s->clock.mm>=60){
		s->clock.hh++;
		s->clock.mm=0;
	}
}

int switch_process(struct queue ** ptr_f,struct queue** ptr_r){
	//insert element from SUDESH to apt queue
	struct queue* temp=NULL;
	if(sudesh.q_turn!=IDLE)
		ins_q(ptr_f,ptr_r,sudesh.r_id,sudesh.r_name);
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


int main(){}

