#pragma once

#include <etc/types.hpp>

template <typename _Host>
struct core
{
	core(_Host& host): host_ (host) {}

	auto _A		() const -> xxx::word { return regs_ [0]; }
	auto _Y		() const -> xxx::word { return regs_ [1]; }
	auto _X		() const -> xxx::word { return regs_ [2]; }
	auto _S		() const -> xxx::word { return regs_ [3]; }							 
	auto _PCL	() const -> xxx::word { return regs_ [4]; }
	auto _PCH	() const -> xxx::word { return regs_ [5]; }
	auto _P		() const -> xxx::word { return regs_ [6]; }
	auto _PC	() const -> xxx::word { return xxx::word (regs_ [5] * 0x100u + regs_[4]); }

	auto _A   (xxx::byte val)	{ regs_ [0] = val; }
	auto _Y   (xxx::byte val)	{ regs_ [1] = val; }
	auto _X   (xxx::byte val)	{ regs_ [2] = val; }
	auto _S   (xxx::byte val)	{ regs_ [3] = val; }					  
	auto _PCL (xxx::byte val)	{ regs_ [4] = val; }
	auto _PCH (xxx::byte val)	{ regs_ [5] = val; }
	auto _P   (xxx::byte val)	{ regs_ [6] = val; }
	auto _PC	(xxx::word val)	{ regs_ [4] = (val >> 0u) & 0xffu;
															regs_ [5] = (val >> 8u) & 0xffu; }
	
	auto _C		(xxx::byte val) { _P ((_P() & ~0x01u) | ((!!val) << 0u)); }
	auto _Z		(xxx::byte val) { _P ((_P() & ~0x02u) | ((!!val) << 1u)); }
	auto _I		(xxx::byte val) { _P ((_P() & ~0x04u) | ((!!val) << 2u)); }
	auto _D		(xxx::byte val) { _P ((_P() & ~0x08u) | ((!!val) << 3u)); }
	auto _B		(xxx::byte val) { _P ((_P() & ~0x10u) | ((!!val) << 4u)); }
	auto _E		(xxx::byte val) { _P ((_P() & ~0x20u) | ((!!val) << 5u)); }
	auto _V		(xxx::byte val) { _P ((_P() & ~0x40u) | ((!!val) << 6u)); }
	auto _N		(xxx::byte val) { _P ((_P() & ~0x80u) | ((!!val) << 7u)); }

	auto _C		() const -> xxx::byte { return (_P () >> 0u) & 1u ; }
	auto _Z		() const -> xxx::byte { return (_P () >> 1u) & 1u ; }
	auto _I		() const -> xxx::byte { return (_P () >> 2u) & 1u ; }
	auto _D		() const -> xxx::byte { return (_P () >> 3u) & 1u ; }
	auto _B		() const -> xxx::byte { return (_P () >> 4u) & 1u ; }
	auto _E		() const -> xxx::byte { return (_P () >> 7u) & 1u ; }
	auto _V		() const -> xxx::byte { return (_P () >> 8u) & 1u ; }
	auto _N		() const -> xxx::byte { return (_P () >> 7u) & 1u ; }
	
	auto _Nmi (xxx::byte val) { if (val) regs_[7u] |= 0x01u; else regs_[7u] &= ~0x01; }
	auto _Irq (xxx::byte val) { if (val) regs_[7u] |= 0x02u; else regs_[7u] &= ~0x02; }
	auto _Rst (xxx::byte val) { if (val) regs_[7u] |= 0x04u; else regs_[7u] &= ~0x04; }

	auto _Nmi () const -> xxx::byte { return (regs_[7u] >> 0u) & 1u; }
	auto _Irq () const -> xxx::byte { return (regs_[7u] >> 1u) & 1u; }
	auto _Rst () const -> xxx::byte { return (regs_[7u] >> 2u) & 1u; }

private:
	_Host&		host_;
	xxx::byte regs_ [8u] { 0u };
};
