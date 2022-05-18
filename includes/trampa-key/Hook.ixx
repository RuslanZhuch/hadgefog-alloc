module;

#include <string_view>
#include <variant>

export module trampa.Hook;

import trampa.Assertion;

export namespace trampa
{

	template <typename T>
	class Hook
	{
	public:
		static bool assertThatEq(auto&& real, auto&& expected, std::string_view comment)
		{

			if (expected != real)
			{
				trampa::Assert::NotEquals notEq;
				notEq.expected = expected;
				notEq.real = real;
				return T::onAssertTriggered(notEq, comment);
			}
			
			return true;

		}

		static bool assertThatNotEq(auto&& real, auto&& expected, std::string_view comment)
		{

			if (expected == real)
			{
				trampa::Assert::Equals notEq;
				notEq.expected = expected;
				notEq.real = real;
				return T::onAssertTriggered(notEq, comment);
			}

			return true;

		}
		
		static bool assertThatGreater(auto&& real, auto&& exclusiveMin, std::string_view comment)
		{

			if (real <= exclusiveMin)
			{
				trampa::Assert::NotGreater notGreater;
				notGreater.exclusiveMin = exclusiveMin;
				notGreater.real = real;
				return T::onAssertTriggered(notGreater, comment);
			}
			
			return true;

		}	
		
		static bool assertThatLess(auto&& real, auto&& exclusiveMax, std::string_view comment)
		{

			if (real >= exclusiveMax)
			{
				trampa::Assert::NotLess cause;
				cause.exclusiveMax = exclusiveMax;
				cause.real = real;
				return T::onAssertTriggered(cause, comment);
			}
			
			return true;

		}

		static bool assertThatGreaterOrEq(auto&& real, auto&& inclusiveMin, std::string_view comment)
		{

			if (real < inclusiveMin)
			{
				trampa::Assert::NotGreaterOrEq cause;
				cause.inclusiveMin = inclusiveMin;
				cause.real = real;
				return T::onAssertTriggered(cause, comment);
			}

			return true;

		}

		static bool assertThatLessOrEq(auto&& real, auto&& inclusiveMax, std::string_view comment)
		{

			if (real > inclusiveMax)
			{
				trampa::Assert::NotLessOrEq cause;
				cause.inclusiveMax = inclusiveMax;
				cause.real = real;
				return T::onAssertTriggered(cause, comment);
			}

			return true;

		}

		static bool assertValidPtr(auto* ptr, std::string_view comment)
		{

			if (ptr == nullptr)
			{
				trampa::Assert::Invalid invalid;
				return T::onAssertTriggered(invalid, comment);
			}

			return true;

		}

	private:

	};

};