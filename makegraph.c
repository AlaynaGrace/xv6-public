#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"
#include "fcntl.h"

// void spin(){
//   int i = 0;
//   int j = 0;
//   int k = 0;
//   for(i = 0; i < 50; ++i){
//         for(j = 0; j < 400000; ++j){
// 	        k = j % 10;
// 	        k = k + 1;
// 	    }
//     }
// }

int main(int argc, char *argv[]){
  int numtickets[]={20,10,30};
  int pid_chds[3];

  pid_chds[0]=getpid();
  settickets(numtickets[0]);

  int i;
  for(i=1;i<3;i++){
    pid_chds[i]=fork();
    while(pid_chds[i]==0){
        sleep(100);
    //   for (;;){
	//     spin();
    //   }
    }
    settickets(numtickets[i]);
  }
    
  struct pstat st;
  int time=0;
  int hticks[3]={0,0,0};
  int lticks[3]={0,0,0};

  while(time<50){
    if(getpinfo(&st)!=0){
      goto Cleanup;
    }
    
    int j;
    int pid;
    for(i=0;i<3;i++){
      pid=pid_chds[i];
      for(j=0;j<NPROC;j++){
	    if(st.pid[j]==pid){
          	hticks[i]=st.hticks[j];
            lticks[i]=st.lticks[j];
	    }
      }
    }

   
   for(i=0;i<3;i++){
      printf(1,"High: %d, ",hticks[i]);
      printf(1,"Low: %d, ",lticks[i]);
    }
    printf(1,"\n");
    // spin();
    sleep(100);
    time++;
  }

  Cleanup: 
  for (i = 0; pid_chds[i] > 0; i++){
    kill(pid_chds[i]);
  }
  while(wait() > -1);

  exit();
}