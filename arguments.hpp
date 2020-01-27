#pragma once

#include <vector>
#include <string_view>
#include <string>
#include <unordered_map>
#include <algorithm>

struct arguments
{
	arguments(int argc, char** argv, char** envp)
	:	args_{ argv, argv+ argc },
		envp_{ parse_envp_ (envp) }
	{}

	auto&& args () { return args_ ; }
	auto&& envp () { return envp_ ; }
	auto&& args () const { return args_ ; }
	auto&& envp () const { return envp_ ; }

protected:

	template <typename I, typename... U>
	static auto find_(I seq, U... value)
	{
		while(!(... || (*seq == value)))
			++seq;
		return seq;
	}

	static inline auto parse_envp_(char** ptr)
		-> std::unordered_map<std::string_view, std::string_view>
	{
		std::unordered_map<std::string_view, std::string_view> result;
		std::for_each (ptr, find_ (ptr, nullptr), [&result](auto&& beg) 
		{ 
			auto mid = find_(beg, '=', '\0');			
			result.insert({
				{beg,	std::uint64_t(mid - beg)}, 
				{mid+1u}
			});
		});
		return result;
	}

	std::unordered_map<std::string_view, std::string_view> envp_;
	std::vector<std::string_view> args_;
};