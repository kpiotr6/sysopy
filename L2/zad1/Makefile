main_sys:
	gcc -c main.c -D SYS
	gcc -o main main.o 
main_lib:
	gcc -c main.c 
	gcc -o main main.o
test_sys:main_sys
	./main a b Lorem.txt Lorem2.txt > tmp1
	rm Lorem2.txt
test_lib:main_lib
	./main a b Lorem.txt Lorem2.txt > tmp2
	rm Lorem2.txt
test_all:test_sys test_lib
	cat tmp1 tmp2 > pomiar_zad_1.txt
	rm tmp1
	rm tmp2
	./main a b Lorem.txt Lorem2.txt ; tr a b < Lorem.txt > tmp ; diff -s tmp Lorem2.txt
	rm tmp
	rm Lorem2.txt