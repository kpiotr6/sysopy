server_b:user_struct_b
	gcc -g -o server server.c libuser_struct.a
user_struct_b:
	gcc -g -c user_struct.c
	ar crs libuser_struct.a user_struct.o
user_b: user_struct_b
	gcc -g -o user user.c libuser_struct.a
