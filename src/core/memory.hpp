#pragma once

#include <src/etc/literals.hpp>

enum class access_type
{
	read,
	write
};

enum class memory_type
{
	read_write,
	read_only
};

template 
<	memory_type _MemoryType = memory_type::read_write,
	types::qword _MemoryLen = 64_k, 
	types::qword _AddressLo	= 0_qw, 
	types::qword _AddressHi	= _AddressLo + _MemoryLen>
class memory_base
{
	using word = types::word;
	using byte = types::byte;

	template <access_type _Direction>
	using data_type = std::conditional_t
		<	_Direction == access_type::read,
			types::byte&,
			types::byte>;

public:
	static inline constexpr auto address_lo = _AddressLo;
	static inline constexpr auto address_hi = _AddressHi;	
	static inline constexpr auto length = _MemoryLen;
	static inline constexpr auto is_read_only = _MemoryType == memory_type::read_only;

	template <access_type _Access>
	auto cycle(word address, data_type<_Access> data)
	{
		static constexpr auto is_read_access = _Access == access_type::read;
		static constexpr auto is_write_access = _Access == access_type::write && !is_read_only;	
		if (address < address_lo)
			return;
		if (address >= address_hi)
			return;
		address = (address - address_lo) % length;
		if constexpr (is_read_access)
			data = bits[address];
		if constexpr (is_write_access)
			bits[address] = data;
	}

	byte bits[length];
};


template 
<	types::qword address_lo,
	types::qword address_hi,
	types::qword memory_len>`  