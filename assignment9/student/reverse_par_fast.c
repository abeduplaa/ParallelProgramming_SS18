#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "helper.h"

void reverse(char *str, int strlen)
{
        // parallelize this function and make sure to call reverse_str()
        // on each processor to reverse the substring.

    int np, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //MPI_Status status;

    int count = strlen / np;
    int countLast = count + (strlen % np);

    char* str1 = (char*)malloc(countLast*sizeof(char));

    if (rank==0)
    {
      MPI_Send(&str[count*(np-1)],countLast,MPI_CHAR,np-1,2,MPI_COMM_WORLD);
      for (int i = 0; i < np-1; i++)
      {
        MPI_Send(&str[count*i],count,MPI_CHAR,i,1,MPI_COMM_WORLD);
      }

    }

    if (rank!=np-1)
    {
      MPI_Recv(str1, count, MPI_CHAR, 0, 1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      reverse_str(str1, count);
      MPI_Send(str1,count,MPI_CHAR,0,4,MPI_COMM_WORLD);

    }else
    {

      MPI_Recv(str1, countLast, MPI_CHAR, 0, 2,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      reverse_str(str1, countLast);
      MPI_Send(str1,countLast,MPI_CHAR,0,3,MPI_COMM_WORLD);
    }

    if (rank == 0)
    {
      int j = 0;
      MPI_Recv(&str[0], countLast, MPI_CHAR, np-1, 3,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      for (int i=np-2; i>-1; i--)
      {
          MPI_Recv(&str[countLast + count*j], count, MPI_CHAR, i, 4,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
          //MPI_Recv(&str[strlen - count*(np-1-i)], countLast, MPI_CHAR, i, 4,MPI_COMM_WORLD,&status);
          j++;
      }
    }




    free(str1);
    //free(str2);
}
