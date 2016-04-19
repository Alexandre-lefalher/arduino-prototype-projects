# 2016_04_13-Calibration-6DOF_shield-leka-ADXL345-ITG3205

# For-Notes

This calibration code is functional with the FreeIMU library installed withing the Moti repository, the following is caracteristiques, buitl with the moti-Makefile. 

- [AUTODETECTED]       CURRENT_OS = MAC
- [AUTODETECTED]       ARDUINO_VERSION = 168
- [DEFAULT]            ARCHITECTURE = avr
- [DEFAULT]            VENDOR = arduino
- [USER]               BOARD_SUB = atmega2560
- [USER]               BOARD_TAG = mega
- [COMPUTED]           CORE = arduino (from build.core)
- [COMPUTED]           VARIANT = mega (from build.variant)
- [USER]               MONITOR_BAUDRATE = 115200
- [DEFAULT]            OPTIMIZATION_LEVEL = s
- [DEFAULT]            MCU_FLAG_NAME = mmcu
- [USER]               CFLAGS_STD = -std=gnu11
- [USER]               CXXFLAGS_STD = -std=gnu++14
- [COMPUTED]           ARDMK_VERSION = 1.5
- [COMPUTED]           CC_VERSION = 4.9.3 (avr-gcc)

# Installation

Retreaving/tapping our code - makefiles. TBD

Getting the freeIMU library, and look into 

## Mac OS  
### Octave 

`brew tap homebrew/science`  

`brew install octave`  

A problem is that the default configuration of Homebrew's gnuplot doesn't 
support any terminals for displaying the plots.

We recommened unistalling and reinstall with the terminal your prefer. 

`brew uninstall gnuplot`

In OS X Yosemite 10.10.2 and El Capitan 10.11.2, the Gnuplot does not package with XQuartz. Answer in bibstha's comment. Much more robust window manager is in qt and it works.  

Therefore we have gone with: 

`brew install gnuplot --with-qt` 

if x11 and XQuartz is your jam, then do: (un-tested area for us)
`brew install gnuplot --with-x11`

Almost done !  

Find the Octave startup file, **octaverc** and with your text editor of choiceset your to the terminal environement of your choice: 

Usually found in: `/usr/local/share/octave/site/m/startup/`

Add following:
`setenv("GNUTERM","qt")`
or
`setenv("GNUTERM","X11")`

### Plotting

Running the calibrate.py files 

## Others

> TBD


## Cariage Return & Line Feed:

CR : Ox0D - 13 DEC
CF : OxOA - 10 DEC

In use as a line break in a text file:

- Windows uses the 2 char sequence: CR LF
- Unix uses a single char: LF
- MacIntosh uses a 1 char: CR 

