#include "clothparticle.h"

using namespace std;

ClothParticle::ClothParticle(){}
ClothParticle::ClothParticle( size_t offset )
{
	posOffset = offset;
}

ClothParticle::~ClothParticle()
{}

void ClothParticle::print(float *mesh)
{
	cout << "Particle at " << (mesh + posOffset)[0] << " : " << (mesh + posOffset)[1] << " : " << (mesh + posOffset)[2] << endl;
}

size_t ClothParticle::getPosOffset()
{
	return posOffset;
}
