#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"
#include "fcntl.h"

int main(int argc, char *argv[]){
  int numberOfTickets[]={30,15,10};
  int numberOfPids = 3;
  int pids[numberOfPids];

  pids[0]=getpid();
  settickets(numberOfTickets[0]);

  for(int i=1;i<numberOfPids;i++){
    pids[i]=fork();
    while(pids[i]==0){
        sleep(100);
    }
    settickets(numberOfTickets[i]);
  }
    
  struct pstat ps;
  int time=0;
  int hticks[3]={0,0,0};
  int lticks[3]={0,0,0};

  while(time<50){
    if(getpinfo(&ps)!=0){
        for (int i = 0; pids[i] > 0; i++){
            kill(pids[i]);
        }
        while(wait() > -1);

        exit();
    }
    
    int j;
    int pid;
    for(int i=0;i<numberOfPids;i++){
      pid=pids[i];
      for(j=0;j<NPROC;j++){
	    if(ps.pid[j]==pid){
          	hticks[i]=ps.hticks[j];
            lticks[i]=ps.lticks[j];
	    }
      }
    }

   for(int i=0;i<numberOfPids;i++){
      printf(1,"High: %d, \n",hticks[i]);
      printf(1,"Low: %d, \n",lticks[i]);
    }
    sleep(100);
    time++;
  }
}
