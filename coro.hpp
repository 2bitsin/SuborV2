#pragma once

#include <memory>
#include <cstddef>
#include <cstdint>
#include <cassert>

#define coro_callconv __stdcall

using coro_t				= struct coro*;
using coro_entry_t	= void (coro_callconv *)(void*);

coro_t	coro_init(void* data);
void		coro_switch_to(coro_t);
coro_t	coro_create(void* data, coro_entry_t start, std::size_t stsz) ;
void		coro_delete(coro_t);
coro_t	coro_this();
void		coro_exit(coro_t = nullptr);
void*		coro_data();


template <typename T>
using uptr = std::unique_ptr<T, void(*)(T*)>;

struct coro_handle
:	uptr<struct coro>
{
	using uptr<struct coro>::uptr;

	void yield()
	{
		coro_switch_to(get());
	}

	static inline void* data() 
	{
		return coro_data();
	};

};

struct coro_slave
:	coro_handle
{
	coro_slave (void* data, coro_entry_t entry, std::size_t stsz = 1024*1024)
	:	coro_handle { coro_create(data, entry, stsz), &coro_delete }
	{}
};

struct coro_master
:	coro_handle
{
	coro_master (void* data = nullptr)
	:	coro_handle { coro_init(data), &coro_exit }
	{}
};
