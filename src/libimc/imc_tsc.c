#include <stdio.h> 

#define RDTSC(x) __asm__ __volatile__ ("rdtsc":"=A"(x))

int main(void) 
{ 
  unsigned long long start, end; 
  unsigned msec = 1;
  unsigned i;
  for (i=1; i<=200; i++)
  {
    RDTSC(start); 
    usleep(msec * 1000);
    RDTSC(end);
    fprintf(stderr,"1 msec=%llu tscs\n", (end - start)/msec);
  }
  return 0; 
}
