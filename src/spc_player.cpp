#include <spc_player.h>
#include <iostream>
#include <vector>
#include <utils.h>
#include <wave_writer.h>
#include <spc.h>

SPCPlayer::SPCPlayer() {
    this->spc_file_path = utils::find_spc_file();
    this->spc_file_name = utils::get_spc_file_name(this->spc_file_path);
    this->out_file_path = utils::get_out_path();
    this->song_length = 20; // 20 seconds default
    init();
}

SPCPlayer::~SPCPlayer() {
    cleanup();
}

void SPCPlayer::init() {
  this->spc_instance = spc_new();
  this->filter = spc_filter_new();

  if (this->spc_instance == nullptr || this->filter == nullptr) {
    throw std::runtime_error("Could not create SPC instance or filter instance");
  }

  // Load SPC file
  try {
    this->spc_file = utils::load_file(this->spc_file_path);
  } catch (std::runtime_error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    throw e;
  }
}

void SPCPlayer::to_wav(int duration) {
  // Load SPC file into SPC instance
  this -> song_length = duration;
  utils::error( spc_load_spc(this->spc_instance, this->spc_file.data(), this->spc_file.size()) );

  // Clear filters and echoes
  spc_filter_clear(this->filter);
  spc_clear_echo(this->spc_instance);

  std::cout << "Converting " << this->spc_file_name << ".spc to WAV" << std::endl;

  char filename[256];
  snprintf(filename, sizeof(filename), "%s/%s.wav", this->out_file_path.c_str(), this->spc_file_name.c_str());
  wave_open(spc_sample_rate, filename);
  wave_enable_stereo();

  while ( wave_sample_count() < song_length * spc_sample_rate * 2) {
    // Define buffer (2048)
    #define BUF_SIZE 2048
		short buf [BUF_SIZE];
    utils::error( spc_play(this->spc_instance, BUF_SIZE, buf));

    // Filter samples
    spc_filter_run(this->filter, buf, BUF_SIZE);

    // Write samples to file
    wave_write(buf, BUF_SIZE);
  }
  wave_close();
  std::cout << "Done!" << std::endl;
}

void SPCPlayer::to_stems(int duration) {
  this -> song_length = duration;
  // Mute masks
  int mute_mask[8] = {254, 253, 251, 247, 239, 223, 191, 127};
  char filename[256];
  std::cout << "Converting " << this->spc_file_name << ".spc to stems." << std::endl;
  
  // Create 8 stems
  for (int i = 0; i < 8; i++) {
    // Load SPC file into SPC instance
    utils::error( spc_load_spc(this->spc_instance, this->spc_file.data(), this->spc_file.size()) );

    // Clear filters and echoes
    spc_filter_clear(this->filter);
    spc_clear_echo(this->spc_instance);
    
    snprintf(filename, sizeof(filename), "%s/%s_%d.wav", this->out_file_path.c_str(), this->spc_file_name.c_str(), i);
    wave_open(spc_sample_rate, filename);
    wave_enable_stereo();

    // Mute all channels except one
    spc_mute_voices(this->spc_instance, mute_mask[i]);

    while ( wave_sample_count() < song_length * spc_sample_rate * 2) {
      // Define buffer (2048)
      #define BUF_SIZE 2048
      short buf [BUF_SIZE];
      utils::error( spc_play(this->spc_instance, BUF_SIZE, buf));

      // Filter samples
      spc_filter_run(this->filter, buf, BUF_SIZE);

      // Write samples to file
      wave_write(buf, BUF_SIZE);
    }
    wave_close();
    std::cout << "Stem " << i + 1 << " done." << std::endl;
  }

  std::cout << "All stems rendered to " << this->out_file_path << std::endl;
}

void SPCPlayer::cleanup() {
  spc_filter_delete(this->filter);
  spc_delete(this->spc_instance);
}