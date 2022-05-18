module;

#include <variant>

export module trampa.Assertion;
import trampa.Value;


export namespace trampa::Assert
{

	struct NotEquals
	{

		Value expected;
		Value real;

	};

	struct Equals
	{

		Value expected;
		Value real;

	};

	struct NotGreater
	{

		Value exclusiveMin;
		Value real;

	};

	struct NotGreaterOrEq
	{

		Value inclusiveMin;
		Value real;

	};

	struct NotLess
	{

		Value exclusiveMax;
		Value real;

	};

	struct NotLessOrEq
	{

		Value inclusiveMax;
		Value real;

	};

	struct Invalid
	{

	};

	using Cause = std::variant<
		Equals,
		NotEquals, 
		NotGreater, 
		NotGreaterOrEq, 
		NotLess,
		NotLessOrEq,
		Invalid
	>;

};
