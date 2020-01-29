#include "coro.hpp"

#include <Windows.h>

coro_t coro_init (void* data)
{	
	return (coro_t)ConvertThreadToFiber(data);
}

coro_t coro_create (void* data, coro_entry_t start, std::size_t stsz)
{
	return (coro_t)CreateFiber(stsz, (LPFIBER_START_ROUTINE)start, data);
}

void coro_delete (coro_t coro)
{
	DeleteFiber(coro);
}

coro_t coro_this ()
{
	return (coro_t)GetCurrentFiber();
}

void coro_exit (coro_t)
{
	ConvertFiberToThread();
}

void* coro_data ()
{
	return GetFiberData();
}

void coro_switch_to(coro_t coro)
{
	SwitchToFiber(coro);
}
