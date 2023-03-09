main_st:memory.a
	gcc -o main main.c -L. libmemory.a
main_sh:memory.so
	gcc -o main main.c -L. libmemory.so
memory.o_sh:
	gcc -fPIC -c memory.c
memory.o_st:
	gcc -c memory.c
memory.a: memory.o_st
	ar cr libmemory.a memory.o
memory.so: memory.o_sh
	gcc -fPIC -shared -o libmemory.so memory.o