SRC := 	main.cpp \
	cloth.cpp \
	clothparticle.cpp \
	graphics.cpp \

FLAGS := -DGL_GLEXT_PROTOTYPES \

LIBS := -lGL \
	-lSDL \

all : $(SRC)
	g++ -o cloth -g  $(FLAGS) $(SRC) $(LIBS)

