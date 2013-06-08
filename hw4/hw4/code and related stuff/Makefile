GXX=g++ -c -g
DEPS=-lGL -lglut -lGLEW -lGLU -lXmu -lXi

main: main.cpp BezierCurve.o
	g++ -g -o main main.cpp $(DEPS)
BezierCurve.o: BezierCurve.cpp BezierCurve.h
	$(GXX) BezierCurve.cpp BezierCurve.h $(DEPS)
