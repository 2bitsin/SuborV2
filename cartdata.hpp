#pragma once

#include <filesystem>
#include <any>
#include <nonstd/span.hpp>

#include "load_file.hpp"

struct cartdata
{
	using file_path_type = std::filesystem::path;

	template <typename type_> 
	using span_type = nonstd::span<type_>;

	void load_ines(file_path_type pfile)
	{
		using bin_t = std::vector<std::byte>;
		rom_lock_ = load_file(pfile);
		auto& binary = std::any_cast<bin_t&> (rom_lock_);
		binary.data();
	}

	std::any rom_lock_;
	std::any bat_lock_;

	std::byte chip_num_;
	span_type<std::byte> chr_data_;
	span_type<std::byte> prg_data_;
	span_type<std::byte> tnr_data_;

	span_type<std::byte> bat_data_;
};