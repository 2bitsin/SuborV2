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
	auto pc  () const -> xxx::word { return bits::pack<8, 8> (pcl (), pch ()); }
	auto c   () const -> xxx::byte { return bits::extract_bit<0u> (p ()) ; }
	auto z   () const -> xxx::byte { return bits::extract_bit<1u> (p ()) ; }
	auto i   () const -> xxx::byte { return bits::extract_bit<2u> (p ()) ; }
	auto d   () const -> xxx::byte { return bits::extract_bit<3u> (p ()) ; }
	auto b   () const -> xxx::byte { return bits::extract_bit<4u> (p ()) ; }
	auto e   () const -> xxx::byte { return bits::extract_bit<5u> (p ()) ; }
	auto v   () const -> xxx::byte { return bits::extract_bit<6u> (p ()) ; }
	auto n   () const -> xxx::byte { return bits::extract_bit<7u> (p ()) ; }
	auto nmi () const -> xxx::byte { return bits::extract_bit<0u> (regs_ [7u]); }
	auto irq () const -> xxx::byte { return bits::extract_bit<1u> (regs_ [7u]); }
	auto rst () const -> xxx::byte { return bits::extract_bit<2u> (regs_ [7u]); }

	auto a   (xxx::byte val) { regs_ [0] = val; }
	auto y   (xxx::byte val) { regs_ [1] = val; }
	auto x   (xxx::byte val) { regs_ [2] = val; }
	auto s   (xxx::byte val) { regs_ [3] = val; }					  
	auto pcl (xxx::byte val) { regs_ [4] = val; }
	auto pch (xxx::byte val) { regs_ [5] = val; }
	auto p   (xxx::byte val) { regs_ [6] = val; }
	auto pc  (xxx::word val) { std::tie(regs_ [4u], regs_ [5u]) = bits::unpack_as_tuple<8u, 8u> (val); }	
	auto c   (xxx::byte val) { p (bits::splice_bit<0u> (p (), val)); }
	auto z   (xxx::byte val) { p (bits::splice_bit<1u> (p (), val)); }
	auto i   (xxx::byte val) { p (bits::splice_bit<2u> (p (), val)); }
	auto d   (xxx::byte val) { p (bits::splice_bit<3u> (p (), val)); }
	auto b   (xxx::byte val) { p (bits::splice_bit<4u> (p (), val)); }
	auto e   (xxx::byte val) { p (bits::splice_bit<5u> (p (), val)); }
	auto v   (xxx::byte val) { p (bits::splice_bit<6u> (p (), val)); }
	auto n   (xxx::byte val) { p (bits::splice_bit<7u> (p (), val)); }	
	auto nmi (xxx::byte val) { bits::splice_bit_inplace<0u> (regs_ [7u], val); }
	auto irq (xxx::byte val) { bits::splice_bit_inplace<1u> (regs_ [7u], val); }
	auto rst (xxx::byte val) { bits::splice_bit_inplace<2u> (regs_ [7u], val); }

	auto clock_ticks_elapsed () const { return clks_; }

	auto peek (xxx::word addr) -> xxx::byte 
	{
		xxx::byte t;
		peek(addr, t);
		return t;
	}

	auto tick (int value)
	{
		clks_ += value;
		host_.tick(value);
	}

	auto peek (xxx::word addr, xxx::byte& data)
	{
		host_.peek(addr, data);
		tick(1u);
	}

	auto poke (xxx::word addr, xxx::byte data)
	{
		host_.poke(addr, data);
		tick(1u);
	}
			
	void exec()
	{
		using namespace xxx;

		auto cross { false };
		byte instr { 0u };

		while(!host_.halt())
		{
			peek(pc(), instr);

			/*
				if (dma)  { .... }
				else
			*/
			

		}
	}

private:
	_Host&      host_;
	xxx::byte   regs_ [8u] { 0u };
	xxx::qword  clks_ { 0ull };

};
