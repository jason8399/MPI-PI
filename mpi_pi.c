#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(void) {
	int		comm_sz;
	int		my_rank;

	long long int number_of_tosses, number_in_circle, total_circle;
	int i;
	double pi, x, y;

	srand(time(NULL));

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	if (my_rank == 0){
		printf("Number of tosses: ");
		scanf("%lld", &number_of_tosses);
	}

	MPI_Bcast(&number_of_tosses, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

	number_in_circle = 0;
	for(i = 0; i < number_of_tosses; i++){
		x = (double) rand() / (double) (RAND_MAX / 2.0) - 1.0;
		y = (double) rand() / (double) (RAND_MAX / 2.0) - 1.0;
		if(x * x + y * y <= 1)
			number_in_circle++;
	}

	MPI_Reduce(&number_in_circle, &total_circle, 1, MPI_LONG_LONG_INT,
			MPI_SUM, 0, MPI_COMM_WORLD);

	if(my_rank == 0){
		pi = 4.0 * (double)total_circle /
			((double)number_of_tosses * comm_sz);
		printf("PI = %lf\n", pi);
	}

	MPI_Finalize();

	return 0;
}
