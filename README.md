<h1 align="center">
<br>
<img src="http://image.usbac.com.ve/nmly.png" alt="Namely logo" width="140">
<br>
Nmly
<br>
</h1>
<p align="center">
<img src="https://img.shields.io/badge/stability-stable-green.svg">
<img src="https://img.shields.io/badge/version-1.0.3-blue.svg">
<img src="https://img.shields.io/badge/license-MIT-orange.svg">
</p>

<h4 align="center">Nmly is a massive file renamer utility with useful functions and written in C.</h4>

Why use it? Because it's extremely easy, includes multiple useful features like preview the changes, filter files based on its extensions and size, an option to work recursively and much more.

And yes, it's obviously extremely fast. ;)

## Arguments

`after [text]` Add the text at the end of the filename

`before [text]` Add the text at the begining of the filename

`lower` All filename characters to lowercase

`remove [text]` Remove the specified text from the filename

`replace [ori] [new]` Replace a text with a new one

`reverse` Reverse the filename

`switch [sep]` Switch the filename order based in a separator

`upper` All filename characters to uppercase

## Options

`-d` `--directory [text]` The target directory

`-e` `--extension [text]` Filter files based on theirs extension

`-f` `--folders` Apply the changes to the folders name too

`-h` `--help` Get help and information

`-l` `--locale` Process special characters (like latin ones)

`-nv` `--no-verbose` No verbose mode (no files will be listed)

`-p` `--preview` Show the changes without applying them

`-r` `--recursive` Work recursively

`-s` `--size [size]` Filter files based on theirs size (+/-)(g/m/k)

`--split` Display the changes horizontally split

`-u` `--unmodifiable` Show non-modifiable files

`-v` `--version` Show the application version

## Install

Move to your Nmly folder and run the following command:

`make; sudo make install`

Now you should be able to access to Nmly running `nmly` in your terminal.

### Naming

You can specify the Nmly name when installing it with the NAME makefile variable:

`sudo make install NAME=renamer`

Now you will be able to access Nmly with the `renamer` command.

## Examples

```console
usbac@pc:~$ nmly switch -
Author - Song.mp3 > Song - Author.mp3
```
```console
usbac@pc:~$ nmly remove " 2017" -d vacations -e mp4
vacations/video 2017.mp4 > vacations/video.mp4
```
```console
usbac@pc:~$ nmly reverse -d folder -s +1g
folder/fileBiggerThan1GB.iso > folder/BG1nahTreggiBelif.iso
```
```console
usbac@pc:~$ nmly replace jpeg jpg
picture.jpeg > picture.jpg
```
```console
usbac@pc:~$ nmly after world -r
hello.pdf > helloworld.pdf
subfolder/file.txt > subfolder/fileworld.txt
```
```console
usbac@pc:~$ nmly upper -r --split
hello.pdf
HELLO.pdf

subfolder/file.txt
subfolder/FILE.txt
```
```console
usbac@pc:~$ nmly -u -d folder -r
folder/filewithpermissions.txt
```
