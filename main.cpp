#include "Header.h"

int main() {

	Galois_Field_PB a("1dc10", 0);
	std::cout << a;
	std::cout << a.return_polynomial_as_binary_string();
}