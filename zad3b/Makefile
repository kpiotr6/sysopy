test_st_O:
	(cd ../zad2;make main_st_O)
	../zad2/main<tests.txt > results_static_O.txt
test_st_O0:
	(cd ../zad2;make main_st_O0)
	../zad2/main<tests.txt > results_static_O0.txt
test_st_O2:
	(cd ../zad2;make main_st_O2)
	../zad2/main<tests.txt > results_static_O2.txt
test_dy_O:
	(cd ../zad2;make main_dy_O)
	../zad2/main<tests.txt > results_dynamic_O.txt
test_dy_O0:
	(cd ../zad2;make main_dy_O0)
	../zad2/main<tests.txt > results_dynamic_O0.txt	
test_dy_O2:
	(cd ../zad2;make main_dy_O2)
	../zad2/main<tests.txt > results_dynamic_O2.txt
test_sh_O:
	(cd ../zad2;make main_sh_O)
	../zad2/main<tests.txt > results_shared_O.txt
test_sh_O0:
	(cd ../zad2;make main_sh_O0)
	../zad2/main<tests.txt > results_shared_O0.txt
test_sh_O2:
	(cd ../zad2;make main_sh_O2)
	../zad2/main<tests.txt > results_shared_O2.txt
test_all: test_st_O test_st_O0 test_st_O2 test_dy_O test_dy_O0 test_dy_O2 test_sh_O test_sh_O0 test_sh_O2

clean:
	rm -f results_dynamic_O.txt
	rm -f results_dynamic_O0.txt
	rm -f results_dynamic_O2.txt
	rm -f results_shared_O.txt
	rm -f results_shared_O0.txt
	rm -f results_shared_O2.txt
	rm -f results_static_O.txt
	rm -f results_static_O0.txt
	rm -f results_static_O2.txt