#include "readcgnasolution.h"


//ReadCgnaSolution::ReadCgnaSolution()
//{
//   nZones = 0;
//   cgnsbase = 1;
//   cgnsfn = 0;
//}
//void  ReadCgnaSolution::FATAL(char *procname, char *errmsg)
//{
//    char *msg = errmsg;
//    if(NULL == msg)
//    {
//        msg = (char*)cg_get_error();
//        if(NULL == msg||!*msg){
//            msg = "unknown error";
//        }
//        if(cgnsfn) cg_close(cgnsfn);

//    }
//}


//int ReadCgnaSolution::open_cgns(char *cgnsfile)
//{
//   int nbases;
//   if(cg_open(cgnsfile,CG_MODE_READ,&cgnsfn)||
//           cg_nbases(cgnsfn,&nbases)){
//       FATAL("open_cgns", NULL);
//       return nbases;
//   }
//   return nbases;
//}

//int ReadCgnaSolution::find_base(char *basename)
//{
//    int nbases,nb,cdum,pdim;
//    char buff[33];
//    if(cg_nbases(cgnsfn,&nbases))
//        FATAL("find base",NULL);
//    for(nb=1;nb<nbases;nb++)
//    {
//        if(cg_base_read(cgnsfn,nb,buff,&cdum,&pdim))
//            FATAL("find base",NULL);
//        if(!strcmp(buff,basename)) return nb;
//    }
//    return 0;
//}

///*---------- read_cgns ------------------------------------------------
// * read the CGNS file
// *---------------------------------------------------------------------*/
//void ReadCgnaSolution::read_cgns()
//{
//    int nz,ns,ne;
//    read_zones();
//    for(nz=1;nz<nZones;nz++){
//        read_zone_solution(nz);
//        for(ns=1;ns<=Zones[nz-1].nsols;ns++){
//            read_solution_field(nz,ns,0);
//        }
//    }
//}
//SZONE *ReadCgnaSolution::new_zone(int count)
//{
//    int n;
//    SZONE * z;
//    z = (SZONE*)calloc(count,sizeof(SZONE));
//    if(NULL == z){
//        FATAL("new zone","calloc failed");

//    }
//    for(n=0; n < count; n++){
//        z[n].id = n+1;
//        sprintf(z[n].name,"Zone%d",n+1);
//        z[n].type = Structured;
//        z[n].vertflags = 0;
//        z[n].datatype = RealDouble;
//    }
//    return z;
//}
///*---------- read_zones -----------------------------------------------
// * read zone information from CGNS file
// *---------------------------------------------------------------------*/
//void ReadCgnaSolution::read_zones()
//{
//    int n,nz,ni,nc,ns,nd,celldim,sizes[9];
//    ZoneType_t zonetype;
//    char buff[33];
//    if(cg_goto(cgnsfn,cgnsbase,"end"))
//        FATAL("read zones",NULL);
//    if (cg_base_read (cgnsfn, cgnsbase, buff, &celldim, &nd) ||
//           cg_nzones (cgnsfn, cgnsbase, &nZones))
//           FATAL ("read_zones", NULL);
//    Zones = new_zone (nZones);

//    /* read the zone information */
//    for(nz = 0; nz < nZones; nz++){
//        if(cg_zone_read(cgnsfn,cgnsbase,nz+1,buff,sizes)||
//                cg_zone_type(cgnsfn,cgnsbase,nz+1,&zonetype))
//            FATAL("read zone",NULL);
//        if(zonetype != Unstructured && zonetype != Structured)
//            FATAL("read zone","invalid zone type");
//        Zones[nz].id = nz + 1;
//        strcpy(Zones[nz].name,buff);
//        Zones[nz].type = zonetype;
//        Zones[nz].idim = zonetype== Structured ? celldim : 1;
//        for(n = 0 ;n < 3; n++){
//            Zones[nz].dim[n] = sizes[n];

//        }

//        /*get units*/
//        if(cg_goto(cgnsfn,cgnsbase,"Zone_t",nz+1,"end"))
//            FATAL("read zones",NULL);
//        /* get zone counts */

//        if (cg_nsections (cgnsfn, cgnsbase, nz+1, &Zones[nz].nesets) ||
//                cg_n1to1 (cgnsfn, cgnsbase, nz+1, &Zones[nz].nints) ||
//                cg_nconns (cgnsfn, cgnsbase, nz+1, &Zones[nz].nconns) ||
//                cg_nbocos (cgnsfn, cgnsbase, nz+1, &Zones[nz].nbocos) ||
//                cg_nsols (cgnsfn, cgnsbase, nz+1, &Zones[nz].nsols))
//                FATAL ("read_zones", NULL);
//    }
//}
///*---------- read_zone_solution ---------------------------------------
// * read zone solution
// *---------------------------------------------------------------------*/

//FIELD * ReadCgnaSolution::new_field(int nflds, int size)
//{
//    int n;
//    FIELD *flds;
//    flds = (FIELD*)calloc(nflds,sizeof(FIELD));
//    if (NULL == flds)
//            FATAL ("new_field", "calloc failed for new field array");
//        for (n = 0; n < nflds; n++) {
//            flds[n].id = n + 1;
//            sprintf (flds[n].name, "Field%d", n+1);
//            flds[n].datatype = RealDouble;
//            if (size > 0) {
//                flds[n].data = (double *) calloc (size, sizeof(double));
//                if (NULL == flds[n].data)
//                    FATAL ("new_field", "calloc failed for field data array");
//            }
//        }
//        return flds;

//}
//SOLUTION * ReadCgnaSolution::new_solution(int nsols)
//{
//       int n;
//       SOLUTION *sols;

//       sols = (SOLUTION *) calloc (nsols, sizeof(SOLUTION));
//       if (NULL == sols)
//           FATAL ("new_solution", "calloc failed for new solution array");
//       for (n = 0; n < nsols; n++) {
//           sols[n].id = n + 1;
//           sprintf (sols[n].name, "FlowSolution%d", n + 1);
//       }
//       return sols;
//}
//int ReadCgnaSolution::read_zone_solution(int nz)
//{
//    int i,j,ns,nd;
//    DataType_t datatype;
//    GridLocation_t location;
//    SZONE * z = &Zones[nz-1];
//    SOLUTION * sols;
//    if(cg_nsols(cgnsfn,cgnsbase,nz,&z->nsols))
//        FATAL("read zone solution",NULL);
//    if(z->nsols){
//        z->sols = sols = new_solution(z->nsols);
//        for(ns = 1; ns <= z->nsols; ns++,sols++){
//            if(cg_sol_info(cgnsfn,cgnsbase,nz,ns,sols->name,&location))
//                FATAL("read zone solution",NULL);
//            sols->location = location;
//            if(z->type == Structured){
//                if(sols->location == Vertex){
//                    for (j = 0; j < 2; j++)
//                       sols->rind[i][j] = 0;
//                       sols->size = z->dim[0] * z->dim[1] * z->dim[2];
//                }else if(sols->location == CellCenter)
//                {
//                    if (cg_goto (cgnsfn, cgnsbase, "Zone_t", nz,
//                               "FlowSolution_t", ns, "end"))
//                          FATAL ("read_zone_solution", NULL);
//                    if (cg_rind_read ((int *)sols->rind)) {
//                            for (i = 0; i < 3; i++)
//                            for (j = 0; j < 2; j++)
//                                sols->rind[i][j] = 0;
//                        }
//                     sols->size = 1;
//                     for (i = 0; i < 3; i++) {
//                           sols->size *= (z->dim[i] - 1 +
//                           sols->rind[i][0] + sols->rind[i][1]);
//                     }
//                }else
//                    FATAL ("read_zone_solution",
//                        "solution location not Vertex or CellCenter");
//            }//structed type end
//            else
//            {
//                sols->size = sols->location == Vertex ? z->dim[0] : z->dim[1];
//                for (i = 0; i < 3; i++)
//                   for (j = 0; j < 2; j++)
//                       sols->rind[i][j] = 0;
//            }
//            if (cg_nfields (cgnsfn, cgnsbase, nz, ns, &sols->nflds))
//                   FATAL ("read_zone_solution", NULL);
//            if (sols->nflds) {
//                    sols->flds = new_field (sols->nflds, 0);
//                    for (i = 0; i < sols->nflds; i++) {
//                        if (cg_field_info (cgnsfn, cgnsbase, nz, ns, i+1,
//                              &datatype, sols->flds[i].name))
//                          FATAL ("read_zone_solution", NULL);
//                       }
//                 }

//        }//nz end
//    }
// return z->nsols;
//}
//int ReadCgnaSolution::read_solution_field(int nz, int ns, int nf)
//{
//       int n, is, ie, min[3], max[3];
//       DataType_t datatype;
//        SZONE *z = &Zones[nz-1];
//        SOLUTION *s = &z->sols[ns-1];
//       FIELD *f;
//       if (z->type == Structured) {
//              for (n = 0; n < 3; n++) {
//                  min[n] = 1;
//                  max[n] = z->dim[n];
//              }
//              if (s->location == CellCenter) {
//                  for (n = 0; n < 3; n++)
//                      max[n] += s->rind[n][0] + s->rind[n][1] - 1;
//              }
//          }//Structured end
//       else{
//           for (n = 0; n < 3; n++) {
//               min[n] = 1;
//               max[n] = s->size;
//           }
//       }//UnStructured end

//       if (nf) {
//              is = ie = nf;
//          }
//          else {
//              is = 1;
//              ie = s->nflds;
//          }

//        f = &s->flds[is-1];

//        for (nf = is; nf <= ie; nf++, f++) {
//                if (cg_field_info (cgnsfn, cgnsbase, nz, ns, nf,
//                    &datatype, f->name))
//                    FATAL ("read_solution_field", NULL);
//                f->id = nf;
//                f->datatype = datatype;
//                f->data = (double *) malloc (s->size * sizeof(double));
//                if (NULL == f->data)
//                    FATAL ("read_solution_field",
//                        "malloc failed for solution field data");
//                if (cg_field_read (cgnsfn, cgnsbase, nz, ns, f->name,
//                    RealDouble, min, max, f->data))
//                    FATAL ("read_solution_field", NULL);

//                if (cg_goto (cgnsfn, cgnsbase, "Zone_t", nz,
//                    "FlowSolution_t", ns, "DataArray_t", nf, "end"))
//                    FATAL ("read_solution_field", NULL);
////                if (!read_units (f->units)) {
////                    for (n = 0; n < 5; n++)
////                        f->units[n] = s->units[n];
////                }

//                /* read data class, conversion and exponents */

//                if (cg_dataclass_read ((DataClass_t *)&f->dataclass))
//                    f->dataclass = s->dataclass;

//                if (cg_conversion_info (&datatype))
//                    f->dataconv[0] = 1.0;
//                else {
//                    f->convtype = datatype;
//                    if (datatype == RealSingle) {
//                        float conv[2];
//                        if (cg_conversion_read (conv))
//                            FATAL ("read_solution_field", NULL);
//                        for (n = 0; n < 2; n++)
//                            f->dataconv[n] = conv[n];
//                    }
//                    else if (datatype == RealDouble) {
//                        if (cg_conversion_read (f->dataconv))
//                            FATAL ("read_solution_field", NULL);
//                    }
//                    else
//                        FATAL ("cg_conversion_info", "invalid data type");
//                }

//                if (!cg_exponents_info (&datatype)) {
//                    f->exptype = datatype;
//                    if (datatype == RealSingle) {
//                        float exp[5];
//                        if (cg_exponents_read (exp))
//                            FATAL ("read_solution_field", NULL);
//                        for (n = 0; n < 5; n++)
//                            f->exponent[n] = exp[n];
//                    }
//                    else if (datatype == RealDouble) {
//                        if (cg_exponents_read (f->exponent))
//                            FATAL ("read_solution_field", NULL);
//                    }
//                    else
//                        FATAL ("cg_exponents_info", "invalid data type");
//                }
//            }
//            return s->size;
//}
