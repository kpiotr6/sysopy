#zad1
memory.o_sh:
	gcc -fPIC -c memory.c -g
memory.o_st:
	gcc -c memory.c -g
memory.a: memory.o_st
	ar cr libmemory.a memory.o
memory.so: memory.o_sh
	gcc -fPIC -shared -o libmemory.so -g memory.o
clean:
	rm memory.o
#zad2 i 3
memory.o_sh_O:
	gcc -fPIC -O -c memory.c -g
memory.o_sh_O0:
	gcc -fPIC -O0 -c memory.c -g
memory.o_sh_O2:
	gcc -fPIC -O2 -c memory.c -g

memory.o_st_O:
	gcc -c -O memory.c -g
memory.o_st_O0:
	gcc -c -O0 memory.c -g
memory.o_st_O2:
	gcc -c -O2 memory.c -g

memory.so_O: memory.o_sh_O
	gcc -fPIC -O -shared -o libmemory.so -g memory.o 
memory.so_O0: memory.o_sh_O0
	gcc -fPIC -O0 -shared -o libmemory.so -g memory.o
memory.so_O2: memory.o_sh_O2
	gcc -fPIC -O2 -shared -o libmemory.so -g memory.o 

