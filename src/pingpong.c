#include <stdio.h>
#include "mpi.h"

#define ONE_MIL      (1000000)
#define MAX_SIZE_LOG (21)
#define MAX_SIZE     (1<<MAX_SIZE_LOG)

char sdata[MAX_SIZE];
char rdata[MAX_SIZE];

int main( int argc, char **argv )
{
    int              rank, i, size, j, next, prev;
    long             rounds, msglen;
    MPI_Status       status;
    double           startt, endt;
    char             buf[128];

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    if (rank == 0) {
        printf("STARTING LATENCY AND BANDWIDTH BENCHMARK\n");
        printf("MSG Length\t Latency \t Bandwidth\n");
        fflush(stdout);
    }

    next = (rank + 1 ) % size;
    prev = (rank + size - 1 ) % size;
    msglen = 0;

    while (msglen <= MAX_SIZE) {

        if (msglen <= (1<<9))       rounds = 1 << 15;
        else if (msglen <= (1<<12)) rounds = 1 << 12;
        else if (msglen <= (1<<18)) rounds = 1 << 10;
        else                        rounds = 1 << 8;

        MPI_Barrier ( MPI_COMM_WORLD );

        startt = MPI_Wtime();

        for (j=rounds; j--; ) {
            if (rank == 0) {
                MPI_Send(sdata,msglen,MPI_BYTE,next,0,MPI_COMM_WORLD);
                MPI_Recv(rdata,msglen,MPI_BYTE,prev,0,MPI_COMM_WORLD,&status);
            } else {
                MPI_Recv(rdata,msglen,MPI_BYTE,prev,0,MPI_COMM_WORLD,&status);
                MPI_Send(sdata,msglen,MPI_BYTE,next,0,MPI_COMM_WORLD);
            }
        }
        if (rank == 0) {
            double latency,bandwidth, elapse;

            endt = MPI_Wtime();
            elapse = endt - startt;

            latency   = elapse / (double)(size*rounds) * (double)ONE_MIL;
            bandwidth = ((double)(size*rounds*msglen))/elapse/(double)ONE_MIL;
            printf("%3d \t %8.3f \t %8.3f\n",(int)msglen,latency,bandwidth);
            fflush(stdout);
        }
        if (msglen == 0) msglen = 1;
        else msglen = 2 * msglen;
    }
    MPI_Finalize();
    return 0;
}