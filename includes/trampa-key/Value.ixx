module;

#include <variant>

export module trampa.Value;

export namespace trampa
{

	using Value = std::variant<
		bool, 
		char, 
		unsigned char,
		signed char,
		short,
		unsigned short,
		float,
		int,
		unsigned int,
		long,
		unsigned long,
		double,
		long double,
		long long,
		unsigned long long
		>;

};