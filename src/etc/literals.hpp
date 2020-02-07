#pragma once

#include <src/etc/types.hpp>

namespace
{
	constexpr inline auto operator "" _K (types::qword v)
	{
		return 0x400ull * v;
	}

	constexpr inline auto operator "" _M (types::qword v)
	{
		return 0x100000ull * v;
	}

	constexpr inline auto operator "" _G (types::qword v)
	{
		return 0x40000000ull * v;
	}

	constexpr inline auto operator "" _K (long double v)
	{
		return types::qword (0x400ull * v);
	}

	constexpr inline auto operator "" _M (long double v)
	{
		return types::qword (0x100000ull * v);
	}

	constexpr inline auto operator "" _G (long double v)
	{
		return types::qword (0x40000000ull * v);
	}
}