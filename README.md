# File-Cypher

The goal of this software was to make the implemenation of a program that can encrypt and decrypt a file
This software uses AES-256 in CBC mode.

## Build

In the cypher folder, there is the Makefile to build the project.
$ cd cypher
$ make

## Utilization

Example :
- Encrytion   --> protect -c -p password -i clear_file.txt -o encrypted_file.bin
- Decryption --> protect -d -p password -i encrypted_file.bin -o clear_file.txt

Display help : cypher -h
