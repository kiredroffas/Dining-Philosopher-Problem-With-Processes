/* Erik Safford
 * Dining Philosophers Problem using IPC Semaphores
 * Spring 2019  */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <errno.h>
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define DEBUG 0

int chopsticks;  //chopsticks semaphore ID
                 //Must be global so all processes can access
char csInUse[5] = {0,0,0,0,0}; //For chopstick debugging, DEBUG must be turned on

/* successive calls to randomGaussian produce integer return values
   having a gaussian distribution with the given mean and standard
   deviation.  Return values may be negative.   */
int randomGaussian(int mean, int stddev) {
        double mu = 0.5 + (double) mean;
        double sigma = fabs((double) stddev);
        double f1 = sqrt(-2.0 * log((double) rand() / (double) RAND_MAX));
        double f2 = 2.0 * 3.14159265359 * (double) rand() / (double) RAND_MAX;
        if (rand() & (1 << 5))
                return (int) floor(mu + sigma * cos(f2) * f1);
        else
                return (int) floor(mu + sigma * sin(f2) * f1);
}

void eatThinkTime(int j) {
	int first, second;
	int eatTime, thinkTime;       
	int totalEatTime = 0, totalThinkTime = 0;
	struct sembuf op;  //sembuf structure to do semop operations on

	//Define the order that philosophers will pick up chopsticks
	//Want the last philosopher to pick up in a different order to avoid deadlock.
	if(j < 4) {  //If philo 0,1,2,3 , want to pickup LEFT then RIGHT
		first = j;
		second = j + 1;	
	}
	else {       //If philo 4 want to pickup RIGHT then LEFT
		first = 0;
		second = 4;
	}

	if(DEBUG) {printf("philosopher %d LEFT: %d RIGHT: %d\n",j,first,second); }
	
	srand(getpid()); //Seed random number generator for rand()
	                 //pid will be different for each process ensuring we get different 'random' values

	while(totalEatTime < 20) { //Want the philo to eat for at least 100 seconds
		//Think
		thinkTime = randomGaussian(11,7);
		if(thinkTime < 0) { //Don't want to think for a negative amount of time
			thinkTime = 0;
		}
		totalThinkTime += thinkTime;
		if(j == 0) {printf(KRED "Philosopher %d is thinking for %d seconds... (Total think time:%d)\n" KNRM,j,thinkTime,totalThinkTime);}
                if(j == 1) {printf(KGRN "Philosopher %d is thinking for %d seconds... (Total think time:%d)\n" KNRM,j,thinkTime,totalThinkTime);}
                if(j == 2) {printf(KYEL "Philosopher %d is thinking for %d seconds... (Total think time:%d)\n" KNRM,j,thinkTime,totalThinkTime);}
                if(j == 3) {printf(KMAG "Philosopher %d is thinking for %d seconds... (Total think time:%d)\n" KNRM,j,thinkTime,totalThinkTime);}
                if(j == 4) {printf(KCYN "Philosopher %d is thinking for %d seconds... (Total think time:%d)\n" KNRM,j,thinkTime,totalThinkTime);}
		sleep(thinkTime);

		//Get first chopstick
		op.sem_op = -1;
		op.sem_num = first;
		if(semop(chopsticks, &op, 1) == -1) {   //'Lock' chopstick
			fprintf(stderr,"semop error %d: %s\n",errno,strerror(errno));
                        exit(1);
		}

		if(DEBUG) {csInUse[first] = 1;}

		//Get second chopstick
		op.sem_op = -1;
		op.sem_num = second;
                if(semop(chopsticks, &op, 1) == -1) {   //'Lock' chopstick
			fprintf(stderr,"semop error %d: %s\n",errno,strerror(errno));
                        exit(1);
		}

		if(DEBUG) {csInUse[second] = 1;}

		//Eat
		eatTime = randomGaussian(9,3);
		if(eatTime < 0) {  //Don't want to eat for a negative amount of time
			eatTime = 0;
		}
		totalEatTime += eatTime;
		if(j == 0) {printf(KRED "Philosopher %d is eating for %d seconds... (Total eat time: %d)\n" KNRM,j,eatTime,totalEatTime);}
                if(j == 1) {printf(KGRN "Philosopher %d is eating for %d seconds... (Total eat time: %d)\n" KNRM,j,eatTime,totalEatTime);}
                if(j == 2) {printf(KYEL "Philosopher %d is eating for %d seconds... (Total eat time: %d)\n" KNRM,j,eatTime,totalEatTime);}
                if(j == 3) {printf(KMAG "Philosopher %d is eating for %d seconds... (Total eat time: %d)\n" KNRM,j,eatTime,totalEatTime);}
                if(j == 4) {printf(KCYN "Philosopher %d is eating for %d seconds... (Total eat time: %d)\n" KNRM,j,eatTime,totalEatTime);}
		if(DEBUG) {printf("[%d %d %d %d %d]\n",csInUse[0],csInUse[1],csInUse[2],csInUse[3],csInUse[4]);}
		sleep(eatTime);	

		//Release first chopstick
		op.sem_op = 1; 
		op.sem_num = first;
		if(semop(chopsticks, &op, 1) == -1) {   //'Unlock' chopstick
			fprintf(stderr,"semop error %d: %s\n",errno,strerror(errno));
                        exit(1);
		}

		if(DEBUG) {csInUse[first] = 0;}

		//Release second chopstick
		op.sem_op = 1; 
                op.sem_num = second;
                if(semop(chopsticks, &op, 1) == -1) {   //'Unlock' chopstick
			fprintf(stderr,"semop error %d: %s\n",errno,strerror(errno));
                        exit(1);
		}

		if(DEBUG) {csInUse[second] = 0;}
	}

	printf("\nPHILOSOPHER %d DONE EATING, WAITING... (Eat time: %d, Think time: %d)\n\n",j,totalEatTime,totalThinkTime);
}

int main(int argc, char **argv) {

	pid_t processes[5]; //Want to keep track of processes so all philos wait for each other before exiting
	int status;

	chopsticks = semget(IPC_PRIVATE, 5, IPC_CREAT | IPC_EXCL | 0600);  //chopsticks is global semID
	if(chopsticks == -1) {
		fprintf(stderr,"semget error %d: %s\n",errno,strerror(errno));
                exit(1);
	}

	for(int i = 0; i < 5; i++) {  //Initialize semaphores to 1 ('unlock' chopsticks)
		struct sembuf incSem = {i, 1, 0};
		if(semop(chopsticks, &incSem, 1) == -1) {
			fprintf(stderr,"semop error %d: %s\n",errno,strerror(errno));
                	exit(1);
		}
	}
	
	for(int i = 0; i < 5; i++) {
		int pID = fork(); //Want to make a process for each philosopher

		if(pID > 0) {   //Parent process
			processes[i] = pID;  //Save all 5 pID's so can wait on them
		}
		else if(pID == 0) {  //Child process is the philosopher
			eatThinkTime(i);  //Eat and think for required eat time, then exit
			exit(0);          //Parent process will wait for all philos to finish
		}
		else {
			fprintf(stderr,"Fork error %d: %s\n",errno,strerror(errno));
			exit(1);	
		}
	}

	for(int i = 0; i < 5; i++) {  //Make each parent process wait for all philos (5 child processes) to be done
		waitpid(processes[i], &status, 0);  //No leaving the table early!
		//Could also do this with just calling wait(NULL) 5 times
	}

	printf("All philosophers finished eating! Everyone leaves the table...\n");
	if(semctl(chopsticks, 0, IPC_RMID, 0) == -1) {  //Eliminate the created semaphore set
		fprintf(stderr,"semctl error %d: %s\n",errno,strerror(errno));
                exit(1);
	}
	return(0);
}


