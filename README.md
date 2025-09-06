# b2c / bytes to colors
Raw byte data to image converter/visualizer.
<img width="968" height="968" alt="thing" src="https://github.com/user-attachments/assets/847f882f-da7c-4292-8b25-8a6c85ab21e3" />
Image shows a Windows executable ^^^

Made with raylib.

## What it does
It takes raw byte data through standard input, whethere typed it manually or piped in, assigned each byte to a red, green or blue values (alpha channel is permanently at 255), those become pixels in an array which becomes an image written to the disk.

## How to use it
stdin.

Flags:

-v --verbose : for some printfs

-d --display --draw : for displaying the image in a window instead of saving it to a file

-w --width : change the preffered width of the window (only has any effect if also doing -d/--display/--draw

-r --reverse : Take an image as input, and spit out a file as output. The reverse of the program's default function.

## To do
- Add option to memcmp 2 files and show the differences between them (if any) as an rgb8 image.
- [DONE] Add option to reverse the effect of the program by taking in an image as the input and outputting a file.

## Known issues
- [FIXED] If started with `-d`, the program will not listen to kill signals, and must be killed with ^C.
- [FIXED] If started with `-d`, and processing a very big file, funky visual bugs happen.

# Building it
Build file exists only for GNU/Linux, and the code may use libraries unavailable on Windows.
Needs Raylib <3
