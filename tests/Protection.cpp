#include "pch.h"

#include <string_view>
#include <optional>
#include <variant>

#include "Protection.h"

import trampa.key;
import hfog.Protect;

[[nodiscard]] bool HfogHook::onAssertTriggered(const trampa::Assert::Cause& assertCause, [[maybe_unused]] std::string_view msg)
{

	std::visit([&](auto&& vArg)
	{
		using T = std::decay_t<decltype(vArg)>;
		hfogprotect::Error error;
		error.msg = msg;
		if constexpr (std::is_same_v<T, trampa::Assert::Invalid>)
		{
			error.code = hfogprotect::ErrorCodes::INVALID_PTR;
		}
		else
		{
			error.code = hfogprotect::ErrorCodes::NOT_IN_RANGE;
		}

		hfogprotect::instance()->setErrorCode(error);

	}, assertCause);

	return false;

}
