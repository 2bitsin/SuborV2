#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <src/etc/coro.hpp>
#include <src/etc/arguments.hpp>
#include <src/etc/literals.hpp>
#include <src/cart/cartdata.hpp>
#include <src/core/core.hpp>

#include <src/data/nestest_log.hpp>
#include <src/data/nestest.hpp>

#include <iostream>
#include <algorithm>

//#include "md5.hpp"


struct fmc
{
	xxx::byte RAM [32_K];
	xxx::byte ROM [32_K];

	fmc ()
	: ricore (*this)
	{
		cartdata test;
		test.load_test ();
		std::fill (std::begin (RAM), std::end (RAM), 0);
		std::copy (test.prg_rom_data.begin (),
			test.prg_rom_data.end (),
			ROM + 00_K);
		std::copy (test.prg_rom_data.begin (),
			test.prg_rom_data.end (),
			ROM + 16_K);
	}

	bool halt () 
	{
		halt_ = !halt_;
		return halt_;
	}

	auto peek (xxx::word addr, xxx::byte& data)
	{
		if (addr >= 32_K)
			data = ROM [addr - 32_K];
		else
			data = RAM [addr];
		//std::printf ("    peek (0x%04X) -> 0x%02X\n", addr, data);
	}

	auto poke (xxx::word addr, xxx::byte data)
	{
		if (addr >= 32_K)
			return;
		RAM [addr] = data;
		//std::printf ("    poke (0x%04X, 0x%02X)\n", addr, data);
	}

	auto tick (int tt)
	{}

	template <typename _Lhs, typename _Rhs>
	bool assert_state (_Lhs&& lhs, _Rhs&& rhs)
	{
		bool is_valid { true };

		is_valid = is_valid && lhs.a  == rhs.a	;
		is_valid = is_valid && lhs.x  == rhs.x	;
		is_valid = is_valid && lhs.y  == rhs.y	;
		is_valid = is_valid && lhs.s  == rhs.s	;
		is_valid = is_valid && lhs.p  == rhs.p	;
		is_valid = is_valid && lhs.pc == rhs.pc	;

		is_valid = is_valid && lhs.ticks_elapsed() == rhs.cpuclock;

		return is_valid;
	}

	char* flags2str(std::string& buff, xxx::byte p)
	{
		constexpr auto es = '.';
		buff.clear();
		buff.push_back(p & 0x01u ? 'C' : es);
		buff.push_back(p & 0x02u ? 'Z' : es);
		buff.push_back(p & 0x04u ? 'I' : es);
		buff.push_back(p & 0x08u ? 'D' : es);
		buff.push_back(p & 0x10u ? 'B' : es);
		buff.push_back(p & 0x20u ? 'E' : es);
		buff.push_back(p & 0x40u ? 'V' : es);
		buff.push_back(p & 0x80u ? 'N' : es);
		return buff.data();
	}

	auto loop ()
	{
		ricore.p.all = 0x24u;
		ricore.a = 0x00u;
		ricore.x = 0x00u;
		ricore.y = 0x00u;
		ricore.s = 0xFDu;
		ricore.pc.w = 0xC000u;
		ricore.clk = 7;
		
		std::printf("         | %-16s", "Initial");
		for (auto&& line : nestest_log ())
		{			
			if (assert_state (ricore, line)) 
			{
				std::printf(" ... PASS | ");
				std::printf("A:%02X X:%02X Y:%02X S:%02X P:%02X PC:%04X\n",
										ricore.a, ricore.x, ricore.y, ricore.s, ricore.p.all, ricore.pc.w);
				switch(line.nbytes)
				{
				case 1: std::printf("%02X       | ", line.opbytes[0]); break;
				case 2: std::printf("%02X %02X    | ", line.opbytes[0], line.opbytes[1]); break;
				case 3: std::printf("%02X %02X %02X | ", line.opbytes[0], line.opbytes[1], line.opbytes[2]); break;
				}
				std::printf("%-16s", line.instruction);				
				ricore.exec ();
				continue;
			}

			std::string s0, s1;
			std::printf (" ... FAIL\n");
			std::printf ("\n");
			std::printf ("    |-------------|-------------|\n");
			std::printf ("    | Actual      | Expected    |\n");
			std::printf ("    |-------------|-------------|\n");
			std::printf ("    | A  = 0x%02X   | A  = 0x%02X   |\n", ricore.a, line.a);
			std::printf ("    | X  = 0x%02X   | X  = 0x%02X   |\n", ricore.x, line.x);
			std::printf ("    | Y  = 0x%02X   | Y  = 0x%02X   |\n", ricore.y, line.y);
			std::printf ("    | S  = 0x%02X   | S  = 0x%02X   |\n", ricore.s, line.s);
			std::printf ("    | P  = 0x%02X   | P  = 0x%02X   |\n", ricore.p.all, line.p);
			std::printf ("    | P[%s] | P[%s] |\n", flags2str (s0, ricore.p.all), flags2str (s1, line.p));
			std::printf ("    | PC = 0x%04X | PC = 0x%04X |\n", ricore.pc.w, line.pc);
			std::printf ("    | CK = %-6lld | CK = %-6lld |\n", ricore.ticks_elapsed(), line.cpuclock);
			std::printf ("    |-------------|-------------|\n");
			std::printf ("\n");
			throw std::runtime_error ("Bad state.");
		}
	}

	core<fmc> ricore;
	bool halt_ { true };
};


int main (int argc, char** argv, char** envp)
try
{
	const arguments cli { argc, argv, envp };

	cartdata cd;
	fmc _fmc;

	_fmc.loop ();


	return 0;
}
catch (const std::exception & ex)
{
	std::cout << ex.what () << "\n";
	return -1;
}