#include "Header.h"

double MeasureTime(std::function<void()> operation, unsigned int amount_of_measurements) {
	double* time_for_all_tryes = new double[amount_of_measurements + 1]{};
	int current_measurement = 0;
	while (current_measurement < amount_of_measurements) {
		auto start_time = std::chrono::high_resolution_clock::now();
		operation();  // Executing operation
		auto end_time = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed_time = end_time - start_time;
		time_for_all_tryes[current_measurement] = elapsed_time.count();
		std::cout << time_for_all_tryes[current_measurement] << '\n';
		time_for_all_tryes[amount_of_measurements] += time_for_all_tryes[current_measurement];
		current_measurement++;
		std::cout << "Total:\t" << time_for_all_tryes[amount_of_measurements] << '\n';
	}
	double average_time = (time_for_all_tryes[amount_of_measurements] / amount_of_measurements);
	delete[] time_for_all_tryes;
	return average_time;
}

std::string convertToLowercase(const std::string& entered_string) {
	std::string result = "";
	for (char current_symbol : entered_string)
		result += tolower(current_symbol);
	return result;
}

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
	out << "Polynomial is:\t"; // << Data.array_of_coefficients_of_polynomial;
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
				current_hex_symbol_in_int = (current_hex_symbol_in_int | (1 << current_index_in_four_bit_subsequence)); // increasing current value, if bit was equal to 1
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

Galois_Field_PB Galois_Field_PB::modulo_by_irreducible_polynomial(const std::bitset<2 * size_of_field - 1>& polymonial_coefficients_after_some_action) {
	std::bitset<2 * size_of_field - 1 > modulo_polynomial; // modulo-element that creates expansion of field, x^173 + x^10 + x^2 + x + 1
	modulo_polynomial[173] = modulo_polynomial[10] = modulo_polynomial[2] = modulo_polynomial[1] = modulo_polynomial[0] = 1; // setting coefficients of modulo polynomial
	std::bitset<2 * size_of_field - 1 > entrance_polynomial_temp = polymonial_coefficients_after_some_action;

	int current_index = 2 * size_of_field - 2; // through coefficients of modulo polynomial that "bigger" than size_of_field
	while (current_index >= size_of_field) {
		if (entrance_polynomial_temp.test(current_index) == 1) // if coefficient of input polynomial is 1
			entrance_polynomial_temp = (entrance_polynomial_temp ^ (modulo_polynomial << (current_index - size_of_field))); // ^ is XOR == + mod 2
		current_index--; // XORing input and modulo polynomial, after right shifting coefficients of modulo, to "standart" bit size of polynomial 
	} //(is like dividing polynomials using corner method, and considering that coefficients is 0 or 1 (element of F_2)
	std::bitset<size_of_field> polynomial_taken_by_modulo_to_standard_size;
	current_index = 0;
	while (current_index < size_of_field) {
		polynomial_taken_by_modulo_to_standard_size[current_index] = entrance_polynomial_temp[current_index]; // resizing result polynonmial (compact)
		current_index++;
	}
	return Galois_Field_PB(polynomial_taken_by_modulo_to_standard_size);
}

Galois_Field_PB Galois_Field_PB::operator*(const Galois_Field_PB& Right_polynomial) {
	std::bitset<2 * size_of_field - 1 > result_of_multiplication{}; // in process result could be grater than modulo, but than will resized
	unsigned int current_index_in_right_polynomial = 0;
	while (current_index_in_right_polynomial < size_of_field) {
		if (Right_polynomial.array_of_coefficients_of_polynomial.test(current_index_in_right_polynomial) == 1) { // coefficient is 1
			unsigned int current_index_in_left_polynomial = 0;
			while (current_index_in_left_polynomial < size_of_field){
				if (this->array_of_coefficients_of_polynomial.test(current_index_in_left_polynomial) == 1) // coefficient is 1
					result_of_multiplication.flip(current_index_in_left_polynomial + current_index_in_right_polynomial);
				current_index_in_left_polynomial++; // using inline in bitset function flip, that inverse (1->0, 0->1) bit in given position
			}
		}
		current_index_in_right_polynomial++;
	}
	Galois_Field_PB multiplication_result_by_modulo(modulo_by_irreducible_polynomial(result_of_multiplication));
	return multiplication_result_by_modulo;
}

Galois_Field_PB Galois_Field_PB::square_polynomial() {
	return *this * *this;
}

Galois_Field_PB Galois_Field_PB::polynomimal_to_power(const Galois_Field_PB& degree) { // degree is number
	Galois_Field_PB polynomial_to_power = *this;
	Galois_Field_PB result_of_getting_to_power("1", 0); // neutral element by multiplication is 1
	unsigned int current_index = 0;
	while (current_index < size_of_field) {
		if (degree.array_of_coefficients_of_polynomial.test(current_index) == 1)
			result_of_getting_to_power = polynomial_to_power * result_of_getting_to_power;
		polynomial_to_power = polynomial_to_power.square_polynomial();
		current_index++;
	}
	return result_of_getting_to_power;
}

unsigned int Galois_Field_PB::trace() {
	Galois_Field_PB polynomial_to_which_needed_find_trace = *this;
	Galois_Field_PB trace("0", 0); // neutral element by addition is 0
	unsigned int current_index = 0;
	while (current_index < size_of_field) {
		trace = (trace + polynomial_to_which_needed_find_trace);
		polynomial_to_which_needed_find_trace = polynomial_to_which_needed_find_trace.square_polynomial(); // trace is sum of all conjugates polynomial to this polynomial, over our field F_2 trace can equal 0 or 1
		current_index++;
	}
	return trace.array_of_coefficients_of_polynomial[0]; // returning the least significant bit
}

Galois_Field_PB Galois_Field_PB::inverse_element() {
	if (*this == Galois_Field_PB("0", 0))
		throw std::exception("Inverse element to null in field didn't exist!"); // In field all elements, exept null, have inverse regard operation of multiplication
	Galois_Field_PB inverse_element = *this, auxiliary_variable = *this;
	unsigned int current_index = 0;
	while (current_index < size_of_field - 2) { //using Gorner algorithm
		auxiliary_variable = auxiliary_variable.square_polynomial();
		inverse_element = (inverse_element * auxiliary_variable);
		current_index++;
	}
	inverse_element = inverse_element.square_polynomial();
	return inverse_element;
}