#!/bin/sh
# This script will help you flash the Foenix from linux. Tested for A2560U only.
python3 C256Mgr/c256mgr.py --flash src/foenixmcp.bin --address 0x00000
