#include "ColoredSquare.h"

int main()
{
	try {
		ColoredSquare Application{};

		Application.run();
	}
	catch (const std::exception& exception) {
		std::cout << "ColoredSquare: runtime exception: " << exception.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
