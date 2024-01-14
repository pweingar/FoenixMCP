#!/bin/sh
echo Converting $1 from VASM to Calypsi AS68k format
# Reminder about reg exs;
# \s* means "any number of white space"
# / must be escaped as \/
# $ means "end of line"
# ^ means "start of line"
# Hexa
sed -i 's/\$/0x/g' $1
# Constants definitions
sed -i 's/ =\s*0x/ .equ 0x/g' $1
sed -i 's/EQU\s/.equ /g' $1
# Comments
sed -i 's/; /\/\/ /g' $1
sed -i 's/;$/\/\//g' $1
sed -i 's/;;/\/\//g' $1
# Import / export / labels
sed -i 's/xdef _/xdef /g' $1
sed -i 's/xref _/xref /g' $1
sed -i 's/xdef/.global/g' $1
sed -i 's/xref/.extern/g' $1
sed -i 's/^_//g' $1
sed -i 's/[ ]_/ /g' $1
sed -i 's/\t_/ /g' $1
sed -i 's/#_/#/g' $1
# Constants
sed -i 's/dc.l/.long/g' $1
sed -i 's/dc.w/.word/g' $1
sed -i 's/dc.b/.byte/g' $1
# Macros
sed -i 's/\macro/.macro/g' $1
sed -i 's/\endm/.endm/g' $1
# Conditional
sed -i 's/^\s*ifne/#if 0 != /g' $1
sed -i 's/^\s*ifeq/#if 0 == /g' $1
sed -i 's/^\s*IF\s/#if /g' $1
sed -i 's/^\s*IFD\s/#if /g' $1
sed -i 's/^\s*ELSE/#else/g' $1
sed -i 's/^\s*ENDC/#endif/g' $1
#sections
sed -i 's/^\s*code/.section text/g' $1
# Hacks to restore stuff. TODO improve things so we don't need this !
sed -i 's/_CALYPSI_ASSEMBLER/__CALYPSI_ASSEMBLER/g' $1
