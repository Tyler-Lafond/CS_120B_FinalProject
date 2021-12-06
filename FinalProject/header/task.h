
// Permission to copy is granted provided that this header remains intact.
// This software is provided with no warranties.

////////////////////////////////////////////////////////////////////////////////

//Setup the task structure
typedef struct task {
	int state; //Current State
	unsigned long period; //Tick rate of task
	unsigned long elapsedTime; //Time since last tick
	int (*TickFct)(int); //Function for task's tick
} task;

//Only 4 tasks in final version
task tasks [4];

//Define periods for tasks as well as the number of tasks
const unsigned char tasksNum = 4;
const unsigned long tasksPeriodGCD = 250;
const unsigned long periodReadSpeed = 500;
const unsigned long periodSpeed = 500;
const unsigned long periodPosition = 500;
const unsigned long periodDisplay = 250; //To accomodate for the screen being slower than the ATMega, the period is lower than the rest of the SMs

void TimerISR() { //moved from timer.h; uses task scheduler system
	unsigned char num;
	for (num = 0; num < tasksNum; num++) {
		if (tasks[num].elapsedTime >= tasks[num].period ) {
			tasks[num].state = tasks[num].TickFct(tasks[num].state);
			tasks[num].elapsedTime = 0;
		}
		tasks[num].elapsedTime += tasksPeriodGCD;
	}
}
