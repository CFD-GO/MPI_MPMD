#include "../MPMD.hpp"

int main(int argc, char *argv[])
{
   MPMDHelper MPMD(true);
   
   printf("DEBUG: WITH_WORKERS: Initializing MPI...\n");
   MPI_Init(&argc, &argv);
   
   std::vector<int> excl;
   excl.push_back(0); // Exclude process 0 from the working group.
   
   printf("DEBUG: WITH_WORKERS: Initializing MPMD...\n");
   MPMD.Init(MPI_COMM_WORLD, "WITH_WORKERS", excl);
   
   for (int i=1; i<argc; i++) {
      int n = atoi(argv[i]);
      i++;
      if (i >= argc) return -1;
      printf("DEBUG: WITH_WORKERS: Spawning %dx%s...\n",n,argv[i]);
      if (! MPMD.Spawn(argv[i], MPI_ARGV_NULL, n, MPI_INFO_NULL)) {
         fprintf(stderr, "WITH_WORKERS: Failed to spawn\n");
         return -1;
      }
   }
   
   printf("DEBUG: WITH_WORKERS: Identify...\n");
   MPMD.Identify();
   printf("DEBUG: WITH_WORKERS: Finalize MPI...\n");
   MPI_Finalize();
   return 0;
}
