GXX=g++ -c -g
DEPS=-lGL -lglut -lGLEW -lGLU -lXmu -lXi

Main: Main.cpp CityObject.o GLUtils.o shaders.o readBMP.o
	g++ -g -o Main Main.cpp shaders.o CityObject.o GLUtils.o readBMP.o $(DEPS)
shaders.o: shaders.cpp shaders.h
	$(GXX) shaders.cpp shaders.h $(DEPS)
CityObject.o: CityObject.h CityObject.cpp
	$(GXX) CityObject.h CityObject.cpp $(DEPS)
GLUtils.o: GLUtils.h GLUtils.cpp
	$(GXX) GLUtils.h GLUtils.cpp $(DEPS)
readBMP.o: readBMP.cpp readBMP.h
	$(GXX) readBMP.cpp readBMP.h $(DEPS)
clean:
	rm -f *.o *~ Main texture_test
texture_test: texture_test.cpp shaders.cpp readBMP.cpp
	g++ -o texture_test texture_test.cpp shaders.cpp readBMP.cpp -lGL -lglut -lGLEW -lGLU -lXmu -lXi
