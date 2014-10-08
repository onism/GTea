#ifndef READCGNASOLUTION_H
#define READCGNASOLUTION_H
#include "Cgns_solution_struct.h"
#include "cgnslib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#pragma comment(lib,"cgnsdll.lib")
class ReadCgnaSolution
{
public:
    ReadCgnaSolution();
    int nZones;
    SZONE *Zones;
    int cgnsfn;
    int cgnsbase;
public:
    void FATAL (char *procname, char *errmsg);
    SZONE *new_zone(int count);
    FIELD *new_field (int nflds, int size);
    int open_cgns(char *cgnsfile);
    int find_base(char *basename);
    void read_cgns();
    void read_zones();
    int read_zone_solution (int nz);
    SOLUTION *new_solution (int nsols);
    int read_solution_field (int nz, int ns, int nf);
};

#endif // READCGNASOLUTION_H
