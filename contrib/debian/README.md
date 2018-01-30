
Debian
====================
This directory contains files used to package Zioncoind/Zioncoin-qt
for Debian-based Linux systems. If you compile Zioncoind/Zioncoin-qt yourself, there are some useful files here.

## Zioncoin: URI support ##


Zioncoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install Zioncoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your Zioncoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/Zioncoin128.png` to `/usr/share/pixmaps`

Zioncoin-qt.protocol (KDE)

