#include "../MPMD.hpp"

int main(int argc, char *argv[])
{
   MPMDHelper MPMD;

   printf("MASTER: Initializing MPI...\n");
   MPI_Init(&argc, &argv);
   printf("MASTER: Initializing MPMD...\n");
   MPMD.Init(MPI_COMM_WORLD, "MASTER");
   for (int i=1; i<argc; i++) {
      int n = atoi(argv[i]);
      i++;
      if (i >= argc) return -1;
      printf("MASTER: Spawning %dx%s...\n",n,argv[i]);
      if (! MPMD.Spawn(argv[i], MPI_ARGV_NULL, n, MPI_INFO_NULL)) {
         fprintf(stderr, "MASTER: Failed to spawn\n");
         return -1;
      }
   }
   
   printf("MASTER: Identify...\n");
   MPMD.Identify();
   printf("MASTER: Finalize MPI...\n");
   MPI_Finalize();
   return 0;
}
