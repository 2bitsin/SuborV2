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

	enum region_type
	{
		region_ntsc,
		region_pal,
		region_dual
	};


	void load_ines (file_path_type pfile);
	void load_test ();

private:
	
	template <typename _Header_callback>
	void _parse_header (_Header_callback&& hdrcall, span_type<const xxx::byte> header);

///////////////////////////////
	std::any cart_data_lock;	// ROM data ptr

	xxx::byte	chip_id;				// Mapper ID
	vram_org_type	vram_org;		// VRAM mirroring
	region_type	region;				// Region

	xxx::word	prg_ram_length;	// PRG RAM size
	xxx::word	bat_ram_length;	// Battery backed ram length

	span_type<const xxx::byte> chr_rom_data;  // CHR ROM 
	span_type<const xxx::byte> prg_rom_data;  // PRG ROM 
	span_type<const xxx::byte> trn_rom_data;  // Trainer ROM 

	xxx::qword lo_hash;	// Hash Low
	xxx::qword hi_hash;	// Hash High
};