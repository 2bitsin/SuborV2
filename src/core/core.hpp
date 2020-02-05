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
		Word (): Word (0)
		{}

		Word (word wv)
		: w (wv)
		{}

		Word (byte lo, byte hi)
		: l (lo), h (hi)
		{}

		word w;
		struct
		{
			byte l;
			byte h;
		};

		friend bool operator == (const Word& lhs, xxx::word rhs) { return lhs.w == rhs; }
		friend bool operator == (xxx::word lhs, const Word& rhs) { return lhs == rhs.w; }
	};

	union Flag
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

		friend bool operator == (const Flag& lhs, byte rhs) { return lhs.all == rhs; }
		friend bool operator == (byte lhs, const Flag& rhs) { return lhs == rhs.all; }
	};

	core (_Host& host): host_ (host)
	{}

	template <typename dst_type, typename src_type>
	static auto xchg (dst_type&& dst, src_type&& src)
	{
		return std::exchange (std::forward<dst_type> (dst), std::forward<src_type> (src));
	}

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

	auto uc_upnz (byte value) 
	{ 
		p.n = !!(value &  0x80u); 
		p.z = !!(value == 0x00u); 
	}
	
	void exec ()
	{
		using namespace bits;

		static constexpr auto Instr_BRK = 0x000u;
		static constexpr auto Instr_IRQ = 0x100u;
		static constexpr auto Instr_NMI = 0x101u;
		static constexpr auto Instr_RST = 0x102u;

		static constexpr const word Isrv [] =
		{
			0xFFFEu,
			0xFFFAu,
			0xFFFCu,
		};

		while (!host_.halt ())
		{
			bool cxpg { false };
			word next { 0u };
			Word addr { 0u };
			byte temp { 0u };
			byte offs { 0u };

			Word tmp0, tmp1 ;

			if (xchg (dma_, false))
			{
				addr.w = 0x100u * dpg_;
				peek (addr.w);
				if (ticks_elapsed() & 1u)
					peek (addr.w);
				for (offs = 0u; offs < 0x100; ++offs)
				{
					peek (addr.w + offs, temp);
					poke (0x2004u, temp);
				}
			}
			else if (xchg (rst, false))
				next = Instr_RST;
			else if (xchg (nmi, false))
				next = Instr_NMI;
			else if (irq && !p.i)
				next = Instr_IRQ;
			else
				next = peek (pc.w++);

			switch (next)
			{
			case Instr_BRK:	// BRK
			case Instr_IRQ:	// IRQ			
			case Instr_NMI:	// NMI			
			case Instr_RST:	// RST			
				addr.w = Isrv [next & 0xFF];
				p.b = (next == Instr_BRK);
				p.i = (next == Instr_IRQ);
				peek (pc.w); // Dummy reads
				peek (pc.w); // Dummy reads
				uc_push (pc.h);
				uc_push (pc.l);
				uc_push (p.all);
				pc.l = peek (addr.w + 0u);
				pc.h = peek (addr.w + 1u);
				break;

			// Implied
			case 0x40:
			case 0x60:
			case 0x08:
			case 0x18:
			case 0x28:
			case 0x38:
			case 0x48:
			case 0x58:
			case 0x68:
			case 0x78:
			case 0x88:
			case 0x98:
			case 0xA8:
			case 0xB8:
			case 0xC8:
			case 0xD8:
			case 0xE8:
			case 0xF8:
			case 0x0A:
			case 0x1A:
			case 0x2A:
			case 0x3A:
			case 0x4A:
			case 0x5A:
			case 0x6A:
			case 0x7A:
			case 0x8A:
			case 0x9A:
			case 0xAA:
			case 0xBA:
			case 0xCA:
			case 0xDA:
			case 0xEA:
			case 0xFA:
				peek (pc.w); // Dummy read
				break;

				// Immediate
			case 0x02:
			case 0x12:
			case 0x22:
			case 0x32:
			case 0x42:
			case 0x52:
			case 0x62:
			case 0x72:
			case 0x82:
			case 0x92:
			case 0xA2:
			case 0xB2:
			case 0xC2:
			case 0xD2:
			case 0xE2:
			case 0xF2:
			case 0x0B:
			case 0x2B:
			case 0x4B:
			case 0x6B:
			case 0x8B:
			case 0xAB:
			case 0xCB:
			case 0xEB:
			case 0x09:
			case 0x29:
			case 0x49:
			case 0x69:
			case 0x80:
			case 0x89:
			case 0xC0:
			case 0xC9:
			case 0xA0:
			case 0xA9:
			case 0xE0:
			case 0xE9:
				addr.w = pc.w++;
				break;

				// Zero Page
			case 0x04:
			case 0x05:
			case 0x06:
			case 0x07:
			case 0x24:
			case 0x25:
			case 0x26:
			case 0x27:
			case 0x44:
			case 0x45:
			case 0x46:
			case 0x47:
			case 0x64:
			case 0x65:
			case 0x66:
			case 0x67:
			case 0x84:
			case 0x85:
			case 0x86:
			case 0x87:
			case 0xA4:
			case 0xA5:
			case 0xA6:
			case 0xA7:
			case 0xC4:
			case 0xC5:
			case 0xC6:
			case 0xC7:
			case 0xE4:
			case 0xE5:
			case 0xE6:
			case 0xE7:
				addr.w = peek (pc.w++);
				break;

				// Zero Page, X
			case 0x14:
			case 0x15:
			case 0x16:
			case 0x17:
			case 0x34:
			case 0x35:
			case 0x36:
			case 0x37:
			case 0x54:
			case 0x55:
			case 0x56:
			case 0x57:
			case 0x74:
			case 0x75:
			case 0x76:
			case 0x77:
			case 0x94:
			case 0x95:
			case 0xB4:
			case 0xB5:
			case 0xD4:
			case 0xD5:
			case 0xD6:
			case 0xD7:
			case 0xF4:
			case 0xF5:
			case 0xF6:
			case 0xF7:
				addr.w = peek (pc.w++);
				addr.l += x;
				peek (addr.w); // Dummy read
				break;

				// Zero Page, Y
			case 0x96:
			case 0x97:
			case 0xB6:
			case 0xB7:
				addr.w = peek (pc.w++);
				addr.l += y;
				peek (addr.w); // Dummy read
				break;

				// Absolute
			case 0x0C:
			case 0x0D:
			case 0x0E:
			case 0x0F:
			case 0x20:
			case 0x2C:
			case 0x2D:
			case 0x2E:
			case 0x2F:
			case 0x4C:
			case 0x4D:
			case 0x4E:
			case 0x4F:
			case 0x6D:
			case 0x6E:
			case 0x6F:
			case 0x8C:
			case 0x8D:
			case 0x8E:
			case 0x8F:
			case 0xAC:
			case 0xAD:
			case 0xAE:
			case 0xAF:
			case 0xCC:
			case 0xCD:
			case 0xCE:
			case 0xCF:
			case 0xEC:
			case 0xED:
			case 0xEE:
			case 0xEF:
				addr.l = peek (pc.w++);
				addr.h = peek (pc.w++);
				break;

				// Absolute, X
			case 0x1C:
			case 0x1D:
			case 0x1E:
			case 0x1F:
			case 0x3C:
			case 0x3D:
			case 0x3E:
			case 0x3F:
			case 0x5C:
			case 0x5D:
			case 0x5E:
			case 0x5F:
			case 0x7C:
			case 0x7D:
			case 0x7E:
			case 0x7F:
			case 0x9C: // SYA
			case 0x9D:
			case 0xBC:
			case 0xBD:
			case 0xDC:
			case 0xDD:
			case 0xDE:
			case 0xDF:
			case 0xFC:
			case 0xFD:
			case 0xFE:
			case 0xFF:
				addr.l = peek (pc.w++);
				addr.h = peek (pc.w++);
				temp = addr.h;
				addr.w += x;
				cxpg = addr.h != temp;
				break;

				// Absolute, Y
			case 0x19:
			case 0x1B:
			case 0x39:
			case 0x3B:
			case 0x59:
			case 0x5B:
			case 0x79:
			case 0x7B:
			case 0x99:
			case 0x9B:
			case 0x9E:	// SXA
			case 0x9F:	// AXA
			case 0xB9:
			case 0xBB:
			case 0xBE:
			case 0xBF:
			case 0xD9:
			case 0xDB:
			case 0xF9:
			case 0xFB:
				addr.l = peek (pc.w++);
				addr.h = peek (pc.w++);
				temp = addr.h;
				addr.w += y;
				cxpg = addr.h != temp;
				break;

				// (Indirect)
			case 0x6C:
				addr.l = peek (pc.w++);
				addr.h = peek (pc.w++);
				temp = peek (addr.w);
				addr.l += 1u;
				addr.h = peek (addr.w);
				addr.l = temp;
				break;

				// (Indirect, X)
			case 0x01:
			case 0x03:
			case 0x21:
			case 0x23:
			case 0x41:
			case 0x43:
			case 0x61:
			case 0x63:
			case 0x81:
			case 0x83:
			case 0xA1:
			case 0xA3:
			case 0xC1:
			case 0xC3:
			case 0xE1:
			case 0xE3:
				temp = peek (pc.w++) + x;
				peek (temp);							// Dummy read
				addr.l = peek (temp);
				addr.h = peek (++temp);
				break;

				// (Indirect), Y
			case 0x11:
			case 0x13:
			case 0x31:
			case 0x33:
			case 0x51:
			case 0x53:
			case 0x71:
			case 0x73:
			case 0x91:
			case 0x93: // AXA
			case 0xB1:
			case 0xB3:
			case 0xD1:
			case 0xD3:
			case 0xF1:
			case 0xF3:
				temp = peek (pc.w++);
				addr.l = peek (temp + 0u);				
				addr.h = peek ((temp + 1u) & 0xff);
				temp = addr.h;
				addr.w += y;
				cxpg = addr.h != temp;
				break;

				// Relative
			case 0x10:
			case 0x30:
			case 0x50:
			case 0x70:
			case 0x90:
			case 0xB0:
			case 0xD0:
			case 0xF0:
				addr.w = peek (pc.w++);
				addr.w += pc.w - ((addr.w & 0x80) << 1u);
				break;

			default:
				assert (false);
				break;
			}

			// Operation
			switch (next)
			{
			case Instr_BRK:
			case Instr_IRQ:
			case Instr_NMI:
			case Instr_RST:
				break;

				// JMP
			case 0x4C:
			case 0x6C:
				pc = addr;
				break;

				// JSR
			case 0x20: // Todo: double-check the order of stores/loads
				--pc.w;
				poke (0x100 + s, addr.l); // Dummy write
				uc_push (pc.h);
				uc_push (pc.l);
				pc.w = addr.w;
				break;

				// RTS
			case 0x60:
				peek (0x100 + s);	// Dummy read
				pc.l = uc_pull ();
				pc.h = uc_pull ();
				peek (pc.w++);
				break;

				// RTI
			case 0x40:
				p.all = uc_pull ();
				p.b = false;
				p.e = true;
				pc.l = uc_pull ();
				pc.h = uc_pull ();
				peek (pc.w);			// Dummy read
				break;

				// LDA		
			case 0xA1:
			case 0xA5:
			case 0xA9:
			case 0xAD:
			case 0xB1:
			case 0xB5:
			case 0xB9:
			case 0xBD:
				if (cxpg) peek (addr.w); //Dummy read
				temp = peek (addr.w);
				uc_upnz(temp);
				a = temp;
				break;

				// LDX
			case 0xB6:
			case 0xA2:
			case 0xA6:
			case 0xAE:
			case 0xBE:
				if (cxpg) peek (addr.w); //Dummy read
				temp = peek (addr.w);
				uc_upnz(temp);
				x = temp;
				break;

				// LAX
			case 0xA7: // LAX ab            ;*=add 1     3
			case 0xB7: // LAX ab,Y          ;if page     4
			case 0xAF: // LAX abcd          ;No. Cycles= 4
			case 0xBF: // LAX abcd,Y        ;            4*
			case 0xA3: // LAX (ab,X)        ;boundary    6
			case 0xB3: // LAX (ab),Y        ;is crossed  5*
				if (cxpg) peek (addr.w); //Dummy read
				temp = peek (addr.w);
				uc_upnz(temp);
				x = temp;
				a = temp;
				break;

				// LDY
			case 0xB4:
			case 0xA0:
			case 0xA4:
			case 0xAC:
			case 0xBC:
				if (cxpg) peek (addr.w); //Dummy read
				temp = peek (addr.w);
				uc_upnz(temp);
				y = temp;
				break;

				// STX
			case 0x96:
			case 0x86:
			case 0x8E:
				poke (addr.w, x);
				break;

				// STY
			case 0x94:
			case 0x84:
			case 0x8C:
				poke (addr.w, y);
				break;

				// STA
			case 0x91:
			case 0x99:
			case 0x9D:
				poke (addr.w, a); // Dummy write
			case 0x81:
			case 0x85:
			case 0x8D:
			case 0x95:
				poke (addr.w, a);
				break;

				// BIT
			case 0x24:
			case 0x2C:
				temp = peek (addr.w);
				p.z = !(temp & a);
				splice_mask_inplace<192>(p.all, temp);
				break;

				// AND
			case 0x21:
			case 0x25:
			case 0x29:
			case 0x2D:
			case 0x31:
			case 0x35:
			case 0x39:
			case 0x3D:
				temp = peek (addr.w);
				temp = (a &= temp);
				uc_upnz(temp);
				break;

				// CMP
			case 0xC1:
			case 0xC5:
			case 0xC9:
			case 0xCD:
			case 0xD1:
			case 0xD5:
			case 0xD9:
			case 0xDD:
				if (cxpg) peek (addr.w); //Dummy read
				temp = peek (addr.w);
				uc_upnz(a - temp);
				p.c = (a >= temp);
				break;

				// CPY
			case 0xC0:
			case 0xC4:
			case 0xCC:
				if (cxpg) peek (addr.w); //Dummy read
				temp = peek (addr.w);
				uc_upnz(y - temp);
				p.c = (y >= temp);
				break;

				// CPX
			case 0xE0:
			case 0xE4:
			case 0xEC:
				if (cxpg) peek (addr.w); //Dummy read
				temp = peek (addr.w);
				uc_upnz(x - temp);
				p.c = (x >= temp);
				break;

				// ORA
			case 0x01:
			case 0x05:
			case 0x09:
			case 0x0D:
			case 0x11:
			case 0x15:
			case 0x19:
			case 0x1D:
				if (cxpg) peek (addr.w); //Dummy read
				temp = peek (addr.w);
				temp = (a |= temp);
				uc_upnz(temp);
				break;

				// EOR
			case 0x49:
			case 0x45:
			case 0x55:
			case 0x4D:
			case 0x5D:
			case 0x59:
			case 0x41:
			case 0x51:
				if (cxpg) peek (addr.w); //Dummy read
				temp = peek (addr.w);
				temp = (a ^= temp);
				uc_upnz(temp);
				break;

				// ADC
			case 0x69:
			case 0x65:
			case 0x75:
			case 0x6D:
			case 0x7D:
			case 0x79:
			case 0x61:
			case 0x71:
				if (cxpg) peek (addr.w); //Dummy read
				tmp0.w = peek (addr.w);
				/* ADC */ 
				tmp1.w = a + tmp0.w + p.c;				
				uc_upnz (tmp1.l);
				uc_upcv (tmp1, tmp0);
				a = tmp1.l;
				/* ADC */ 
				break;

				// SBC
			case 0xEB:
			case 0xE1:
			case 0xE5:
			case 0xE9:
			case 0xED:
			case 0xF1:
			case 0xF5:
			case 0xF9:
			case 0xFD:
				if (cxpg) peek (addr.w);
				tmp0.w = peek (addr.w);
				/* SBC */ tmp0.w = ~tmp0.w;
				tmp1.w = a + tmp0.w + p.c;
				uc_upnz (tmp1.l);
				uc_upcv (tmp1, tmp0);
				a = tmp1.l;
				/* SBC */ p.c = !p.c;
				break;

				// SEC
			case 0x38:
				p.c = 1;
				break;

				// SED
			case 0xF8:
				p.d = 1;
				break;

				// SEI
			case 0x78:
				p.i = 1;
				break;

				// CLC
			case 0x18:
				p.c = 0;
				break;

				// CLD
			case 0xD8:
				p.d = 0;
				break;

				// CLI
			case 0x58:
				p.i = 0;
				break;

				// CLV
			case 0xB8:
				p.v = 0;
				break;

				// DEX
			case 0xCA:
				tmp0.w = --x;
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80u);
				break;

				// DEY
			case 0x88:
				tmp0.w = --y;
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80u);
				break;

				// INX
			case 0xE8:
				tmp0.w = ++x;
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80u);
				break;

				// INY
			case 0xC8:
				tmp0.w = ++y;
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80u);
				break;

				// ASL A
			case 0x0A:
				(tmp0.w = a) <<= 1u;
				a = tmp0.l;
				p.c = !!(tmp0.w & 0x100u);
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80u);
				break;

				// LSR A
			case 0x4A:
				p.c = !!(a & 0x1u);
				tmp0.l = (a >>= 1u);
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80u);
				break;

				// ROR A
			case 0x6A:
				tmp0.h = !!p.c;
				p.c = !!(a & 0x1u);
				tmp0.l = (a = (a >> 1u) | (tmp0.h << 7u));
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80u);
				break;

				// ROL A
			case 0x2A:
				tmp0.h = !!p.c;
				p.c = !!(a & 0x80u);
				tmp0.l = (a = (a << 1u) | tmp0.h);
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80u);
				break;

				// TAX
			case 0xAA:
				tmp0.w = x = a;
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80u);
				break;

				// TAY
			case 0xA8:
				tmp0.w = y = a;
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80u);
				break;

				// TXA
			case 0x8A:
				tmp0.w = a = x;
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80u);
				break;

				// TYA
			case 0x98:
				tmp0.w = a = y;
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80u);
				break;

				// TSX
			case 0xBA:
				tmp0.w = x = s;
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80u);
				break;

				// TXS
			case 0x9A:
				tmp0.w = s = x;
				break;

				// BCS
			case 0xB0:
				if (!p.c)
					break;
				if (pc.h != addr.h)
					peek ((pc.l = addr.l, pc.w), tmp0.l);
				peek (pc.w = addr.w, tmp0.l);
				break;

				// BCC
			case 0x90:
				if (p.c)
					break;
				if (pc.h != addr.h)
					peek ((pc.l = addr.l, pc.w), tmp0.l);
				peek (pc.w = addr.w, tmp0.l);
				break;

				// BEQ
			case 0xF0:
				if (!p.z)
					break;
				if (pc.h != addr.h)
					peek ((pc.l = addr.l, pc.w), tmp0.l);
				peek (pc.w = addr.w, tmp0.l);
				break;

				// BNE
			case 0xD0:
				if (p.z)
					break;
				if (pc.h != addr.h)
					peek ((pc.l = addr.l, pc.w), tmp0.l);
				peek (pc.w = addr.w, tmp0.l);
				break;

				// BVS
			case 0x70:
				if (!p.v)
					break;
				if (pc.h != addr.h)
					peek ((pc.l = addr.l, pc.w), tmp0.l);
				peek (pc.w = addr.w, tmp0.l);
				break;

				// BVC
			case 0x50:
				if (p.v)
					break;
				if (pc.h != addr.h)
					peek ((pc.l = addr.l, pc.w), tmp0.l);
				peek (pc.w = addr.w, tmp0.l);
				break;

				// BMI
			case 0x30:
				if (!p.n)
					break;
				if (pc.h != addr.h)
					peek ((pc.l = addr.l, pc.w), tmp0.l);
				peek (pc.w = addr.w, tmp0.l);
				break;

				// BPL
			case 0x10:
				if (p.n)
					break;
				if (pc.h != addr.h)
					peek ((pc.l = addr.l, pc.w), tmp0.l);
				peek (pc.w = addr.w, tmp0.l);
				break;

				// PHP
			case 0x08:
				tmp0.l = p.all | 0x10; // Set break to 1
				poke (0x100 + s--, tmp0.l);
				break;

				// PHA
			case 0x48:
				tmp0.l = a;
				poke (0x100 + s--, tmp0.l);
				break;

				// PLP
			case 0x28:
				peek (0x100 + s, tmp0.l);
				peek (0x100 + ++s, tmp0.l);
				p.all = tmp0.l;
				p.b = 0;
				p.e = 1;
				break;

				// PLA
			case 0x68:
				peek (0x100 + s, tmp0.l);
				peek (0x100 + ++s, tmp0.l);
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80u);
				a = tmp0.l;
				break;

				// ASL
			case 0x1E:
				peek (addr.w, tmp0.l);
			case 0x06:
			case 0x16:
			case 0x0E:
				peek (addr.w, tmp0.l);
				/* dummy */ poke (addr.w, tmp0.l);
				p.c = !!(tmp0.l & 0x80);
				tmp0.l <<= 1u;
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80u);
				poke (addr.w, tmp0.l);
				break;

				// LSR
			case 0x5E:
				peek (addr.w, tmp0.l);
			case 0x46:
			case 0x56:
			case 0x4E:
				peek (addr.w, tmp0.l);
				/* dummy */ poke (addr.w, tmp0.l);
				p.c = !!(tmp0.l & 0x1u);
				tmp0.l >>= 1u;
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80u);
				poke (addr.w, tmp0.l);
				break;

				// ROL
			case 0x3E:
				peek (addr.w, tmp0.l);
			case 0x26:
			case 0x36:
			case 0x2E:
				peek (addr.w, tmp0.l);
				/* dummy */ poke (addr.w, tmp0.l);
				tmp0.h = !!p.c;
				p.c = !!(tmp0.l & 0x80);
				tmp0.l = (tmp0.l << 1u) | tmp0.h;
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80u);
				poke (addr.w, tmp0.l);
				break;

				// ROR
			case 0x7E:
				peek (addr.w, tmp0.l);
			case 0x66:
			case 0x76:
			case 0x6E:
				peek (addr.w, tmp0.l);
				/* dummy */ poke (addr.w, tmp0.l);
				tmp0.h = !!p.c;
				p.c = !!(tmp0.l & 0x1u);
				tmp0.l = (tmp0.l >> 1u) | (tmp0.h << 7u);
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80u);
				poke (addr.w, tmp0.l);
				break;

				// INC
			case 0xFE:
				peek (addr.w, tmp0.l);
			case 0xE6:
			case 0xF6:
			case 0xEE:
				peek (addr.w, tmp0.l);
				/* dummy */ poke (addr.w, tmp0.l);
				++tmp0.l;
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80u);
				poke (addr.w, tmp0.l);
				break;

				// DEC
			case 0xDE:
				peek (addr.w, tmp0.l);
			case 0xC6:
			case 0xD6:
			case 0xCE:
				peek (addr.w, tmp0.l);
				/* dummy */ poke (addr.w, tmp0.l);
				--tmp0.l;
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80u);
				poke (addr.w, tmp0.l);
				break;

				// DCP/DCM
			case 0xC7: // DCM ab                      5
			case 0xD7: // DCM ab,X                    6
			case 0xCF: // DCM abcd        No. Cycles= 6
			case 0xDF: // DCM abcd,X                  7
			case 0xDB: // DCM abcd,Y                  7
			case 0xC3: // DCM (ab,X)                  8
			case 0xD3: // DCM (ab),Y                  8
				if (cxpg == true)
					tmp0.l = peek (addr.w);		// Dummy read
				tmp0.l = peek (addr.w);			// Dummy read
				tmp0.l = peek (addr.w);
				--tmp0.l;
				p.z = !!(a == tmp0.l);
				p.n = !!((a - tmp0.l) & 0x80u);
				p.c = !!(tmp0.l <= a);
				poke (addr.w, tmp0.l);
				break;

				// AXS
			case 0x87: 	//AXS ab          ;ab       ;            3
			case 0x97: 	//AXS ab,Y        ;ab       ;            4
			case 0x8F: 	//AXS abcd        ;cd ab    ;No. Cycles= 4
			case 0x83: 	//AXS (ab,X)      ;ab       ;            6
				poke (addr.w, a & x);
				break;

				// ISB/INS/ISC
			case 0xE7: // INS ab                   5
			case 0xF7: // INS ab,X                 6
			case 0xEF: // INS abcd     No. Cycles= 6
			case 0xFF: // INS abcd,X               7
			case 0xFB: // INS abcd,Y               7
			case 0xE3: // INS (ab,X)               8
			case 0xF3: // INS (ab),Y               8
				if (cxpg == true)
					tmp0.w = peek (addr.w);
				tmp0.w = peek (addr.w);
				poke (addr.w, tmp0.l++); // Dummy write 
				poke (addr.w, tmp0.l);
				tmp1.w = a - tmp0.w - !p.c;
				p.z = !tmp1.l;
				p.n = !!(tmp1.l & 0x80);
				p.c = !tmp1.h;
				p.v = !!(((a ^ tmp0.l) & (a ^ tmp1.l)) >> 7u);
				a = tmp1.l;
				break;

				// ASO/SLO
			case 0x1F: // ASO abcd,X               7
			case 0x1B: // ASO abcd,Y               7
			case 0x13: // ASO (ab),Y               8
				peek (addr.w);				// Dummy read
			case 0x07: // ASO ab                   5
			case 0x17: // ASO ab,X                 6
			case 0x0F: // ASO abcd     No. Cycles= 6
			case 0x03: // ASO (ab,X)               8
				temp = peek (addr.w);
				p.c = !!(temp & 0x80);
				temp <<= 1u;
				poke (addr.w, temp);
				temp = (a |= temp);
				peek (addr.w);
				p.z = !temp;
				p.n = !!(temp & 0x80u);
				break;

				// RLA
			case 0x33: // RLA (ab),Y               8
			case 0x3B: // RLA abcd,Y               7
			case 0x3F: // RLA abcd,X               7
				peek (addr.w, tmp0.l);
			case 0x27: // RLA ab                   5
			case 0x37: // RLA ab,X                 6
			case 0x2F: // RLA abcd     No. Cycles= 6
			case 0x23: // RLA (ab,X)               8
				peek (addr.w, tmp0.l);
				tmp0.h = !!p.c;
				p.c = !!(tmp0.l & 0x80);
				tmp0.l = (tmp0.l << 1u) | tmp0.h;
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80u);
				poke (addr.w, tmp0.l);
				a &= tmp0.l;
				peek (addr.w);
				break;

				// LSE/SRE
			case 0x5F: // LSE abcd,X              7
			case 0x5B: // LSE abcd,Y              7
			case 0x53: // LSE (ab),Y              8
				peek (addr.w, tmp0.l);
			case 0x47: // LSE ab                  5
			case 0x57: // LSE ab,X                6
			case 0x4F: // LSE abcd    No. Cycles= 6
			case 0x43: // LSE (ab,X)              8
				peek (addr.w, tmp0.l);
				p.c = !!(tmp0.l & 0x1u);
				tmp0.l >>= 1u;
				poke (addr.w, tmp0.l);
				tmp0.l = (a ^= tmp0.l);
				peek (addr.w);
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80u);
				break;

			case 0x7B: // RRA abcd,Y              7
			case 0x7F: // RRA abcd,X              7
			case 0x73: // RRA (ab),Y              8
				peek (addr.w, tmp0.l);
			case 0x67: // RRA ab                  5
			case 0x77: // RRA ab,X                6
			case 0x6F: // RRA abcd    No. Cycles= 6
			case 0x63: // RRA (ab,X)              8
				peek (addr.w, tmp0.l);
				tmp0.h = !!p.c;
				p.c = !!(tmp0.l & 0x1u);
				tmp0.l = (tmp0.l >> 1u) | (tmp0.h << 7u);
				poke (addr.w, tmp0.w &= 0xff);
				tmp0.w = peek (addr.w);
				tmp1.w = tmp0.w + (!!p.c) + a;
				p.z = !tmp1.l;
				p.n = !!(tmp1.l & 0x80);
				p.c = !!tmp1.h;
				p.v = !!((~(a ^ tmp0.l) & (a ^ tmp1.l)) >> 7u);
				a = tmp1.l;
				break;
				
			case 0xBB: // LAS
				if (cxpg)
					peek (addr.w, tmp0.l);
				tmp0.l &= s;
				a = tmp0.l;
				x = tmp0.l;
				s = tmp0.l;
				p.z = !tmp0.l;
				p.n = !!(tmp0.l & 0x80);
				peek (addr.w); // Dummy read
				break;

				// NOP
			case 0x1C:
			case 0x3C:
			case 0x5C:
			case 0x7C:
			case 0xDC:
			case 0xFC:
				if (cxpg == true)
					peek (addr.w);  // Dummy Read
			case 0x0C:
			case 0x04:
			case 0x14:
			case 0x34:
			case 0x44:
			case 0x54:
			case 0x64:
			case 0x74:
			case 0x80:
			case 0xD4:
			case 0xF4:
				peek (addr.w); // Dummy Read
			case 0x1A:
			case 0x3A:
			case 0x5A:
			case 0x7A:
			case 0xDA:
			case 0xEA:
			case 0xFA:
				break;

			default:
				assert (false);
				break;
			}

		}
	}

	void uc_upcv (Word& lhs, Word rhs)
	{
		p.v = !!(((a ^ lhs.l) & (a ^ ~rhs.l)) >> 7u);
		p.c = !!lhs.h;
	}

private:
	_Host& host_;

	// Various

	bool		dma_ { false };	// DMA Trigger
	byte		dpg_ { 0u };  // DMA Page

public:

	bool		nmi		{ false };
	bool		rst		{ false };
	bool		irq		{ false };
	qword		clk		{ 0ull };

	/// Registers

	byte		a, x, y, s;
	Word		pc;
	Flag		p;

};
