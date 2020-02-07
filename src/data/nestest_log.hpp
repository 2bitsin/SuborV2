#pragma once

#include <src/etc/types.hpp>
#include <nonstd/span.hpp>

struct nestest_state
{  
	using byte = types::byte;
	using word = types::word;
	using dword = types::dword;
	using qword = types::qword;

	word pc;
	struct 
	{ 
		byte a; 
		byte x; 
		byte y; 
		byte p; 
		byte s; 
	};

	byte nbytes;
	byte opbytes[3u];    

	qword cpuclock;
	qword ppuclock;
	qword column;
	qword scanline;

	bool unofficial;
	const char* instruction;
	const char* disassembly;
};

auto nestest_log() -> nonstd::span<const nestest_state> ;
	