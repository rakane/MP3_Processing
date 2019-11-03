# MP3 Processing Library (in progress)
### Ryan Kane

The goal of this library is to input MP3 files, and be able to apply audio filters, and resave MP3 file

## Functionality
1. Read and Write Audio files byte by byte (done)
2. Process first frame header and data (done)
3. Process frame header and data for entire file
4. From frame struct, write MP3 file back
5. Apply audio filters to data

## Project Structure
.<br>
|---- io&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# File IO functions <br>
|---- processing&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# Audio processing from bytes to struct<br>
|---- samples &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# Sample MP3 songs<br>
|---- test &nbsp;&nbsp;&nbsp;# Test Suite functions<br>
|---- Makefile &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# Build file<br>
|---- main.c &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# Main driver<br>
|---- header.h  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# Header file<br>

