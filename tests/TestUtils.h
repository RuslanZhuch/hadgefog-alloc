#pragma once

#include <cstddef>

static bool getValuesAre(auto* buffer, size_t beginId, size_t endId, auto val)
{
	auto currId{ beginId };
	while (currId != endId)
	{
		if (buffer[currId++] != static_cast<std::remove_pointer_t<decltype(buffer)>>(val))
			return false;
	}
	return true;
}