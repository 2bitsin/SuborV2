#include <src/tests/kevtris_test.hpp>
#include <src/etc/arguments.hpp>
#include <src/core/core.hpp>
#include <src/core/memory.hpp>

int main(int argc, char** argv, char** envp)
{
	arguments _args {argc, argv, envp};

	memory<8_k, 16_k, 32_k> mm;

	mm.cycle<write>(0, 0);

	//return kevtris_nestest();
}
