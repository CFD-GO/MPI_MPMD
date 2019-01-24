#include "../MPMD.hpp"

int main(int argc, char *argv[])
{
   MPMDHelper MPMD(true);

   printf("DEBUG: MASTER: Initializing MPI...\n");
   MPI_Init(&argc, &argv);
   printf("DEBUG: MASTER: Initializing MPMD...\n");
   MPMD.Init(MPI_COMM_WORLD, "MASTER");
   for (int i=1; i<argc; i++) {
      int n = atoi(argv[i]);
      i++;
      if (i >= argc) return -1;
      printf("DEBUG: MASTER: Spawning %dx%s...\n",n,argv[i]);
      if (! MPMD.Spawn(argv[i], MPI_ARGV_NULL, n, MPI_INFO_NULL)) {
         fprintf(stderr, "MASTER: Failed to spawn\n");
         return -1;
      }
   }
   
   printf("DEBUG: MASTER: Identify...\n");
   MPMD.Identify();
   printf("DEBUG: MASTER: Finalize MPI...\n");
   MPI_Finalize();
   return 0;
}
