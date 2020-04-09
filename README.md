# Dining-Philosopher-Problem-With-Processes
* The Dining Philosopher Problem is as follows:
    * Five silent philosophers sit at a round table with bowls of noodles. Forks are placed between each pair of adjacent philosophers.
    * Each philosopher must alternately think and eat. However, a philosopher can only eat noodles when they have both left and right forks. Each fork can be held by only one philosopher and so a philosopher can use the fork only if it is not being used by another philosopher. After an individual philosopher finishes eating, they need to put down both forks so that the forks become available to others. A philosopher can only take the fork on their right or the one on their left as they become available and they cannot start eating before getting both forks.
    * Eating is not limited by the remaining amounts of noodles or stomach space; an infinite supply and an infinite demand are assumed.
    * The problem is how to design a discipline of behavior (a concurrent algorithm) such that no philosopher will starve; i.e., each can forever continue to alternate between eating and thinking, assuming that no philosopher can know when others may want to eat or think. <br>
![Alt text](/screenshots/dp.png?raw=true "dp") 
## Implementation
* This C program uses fork() to create 5 child processes, each representing one philosopher.
* Using semaphores each philosopher repeats a cycle of eating and thinking until all philosophers have eaten for at least 50 seconds, at which point the philosopher process will "leave the table" (terminate).
* There are 5 chopsticks at the table (one between each pair of adjacent philosophers).
* In order to eat, a philosopher must acquire both the chopsticks adjacent to them. If they cannot they are made to wait until they can. The wait time does not count as thinking time.
* Once a philospher has a pair of chopsticks, they eat for the expected period of time and then replace (release) the chopsticks onto the table and begin their period of thinking.
* Each philosopher repeats this process until they have eaten for a total of 50 seconds or more.
* A function called randomGaussian() is used to generate random thinking/eating times for each philosopher. The mean eating times are 9 and 11 seconds, with standard deviations of 3 and 7 seconds. If randomGaussian() returns a negative number as a time value, it is changed to 0.
* Chopsticks are represented with an array of IPC semaphores, each representing one chopstick.
* Program output includes the eating and thinking time of each philospher respectively, a message is printed once a philospher is done eating.
* A colored implementation is available to see visualize the different philosphers better.
* A DEBUG flag is included to see certain semaphore/chopstick output between each process, and can be set by changing the the define value to 1 (initially 0).
## Screenshots
### Original (Philosophers eat for minimum of 50 seconds)
![Alt text](/screenshots/sc1.png?raw=true "sc1") 
![Alt text](/screenshots/sc2.png?raw=true "sc2") 
![Alt text](/screenshots/sc3.png?raw=true "sc3")
### Colored (Philosophers eat for minimum of 20 seconds) 
![Alt text](/screenshots/sc4.png?raw=true "sc4") 