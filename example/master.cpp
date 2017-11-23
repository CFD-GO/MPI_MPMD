#include "../MPMD.hpp"

int main(int argc, char *argv[])
{
   MPI_Init(&argc, &argv);

   MPMDHelper MPMD;
   
   std::vector<int> excl;
   excl.push_back(0);
   MPMD.Init(MPI_COMM_WORLD, argv[0],excl);
   
   for (int i=1; i<argc; i++) MPMD.Spawn(argv[i], MPI_ARGV_NULL, 2, MPI_INFO_NULL);

   MPMD.Identify();

   MPI_Finalize();
   return 0;
}
