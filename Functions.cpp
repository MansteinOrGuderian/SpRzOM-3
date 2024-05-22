#include "Header.h"



Galois_Field_PB Galois_Field_PB::convert_line_to_array_of_polynomial_coefficients(const std::string& line_with_coefficients, bool if_line_binary) {
	if (if_line_binary == 1) {
		std::string binary_line_with_each_coefficients = line_with_coefficients;
		unsigned int line_with_coefficients_length = binary_line_with_each_coefficients.length();
		if (line_with_coefficients_length > size_of_field)
			throw std::exception("Unsupported length. Entered line is too big!"); // or maybe good idea in this case make each coefficients = 0
		while (line_with_coefficients_length < size_of_field) {
			binary_line_with_each_coefficients = '0' + binary_line_with_each_coefficients; // if some coefficients didn't entered, set them 0
			line_with_coefficients_length++;
		}
		unsigned int current_index = 0;
		while (current_index < line_with_coefficients_length) { // checking is symbols in string are 0 or 1
			if (binary_line_with_each_coefficients[current_index] != '1' && binary_line_with_each_coefficients[current_index] != '0')
				throw std::exception("Unsupported symbol in line!");
			current_index++;
		}
		return Galois_Field_PB(std::bitset<size_of_field>(binary_line_with_each_coefficients));
	}
	else {
		std::string binary_line_with_each_coefficients;
		unsigned int line_with_coefficients_length = line_with_coefficients.length();
		unsigned int current_index = 0;
		while (current_index < line_with_coefficients_length) {
			int current_hex_symbol_to_int = 0;
			if (line_with_coefficients[current_index] >= '0' && line_with_coefficients[current_index] <= '9') // numbers 0...9
				current_hex_symbol_to_int = line_with_coefficients[current_index] - '0'; // numbers 0...9
			else if (line_with_coefficients[current_index] >= 'a' && line_with_coefficients[current_index] <= 'f') // letters a...f
				current_hex_symbol_to_int = line_with_coefficients[current_index] - 'a' + 10;
			else
				throw std::exception("Unsupported symbol in line!");
			for (int index = 4; index >= 0; index--) //convert hex value to binary; f == 1111 (most possible 4 bit)
				binary_line_with_each_coefficients += (current_hex_symbol_to_int & (1 << index)); //check each bit of binary_code_of_hex_letter from most significant to least significant
		}
		unsigned int binary_line_with_each_coefficients_length = binary_line_with_each_coefficients.length();
		if (binary_line_with_each_coefficients_length > size_of_field)
			throw std::exception("Unsupported length. Entered line is too big!"); // or maybe good idea in this case make each coefficients = 0
		while (binary_line_with_each_coefficients_length < size_of_field) {
			binary_line_with_each_coefficients = '0' + binary_line_with_each_coefficients; // if some coefficients didn't entered, set them 0
			binary_line_with_each_coefficients_length++;
		}
		return Galois_Field_PB(std::bitset<size_of_field>(binary_line_with_each_coefficients));
	}
}

Galois_Field_PB::Galois_Field_PB(const std::string& line_with_coefficients, bool if_line_binary) {
	*this = convert_line_to_array_of_polynomial_coefficients(line_with_coefficients, if_line_binary);
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