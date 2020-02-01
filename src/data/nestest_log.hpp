#pragma once

#include <src/etc/types.hpp>

	struct nestest_state
	{  
		using byte = xxx::byte;
		using word = xxx::word;
		using dword = xxx::dword;
		using qword = xxx::qword;

		word pc;
		struct { byte a, x, y, p, s; };
	
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
