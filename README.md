# Set up GBDK

Utilize the following repository - https://github.com/gheja/gbdk

Unfortunately, the files hosted at [https://sourceforge.net/projects/gbdk/files/gbdk/2.96/](https://sourceforge.net/projects/gbdk/files/gbdk/2.96/) run into the issue solved by [https://github.com/gheja/gbdk/pull/16](https://github.com/gheja/gbdk/pull/16) so the updated version is necessary.

### On Unix
1. Extract GBDK somewhere and remember the location
2. Change directories to the GBDK folder and run 'make'
	- May need to install 'make', 'gcc', 'g++', 'bison', and 'flex'
3. Run 'make install'

### To run via CLion
1. Add BashSupport plugin
2. For program choose 'make.sh'
3. For arguments add the absolute path to LCC (eg /opt/gbdk/bin/lcc) and then the absolute path to the emulator (eg VBA)

### Tools used
- Gameboy Emulator
- GBMB - http://www.devrs.com/gb/hmgd/gbmb.html
- GBTD - http://www.devrs.com/gb/hmgd/gbtd.html
