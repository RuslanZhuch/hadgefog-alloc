#pragma once
#include "pch.h"

#include <string_view>
#include <optional>
//#include <variant>

import trampa.key;

namespace hfogprotect
{
	enum class ErrorCodes : uint64_t
	{
		INVALID_PTR,
		NOT_IN_RANGE
	};

	struct Error
	{
		ErrorCodes code;
		std::string_view msg;
	};

	[[nodiscard]] std::optional<hfogprotect::Error> getLastError();

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