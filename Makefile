all: sample2D

sample2D: Sample_GL3_2D.cpp glad.c
	g++ -o sample2D Sample_GL3_2D.cpp glad.c -framework OpenGL -framework Cocoa -lglfw -lSOIL
clean:
	rm sample2D
