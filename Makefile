SOURSE = src/app/**/*.cpp src/app/**/*.h
APPDIRS = src/app/ src/libs/lib_qt_gifimage/ objects/ src/app/styles/
CHFLAGS = --std=c++17 --language=c++ --enable=all --suppress=unusedFunction --suppress=missingInclude --suppress=useStlAlgorithm

ifeq ($(shell uname -s), Linux) 
	OPEN = x-www-browser
else
	OPEN = open
endif


all: 3DViewer
	./build/code/3DViewer

install: uninstall 3DViewer
	mkdir ~/Desktop/3DViewer
	cp build/code/3DViewer ~/Desktop/3DViewer

uninstall: clean
	rm -rf ~/Desktop/3DViewer
	
dist: 3DViewer 
	tar -cf 3DViewer.tgz build/code/3DViewer

dvi:
	$(OPEN) docum/README.html

3DViewer:
	rm -rf build
	mkdir build && cp -r $(APPDIRS) build/
	cd build/code && qmake -makefile QViewer.pro && make

test: 
	cd tests && rm -rf tests .qmake.stash test.o
	cd tests && qmake tests.pro && make
	./tests/tests

gcov_report: test
	lcov -c -d ./tests -o coverage.info
	genhtml coverage.info -o gcov_report
	$(OPEN) ./gcov_report/index.html

clang:
	cp ../materials/linters/.clang-format .
	clang-format -n $(SOURSE)
	rm .clang-format

check: clang
	cppcheck $(CHFLAGS) $(SOURSE) tests/test.cpp

valgrind: clean test
	valgrind --leak-check=full -s ./tests/tests

leaks: clean test
	CK_FORK=no leaks -atExit -- ./tests/tests

clean:
	rm -rf build/ gcov_report/
	rm -rf *.ini *.info 3DViewer.tgz
	cd tests && rm -rf tests .qmake.stash test.o *.gcda *.gcno Makefile
