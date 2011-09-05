dvdspeedcontrol: set the speed of DVD-drives
============================================

Set the rotation speed of DVD (and possibly CDrom) drives to a fixed
value, for example when watching a movie and the drive likes to spin
faster and hence make more noise than necessary, or when copying data
from a disc that's not properly balanced anymore and causes excessive
vibrations with the default speed.

Note: speed settings persist only for the currently inserted disc (at
least with my hardware).

This tool for Linux (tested on Debian) has originally been written by
Thomas Fritzsche, http://noto.de, and I found it through some forum
posts, some of which with (modified) copies. I've collected this
information here (see Git history), and have renamed the tool from
speedcontrol to dvdspeedcontrol to make it sufficient to remember
"dvd" then hit the tab key to find it through autocompletion; also,
the original program description already explicitely said it is for
DVD-drives.

In the meantime, Thomas Fritzsche has moved the code to Github too[2],
and I have forked his repo and merged it with my changes.

NOTE: hdparm now has an -E option that should be doing the same thing.

Christian Jaeger

[1] https://github.com/pflanze/speedcontrol
[2] https://github.com/fritzsche/speedcontrol


INSTALLATION
------------

Just run "make install" (edit the Makefile to change the
target).

