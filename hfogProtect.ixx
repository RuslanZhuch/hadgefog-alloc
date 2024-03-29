module;

#include <string_view>
#include <cassert>
#include <variant>
#include <optional>


export module hfog.Protect;


import trampa.key;


export class HfogHook
{

public:
	static bool onAssertTriggered([[maybe_unused]] const trampa::Assert::Cause& assertCause, std::string_view msg);
	

//	{
//		return T();
////		assert(msg.data());
////		return true;
//	}

};

export namespace hfog
{
	bool assertThatValidPtr(auto* ptr, std::string_view message)
	{
		return trampa::Hook<HfogHook>::assertValidPtr(ptr, message);
	}

	bool assertThatGreater(auto&& real, auto&& exclusiveMin, std::string_view message)
	{
		return trampa::Hook<HfogHook>::assertThatGreater(
			std::forward<decltype(real)>(real), std::forward<decltype(exclusiveMin)>(exclusiveMin), message);
	}	
	
	bool assertThatGreaterOrEq(auto&& real, auto&& exclusiveMin, std::string_view message)
	{
		return trampa::Hook<HfogHook>::assertThatGreaterOrEq(
			std::forward<decltype(real)>(real), std::forward<decltype(exclusiveMin)>(exclusiveMin), message);
	}

	bool assertThatLess(auto&& real, auto&& exclusiveMax, std::string_view message)
	{
		return trampa::Hook<HfogHook>::assertThatLess(
			std::forward<decltype(real)>(real), std::forward<decltype(exclusiveMax)>(exclusiveMax), message);
	}

	bool assertThatLessOrEq(auto&& real, auto&& exclusiveMax, std::string_view message)
	{
		return trampa::Hook<HfogHook>::assertThatLessOrEq(
			std::forward<decltype(real)>(real), std::forward<decltype(exclusiveMax)>(exclusiveMax), message);
	}

	bool assertThatNotEq(auto&& real, auto&& exclusiveMax, std::string_view message)
	{
		return trampa::Hook<HfogHook>::assertThatEq(
			std::forward<decltype(real)>(real), std::forward<decltype(exclusiveMax)>(exclusiveMax), message);
	}
}