#pragma once
#include <string>
#include <vector>
#include <gme/Music_Emu.h>

namespace utils {
  std::vector<unsigned char> load_file(const std::string& path);
  void error( const char* str );

  std::string find_spc_file();
  std::string get_spc_file_name(std::string spc_file_path);
  std::string get_out_path();
}

