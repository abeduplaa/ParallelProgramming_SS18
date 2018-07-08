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
        int substrlen;


    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   
  // Calculate the lenth of the substring for the "normal" case and the remaining last process
    int substrlen_normal = strlen / np;
    int substrlen_rem = substrlen_normal + strlen%np;

  if (rank == np - 1) {
    substrlen = substrlen_rem;
  } else {
    substrlen = substrlen_normal;
  }

  char* substr = (char*)malloc(substrlen_normal*sizeof(char));  

    if (rank == 0)
    {
	MPI_Send(str, strlen, MPI_CHAR, rank, 0, MPI_COMM_WORLD);
        //reverse_str(str, strlen);
    }
	
    else{
	//char* substr = (char*)malloc(substrlen*sizeof(char));  
	MPI_Recv(substr, substrlen, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
	reverse_str(substr, substrlen);	
    }

if (rank != 0) {
    // Send the result substring back to the master process
    MPI_Send(substr, substrlen, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  } else {
    // Receive the substr of the last process first
    char* buffer_substr = (char*)malloc(substrlen_rem);
    MPI_Recv(buffer_substr, substrlen_rem, MPI_CHAR, np-1, 0, MPI_COMM_WORLD, &status);
    
    int str_index = 0;
    for (int i = 0; i < substrlen_rem; i++) {
      str[str_index] = buffer_substr[i];
      str_index++;
    }
    free(buffer_substr);

    // Receive the substr of the other processes
    char* buffer_substr_normal = (char*)malloc(substrlen_normal);
    for (int p = np-2; p > 0; --p) {
      MPI_Recv(buffer_substr_normal, substrlen_normal, MPI_CHAR, p, 0, MPI_COMM_WORLD, &status);

      for (int i = 0; i < substrlen_normal; ++i) {
        str[str_index] = buffer_substr_normal[i];
        str_index++;
      }
    }
    free(buffer_substr_normal);
    // Add the master process' substring
    for (int i = 0; i < substrlen_normal; ++i) {
      str[str_index] = substr[i];
      str_index++;
    }
    
    
  }

free(substr);


}


