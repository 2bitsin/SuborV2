#include "cartdata.hpp"
#include <src/data/nestest.hpp>

using vector_of_byte = std::vector<types::byte>;

void cartdata::load_ines (file_path_type pfile)
{
	load_vect (load_file(pfile));
}

void cartdata::load_vect (std::vector<types::byte> buff)
{
	cart_data_lock = std::move (buff);
	auto& binary = std::any_cast<vector_of_byte&> (cart_data_lock);
	_parse (binary);
}

void cartdata::load_test ()
{
	cart_data_lock.reset();
	_parse(_test_rom);	
}

void cartdata::load_span (span_type<const types::byte> buff)
{
	cart_data_lock.reset();
	_parse(buff);
}


void cartdata::_parse (span_type<const types::byte> inesrom)
{
	bool is_valid = true;

	auto header = inesrom.subspan(0u, 16u);

	is_valid = is_valid && header[0u] == 'N';
	is_valid = is_valid && header[1u] == 'E';
	is_valid = is_valid && header[2u] == 'S';
	is_valid = is_valid && header[3u] == '\x1a';

	if (!is_valid)
		throw std::runtime_error("Bad iNes header.");

	is_valid = ((header[10u] >> 1u) & 1u) 
		== (header[9u] & 1u);

	if (!is_valid)
		throw std::runtime_error("Bad iNes header.");

	if (header[6u] & 8u)
		vram_org = vram_org_type::vram_4k;
	else if (header [6u] & 1u)
		vram_org = vram_org_type::vram_2k_vert;
	else 
		vram_org = vram_org_type::vram_2k_hori;

	chip_id
		= ((header[6u] & 0xf0u) >> 4u)
		| ((header[7u] & 0xf0u));
	
	if (header[9u] & 1u)
		region = region_type::region_pal;
	else
		region = region_type::region_ntsc;
	if (header[10u] & 1u)
		region = region_type::region_dual;

	chr_ram_size = header[5u] ? 0u : ((header[8] + 1u) * 8192u);
	prg_ram_size = (header[6u] & 2u) ? 8192u : 0u;

	types::dword prg_rom_size = header[4u] * 16384u;
	types::dword chr_rom_size = header[5u] * 8192u;
	types::dword trn_rom_size = (header[6u] & 4u) ? 512u : 0u;
	
	types::dword offset = 16u;

	trn_rom_data = inesrom.subspan(offset, trn_rom_size);
	offset += trn_rom_size;
	
	prg_rom_data = inesrom.subspan(offset, prg_rom_size);
	offset += prg_rom_size;

	chr_rom_data = inesrom.subspan(offset, chr_rom_size);
	offset += chr_rom_size;
}

