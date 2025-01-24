#include "gyre.h"

volatile sig_atomic_t done = 0;
volatile sig_atomic_t catcher = 0;


char hname[256];

void term(int signum)
{
   printf("SIGUSR1 caught\n");
   done = 1;
   catcher = 1;
}

int communicate(int done,int rank,int world,int *v){
    int rv = done;
//    int v[8];

    long q;
    q = fact(5);

    for(int i = 0; i < 8; i++)
    {
	if(done==1){
            v[i] = -1;
	}else{
            v[i] = 100 + rank * 800 + i * 100;
	}
    }
//    printf("Process %d, my values = %d, %d, %d, %d, %d, %d, %d, %d.\n", rank, v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]);
 
    int r[8];
    MPI_Alltoall(&v, 1, MPI_INT, r, 1, MPI_INT, MPI_COMM_WORLD);
//    printf("Values collected on process %d: %d, %d, %d %d, %d, %d, %d, %d.\n", rank, r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7]);

    for(int i = 0; i < 8; i++)
    {
      if(r[i]==-1){
	      rv = 1;
      }
    }
    return rv;
}

int main(int argc, char** argv) {

    gethostname(hname, 256);
    hname[255]=0;
    printf("hostname: %s ",hname);

    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = term;
    sigaction(SIGUSR1, &action, NULL);
    
    MPI_Init(NULL, NULL);
    int rank;
    int world;
    int got;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);

    int ok = prepif(7000+rank);

    if( ok != 0){
      printf("Couldn't open interface udp port.");
    }else{
      int *v = NULL;

      v = malloc(world * sizeof *v);
      if (v == NULL) {
        fprintf(stderr, "Out of memory!\n");
        exit(1);
      }

      for(int i = 0; i < world; ++i)
        v[i] = i;



      printf("start: pid %d rank %d, world: %d\n",getpid(), rank, world);fflush(stdout);
      for(int i=0;(done==0)&&(i<60);i++){
        MPI_Barrier(MPI_COMM_WORLD);
        done = communicate(0,rank,world,v);
	if(done==0){
	  done = checkif(hname);
	}
	if(done==0){
          sleep(1);
          if(rank==0){
            printf("%d: pid %d rank %d, world: %d\n",i,getpid(), rank, world);fflush(stdout);
          }
	}
      }
      if((done==1)&&(catcher==1)){
        MPI_Barrier(MPI_COMM_WORLD);
        communicate(done,rank,world,v);
      }
      MPI_Finalize();
    }
}
// end generated source
