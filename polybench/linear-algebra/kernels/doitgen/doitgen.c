/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/* doitgen.c: this file is part of PolyBench/C */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

/* Include polybench common header. */
#include <polybench.h>

/* Include benchmark-specific header. */
#include "doitgen.h"


/* Array initialization. */
static
void init_array(int nr, int nq, int np,
		DATA_TYPE POLYBENCH_3D(A,NR,NQ,NP,nr,nq,np),
		DATA_TYPE POLYBENCH_2D(C4,NP,NP,np,np))
{
  int i, j, k;

  for (i = 0; i < nr; i++)
    for (j = 0; j < nq; j++)
      for (k = 0; k < np; k++)
	A[i][j][k] = (DATA_TYPE) ((i*j + k)%np) / np;
  for (i = 0; i < np; i++)
    for (j = 0; j < np; j++)
      C4[i][j] = (DATA_TYPE) (i*j % np) / np;
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int nr, int nq, int np,
		 DATA_TYPE POLYBENCH_3D(A,NR,NQ,NP,nr,nq,np))
{
  int i, j, k;

  POLYBENCH_DUMP_START;
  POLYBENCH_DUMP_BEGIN("A");
  for (i = 0; i < nr; i++)
    for (j = 0; j < nq; j++)
      for (k = 0; k < np; k++) {
	if ((i*nq*np+j*np+k) % 20 == 0) fprintf (POLYBENCH_DUMP_TARGET, "\n");
	fprintf (POLYBENCH_DUMP_TARGET, DATA_PRINTF_MODIFIER, A[i][j][k]);
      }
  POLYBENCH_DUMP_END("A");
  POLYBENCH_DUMP_FINISH;
}





void kernelP_doitgen0(int nr, int nq, int np,  int r, int q,
		    DATA_TYPE POLYBENCH_3D(A,NR,NQ,NP,nr,nq,np),
		    DATA_TYPE POLYBENCH_2D(C4,NP,NP,np,np),
		    DATA_TYPE POLYBENCH_1D(sum,NP,np))
{
  int p, s;
	  #pragma omp parallel for private(s)	
      for (p = 0; p < _PB_NP; p++)  
	  {
		sum[p] = SCALAR_VAL(0.0);
		for (s = 0; s < _PB_NP; s++)
		  sum[p] += A[r][q][s] * C4[s][p];
	  }
}

void kernelP_doitgen1(int nr, int nq, int np, int r, int q,
		    DATA_TYPE POLYBENCH_3D(A,NR,NQ,NP,nr,nq,np),
		    DATA_TYPE POLYBENCH_2D(C4,NP,NP,np,np),
		    DATA_TYPE POLYBENCH_1D(sum,NP,np))
{
  int p;
	  #pragma omp parallel for
	  for (p = 0; p < _PB_NP; p++)
		A[r][q][p] = sum[p];
}

void kernel_doitgen1(int nr, int nq, int np,
		    DATA_TYPE POLYBENCH_3D(A,NR,NQ,NP,nr,nq,np),
		    DATA_TYPE POLYBENCH_2D(C4,NP,NP,np,np),
		    DATA_TYPE POLYBENCH_1D(sum,NP,np))
{
  int r, q;

#pragma scop
  for (r = 0; r < _PB_NR; r++)
    for (q = 0; q < _PB_NQ; q++)  
	{
	  kernelP_doitgen0 (nr, nq, np, r, q, POLYBENCH_ARRAY(A), POLYBENCH_ARRAY(C4), sum /*POLYBENCH_ARRAY(sum)*/);
	  kernelP_doitgen1 (nr, nq, np, r, q, POLYBENCH_ARRAY(A), POLYBENCH_ARRAY(C4), sum /*POLYBENCH_ARRAY(sum)*/);
    }
#pragma endscop

}

/* Main computational kernel. The whole function will be timed,
   including the call and return. */
void kernel_doitgen(int nr, int nq, int np,
		    DATA_TYPE POLYBENCH_3D(A,NR,NQ,NP,nr,nq,np),
		    DATA_TYPE POLYBENCH_2D(C4,NP,NP,np,np),
		    DATA_TYPE POLYBENCH_1D(sum,NP,np))
{
  int r, q, p, s;

#pragma scop
  for (r = 0; r < _PB_NR; r++)
    for (q = 0; q < _PB_NQ; q++)  
	{
	  #pragma omp parallel for private(s)	
      for (p = 0; p < _PB_NP; p++)  
	  {
		sum[p] = SCALAR_VAL(0.0);
		for (s = 0; s < _PB_NP; s++)
		  sum[p] += A[r][q][s] * C4[s][p];
	  }
	  #pragma omp parallel for
	  for (p = 0; p < _PB_NP; p++)
		A[r][q][p] = sum[p];
    }
#pragma endscop

}


int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int nr = NR;
  int nq = NQ;
  int np = NP;
  MPI_Init(NULL, NULL);

  /* Variable declaration/allocation. */
  POLYBENCH_3D_ARRAY_DECL(A,DATA_TYPE,NR,NQ,NP,nr,nq,np);
  POLYBENCH_1D_ARRAY_DECL(sum,DATA_TYPE,NP,np);
  POLYBENCH_2D_ARRAY_DECL(C4,DATA_TYPE,NP,NP,np,np);

  /* Initialize array(s). */
  init_array (nr, nq, np,
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(C4));

		  
  int iter = 0;
  for (iter = 0; iter < 5; iter++)
  {
	  /* Start timer. */
	  polybench_start_instruments;

	  /* Run kernel. */
	  /*kernel_doitgen (nr, nq, np,
			  POLYBENCH_ARRAY(A),
			  POLYBENCH_ARRAY(C4),
			  POLYBENCH_ARRAY(sum));*/
			  
		kernel_doitgen1 (nr, nq, np,
			  POLYBENCH_ARRAY(A),
			  POLYBENCH_ARRAY(C4),
			  POLYBENCH_ARRAY(sum));

	  /* Stop and print timer. */
	  polybench_stop_instruments;
	  polybench_print_instruments;
  }
	  
	  

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(nr, nq, np,  POLYBENCH_ARRAY(A)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(sum);
  POLYBENCH_FREE_ARRAY(C4);
  MPI_Finalize();

  return 0;
}
