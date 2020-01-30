#pragma once

#include <src/etc/types.hpp>

template <typename _Host>
struct core
{
	core(_Host& host): host_ (host) {}

	auto A   () const -> xxx::word { return regs_ [0]; }
	auto Y   () const -> xxx::word { return regs_ [1]; }
	auto X   () const -> xxx::word { return regs_ [2]; }
	auto S   () const -> xxx::word { return regs_ [3]; }							 
	auto PCL () const -> xxx::word { return regs_ [4]; }
	auto PCH () const -> xxx::word { return regs_ [5]; }
	auto P   () const -> xxx::word { return regs_ [6]; }
	auto PC  () const -> xxx::word { return xxx::word (regs_ [5] * 0x100u + regs_[4]); }
	auto C   () const -> xxx::byte { return (P () >> 0u) & 1u ; }
	auto Z   () const -> xxx::byte { return (P () >> 1u) & 1u ; }
	auto I   () const -> xxx::byte { return (P () >> 2u) & 1u ; }
	auto D   () const -> xxx::byte { return (P () >> 3u) & 1u ; }
	auto B   () const -> xxx::byte { return (P () >> 4u) & 1u ; }
	auto E   () const -> xxx::byte { return (P () >> 7u) & 1u ; }
	auto V   () const -> xxx::byte { return (P () >> 8u) & 1u ; }
	auto N   () const -> xxx::byte { return (P () >> 7u) & 1u ; }
	auto Nmi () const -> xxx::byte { return (regs_[7u] >> 0u) & 1u; }
	auto Irq () const -> xxx::byte { return (regs_[7u] >> 1u) & 1u; }
	auto Rst () const -> xxx::byte { return (regs_[7u] >> 2u) & 1u; }

	auto A   (xxx::byte val) { regs_ [0] = val; }
	auto Y   (xxx::byte val) { regs_ [1] = val; }
	auto X   (xxx::byte val) { regs_ [2] = val; }
	auto S   (xxx::byte val) { regs_ [3] = val; }					  
	auto PCL (xxx::byte val) { regs_ [4] = val; }
	auto PCH (xxx::byte val) { regs_ [5] = val; }
	auto P   (xxx::byte val) { regs_ [6] = val; }
	auto PC  (xxx::word val) { regs_ [4] = (val >> 0u) & 0xffu; regs_ [5] = (val >> 8u) & 0xffu; }	
	auto C   (xxx::byte val) { P ((P() & ~0x01u) | ((!!val) << 0u)); }
	auto Z   (xxx::byte val) { P ((P() & ~0x02u) | ((!!val) << 1u)); }
	auto I   (xxx::byte val) { P ((P() & ~0x04u) | ((!!val) << 2u)); }
	auto D   (xxx::byte val) { P ((P() & ~0x08u) | ((!!val) << 3u)); }
	auto B   (xxx::byte val) { P ((P() & ~0x10u) | ((!!val) << 4u)); }
	auto E   (xxx::byte val) { P ((P() & ~0x20u) | ((!!val) << 5u)); }
	auto V   (xxx::byte val) { P ((P() & ~0x40u) | ((!!val) << 6u)); }
	auto N   (xxx::byte val) { P ((P() & ~0x80u) | ((!!val) << 7u)); }	
	auto Nmi (xxx::byte val) { if (val) regs_[7u] |= 0x01u; else regs_[7u] &= ~0x01; }
	auto Irq (xxx::byte val) { if (val) regs_[7u] |= 0x02u; else regs_[7u] &= ~0x02; }
	auto Rst (xxx::byte val) { if (val) regs_[7u] |= 0x04u; else regs_[7u] &= ~0x04; }

private:
	_Host&		host_;
	xxx::byte regs_ [8u] { 0u };
};
