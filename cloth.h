#if !defined(__CLOTH_H__)
#define __CLOTH_H__

#include <iostream>
#include "clothparticle.h"
#include "spring.h"

class Cloth
{
	private:
		ClothParticle *particles;
		Spring *springs;
		float *particleVerts;
		unsigned int pWidth, pHeight;
		unsigned int VBO;

		//for the time being use this to signal that the constructor failed. Swap for exceptions or something later
		bool constructorSuccessful;
	public:
		Cloth();
		Cloth(float xPos, float yPos, float zPos, float dimWidth, float dimHeight, unsigned int partWidth, unsigned int partHeight);
		~Cloth();
		void setVBO(unsigned int vbo);
		unsigned int getVBO();
		float* getVerts();
		unsigned int getVertCount();
		unsigned int getPartWidth();
		unsigned int getPartHeight();
		bool constructorSucceeded();
		void print();
};

#endif

