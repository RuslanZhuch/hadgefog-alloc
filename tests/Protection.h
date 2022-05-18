#pragma once
#include <string_view>
#include <optional>
//#include <variant>

import trampa.key;

class hfogprotect
{

public:

	enum class ErrorCodes
	{
		INVALID_PTR,
		NOT_IN_RANGE
	};

	struct Error
	{
		std::string_view msg;
		ErrorCodes code;
	};

public:
	[[nodiscard]] auto getLastError()
	{
		const auto error{ errorCause };
		errorCause.reset();
		return error;
		//return nullptr;
	}

	void setErrorCode(const Error& cause)
	{
		this->errorCause = cause;
	}

	static auto* instance()
	{
		static hfogprotect p;
		return &p;
	}

private:
	std::optional<Error> errorCause;

};

//class HfogHook
//{
//public:
//
//
//	[[nodiscard]] static auto getLastError()
//	{
//		const auto error{ errorCause };
//		errorCause.reset();
//		return error;
//	}
//
//private:
//	static std::optional<trampa::Assert::Cause> errorCause;
//
//};