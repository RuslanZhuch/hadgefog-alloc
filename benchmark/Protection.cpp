#include <string_view>
#include <cassert>

import trampa.key;
import hfog.Protect;

[[nodiscard]] bool HfogHook::onAssertTriggered(const trampa::Assert::Cause& assertCause, [[maybe_unused]] std::string_view msg)
{

	assert(msg.data());
	return false;

}
