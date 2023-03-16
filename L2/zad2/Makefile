main: main_o 
	gcc -o main main.o
main_o: 
	gcc -c main.c
test:main
	./main Lorem.txt tmp1 > pomiar_zad_2.txt
	./main tmp1 tmp2
	diff -s tmp2 Lorem.txt
	diff -s tmp2_buff Lorem.txt
	rm tmp1
	rm tmp1_buff
	rm tmp2
	rm tmp2_buff