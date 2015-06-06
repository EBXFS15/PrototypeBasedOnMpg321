This is a qt based user interface to play back audio files with the mplayer.

System requirements:
- Default EBX distribution as described in the Lab-1.1_Embedded_Linux_Configuration.pdf

To build on the development system run following commands:
    $ /opt/embedded/bbb/rootfs/usr/local/qt-5.3/bin/qmake
    $ make

Installation:
    $ sudo cp ./mp3player /opt/embedded/bbb/rootfs/usr/local/bin/mp3player
    $ sudo cp ./.ebxfs15_mp3player_config /opt/embedded/bbb/rootfs/root/.ebxfs15_mp3player_config

Start application:
    $ mp3player



Textfile ${HOME}/.ebxfs15_mp3player_config contains the configuration. Please see example in the repository or the one below.

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
    name=Cedric
    image=/media/mp3/bonjovi.jpg
    rfid=04C5E13A5C2B80
    path1=/usr/local/music/Guns.wav


The rfid reader is a MILFARE OMNIKEY(R) 5513 Reader Board Compact.
http://www.rfid-webshop.com/shop/download/Reader/HF%2013.56%20MHz/ACG/ISO%2014443/TAGnology_Datasheet_OK5513_RB_Compact_V1_0_0502.pdf
    The reader is preconfigured to send a string with the newly detected tag id through RS232.
    The application:
        - listens on the port /dev/ttyO1.
        - listens in ablocking manner to avoid polling.

