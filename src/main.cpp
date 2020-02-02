#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <src/etc/coro.hpp>
#include <src/etc/arguments.hpp>
#include <src/cart/cartdata.hpp>
#include <src/core/core.hpp>

#include <iostream>

//#include "md5.hpp"


struct fmc
{
	fmc(): ricore(*this) {}

	bool halt() const
	{
		return false;
	}

	auto peek(xxx::word addr, xxx::byte& data)
	{}

	auto poke(xxx::word addr, xxx::byte data)
	{}

	auto tick(int)
	{}

	core<fmc> ricore;
};


int main(int argc, char** argv, char** envp)
try
{
	const arguments cli{ argc, argv, envp };

	cartdata cd;
	fmc _fmc;

	_fmc.ricore.exec();


	return 0;
}
catch(const std::exception& ex)
{
	std::cout << ex.what () << "\n";
	return -1;
}