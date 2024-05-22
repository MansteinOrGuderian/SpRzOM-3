#include <iostream>
#include <string>
#include <bitset>
#include <chrono> // for tracking time 
#include <functional> // for tracking time
#include <vector> // for tracking time

struct Galois_Field_PB { // Irreducible polynomial is p(x) = x^173 + x^10 + x^2 + x + 1 //generator of the field
private:
	static const int size_of_field = 173;
	std::bitset<size_of_field> array_of_coefficients_of_polynomial;
public:
	Galois_Field_PB() : array_of_coefficients_of_polynomial{0} {}; // default constructor, coefficients of polynomial is 0 
	Galois_Field_PB(unsigned int amount_of_bits) : array_of_coefficients_of_polynomial(amount_of_bits) {};
	Galois_Field_PB(const std::bitset<size_of_field>& entered_coefficients) : array_of_coefficients_of_polynomial(entered_coefficients) {};
	Galois_Field_PB(const std::string& line_of_coefficients);

	static Galois_Field_PB convert_line_to_array_of_polynomial_coefficients(const std::string&);
	bool operator==(const Galois_Field_PB&);
	bool operator!=(const Galois_Field_PB&);
	friend std::ostream& operator<<(std::ostream&, const Galois_Field_PB&); // output polynomial as string
};
