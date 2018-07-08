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
    //int count = 2;
    int countLast = count + (strlen % np);

    char* str1 = (char*)malloc(countLast*sizeof(char));
    char* str2 = (char*)malloc(count*sizeof(char));
    //char str1[countLast];
    if (rank==0)
    {
      for (int i = 0; i < np; i++)
      {

        if (i == np-1)
        {
        //Send last chunk to last rank:
        MPI_Send(&str[count*i],countLast,MPI_CHAR,i,2,MPI_COMM_WORLD);
      }else
      {
        //Send to other ranks:
        MPI_Send(&str[count*i],count,MPI_CHAR,i,1,MPI_COMM_WORLD);
      }

      }
    }

    if (rank==np-1)
    {
      MPI_Recv(str1, countLast, MPI_CHAR, 0, 2,MPI_COMM_WORLD,&status);
      reverse_str(str1, countLast);
      MPI_Send(str1,countLast,MPI_CHAR,0,3,MPI_COMM_WORLD);
      //printf("Sent! \n");

    }else
    {
      MPI_Recv(str2, count, MPI_CHAR, 0, 1,MPI_COMM_WORLD,&status);
      reverse_str(str2, count);
      MPI_Send(str2,count,MPI_CHAR,0,4,MPI_COMM_WORLD);


    }

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
