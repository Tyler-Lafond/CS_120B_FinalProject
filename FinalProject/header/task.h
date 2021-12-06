
// Permission to copy is granted provided that this header remains intact.
// This software is provided with no warranties.

////////////////////////////////////////////////////////////////////////////////

typedef struct task {
	int state; //Current State
	unsigned long period; //Tick rate of task
	unsigned long elapsedTime; //Time since last tick
	int (*TickFct)(int); //Function for task's tick
} task;

task tasks [4];

const unsigned char tasksNum = 4;
const unsigned long tasksPeriodGCD = 1000;
const unsigned long periodReadSpeed = 1000;
const unsigned long periodSpeed = 1000;
const unsigned long periodPosition = 1000;
const unsigned long periodDisplay = 1000;

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
