
simulation_b: hairdresser_b client_b
	gcc simul.c sem.c shm.c queue.c  -lrt -lpthread -o simul

hairdresser_b:
	gcc hairdr.c sem.c shm.c queue.c  -lrt -lpthread -o hairdr

client_b:
	gcc client.c sem.c shm.c queue.c  -lrt -lpthread -o client
