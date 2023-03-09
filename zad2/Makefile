main_sh: memory.so
	gcc -o main main.c ../zad1/libmemory.so
main_st: memory.a
	gcc -o main main.c ../zad1/libmemory.a
main_dy: memory.so
	gcc -o main main.c -ldl -D DYNAMIC
memory.a:
	(cd ../zad1; make memory.a)
memory.so:
	(cd ../zad1; make memory.so)
