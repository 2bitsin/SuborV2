#pragma once

#include <cstddef>
#include <cstdint>

namespace types
{
	using byte = std::uint8_t;
	using word = std::uint16_t;
	using dword = std::uint32_t;
	using qword = std::uint64_t;
	using zword = std::size_t;

	union Word
	{
		word w;
		struct
		{
			byte l;
			byte h;
		};

		Word (): Word (0) {}
		Word (word w): w { w } {}
		Word (byte lo, byte hi): l{ lo }, h{ hi } {}

		friend bool operator == (const Word& lhs, word rhs)
		{
			return lhs.w == rhs;
		}
		friend bool operator == (word lhs, const Word& rhs)
		{
			return lhs == rhs.w;
		}
	};

}

