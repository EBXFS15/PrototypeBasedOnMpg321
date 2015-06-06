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



Textfile .ebxfs15_mp3player_config contains the configuration. Please see example in the repository or the one below.

        [Playlist1]
        name=Huber
        image=/home/huber/picture/mypicture.jpg
        path1=/home/huber/music/mysong.mp3
        path2=/home/huber/video/mysong2.mp3
        path3=/usr/local/music/anothersong.wav

        [Playlist2]
        name=Sandmann
        image=/media/sandmann/picture/velo.jpg
        path1=/media/sandmann/mp3/ifeelgood.mp3
