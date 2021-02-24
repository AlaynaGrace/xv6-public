#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"
#include "fcntl.h"

int main(int argc, char *argv[]){
  int numberOfTickets[]={20,10,30};
  int pids[3];

  pids[0]=getpid();
  settickets(numberOfTickets[0]);

  int i;
  for(i=1;i<3;i++){
    pids[i]=fork();
    while(pids[i]==0){
        sleep(100);
    }
    settickets(numberOfTickets[i]);
  }
    
  struct pstat st;
  int time=0;
  int hticks[3]={0,0,0};
  int lticks[3]={0,0,0};

  while(time<50){
    if(getpinfo(&st)!=0){
        for (i = 0; pids[i] > 0; i++){
            kill(pids[i]);
        }
        while(wait() > -1);

        exit();
    }
    
    int j;
    int pid;
    for(i=0;i<3;i++){
      pid=pids[i];
      for(j=0;j<NPROC;j++){
	    if(st.pid[j]==pid){
          	hticks[i]=st.hticks[j];
            lticks[i]=st.lticks[j];
	    }
      }
    }

   for(i=0;i<3;i++){
      printf(1,"High: %d, \n",hticks[i]);
      printf(1,"Low: %d, \n",lticks[i]);
    }
    sleep(100);
    time++;
  }
}
