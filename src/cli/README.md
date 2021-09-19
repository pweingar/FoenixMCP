# Folder: CLI -- Command Line Interpreter

This folder contains the code for a simple command line interpreter (CLI) for Foenix/MCP.
This CLI supports several built-in commands tailored to the Foenix computers, as well as
running executable binary files that are supported by the operating system.

## Paths

Drives, files, and directories are specified using paths.
A path starts with an optional drive specification, which starts with an "@" character and ends with a ":". Some drive specifications:

* ``@S:`` -- The SD card slot.
* ``@F:`` -- The floppy drive (if installed).
* ``@H:`` -- The first partition of the IDE hard drive (if installed).

After the drive specification, comes the directories, separated by ":".
After the directories comes the file name, which is a name followed by a dot,
followed by the file extension.

Example paths:

* ``@S:Project Files:Hello.txt`` -- the file "Hello.txt" in the "Project Files" directory in the SD card.
* ``@H:STARTUP.SYS`` -- The "STARTUP.SYS" file in the root directory of the hard drive.
* ``:Samples:PrettyPicture.PGZ`` -- The "PrettyPicture.PGZ" file in the "Samples" directory of the current drive.
* ``game.pgz`` -- The "game.pgz" file in the current drive and directory.

## External Commands

If the first "word" entered on a line is not recognized as a built-in command, the CLI
will look to see if there is an executable file in the search path corresponding to that word.
To match, the word must be the same as the file name (case does not matter), and the extension
of the file must be that of an executable file format supported by the Foenix/MCP.

If a matching executable is found, it will be loaded and executed.

**How exactly this is to be done is TBD.**

## Built-in Commands

Several commands accept numbers as arguments.
All numbers may be specified using decimal, binary, or hexidecimal formats.
Hexidecimal numbers may be specified either with a leading "$" or a leading "0x".
Binary numbers may be specified using either "%" or "0b" as a prefix.

* ``1234`` -- Decimal 1234
* ``0`` -- Decimal 0
* ``0xff`` -- Hexidecimal 255
* ``$000A`` -- Hexidecimal 10
* ``%111`` -- Binary 7
* ``0b1010`` -- Binary 10

### Storage Media Commands

* ``DIR <path>`` -- Print a directory listing for a given drive or directory.
* ``DEL <path>`` -- Delete a file.
* ``MKDIR <path>`` -- Create a directory.
* ``RMDIR <path>`` -- Delete a directory.
* ``RENAME <old path> <new name>`` -- Rename a file.
* ``COPY <src path> <dest path>`` -- Copy a file (possibly to another drive or folder).
* ``PWD`` -- Print the current default drive/directory.
* ``CD <path>`` -- Change the default drive/directory.

### Memory Commands

* ``DUMP <address> <count>`` -- Print out ``<count>`` bytes of memory starting at ``<address>``
* ``POKE8 <address> <value>`` -- Store the 8-bit ``<value>`` to memory at ``<address>``
* ``POKE16 <address> <value>`` -- Store the 16-bit ``<value>`` to memory at ``<address>``
* ``POKE32 <address> <value>`` -- Store the 32-bit ``<value>`` to memory at ``<address>``
* ``PEEK8 <address>`` -- Print the 8-bit value in memory at ``<address>``
* ``PEEK16 <address>`` -- Print the 8-bit value in memory at ``<address>``
* ``PEEK32 <address>`` -- Print the 8-bit value in memory at ``<address>``
* ``BLOAD <path> [<address>]`` -- Load a binary file into memory (optionally force an address)
* ``BSAVE <path> <start address> <size>`` -- Save a block of memory to a file

### Graphics Commands

All graphics commands take a screen number: 0 = Channel A, or the main screen; 1 = Channel B, or the EVID card (if present).

* ``TEXTCOLOR <screen> <foreground> <background>`` -- Set the text foreground and background colors using color numbers in the text color look up tables.
* ``SETBORDER <screen> <visible> [<size> [<r> <g> <b>]]`` -- Set the border visibility, size, and color. Visible is 0 for "no", any other number for "yes". ``<size>`` is in pixels. The colors are specified in RGB color components from 0 to 255.
* ``SETCOLOR <lut #> <color #> <r> <g> <b>`` -- Set a color in a look up table to a specific RGB color using components in the range 0 to 255.
* ``GRAPHICS <screen> [<mode>]`` -- Set the graphics mode (if no mode specified, go back to text only mode)
* ``CLS <screen>`` -- Clear the text screen.

## Possible Future Developments

* Environment Variables
* Simple scripting ability to be executed at boot time from the selected boot device
