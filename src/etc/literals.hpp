#pragma once

#include <src/etc/types.hpp>

namespace
{
	constexpr inline auto operator "" _K (xxx::qword v)
	{
		return 0x400ull * v;
	}

	constexpr inline auto operator "" _M (xxx::qword v)
	{
		return 0x100000ull * v;
	}

	constexpr inline auto operator "" _G (xxx::qword v)
	{
		return 0x40000000ull * v;
	}

	constexpr inline auto operator "" _K (long double v)
	{
		return xxx::qword (0x400ull * v);
	}

	constexpr inline auto operator "" _M (long double v)
	{
		return xxx::qword (0x100000ull * v);
	}

	constexpr inline auto operator "" _G (long double v)
	{
		return xxx::qword (0x40000000ull * v);
	}
}