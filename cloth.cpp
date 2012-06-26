#include "cloth.h"

using namespace std;

#define CONSTRUCTORFAIL(X) { \
				cout << X "\n" ; \
				constructorSuccessful = false; \
				return; \
			} \

Cloth::Cloth(){}
Cloth::Cloth(float xPos, float yPos, float zPos, float dimWidth, float dimHeight, unsigned int partWidth, unsigned int partHeight)
{
	//check params are valid
	if( dimWidth < 0.0f || dimHeight < 0.0f )
		CONSTRUCTORFAIL( "Negative dimensions specified for cloth");

	if( partWidth < 2 || partHeight < 2 )
		CONSTRUCTORFAIL("Invalid particle dimensions specified");

	//store width and height in particles
	pWidth = partWidth; pHeight = partHeight;

	//work out steps
	float xStep = dimWidth / (float)partWidth, yStep = dimHeight / (float)partHeight;

	//create mesh of particles
	try{
		particles = new ClothParticle[partWidth * partHeight];
		particleVerts = new float [partWidth * partHeight * 3];
	}
	catch(bad_alloc& exc){
		CONSTRUCTORFAIL("Not enough memory to create cloth");
	}
	for ( int i = 0; i < partWidth; i++)
	{
		for ( int j = 0; j < partHeight; j++)
		{
			particleVerts[(j * partWidth + i) * 3 + 0] = i*xStep + xPos;
			particleVerts[(j * partWidth + i) * 3 + 1] = j*yStep + yPos;
			particleVerts[(j * partWidth + i) * 3 + 2] = zPos;

			particles[j * partWidth + i] = (j * partWidth + i) * 3;
		}
	}
}

Cloth::~Cloth()
{
	cout << "About to delete particles\n";
	delete[] particles;
	delete[] particleVerts;
	cout << "Deleted particles\n";
}

unsigned int Cloth::getVBO()
{
	return VBO;
}

void Cloth::setVBO( unsigned int vbo )
{
	VBO = vbo;
}

float* Cloth::getVerts()
{
	return particleVerts;
}

unsigned int Cloth::getVertCount()
{
	return pWidth * pHeight;
}

unsigned int Cloth::getPartWidth()
{
	return pWidth;
}

unsigned int Cloth::getPartHeight()
{
	return pHeight;
}

bool Cloth::constructorSucceeded()
{
	return constructorSuccessful;
}

void Cloth::print()
{
	for(int i = 0 ; i < pWidth; i++)
		for(int j = 0 ; j < pHeight; j++)
			particles[j * pWidth + i].print(particleVerts);
}
