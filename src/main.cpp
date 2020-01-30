#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <src/etc/coro.hpp>
#include <src/etc/arguments.hpp>
#include <src/cart/cartdata.hpp>
#include <src/core/core.hpp>

#include <iostream>

//#include "md5.hpp"

int main(int argc, char** argv, char** envp)
try
{
	const arguments cli{ argc, argv, envp };

	cartdata cd;

	//cd.load_test();
	cd.load_ines("smb.nes");

	core<cartdata> sixtyfiveohtwo { cd } ;
	return 0;
}
catch(const std::exception& ex)
{
	std::cout << ex.what () << "\n";
	return -1;
}