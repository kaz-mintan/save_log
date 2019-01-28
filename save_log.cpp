#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>//for memcpy()
#include <unistd.h>//for write()

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LOGBUFSIZE (10)
#define BUF_SIZE  (LOGBUFSIZE * 2) //save 20 data

double gBuf[BUF_SIZE];
unsigned int  gIndex = 0;

double *rp, *wp;
int r_count, w_count;

void put_log(double *data)
{
  if(w_count == 0) wp = gBuf;
  memcpy(gBuf,data,sizeof(double)*LOGBUFSIZE);
  wp+=LOGBUFSIZE;//?!
  w_count++;
  if(w_count == BUF_SIZE/LOGBUFSIZE) w_count=0;
}

void save_log(void)
{
  int i;
  int fdf;

  if((fdf=open("test.dat",O_WRONLY|O_CREAT|O_TRUNC, 0755))==-1){
    perror("cantopen\n");
    exit(EXIT_FAILURE);
  }

  if(r_count == 0) rp = gBuf;
  write(fdf,rp,sizeof(double)*LOGBUFSIZE);
  close(fdf);
  printf("saved?\n");
  rp+=LOGBUFSIZE;
  r_count++;
  if(r_count == BUF_SIZE/LOGBUFSIZE) r_count = 0;
}

int main(int argc, char *argv[])
{
  int i;
  double data[LOGBUFSIZE];

  for (i=0;i<LOGBUFSIZE;i++) {
    data[i] = (double)i;
  }
  put_log(data);
  save_log();
}

