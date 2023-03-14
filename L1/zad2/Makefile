#zad2
main_sh: memory.so
	gcc -o main  main.c ../zad1/libmemory.so
main_st: memory.a
	gcc -o main  main.c ../zad1/libmemory.a
main_dy: memory.so
	gcc -o main  main.c -ldl -D DYNAMIC
memory.a:
	(cd ../zad1; make memory.a)
memory.so:
	(cd ../zad1; make memory.so)
#zad 3 


memory.o_st_O:
	(cd ../zad1; make memory.o_st_O)
memory.o_st_O0:
	(cd ../zad1; make memory.o_st_O0)
memory.o_st_O2:
	(cd ../zad1; make memory.o_st_O2)

memory.so_O:
	(cd ../zad1; make memory.so_O)
memory.so_O0:
	(cd ../zad1; make memory.so_O0)
memory.so_O2:
	(cd ../zad1; make memory.so_O2)

main_sh_O: memory.so_O
	gcc -O -o  main   main.c ../zad1/libmemory.so
main_sh_O2: memory.so_O2
	gcc -O2 -o main  main.c ../zad1/libmemory.so
main_sh_O0: memory.so_O0
	gcc -O0 -o main   main.c ../zad1/libmemory.so
main_st_O: memory.a
	gcc -O -o main  main.c ../zad1/libmemory.a
main_st_O0: memory.a
	gcc -O0 -o main  main.c ../zad1/libmemory.a
main_st_O2: memory.a
	gcc -O2 -o main  main.c ../zad1/libmemory.a
main_dy_O: memory.so_O
	gcc -O -o main  main.c -ldl -D DYNAMIC
main_dy_O0: memory.so_O0
	gcc -O0 -o main  main.c -ldl -D DYNAMIC
main_dy_O2: memory.so_O2
	gcc -O2 -o main  main.c -ldl -D DYNAMIC