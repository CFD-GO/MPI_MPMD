#include "../MPMD.hpp"

int main(int argc, char *argv[])
{
   MPMDHelper MPMD;
   
   MPI_Init(&argc, &argv);
   
   std::vector<int> excl;
   excl.push_back(0); // Exclude process 0 from the working group.
   
   MPMD.Init(MPI_COMM_WORLD, argv[0],excl);
   
   MPMD.Identify();
   
   /// Do some stuff

   MPI_Finalize();
   return 0;
}
