#pragma once

#include <filesystem>
#include <any>
#include <nonstd/span.hpp>
#include <src/etc/load_file.hpp>

struct cartdata
{
	using file_path_type = std::filesystem::path;

	template <typename type_>
	using span_type = nonstd::span<type_>;

	enum class vram_org_type
	{
		vram_1k,
		vram_2k_hori,
		vram_2k_vert,
		vram_4k
	};

	enum class region_type
	{
		region_ntsc,
		region_pal,
		region_dual
	};


	void load_ines (file_path_type pfile);
	void load_test ();
	void load_span (span_type<const types::byte> buff);
	void load_vect (std::vector<types::byte> buff);

private:
	void _parse (span_type<const types::byte> inesrom);

///////////////////////////////
	std::any cart_data_lock;	// ROM data ptr

public:
	types::byte	chip_id;				// Mapper ID
	vram_org_type	vram_org;		// VRAM mirroring
	region_type	region;				// Region

	types::word	prg_ram_size;	// PRG RAM size
	types::word chr_ram_size; // CHR RAM size

	span_type<const types::byte> chr_rom_data;  // CHR ROM 
	span_type<const types::byte> prg_rom_data;  // PRG ROM 
	span_type<const types::byte> trn_rom_data;  // Trainer ROM 
};