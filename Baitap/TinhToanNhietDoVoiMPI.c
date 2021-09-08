#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <mpi.h>
#include <time.h>

#define  M       20
#define  Time    1
#define  dt      0.01
#define  dx      0.1
#define  D       0.1
//=========================

int main(int argc, char **argv)
{
	int NP, ID, Mc;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &NP);
	MPI_Comm_rank(MPI_COMM_WORLD, &ID);
	MPI_Status stat;

	void KhoiTao(float *T)
	{
		int i;
		for (i = 0; i < M; i++)
			*(T + i) = 25.0;
	}

	int i, t, Ntime;
	float *T, *dT;
	Ntime = Time / dt;
	T = (float *)malloc((M) * sizeof(float));
	dT = (float *)malloc((M) * sizeof(float));

	if (ID == 0)
		KhoiTao(T);

	Mc = M / NP;
	Tc = (float *)malloc((Mc) * sizeof(float));
	dTc = (float *)malloc((Mc) * sizeof(float));

	// Gui thong tin
	MPI_Scatter(T, Mc, MPI_FLOAT, Tc, Mc, MPI_FLOAT, 0, MPI_COMM_WORLD);

	if (ID == 0)
	{
		Tl = 100;
		MPI_Send(Tc + Mc - 1, 1, MPI_FLOAT, ID + 1, ID, MPI_COMM_WORLD);
	}

	void DHB2(float *T, float *dT)
	{
		int i;
		float c, l, r;
		for (i = 0; i < M; i++)
		{
			c = *(T + i);
			l = (i == 0) ? 100.0 : *(T + (i - 1));
			r = (i == M - 1) ? 25.0 : *(T + (i - 1));
			*(dT + i) = D * (r - 2 * c + l) / (dx * dx);
		}
	}

	for (t = 0; t < Ntime; t++)
	{
		DHB2(T, dT);
		for (i = 0; i < M; i++)
			*(T + i) = *(T + i) + dt * (*(dT + i));
	}
	return 0;
}
