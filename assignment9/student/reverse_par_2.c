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
    MPI_Status status;

    int count = strlen / np;
    int countLast = count + (strlen % np);


    if (rank == np - 1) {
        int substrlen = count;
        char* str1 = (char*)malloc(substrlen*sizeof(char));

    } else {
        int substrlen = countLast;
        char* str1 = (char*)malloc(substrlen*sizeof(char));

    }

    if (rank==0)
    {
      for (int i = 0; i < np; i++)
      {
        //Send to ranks:
        MPI_Send(&str[count*i],count,MPI_CHAR,i,1,MPI_COMM_WORLD);

        if (i == np-1)
        {
        //Send last chunk to last rank:
        MPI_Send(&str[count*i],countLast,MPI_CHAR,i,1,MPI_COMM_WORLD);
        }

      }
    }

      MPI_Recv(str1, substrlen, MPI_CHAR, 0, 1,MPI_COMM_WORLD,&status);
      reverse_str(str1, strlen);
      MPI_Send(str1,substrlen,MPI_CHAR,0,3,MPI_COMM_WORLD);

    if (rank == 0)
    {
      int j = 0;

      for (int i=np-1; i > -1; i--)
      {
        //Rank 0 receives 0 in reverse order
        if (i==np-1)
        {
          MPI_Recv(&str[0], countLast, MPI_CHAR, i, 3,MPI_COMM_WORLD,&status);
        }else
        {
          MPI_Recv(&str[countLast+ count*j], count, MPI_CHAR, i, 4,MPI_COMM_WORLD,&status);
          j = j + 1;
        }
      }
    }

free(str1);
}
