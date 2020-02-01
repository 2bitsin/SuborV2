#pragma once

#include <src/etc/types.hpp>
#include <src/etc/bits.hpp>

template <typename _Host>
struct core
{
	using byte = xxx::byte;
	using word = xxx::word;

	using dword = xxx::dword;
	using qword = xxx::qword;

	union Word
	{
		word w;
		struct
		{
			byte l;
			byte h;
		};
	};

	union Flags
	{
		byte all;
		struct
		{
			bool c : 1;
			bool z : 1;
			bool i : 1;
			bool d : 1;
			bool b : 1;
			bool e : 1;
			bool v : 1;
			bool n : 1;
		};
	};

	core (_Host& host): host_ (host)
	{}

	auto ticks_elapsed () const
	{
		return clk;
	}

	auto tick (int value)
	{
		clk += value;
		host_.tick (value);
	}

	auto poke (word addr, byte data)
	{
		host_.poke (addr, data);
		tick (1u);
	}

	auto peek (word addr, byte& data)
	{
		host_.peek (addr, data);
		tick (1u);
	}

	auto peek (word addr) -> byte
	{
		byte t;
		peek (addr, t);
		return t;
	}

	void uc_push (byte data)
	{
		poke (0x100 + s--, data);
	}

	auto uc_pull (byte& data)
	{
		peek (0x100 + ++s, data);
		return data;
	}

	auto uc_pull ()
	{
		byte data;
		uc_pull (data);
		return data;
	}

	void exec ()
	{
		static constexpr auto Instr_BRK = 0x000u;
		static constexpr auto Instr_IRQ = 0x100u;
		static constexpr auto Instr_NMI = 0x101u;
		static constexpr auto Instr_RST = 0x102u;

		static constexpr const word Isr_addr [] =
		{
			0xFFFEu,
			0xFFFAu,
			0xFFFCu,
		};

		while (!host_.halt ())
		{
			auto pgcx { false };
			byte next { 0u };
			Word addr { 0u };

			if (false /* DMA ? */);
			else if (Rst (0))
				instr = Instr_RST;
			else if (Nmi (0))
				instr = Instr_NMI;
			else if (Irq () && !p.i)
				instr = Instr_IRQ;
			else
			{
			}


			// Operation

		}
	}

private:
	_Host&	host_;
	byte		a, x, y, s;
	Word		pc;
	Flags   p;
	byte		irq;
	qword		clk { 0ull };

};
