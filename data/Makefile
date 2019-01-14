all: assets

gobs: ../gobpack
	cd ../tools && make

data: ../jnbpack
	cd ../tools && make

assets: gobs data
	cd assets && make

clean:
	rm -f jumpbump.dat
	cd assets && make clean

