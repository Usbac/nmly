<h1 align="center">Nmly</h1>

<p align="center">
<img src="https://img.shields.io/badge/version-0.9.6-blue.svg"> <img src="https://img.shields.io/badge/license-MIT-orange.svg">
</p>

<h4 align="center">Nmly is a massive file renamer utility with useful functions and written in C.</h4>

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

`-d` `--directory [text]` The directory where the changes will be applied

`-e` `--extension [text]` Apply the changes only to the files with that extension 

`-f` `--folders` Apply the changes to the folders name too

`-h` `--help` Get help and information about the application

`-l` `--locale` Accept special characters (like latin characters)

`-p` `--preview` Show the changes without applying them

`-r` `--recursive` Apply the changes recursively in the directory

`-u` `--unmodifiable` Show the files that cannot be modified

`-v` `--version` Show the application version

## Examples

```console
usbac@pc:~$ nmly switch - -d ./
Author - Song.mp3 > Song - Author.mp3
```
```console
usbac@pc:~$ nmly remove " 2017" -d ./vacations -e mp4
./vacations/video 2017.mp4 > ./vacations/video.mp4
```
```console
usbac@pc:~$ nmly replace jpeg jpg -d ./
picture.jpeg > picture.jpg
```
```console
usbac@pc:~$ nmly after world -d ./ -r
hello.pdf > helloworld.pdf
subfolder/file.txt > subfolder/fileworld.txt 
```
```console
usbac@pc:~$ nmly -u -d ./folder -r
./folder/filewithpermissions.txt
```
