DATAFILES = ack.smp bump.mod calib.dat death.smp fly.smp jump.mod \
	jump.smp levelmap.txt level.pcx mask.pcx menu.pcx \
	menumask.pcx scores.mod splash.smp spring.smp
GOBS = font.gob numbers.gob objects.gob rabbit.gob

all: jumpbump.dat

font.gob:
	../../gobpack font

rabbit.gob:
	../../gobpack rabbit

numbers.gob:
	../../gobpack numbers

objects.gob:
	../../gobpack objects

jumpbump.dat: $(DATAFILES) $(GOBS) ../../jnbpack
	../../jnbpack -o ../jumpbump.dat $(DATAFILES) $(GOBS)

clean:
	rm -f $(GOBS)

