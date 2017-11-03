// A simple program to illustrate the use of mmap to map a file into memory.  Our
// target file (memmap.dat) is smaller than the pagesize (which is the minimum
// amount of data that we can map) so we get a memory region padded with a bunch of 0s.
// Note that we have an astounding lack of error checking in this program.  Wouldn't hurt 
// to add some to the code later on.
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

// Declare the variables.  databuff is a char buffer which will hold the contents of the file.
int fd;
int pagesize;
char * databuff;

int main (int argc, char * argv[]) {
  
  // Open the file to get a file descriptor
  fd = open ("memmap.dat", O_RDWR);

  // Now get the page size for this OS ... I bet it is 4K
  pagesize = getpagesize();
  printf ("Page size: %d\n", pagesize);

  // Finally ... it's time to map the file into memory. NULL is the lazy man's
  // way of specifying the address we want to use to hold our data. But it serves 
  // the purpose of allowing the OS to choose the address for the file memory mapping. 
  // We will map 1 page (4096 bytes) of data into our memory region.  And we will start at
  // the offset of 0.  The PROTection flag must match the file descriptor permission.  
  // And MAP_SHARED makes the changes to the file available to other processes 
  // (just like shared memory). 
  databuff = mmap(NULL, pagesize, PROT_WRITE, MAP_SHARED, fd, 0);

  // To prove that we actually mapped the file into memory, let's use the data buffer as an
  // array and print some stuff from it
  printf ("Data at offset 5: %c\n", databuff[5]);

  // Now let's change the data in the array and see if it actually persists.  Run the program
  // and then check the memmap.dat file to make sure it changed!
  databuff[5] = 'Y';
  printf ("Data at offset 5 is now:  %c\n", databuff[5]);

  // Our target file is pretty small so the mmap system call should pad the memory region
  // with 0x0s.  Let's check and see.
  printf ("Data at offset 1024 should be 0x0:  %x\n", databuff[1024]);

  // Yep, it all works.  Man this is a lot easier than the standard method of I/O with
  // read and write system calls.  When the program exits it writes the databuff buffer
  // back to the file.  Pretty neat.  There's a lot more you can do with the other mmap
  // system calls (unmap, sync the file, etc).  But this is a good start.

  return 0;
}
