#include "Header.h"



Galois_Field_PB Galois_Field_PB::convert_line_to_array_of_polynomial_coefficients(const std::string& line_with_coefficients) {
	std::string whole_line_with_each_coefficients = line_with_coefficients;
	unsigned int line_with_coefficients_length = whole_line_with_each_coefficients.length();
	if (line_with_coefficients_length > size_of_field)
		throw std::exception("Unsupported length. Entered line is too big!"); // or maybe good idea in this case make each coefficients = 0
	while (line_with_coefficients_length < size_of_field) {
		whole_line_with_each_coefficients = '0' + whole_line_with_each_coefficients; // if some coefficients didn't entered, set them 0
		line_with_coefficients_length++;
	}
	std::cout << whole_line_with_each_coefficients;
	unsigned int current_index = 0;
	while (current_index < line_with_coefficients_length) { // checking is symbols in string are 0 or 1
		if (whole_line_with_each_coefficients[current_index] != '1' && whole_line_with_each_coefficients[current_index] != '0')
			throw std::exception("Unsupported symbol in line!");
		current_index++;
	}
	return Galois_Field_PB(std::bitset<size_of_field>(whole_line_with_each_coefficients));
}

Galois_Field_PB::Galois_Field_PB(const std::string& line_with_coefficients) {
	*this = convert_line_to_array_of_polynomial_coefficients(line_with_coefficients);
}

bool Galois_Field_PB::operator==(const Galois_Field_PB& Right_polynomial) {
	return (this->array_of_coefficients_of_polynomial == Right_polynomial.array_of_coefficients_of_polynomial);
}

bool Galois_Field_PB::operator!=(const Galois_Field_PB& Right_polynomial) {
	return !(*this == Right_polynomial);
}

std::ostream& operator<< (std::ostream& out, const Galois_Field_PB& Data) {
	out << "Polynomial is:\t" << "\n\n";
	return out;
}