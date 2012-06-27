#include <iostream>
#include "graphics.h"
#include "cloth.h"

using namespace std;

int main(int argc, char **argv)
{
	cout << "Hello World\n";

	if(Graphics::init())
	{
		cout << "Failed to initialise graphics, exiting\n";
		exit(1);
	}

	Cloth myCloth = Cloth(-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 100, 100);

	myCloth.print();

	myCloth.setVBO( Graphics::createVBO());

	if(Graphics::copyVertsToVBO(myCloth.getVBO(), myCloth.getVertCount(),  myCloth.getVerts()))
	{
		cout << "Failed to copy vertex data to VBO, exiting\n";
		exit(1);
	}

	if( Spring::initialiseCL())
	{
		cout << "Failed to initialise OpenCL\n";
		exit(1);
	}

	//while(whatever)
	{
		Graphics::clear();

		if(Graphics::drawCloth(myCloth.getVBO(), myCloth.getPartWidth(), myCloth.getPartHeight()))
		{
			cout << "drawCloth failed, exiting\n";
			exit(1);
		}

		Graphics::swapBuffers();
	}
	cout << "press any key to continue\n";
	getchar();

	Graphics::term();

	return 0;
}
