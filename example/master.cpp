#include "../MPMD.hpp"

int main(int argc, char *argv[])
{
   MPMDHelper MPMD;

   MPI_Init(&argc, &argv);
   MPMD.Init(MPI_COMM_WORLD, "MASTER");
   
   for (int i=1; i<argc; i++) {
      int n = atoi(argv[i]);
      i++;
      if (i >= argc) return -1;
      if (! MPMD.Spawn(argv[i], MPI_ARGV_NULL, n, MPI_INFO_NULL)) {
         fprintf(stderr, "Failed to spawn\n");
         return -1;
      }
   }

   MPMD.Identify();

   MPI_Finalize();
   return 0;
}
