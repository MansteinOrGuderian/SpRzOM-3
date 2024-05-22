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
			for (int index = 3; index >= 0; index--) //convert hex value to binary; f == 1111 (most possible 4 bit)
				binary_line_with_each_coefficients += ((current_hex_symbol_to_int & (1 << index)) ? '1' : '0'); //check each bit of binary_code_of_hex_letter from most significant to least significant
			current_index++;
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
	out << "Polynomial is:\t";// << Data.array_of_coefficients_of_polynomial;
	int degree = Data.size_of_field - 1;
	int index_of_coefficient = Data.size_of_field - 1;
	while (degree >= 0) {
		out << Data.array_of_coefficients_of_polynomial[index_of_coefficient] << " * x^" << degree << (degree != 0 ? " + " : "\n\n");
		degree--; index_of_coefficient--;
	}
	return out; 
}

std::string Galois_Field_PB::return_polynomial_as_binary_string() const {
	std::string polynomial_as_binary_string;
	int current_position = size_of_field - 1;
	while (current_position >= 0) {
		polynomial_as_binary_string += (array_of_coefficients_of_polynomial.test(current_position) ? '1' : '0');
		current_position--; // using inline in bitset function test(), which return 1, if bit in current_position set to 1
	}
	return polynomial_as_binary_string;
}

std::string Galois_Field_PB::return_polynomial_as_hex_string() const {
	std::string polynomial_as_hex_string;
	unsigned int current_index_of_bit = 0; 
	while (current_index_of_bit < size_of_field) {
		int current_hex_symbol_in_int = 0;
		unsigned int current_index_in_four_bit_subsequence = 0;
		while (current_index_in_four_bit_subsequence < 4 && (current_index_of_bit + current_index_in_four_bit_subsequence) < size_of_field) { // while we are inside the number
			if (array_of_coefficients_of_polynomial.test(current_index_of_bit + current_index_in_four_bit_subsequence)) // check, if under consideration position of binary string setted to 1
				current_hex_symbol_in_int = (current_hex_symbol_in_int | (1 << current_index_in_four_bit_subsequence)); // increasing value, if bit was equal to 1
			current_index_in_four_bit_subsequence++;
		}
		char current_hex_digit;
		if (current_hex_symbol_in_int >= 0 && current_hex_symbol_in_int <= 9)
			current_hex_digit = (char)current_hex_symbol_in_int + '0'; // 1...9
		else 
			current_hex_digit = (char)current_hex_symbol_in_int + 'a' - 10; // a...f
		polynomial_as_hex_string = current_hex_digit + polynomial_as_hex_string;
		current_index_of_bit += 4; // 4 bit == 1 hex digit
	}
	long int index_of_first_significant_digit = polynomial_as_hex_string.find_first_not_of('0'); // index, before which delete 0
	if (index_of_first_significant_digit != -1) // if not empty string
		polynomial_as_hex_string.erase(polynomial_as_hex_string.begin(), polynomial_as_hex_string.begin() + index_of_first_significant_digit);
	else
		polynomial_as_hex_string = "0"; 
	return polynomial_as_hex_string;
}


Galois_Field_PB Galois_Field_PB::operator+(const Galois_Field_PB& Right_polynomial) {
	Galois_Field_PB result_of_summing = this->array_of_coefficients_of_polynomial ^ Right_polynomial.array_of_coefficients_of_polynomial; // ^ is XOR == + mod 2
	return result_of_summing;
}

Galois_Field_PB Galois_Field_PB::operator*(const Galois_Field_PB& Right_polynomial) {
	Galois_Field_PB result_of_multiplication;
	// will be soon
	return result_of_multiplication;
}