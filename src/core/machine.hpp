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

	core<machine> core_;
	video<machine> video_;
	
};