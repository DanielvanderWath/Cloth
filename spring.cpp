#include "spring.h"

float Spring::stretchMinX;
float Spring::stretchMaxX;
float Spring::stretchRestX;
float Spring::stretchMinY;
float Spring::stretchMaxY;
float Spring::stretchRestY;
bool Spring::initialisedCL=false;
cl_device_id Spring::device;
cl_context Spring::context;

using namespace std;

Spring::Spring(){}
Spring::Spring(size_t a, size_t b)
{
	A = a;
	B = b;
}
Spring::~Spring(){}

void Spring::stretches(float minX, float maxX, float restX, float minY, float maxY, float restY)
{
	stretchMinX = minX;
	stretchMaxX = maxX;
	stretchRestX = restX;
	stretchMinY = minY;
	stretchMaxY = maxY;
	stretchRestY = restY;
}

bool Spring::initialiseCL()
{
	cl_platform_id platform;
	cl_context_properties properties[] = { CL_CONTEXT_PLATFORM, 0, 0};
	cl_int eCode;

	//create an OpenCL context using the first platform and device proffered
	clGetPlatformIDs(1, &platform, NULL);
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	properties[1] = (cl_context_properties)platform;
	context = clCreateContext(properties, 1, &device, NULL, NULL, &eCode);

	if(context == NULL)
	{
		//TODO: a proper CL error handler
		cout << "Failed to create CL context. Error: 0x" << hex << eCode;
		return true;
	}

	//load in the kernel source
	ifstream file;
	file.open("cloth.cl");
	if( file.is_open())
	{
		long fileStart = file.tellg();
		file.seekg(0, ios::end);
		long fileSize = file.tellg() - fileStart;

		file.close();
	}

	return false;
}

bool Spring::move(int iterations)
{
	if( !initialisedCL )
		if( initialiseCL() )
		{
			cout << "Failed to initialise OpenCL\n";
			return true;
		}

	
	
}
