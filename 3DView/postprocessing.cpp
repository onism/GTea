#include "postprocessing.h"
#include "CGNSIO.h"
PostProcessing::PostProcessing()
{
}

int PostProcessing::post_solutions(char *file)
{
//    int nbases,ncoords,celldim,phydim;
//    char basename[33];
//    ZONE *z;
//    int i,j,nz;
//    SOLUTION *s;
//    if(!file_exists(file))
//    {
//        FATAL(NULL, "File does not exist");

//    }
//    nbases = open_cgns(file,0);
//    if(!nbases)
//    {
//         FATAL(NULL, "No bases in CGNS file");
//    }

//    read_cgns();

//       for (z = Zones, nz = 1; nz <= nZones; nz++, z++) {
//           if (z->nsols) {
//               printf("No of Solutions Nodes = %d\n", z->nsols);
//               for (s = z->sols, i = 1; i <= z->nsols; i++, s++) {
//                   printf("Solution Node = %d\n", i);

//                   if (s->nflds == 0)
//                       FATAL(NULL, "No Solution Fields: Exiting");

//                   printf("\tNo of Fields = %d\n", s->nflds);
//                   for (j = 0; j < s->nflds; j++) {
//                       printf("\tField = %s\n", s->flds[j].name);
//                   }
//               }
//           } else
//               FATAL(NULL, "No Solution Node Available: Exiting");
//       }

//       int solution_no,fld_no1,fld_no2,zone_no;
//       solution_no = 2;
//       fld_no1 = 2;
//       fld_no2 = 3;
//       zone_no = 1;
//       SOLUTION *s1 = Zones[zone_no-1].sols[solution_no];
//       int NoNodes = Zones[zone_no-1].dim[0];
//       int NoCells = Zones[zone_no-1].dim[1];
//       double MinValueSol,MaxValueSol;
//       SolutionMinMaxVector_2D(zone_no-1,solution_no, fld_no1,fld_no2, &MinValueSol, &MaxValueSol);

//           for(int i=0;i<s1->size;i++)
//           {
//             double solx = s1->flds[fld_no1-1].data[i];
//             double soly = s1->flds[fld_no2-1].data[i];
//             int color_index = (int) (((sqrt(solx * solx + soly * soly) - MinValueSol) / (MaxValueSol - MinValueSol)) * 255);
//             //coord
//             double x1 = Zones[zone_no-1].verts[i].x;
//             double y1 = Zones[zone_no-1].verts[i].y;

//           }

return 1;


}

void PostProcessing::SolutionMinMaxVector_2D(int zone_no, int sol_no, int SolLocation1, int SolLocation2, double *MinValueSol, double *MaxValueSol)
{

//    SOLUTION *s = Zones[zone_no-1].sols[sol_no];

//        *MinValueSol = sqrt(s->flds[SolLocation1].data[0]*s->flds[SolLocation1].data[0]+s->flds[SolLocation2].data[0]*s->flds[SolLocation2].data[0]);
//        *MaxValueSol = *MinValueSol;
//        for(int i=1;i<s->size;i++){
//            double sol =  sqrt(s->flds[SolLocation1].data[i]*s->flds[SolLocation1].data[i]
//                    +s->flds[SolLocation2].data[i]*s->flds[SolLocation2].data[i]);
//             if(*MinValueSol > sol)
//                 *MinValueSol = sol;
//             if(*MaxValueSol < sol)
//                 *MaxValueSol = sol;
//        }


}
