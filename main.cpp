#include "Header.h"

int main() {

	Galois_Field_PB a("1dffffec10", 0);
	Galois_Field_PB b("cd842c21", 0);

	std::cout << (a + b).return_polynomial_as_hex_string(); // +
	
	//std::cout << a.return_polynomial_as_hex_string() <<"\n\n";
}