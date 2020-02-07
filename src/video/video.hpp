#pragma once

#include <src/etc/types.hpp>
#include <src/etc/literals.hpp>

template <typename _Host>
struct video
{
	video(_Host& host)
	:	host_ { host }
	{}

	void exec ();

	void peek(word addr)

private:
	_Host& host;
};

template <typename _Host>
inline void video<_Host>::exec ()
{
}
