#pragma once

#include <src/etc/types.hpp>
#include <src/etc/bits.hpp>

template <typename _Host>
struct core
{
	core(_Host& host): host_ (host) {}

	auto a   () const -> xxx::word { return regs_ [0]; }
	auto y   () const -> xxx::word { return regs_ [1]; }
	auto x   () const -> xxx::word { return regs_ [2]; }
	auto s   () const -> xxx::word { return regs_ [3]; }							 
	auto pcl () const -> xxx::word { return regs_ [4]; }
	auto pch () const -> xxx::word { return regs_ [5]; }
	auto p   () const -> xxx::word { return regs_ [6]; }
	auto pc  () const -> xxx::word { return bits::pack<8, 8> (regs_ [4u], regs_ [5u]); }
	auto c   () const -> xxx::byte { return bits::extract_bit<0u> (regs_ [6u]) ; }
	auto z   () const -> xxx::byte { return bits::extract_bit<1u> (regs_ [6u]) ; }
	auto i   () const -> xxx::byte { return bits::extract_bit<2u> (regs_ [6u]) ; }
	auto d   () const -> xxx::byte { return bits::extract_bit<3u> (regs_ [6u]) ; }
	auto b   () const -> xxx::byte { return bits::extract_bit<4u> (regs_ [6u]) ; }
	auto e   () const -> xxx::byte { return bits::extract_bit<5u> (regs_ [6u]) ; }
	auto v   () const -> xxx::byte { return bits::extract_bit<6u> (regs_ [6u]) ; }
	auto n   () const -> xxx::byte { return bits::extract_bit<7u> (regs_ [6u]) ; }
	auto Nmi () const -> xxx::byte { return bits::extract_bit<0u> (regs_ [7u]); }
	auto Irq () const -> xxx::byte { return bits::extract_bit<1u> (regs_ [7u]); }
	auto Rst () const -> xxx::byte { return bits::extract_bit<2u> (regs_ [7u]); }

	auto a   (xxx::byte val) { regs_ [0] = val; }
	auto y   (xxx::byte val) { regs_ [1] = val; }
	auto x   (xxx::byte val) { regs_ [2] = val; }
	auto s   (xxx::byte val) { regs_ [3] = val; }					  
	auto pcl (xxx::byte val) { regs_ [4] = val; }
	auto pch (xxx::byte val) { regs_ [5] = val; }
	auto p   (xxx::byte val) { regs_ [6] = val; }
	auto pc  (xxx::word val) { std::tie(regs_ [4u], regs_ [5u]) = bits::unpack_as_tuple<8u, 8u> (val); }	
	auto c   (xxx::byte val) { bits::splice_bit_inplace<0u> (regs_ [6u], val); }
	auto z   (xxx::byte val) { bits::splice_bit_inplace<1u> (regs_ [6u], val); }
	auto i   (xxx::byte val) { bits::splice_bit_inplace<2u> (regs_ [6u], val); }
	auto d   (xxx::byte val) { bits::splice_bit_inplace<3u> (regs_ [6u], val); }
	auto b   (xxx::byte val) { bits::splice_bit_inplace<4u> (regs_ [6u], val); }
	auto e   (xxx::byte val) { bits::splice_bit_inplace<5u> (regs_ [6u], val); }
	auto v   (xxx::byte val) { bits::splice_bit_inplace<6u> (regs_ [6u], val); }
	auto n   (xxx::byte val) { bits::splice_bit_inplace<7u> (regs_ [6u], val); }	
	auto Nmi (xxx::byte val) { return bits::exchange_bit_inplace<0u> (regs_ [7u], val); }
	auto Irq (xxx::byte val) { return bits::exchange_bit_inplace<1u> (regs_ [7u], val); }
	auto Rst (xxx::byte val) { return bits::exchange_bit_inplace<2u> (regs_ [7u], val); }

	auto clock_ticks_elapsed () const { return clks_; }

	auto tick (int value)
	{
		clks_ += value;
		host_.tick(value);
	}

	auto poke (xxx::word addr, xxx::byte data)
	{
		host_.poke(addr, data);
		tick(1u);
	}
			
	auto peek (xxx::word addr, xxx::byte& data)
	{
		host_.peek(addr, data);
		tick(1u);
	}

	auto peek (xxx::word addr) -> xxx::byte 
	{
		xxx::byte t;
		peek(addr, t);
		return t;
	}

	void exec()
	{
		using namespace xxx;

		auto cross { false };
		byte instr { 0u };

		while(!host_.halt())
		{
			static constexpr auto Instr_BRK = 0x000u;
			static constexpr auto Instr_IRQ = 0x100u;	
			static constexpr auto Instr_NMI = 0x101u;	
			static constexpr auto Instr_RST = 0x102u;	

			if (false /* DMA ? */);
			else if (Rst (0))
				instr = Instr_RST;
			else if (Nmi (0))
				instr = Instr_NMI;
			else if (Irq() && !i ())
				instr = Instr_IRQ;
			else
			{
				peek(pc (), instr);
				pc(pc () + 1u);
			}
		}
	}

private:
	_Host&      host_;
	xxx::byte   regs_ [8u] { 0u };
	xxx::qword  clks_ { 0ull };

};
