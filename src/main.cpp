#include <iostream>
#include <string>
#include <spc_player.h>
#include <utils.h>
#include <wave_writer.h>

int main( int argc, char** argv ) {

		// Usage: spc full <duration> or spc stems <duration>
		if (argc != 3) {
			std::cerr << "Usage: snes-spc <full|stems> <duration>" << std::endl;
			return 1;
		}

		std::string mode = argv[1];
		int duration = std::stoi(argv[2]);

		SPCPlayer player;

		if (mode == "full") {
			player.to_wav(duration);
		} else if (mode == "stems") {
			player.to_stems(duration);
		} else {
			std::cerr << "Usage: snes-spc <full|stems> <duration>" << std::endl;
			return 1;
		}
    return 0;
}