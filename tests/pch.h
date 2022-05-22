//
// pch.h
//

#pragma once

#pragma warning(disable : 5050)
#pragma warning(disable : 4710)
#pragma warning(disable : 4711)
#pragma warning(disable : 5045)

#pragma warning( push )

#pragma warning(disable : 4668)
#pragma warning(disable : 4820)
#pragma warning(disable : 4625)
#pragma warning(disable : 4514)
#pragma warning(disable : 4626)
#pragma warning(disable : 5026)
#pragma warning(disable : 5027)

#include "gtest/gtest.h"

#pragma warning( pop )

#include "TestUtils.h"

static constexpr auto SET_BYTES{ static_cast<char>(0xFA) };
static constexpr auto CLEAR_BYTES{ static_cast<char>(0xAF) };