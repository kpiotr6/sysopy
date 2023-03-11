test_st:
	(cd ../zad2;make main_st)
	../zad2/main<tests.txt > results_static.txt
test_dy:
	(cd ../zad2;make main_dy)
	../zad2/main<tests.txt > results_dynamic.txt
test_sh:
	(cd ../zad2;make main_sh)
	../zad2/main<tests.txt > results_shared.txt
test_all: test_st test_dy test_sh
clean:
	rm -f results_dynamic.txt
	rm -f results_shared.txt
	rm -f results_static.txt
