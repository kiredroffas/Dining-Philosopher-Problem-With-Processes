.PHONY: clean run runcolor #Tell make that not associated with building files

#Compile source file and produce executable
all : philo.o
	cc -o philo philo.c -lm
#Runs compiled executable
run :
	./philo
#Made a colored version for fun, its cool to watch (only goes to 20 seconds eating)
color : philoColor.o
	cc -o philoColor philoColor.c -lm
#Run colored version
runcolor :
	./philoColor
#Remove object and temp files
clean :
	rm philo philoColor *.o

