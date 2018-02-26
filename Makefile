TARGET=matrix 

CFLAGS+=-g

$(TARGET):	matrix.o

clean:
	rm -f *.o $(TARGET)

sync:	clean
	aws s3 sync /home/cplus/projects/matrix/ s3://prjdoc/matrix/

test:	clean $(TARGET)
	./matrix 1 1 7 2 7 2 2 3 2 4

