#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>//for memcpy()
#include <unistd.h>//for write()

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <iostream>

#define LOGBUFSIZE (2)
#define BUF_SIZE  (LOGBUFSIZE * 3) //save 20 data

float gBuf[BUF_SIZE];
unsigned int  gIndex = 0;

float *rp, *wp;
int r_count, w_count;

int co_count;

void put_log(float *data)
{
  if(w_count == 0) wp = gBuf;
  //memcpy(gBuf,data,sizeof(float)*LOGBUFSIZE);
  memcpy(wp,data,sizeof(float)*LOGBUFSIZE);

  wp+=LOGBUFSIZE;//?!
  w_count++;
  if(w_count == BUF_SIZE/LOGBUFSIZE) w_count=0;
}

void save_log(void)
{
  int i;
  int fdf;

  char file[50];
  sprintf(file,"data/test%d.dat",co_count);

  if((fdf=open(file,O_WRONLY|O_CREAT|O_TRUNC, 0755))==-1){
    perror("cantopen\n");
    exit(EXIT_FAILURE);
  }

  if(r_count == 0) rp = gBuf;

  write(fdf,rp,sizeof(float)*LOGBUFSIZE);

//  printf("saved %d on %s\n",co_count,file);
  rp+=LOGBUFSIZE;
  r_count++;
  co_count++;

  if(r_count == BUF_SIZE/LOGBUFSIZE) r_count = 0;

  close(fdf);
}

int main(int argc, char *argv[])
{
  int i;
  float data[3];
  float dat[2];
  w_count =0;
  r_count=0;

  for (i=0;i<3;i++) {
    dat[0] = (float)(2*i+2);
    dat[1] = (float)(i+2);
    printf("data%d:%f-%f\n",i,dat[0],dat[1]);
    put_log(dat);
    save_log();
  }
  float new_data;

  for (i=0;i<3;i++) {
    char file[50];
    sprintf(file,"data/test%d.dat",i);
    int fd = open(file, O_RDONLY);
    read(fd,&new_data,sizeof(float));
    printf("read %d:%f from %s\n",i,new_data, file);
    close(fd);
  }

}

