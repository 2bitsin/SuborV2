#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <etc/coro.hpp>
#include <etc/arguments.hpp>
#include <cart/cartdata.hpp>
#include <core/core.hpp>

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

	sixtyfiveohtwo._N(1);
	sixtyfiveohtwo._B(1);
	sixtyfiveohtwo._C(1);
	return 0;
}
catch(const std::exception& ex)
{
	std::cout << ex.what () << "\n";
	return -1;
}