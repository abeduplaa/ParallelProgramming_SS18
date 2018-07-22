#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "heat.h"
#include "helper.h"

#define SIZE 10



int main (int argc, char* argv[])
{
int rank, size;
int message[SIZE];
MPI_Init(&argc, &argv); /* starts MPI */
MPI_Comm_rank(MPI_COMM_WORLD , &rank); /* process id */
MPI_Comm_size(MPI_COMM_WORLD , &size); /* number processes */
MPI_Request req[2];
MPI_Isend(message , SIZE, MPI_INT , (rank+1)%size, 0, MPI_COMM_WORLD , &req[0]);
MPI_Irecv(message , SIZE, MPI_INT , (rank+size -1)%size, 0, MPI_COMM_WORLD , &req[1]);
MPI_Waitall(2, req, MPI_STATUS_IGNORE);
MPI_Finalize();
return 0;
}
