#include <utils.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cerrno>
#include <cstring>

namespace utils {

	std::vector<unsigned char> load_file(const std::string& path) {

	// Open file and set stream to end of file
	std::ifstream file(path, std::ios::binary | std::ios::ate);

	// Check if file was opened
	if (!file.is_open()) {
		std::string errorMsg = "Unable to open file: " + path + " - " + std::strerror(errno);
		throw std::runtime_error(errorMsg);
	}

	// Determine file size
	std::size_t fileSize = file.tellg();

	// Go back to beginning of file and read the data
	file.seekg(0, std::ios::beg);

	// Allocate a vector to hold all the data
	std::vector<unsigned char> fileData(fileSize);

	// Read the data into the vector
	file.read(reinterpret_cast<char*>(fileData.data()), fileSize);

	// Check if all the data was read
	if (!file) {
		throw std::runtime_error("Failed to read file: " + path);
	}

	// close the file
	file.close();

	return fileData;
	}

	void error( const char* str ) {
		if ( str )
		{
			fprintf( stderr, "Error: %s\n", str );
			exit( EXIT_FAILURE );
		}
	}

	std::string find_spc_file() {
		std::cout << "Searching for .spc file in " << std::filesystem::current_path() << std::endl;
    std::vector<std::string> spc_files;

    // Collect all .spc files
    for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
        if (entry.path().extension() == ".spc") {
            spc_files.push_back(entry.path().string());
        }
    }

    // If no .spc file is found
    if (spc_files.empty()) {
        std::cerr << "Could not find .spc file in " << std::filesystem::current_path() << std::endl;
        exit(EXIT_FAILURE);
    }
    // If only one .spc file is found, return it
    else if (spc_files.size() == 1) {
        return spc_files[0];
    }
    // If multiple .spc files are found
    else {
			std::cout << "Multiple .spc files found. Please choose one:\n";
			for (size_t i = 0; i < spc_files.size(); ++i) {
					std::string spc_file_name = spc_files[i].substr(spc_files[i].find_last_of("/\\") + 1);
					std::cout << i + 1 << ". " << spc_file_name << "\n";
			}

			size_t choice = 0;
			while (true) {
				std::cout << "Enter the number of your choice: ";
				std::cin >> choice;

				if (std::cin.fail()) {  // Check if the input operation failed
						std::cin.clear();  // Clear the error flag
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discard invalid input
						std::cerr << "Invalid input. Please enter a number.\n";
				}
				else if (choice > 0 && choice <= spc_files.size()) {
						std::cout << choice << ".\n";
						return spc_files[choice - 1];
				}
				else {
						std::cerr << "Invalid choice. Please choose a number between 1 and " << spc_files.size() << ".\n";
				}
			}
		}
	}


	std::string get_spc_file_name(std::string spc_file_path) {
		std::string spc_file_name = spc_file_path.substr(spc_file_path.find_last_of("/\\") + 1);

		// Remove .spc extension from file name
		spc_file_name.erase(spc_file_name.find_last_of("."), std::string::npos);
		return spc_file_name;
	}

	// If an out folder doesn't exist, create it. Otherwise, return the path to the out folder
	std::string get_out_path() {
		std::string out_folder_path = std::filesystem::current_path().string() + "/out";
		if (!std::filesystem::exists(out_folder_path)) {
			std::filesystem::create_directory(out_folder_path);
		}
		return out_folder_path;
	}
}
