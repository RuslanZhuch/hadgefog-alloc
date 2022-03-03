#pragma once

#include <cstddef>

static bool getValuesAre(auto* buffer, auto beginId, auto endId, auto val)
{
	auto currId{ beginId };
	while (currId != endId)
	{
		if (buffer[currId++] != static_cast<std::remove_pointer_t<decltype(buffer)>>(val))
			return false;
	}
	return true;
}