#include <src/tests/kevtris_test.hpp>
#include <src/etc/arguments.hpp>
#include <src/core/machine.hpp>


int main(int argc, char** argv, char** envp)
{
	//arguments _args {argc, argv, envp};
	//machine m { };

	if (kevtris_nestest())
		__debugbreak();
}
