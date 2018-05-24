#!/bin/bash
make
./xor
gcc -o export_xor_c export_xor.c -lm
./export_xor_c zero zero
./export_xor_c one zero
./export_xor_c zero one
./export_xor_c one one
echo ""
chmod +x export_xor.py
./export_xor.py zero zero
./export_xor.py one zero
./export_xor.py zero one
./export_xor.py one one
exit
