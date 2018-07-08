#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <pthread.h>
#include "mandelbrot_set.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//Shared resources:
int i_min = 0;
int i_max = 0;

struct pthread_args
{
	int max_iter;
	int palette_shift;
	int blocksize;
	int leftover;
	int j_max;
	int j_min;
	double y_stepsize;
	double view_y1;
	double view_x0;
	double x_stepsize;
	int resolution;
	void *imag;
	int id;
};


void* kernel(void * args){
	//initialize:
	struct pthread_args *arg = (struct pthread_args*) args;
	unsigned char (*img)[arg->j_max][3] = arg->imag;

	int li_min = 0;
	int li_max = 0;
	double y;
	double x;
	pthread_mutex_lock(&mutex);
	i_min = 0;
	i_max = 0;
	pthread_mutex_unlock(&mutex);


	int flag = 1;

	complex double Z;
	complex double C;

	int k;

	while (flag != 0 ){

		pthread_mutex_lock(&mutex);
		i_min = i_max;
		li_min = i_min;

		if ((arg->resolution - i_max) < arg->blocksize){
			i_max = arg->resolution;
		}else{
			i_max += arg->blocksize;
		}

		li_max = i_max;

		pthread_mutex_unlock(&mutex);

		if (li_min == arg->resolution)
		{
			flag = 0;
			break;
			return NULL;
		}else
		{
			printf("%d ", arg->id);
			printf("%d ", li_min);
			printf("%d \n", li_max);

			for (int i = li_min; i < li_max; i++)
			{
				for (int j = arg->j_min; j <  arg-> j_max; j++)
				{
					y = arg->view_y1 - i * arg->y_stepsize;
					x = arg->view_x0 + j * arg->x_stepsize;

					Z = 0 + 0 * I;
					C = x + y * I;

					k = 0;

					do
					{
						Z = Z * Z + C;
						k++;
					} while (cabs(Z) < 2 && k <  arg->max_iter);

					if (k == arg->max_iter)
					{
						memcpy(img[i][j], "\0\0\0", 3);
					}
					else
					{
						int index = (k + arg->palette_shift)
							    % (sizeof(colors) / sizeof(colors[0]));
						memcpy(img[i][j], colors[index], 3);
					}
				}
			}
		}
	}
	return NULL;
}

void mandelbrot_draw(int x_resolution, int y_resolution, int max_iter,
			                double view_x0, double view_x1, double view_y0, double view_y1,
						                double x_stepsize, double y_stepsize,
									                int palette_shift, unsigned char (*image)[x_resolution][3],
																	 int num_threads) {
	//Initialize:

	//Create array of threads:
	pthread_t *threads = (pthread_t*) malloc (num_threads*sizeof(pthread_t));
	//int pthread_mutex_init(pthread_mutex_t *mutex);
	//Create struct to pass to threads:
	struct pthread_args* args = (struct pthread_args*) calloc(num_threads,sizeof(struct pthread_args));

	int blocksize;
	int leftover;

	//determine number of large loops of image each thread gets:
	//yblocksize = y_resolution / num_threads;
	blocksize = 4;

	for(int i = 0 ; i<num_threads; i++){

		args[i].max_iter = max_iter;
		args[i].palette_shift = palette_shift;
		args[i].j_min = 0;
		args[i].j_max = x_resolution;
		args[i].x_stepsize = x_stepsize;
		args[i].y_stepsize = y_stepsize;
		args[i].view_x0 = view_x0;
		args[i].view_y1 = view_y1;
		args[i].blocksize = blocksize;
		args[i].resolution = y_resolution;
		args[i].imag = image;
		args[i].id = i;

		pthread_create(&threads[i],NULL,kernel, args+i);

	}

	//	sleep(10);

	for (int i = 0 ; i < num_threads; i++){
			pthread_join(threads[i], NULL);
	}

	free(threads);
	free(args);
}
