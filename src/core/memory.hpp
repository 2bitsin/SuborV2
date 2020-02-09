#pragma once

#include <src/etc/literals.hpp>

enum direction
{
	read,
	write
};

template 
<	auto _MemoryLen = 0_w, 
	auto _AddressLo	= 0_w, 
	auto _AddressHi	= _AddressLo + _MemoryLen>
class memory
{
	using word = types::word;
	using byte = types::byte;

	template <direction _Direction>
	using data_type = std::conditional_t
		<	_Direction == read,
			types::byte&,
			types::byte>;

public:
	static inline constexpr auto addrlo = _AddressLo;
	static inline constexpr auto addrhi = _AddressHi;	
	static inline constexpr auto length = _MemoryLen;

	template <direction _Direction>
	auto cycle(word address, data_type<_Direction> data)
	{
		if (address < addrlo)
			return;
		if (address >= addrhi)
			return;
		address = (address - addrlo) % length;
		if constexpr (_Direction == read)
			data = bits[address];
		if constexpr (_Direction == write)
			bits[address] = data;
	}

	byte bits[length];
};
