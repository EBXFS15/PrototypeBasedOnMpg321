This is a QT based user interface to play back audio files with the mplayer.

System requirements:
- Default EBXFS2015 distribution as described in the Lab-1.1_Embedded_Linux_Configuration.pdf
- Beaglebone Black with the Chipsee 7 inch capacitive touch screen cape.
- Optional: RF id reader MILFARE OMNIKEY(R) 5513 Reader Board Compact for basic playlist control through UART.
  Check below for an explanation how to emulate the RF id reader.

To build on the development system follow this procedure (may also be done with QT):
    $ git clone https://github.com/EBXFS15/musicplayer.git
    $ /opt/embedded/bbb/rootfs/usr/local/qt-5.3/bin/qmake
    $ make

Modification of the default EBX build:
    Edit uEnv.txt in the boot partition of the SD cart:
    - Change line 3 as follows (redirection of debug console to avoid any conflict):
      old: serial_console=ttyO1,115200n8
      new: serial_console=ttyO2,115200n8
    - Boot with the new uEnv.txt

Installation on EBX beaglebone:
    $ sudo cp ./musicplayer /opt/embedded/bbb/rootfs/usr/local/bin/musicplayer

Configuration file
  Create initial configuration file for root user on EBX beaglebone:
    $ sudo cp ./.ebxfs15_musicplayer_config /opt/embedded/bbb/rootfs/root/.ebxfs15_musicplayer_config

  And/or create initial configuration file for debian user on EBX beaglebone:
    $ sudo cp ./.ebxfs15_musicplayer_config /opt/embedded/bbb/rootfs/home/debian/.ebxfs15_musicplayer_config

  And/or create initial configuration file for any user on EBX beaglebone:
    $ sudo cp ./.ebxfs15_musicplayer_config /opt/embedded/bbb/rootfs/home/>>useraccount-to-be-defined-here<</.ebxfs15_musicplayer_config

  The file ${HOME}/.ebxfs15_musicplayer_config contains the configuration. Please see example in the repository or the one below.
    # This example playlist lists files available in the default distribution EBX FS 2015 (see Lab-1.1_Embedded_Linux_Configuration.pdf).

    [PlayList1]
    name=Default
    image=/media/mp3/bruceSpring.jpg
    rfid=04CF813A5C2B80
    path1=/usr/local/music/Guns.wav
    path2=/usr/local/music/John_Lee_Hooker_and_Bonnie_Raitt_Im_In_The_Mood.wav
    path3=/usr/local/music/BruceSpringsteen_JonBonJovi.mp3

    [PlayList2]
    name=Michel
    image=/media/mp3/bonjovi.jpg
    rfid=04CA293A5C2B80
    path1=/usr/local/music/BruceSpringsteen_JonBonJovi.mp3

    [PlayList3]
    name=Damien
    image=/media/mp3/bonjovi.jpg
    rfid=04C5E13A5C2B81
    path1=/usr/local/music/Guns.wav

    [PlayList4]
    name=Cedric
    image=/media/mp3/bonjovi.jpg
    rfid=04C5E13A5C2B80
    path1=/usr/local/music/Guns.wav


RFID
    Used RFID reader is a MILFARE OMNIKEY(R) 5513 Reader Board Compact.
    http://www.rfid-webshop.com/shop/download/Reader/HF%2013.56%20MHz/ACG/ISO%2014443/TAGnology_Datasheet_OK5513_RB_Compact_V1_0_0502.pdf

    The reader is preconfigured to send only a string with the detected tag id through RS232.

    Minimum hardware connection (Use P10 instead of the green header to avoid loose connection):
        - RFID reader RS232-TX connected to development board RS232-RX
        - GND

    The application:
        - listens on the port /dev/ttyO1.
        - Speed 9600, 8N1, No HW/SW flowcontrol(Default setting RFID reader)
        - listens in a blocking manner to avoid polling.
        - does not write anything through RS232.

    For testing purpose you may send the tag-id from the development PC to the development board:
        Easy way is to use minicom with the settings shown above.

        As alternative you can use the shell to send the TAG-ID with echo > /dev/ttyUSB0:
        Configuration of output port:
        $ stty -F /dev/ttyUSB0 9600 cs8 -parenb -cstopb

        Send TAG-ID:
        $ echo TAG-ID > /dev/ttyUSB0

        Example:
        $ echo 04CA293A5C2B80 > /dev/ttyUSB0


How to start the application:
    $ musicplayer
