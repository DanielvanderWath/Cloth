SRC := 	main.cpp \
	cloth.cpp \
	clothparticle.cpp \
	graphics.cpp \
	spring.cpp \

FLAGS := -DGL_GLEXT_PROTOTYPES \

LIBS := -lGL \
	-lSDL \
	-lOpenCL \

all : $(SRC)
	g++ -o cloth -g  $(FLAGS) $(SRC) $(LIBS)

