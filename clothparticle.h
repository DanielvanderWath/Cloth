#if !defined(__CLOTH_PARICLE_H__)
#define __CLOTH_PARICLE_H__

#include <iostream>

class ClothParticle
{
	private:
		size_t posOffset;
	public:
		ClothParticle();
		ClothParticle( size_t offset);
		~ClothParticle();
		void print(float *mesh);
		size_t getPosOffset();
};

#endif
