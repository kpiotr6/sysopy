CC = gcc -c
BUILD = build
SOURCE = src

all: server user

server:user_struct
	${CC} -o ${BUILD}/server ${SOURCE}/server.c ${BUILD}/libuser_struct.a 

user: user_struct
	${CC} -o ${BUILD}/user ${SOURCE}/user.c ${BUILD}/libuser_struct.a 

user_struct:
	${CC} ${SOURCE}/user_struct.c -o ${BUILD}/user_struct.o 
	ar crs ${BUILD}/libuser_struct.a ${BUILD}/user_struct.o 
	rm ${BUILD}/user_struct.o

clean:
	rm -r ${BUILD}
	mkdir ${BUILD}