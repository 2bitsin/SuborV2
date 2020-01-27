#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "coro.hpp"
#include "arguments.hpp"
#include "cartdata.hpp"

int main(int argc, char** argv, char** envp)
{
	const arguments cli{ argc, argv, envp };


	return 0;
}