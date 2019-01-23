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

void put_log(double *data)
{
  wp = gBuf;
  memcpy(gBuf,data,sizeof(double)*LOGBUFSIZE);
  //wp+=LOGBUFSIZE;//?!
}

void set_str(char *str, double *data){
  int i =0;
  for(i =0;i<sizeof(data);i++){
  // sprintf(str,",")
  }
}

void save_log(void)
{
  int i;
  int fdf;

  if((fdf=open("test.dat",O_WRONLY|O_CREAT|O_TRUNC, 0755))==-1){
    perror("cantopen\n");
    exit(EXIT_FAILURE);
  }

  rp = gBuf;
  write(fdf,rp,sizeof(double)*LOGBUFSIZE);
  //write(fdf,rp,LOGBUFSIZE);
  close(fdf);
  printf("saved?\n");
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

