#pragma once

#include <filesystem>
#include <fstream>
#include <cstddef>

auto load_file(std::filesystem::path pfile)
	-> std::vector<std::byte>
{
	using namespace std::filesystem;
	const auto length = file_size(pfile);
	if (length < 1u)
		return {};
	std::vector<std::byte> bin { length };
	std::ifstream rdfl;
	rdfl.exceptions(rdfl.badbit|rdfl.failbit|rdfl.eofbit);
	rdfl.open(pfile, std::ios::binary);
	rdfl.read((char*)bin.data(), length);	
	return bin;
}