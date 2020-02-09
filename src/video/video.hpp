#pragma once

#include <src/etc/types.hpp>
#include <src/etc/literals.hpp>

template <typename _Host>
struct video
{
	using byte = types::byte;
	using word = types::word;

	video(_Host& host)
	:	host_ { host }
	{}

	void exec () 
	{}

	void peek(word addr, byte& data)
	{}

	void poke(word addr, byte data)
	{}

private:
	_Host& host_;
};

