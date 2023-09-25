# Snes Stems
Command-line tool that converts SNES SPC files into either a playable .wav file, or 8 .wav files for each audio channel.

## Installation and Compilation
In a blank directory, run the following in the commandline:
```bash
git clone https://github.com/bgevko/snes-stems.git
```

To compile, run the following from the root directory:
```bash
mkdir build
cd build
cmake -S .. # Wait for the build to finish
make
```

There should now be an executable in the `build` folder called `snes-spc`. To run from anywhere in the terminal, system link it like this:
```bash
sudo -ln -s $(pwd)/snes-spc /usr/local/bin/snes-spc
```

Give the tool proper permissions for execution:
```bash
chmod +x $(pwd)/snes-spc
```

## Usage
To use, `cd` into a directory that holds an SPC file, and use the command:
``` bash
snes-spc <full|stems> <duration>
```

For example, `snes-spc full 30` will render a 30 second .wav file from the specified SPC. `snes-spc stems 10` will render 8 .wav files, with each corresponding to their own seperate audio channel. Implementation is based off of Blaarg's snes-spc library, which can be found [here](https://github.com/elizagamedev/snes_spc).
