#pragma once

#include <src/etc/types.hpp>
#include <src/etc/literals.hpp>
#include <src/core/core.hpp>
#include <src/video/video.hpp>
#include <src/core/memory.hpp>

struct machine
{
	bool halt ()
	{
		return false;
	}

	void poke (types::word addr, const types::byte& data)
	{}

	void peek (types::word addr, types::byte& data)
	{}

	void tick (int)
	{}

	core  <machine>						core_		{ *this };
	video <machine>						video_	{ *this };
	ram   < 2_k,  0_k,  8_k>	sram_   { *this };
	ram   < 4_k,  8_k, 16_k>	vram_   { *this }; 
	rom		<32_k, 32_k, 64_k>	prgr_		{ *this };
	rom		< 8_k,  0_k,  8_k>	chrr_		{ *this };

};