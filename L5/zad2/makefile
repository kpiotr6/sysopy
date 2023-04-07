main_b:
	gcc -o main main.c
test: main_b
	rm -f restults.txt
	while read -r line; do ./main $$line >> restults.txt; done <test.txt