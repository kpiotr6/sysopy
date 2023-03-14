main_sh: memory.so
	(cd zad2;gcc -o main -g main.c ../zad1/libmemory.so)
main_st: memory.a
	(cd zad2;gcc -o main -g main.c ../zad1/libmemory.a)
main_dy: memory.so
	(cd zad2;gcc -o main -g main.c -ldl -D DYNAMIC)
memory.a:
	(cd zad1; make memory.a)
memory.so:
	(cd zad1; make memory.so)
