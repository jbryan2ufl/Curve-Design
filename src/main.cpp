
#include "application.h"

int main()
{
	Application app{};
	app.init();
	app.run();
	app.close();
}