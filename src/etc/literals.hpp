#pragma once

#include <src/etc/types.hpp>

namespace
{
	constexpr inline auto operator "" _k (types::qword v)
	{
		return 0x400ull * v;
	}

	constexpr inline auto operator "" _m (types::qword v)
	{
		return 0x100000ull * v;
	}

	constexpr inline auto operator "" _g (types::qword v)
	{
		return 0x40000000ull * v;
	}

	constexpr inline auto operator "" _k (long double v)
	{
		return types::qword (0x400ull * v);
	}

	constexpr inline auto operator "" _m (long double v)
	{
		return types::qword (0x100000ull * v);
	}

	constexpr inline auto operator "" _g (long double v)
	{
		return types::qword (0x40000000ull * v);
	}

	constexpr inline auto operator "" _b (types::qword v)
	{
		return types::byte(v);
	}

	constexpr inline auto operator "" _w (types::qword v)
	{
		return types::word(v);
	}

	constexpr inline auto operator "" _dw (types::qword v)
	{
		return types::dword(v);
	}

	constexpr inline auto operator "" _qw (types::qword v)
	{
		return types::qword(v);
	}
}