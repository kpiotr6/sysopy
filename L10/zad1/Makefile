CC = gcc -g -D_GNU_SOURCE 
BUILD = build
SOURCE = src

all:clean server user

run: all
	build/server 30001 ./abc

server:user_struct
	${CC} -o ${BUILD}/server ${SOURCE}/server.c ${BUILD}/libuser_struct.a -lpthread

user: user_struct
	${CC} -o ${BUILD}/user ${SOURCE}/user.c ${BUILD}/libuser_struct.a 

user_struct:
	${CC} -c ${SOURCE}/user_struct.c -o ${BUILD}/user_struct.o 
	ar crs ${BUILD}/libuser_struct.a ${BUILD}/user_struct.o 
	rm ${BUILD}/user_struct.o

clean:
	rm -rf ${BUILD}
	mkdir ${BUILD}