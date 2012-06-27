#if !defined(__SPRING_H__)
#define __SPRING_H__
#include "clothparticle.h"
#include <iostream>
#include <fstream>
#include <CL/opencl.h>

class Spring
{
	private:
		size_t A, B;
		static float stretchMaxX, stretchMinX, stretchRestX;
		static float stretchMaxY, stretchMinY, stretchRestY;
		static bool initialisedCL;
		static cl_device_id device;
		static cl_context context;
	public:
		Spring();
		Spring(size_t a, size_t b);
		~Spring();
		static void stretches(float minX, float maxX, float restX, float minY, float maxY, float restY);
		static bool initialiseCL();
		static bool move(int iterations);
};

#endif
