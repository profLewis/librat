#include <stdio.h>

void systemStats(){
  char cmd[1024];
  FILE *fp;
  int i;
  sprintf(cmd,"ps -p %d -o rss",getpid());
  fp=popen(cmd,"r");
  for(i=0;i<2;i++)fgets(cmd,1024,fp); 
  pclose(fp);
  fprintf(stderr,"%s\n ",cmd);
}
main(){
  int i=0;
  while(i++<100){
    fprintf(stderr,"%d ",i);
    calloc(1024,sizeof(char));
    systemStats();
  }
}
