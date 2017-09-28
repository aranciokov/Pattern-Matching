all:
	gcc -c zeta.c -o zeta.o
	gcc -c pattern_matching_using_zeta.c -o pm_z.o
	gcc zeta.o pm_z.o -o pm_z.exe
	gcc -c sp.c -o sp.o
	gcc zeta.o sp.o -o sp.exe

pmz:
	gcc -c zeta.c -o zeta.o
	gcc -c pattern_matching_using_zeta.c -o pm_z.o
	gcc zeta.o pm_z.o -o pm_z.exe

sp:
	gcc -c zeta.c -o zeta.o
	gcc -c sp.c -o sp.o
	gcc zeta.o sp.o -o sp.exe

kmp:
	gcc -c kmp.c -o kmp.o
	gcc -c zeta.c -o zeta.o
	gcc -c sp.c -o sp.o
	gcc zeta.o sp.o kmp.o -o kmp.exe

tests:
	

clean:
