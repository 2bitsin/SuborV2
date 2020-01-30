#pragma once

#include "Types.hpp"

namespace
{
	constexpr inline auto operator "" _K (qword v)
	{
		return 0x400ull * v;
	}

	constexpr inline auto operator "" _M (qword v)
	{
		return 0x100000ull * v;
	}

	constexpr inline auto operator "" _G (qword v)
	{
		return 0x40000000ull * v;
	}

	constexpr inline auto operator "" _K (long double v)
	{
		return qword (0x400ull * v);
	}

	constexpr inline auto operator "" _M (long double v)
	{
		return qword (0x100000ull * v);
	}

	constexpr inline auto operator "" _G (long double v)
	{
		return qword (0x40000000ull * v);
	}
}