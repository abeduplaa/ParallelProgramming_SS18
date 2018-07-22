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
MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD , &rank);
MPI_Comm_size(MPI_COMM_WORLD , &size);
MPI_Recv(message , SIZE, MPI_INT , (rank+size -1)%size,
0, MPI_COMM_WORLD , MPI_STATUS_IGNORE);
MPI_Send(message , SIZE, MPI_INT , (rank+1)%size,
0, MPI_COMM_WORLD);
MPI_Finalize();
return 0;
}
