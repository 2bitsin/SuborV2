#pragma once

#include <src/core/core.hpp>
#include <src/video/video.hpp>

struct machine
{
	bool halt();
	void peek(types::word addr, types::byte& data);
	void peek(types::word addr, types::byte data);


	core<machine>	core_;	
	video<machine> video_;
	//sram<machine, 0, 8_K, 2_K> sram_;
	//sram<machine, 8_K, 16_K, 4_K>	vram_;
};