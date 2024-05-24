#include <iostream>
#include <string>
#include <bitset>
#include <chrono> // for tracking time 
#include <functional> // for tracking time
#include <vector> // for tracking time
double MeasureTime(std::function<void()> operation, unsigned int amount_of_measurements);

struct Galois_Field_PB { // Irreducible polynomial is p(x) = x^173 + x^10 + x^2 + x + 1 //generator of the field
private:
	static const int size_of_field = 173;
	std::bitset<size_of_field> array_of_coefficients_of_polynomial;
public:
	static const int array[];
	Galois_Field_PB() : array_of_coefficients_of_polynomial{0} {}; // default constructor, coefficients of polynomial is 0 
	Galois_Field_PB(unsigned int amount_of_bits) : array_of_coefficients_of_polynomial(amount_of_bits) {};
	Galois_Field_PB(const std::bitset<size_of_field>& entered_coefficients) : array_of_coefficients_of_polynomial(entered_coefficients) {};
	Galois_Field_PB(const std::string& line_of_coefficients, bool if_binary_string_or_hex); // binary = 1, hex = 0

	static Galois_Field_PB convert_line_to_array_of_polynomial_coefficients(const std::string&, bool if_binary_string_or_hex); // binary = 1, hex = 0
	bool operator==(const Galois_Field_PB&);
	bool operator!=(const Galois_Field_PB&);
	friend std::ostream& operator<<(std::ostream&, const Galois_Field_PB&); // output polynomial as polynomial
	std::string return_polynomial_as_binary_string() const;
	std::string return_polynomial_as_hex_string() const;
	Galois_Field_PB operator+(const Galois_Field_PB&);
	Galois_Field_PB operator*(const Galois_Field_PB&);
	Galois_Field_PB modulo_by_irreducible_polynomial(const std::bitset<2 * size_of_field - 1>&); // by some operation, increasing size of array_of_coefficients_of_polynomial
	Galois_Field_PB square_polynomial();
	Galois_Field_PB polynomimal_to_power(const Galois_Field_PB& degree_polymonial);
	unsigned int trace();
};