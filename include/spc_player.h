#pragma once
#include <spc.h>
#include <string>
#include <vector>

class SPCPlayer {
  private:
    SNES_SPC* spc_instance;
    SPC_Filter* filter;
    int song_length;
    std::string spc_file_name;
    std::string spc_file_path;
    std::string out_file_path;
    std::vector<unsigned char> spc_file;
  
  public:
    SPCPlayer();
    ~SPCPlayer();
    void to_wav(int duration);
    void to_stems(int duration);
  
  private:
    void init();
    void cleanup();
};