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

	enum vram_org_type
	{
		vram_1k,
		vram_2k_hori,
		vram_2k_vert,
		vram_4k
	};


	void load_ines (file_path_type pfile)
	{
		using bin_t = std::vector<std::byte>;
		rom_lock_ = load_file (pfile);
		auto& binary = std::any_cast<bin_t&> (rom_lock_);
		binary.data ();
	}

///////////////////////////////

	std::any							rom_lock_;	// ROM data ptr
	std::any							bat_lock_;	// Battery data ptr

	std::byte							chip_num_;	// Mapper ID
	vram_org_type					vram_org_;	// VRAM mirroring
	std::uint16_t					pram_len_;	// PRG RAM size

	span_type<std::byte>	chr_data_;  // CHR ROM 
	span_type<std::byte>	prg_data_;  // PRG ROM 
	span_type<std::byte>	trn_data_;  // Trainer ROM 

	span_type<std::byte>	bat_data_;	// Battery RAM
};