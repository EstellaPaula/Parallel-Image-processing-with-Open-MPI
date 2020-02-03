Image processing using Open MPI


Filters will be applied depending on the number of processes:
  - one process: the "root" process will read the binary file received as
    input and will apply the respective matrix to the received filters as an argument
    for each pixel; the "root" process will write the resulting image to
    output file;

  - for several processes: the "root" process will read the received binary file
    as an input and will send the pixel array to all processes; every
    process (including the "root" process) will apply the respective matrix to the filters
    received as an argument on the pixels of a number N of lines in the matrix of
    pixels:
N = number of lines / number of processes
    if the partition is not exact, the "root" process will handle
    the rest of the lines; the "root" process will receive the rest of the image from the others
    processes and writes the resulting image to the output file;

Computer specifications:

Architecture: x86_64
CPU op-mode (s): 32-bit, 64-bit
Byte Order: Little Endian
CPU (s): 4
Online CPU (s) list: 0-3
Thread (s) per core: 2
Core (s) per socket: 2
Socket (s): 1
(Linux subsystem)

Execution time (s):

Image: landscape.pgm (325 KB)
no. process -> 1 2 3 4 5 6 7 8
applied 
filters 
\ /
smooth 0.875 0.76 0.698 0.656 0.535 0.512 0.486 0.442

bssembssem 0.918 0.854 0.767 0.709 0.638 0.595 0.5058 0.472

-------------------------------------------------- ---------------------------------
Image: rorschach.pgm (7526 KB)
no. process -> 1 2 3 4 5 6 7 8
applied 
filters 
\ /
smooth 1,741 1,680 1,634 1,616 1,442 1,141 1,373 1,279

bssembssem 4.733 4.268 3.97 3.622 3.48 3.370 2.977 2.696

-------------------------------------------------- -------------------------------
Image: scroll-of-truth.pnm (756 KB)
no. process -> 1 2 3 4 5 6 7 8
applied 
filters 
\ /
smooth 0.832 0.779 0.697 0.664 0.613 0.542 0.527 0.451

bssembssem 1,227 1,088 0.972 0.917 0.811 0.775 0.691 0.622

-------------------------------------------------- --------------------------------
Image: landscape.pnm (24301 KB)

smooth 4.449 4.297 4.226 4.145 4.011 3.901 3.481 3.34

bssembssem 12.93 12.44 12.011 12.004 11.705 11.683 11.339 10.801
