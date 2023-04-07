all: calc_b gath_b

calc_b:
	gcc -o calculator calculator.c
gath_b:
	gcc -o gatherer gatherer.c
test: all
	rm -f restults.txt
	while read -r line; do ./gatherer $$line >> restults.txt; done <test.txt