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
		Word(word wv)
		:	w (wv)
		{}

		Word(byte lo, byte hi)
		:	l (lo), h (hi)
		{}
			
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

	template <typename dst_type, typename src_type>
	static auto&& xchg (dst_type&& dst, src_type&& src)
	{
		return std::exchange (std::forward<dst_type>(dst), std::forward<src_type>(src));
	}

	auto ticks_elapsed () const
	{
		return clk_;
	}

	auto tick (int value)
	{
		clk_ += value;
		host_.tick (value);
	}

	auto poke (word addr, byte data)
	{
		if (addr != 0x4014u)	
			host_.poke (addr, data);
		else
			dma_ = true, 
			dpg_ = data;		
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
			byte temp { 0u };
			byte offs { 0u };
			
			if (xchg(dma_, false))
			{
				addr.w = 0x100u * dpg_;
				peek (addr.w);
				if (clk_ & 1u)
					peek (addr.w);
				for (offs = 0u; offs < 0x100; ++offs) {
					peek (addr.w + offs, temp);
					poke (0x2004u, temp);					
				}
			}
			else if (xchg(rst_, false))
				instr = Instr_RST;
			else if (xchg(nmi_, false))
				instr = Instr_NMI;
			else if (irq_ && !p.i)
				instr = Instr_IRQ;
			else
				instr = peek (pc.w++);



			// Operation

		}
	}

private:
	_Host&	host_;

	/// Registers

	byte		a, x, y, s;
	Word		pc;
	Flags   p;

	// Various

	bool		dma_ { false	};	// DMA Trigger
	byte		dpg_ { 0u			};  // DMA Page

	qword		clk_ { 0ull		};

	bool		nmi_ { false	};
	bool		rst_ { false	};
	bool		irq_ { false	};
	

};
