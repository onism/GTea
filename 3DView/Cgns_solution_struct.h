#ifndef CGNS_SOLUTION_STRUCT_H
#define CGNS_SOLUTION_STRUCT_H
typedef struct   {
    int id;
    char name[33];
    char *desc;
} DESC;

typedef struct  {
    int id;
    double x, y, z, w;
} VERTEX;

typedef struct   {
    int id;
    char name[33];
    int type;
    int start;
    int end;
    int nbndry;
    int *conn;
    int *parent;
} ELEMSET;



typedef struct   {
    int id;
    char name[33];
    int type;
    int location;
    int ptype;
    int npnts;
    int *pnts;
    char d_name[33];
    int d_ztype;
    int d_ptype;
    int d_npnts;
    int *d_pnts;
    int d_zone;
} CONNECT;

typedef struct   {
    int id;
    char name[33];
    int type;
    int ptype;
    int npnts;
    int *pnts;
    int n_index[3];
    int n_cnt;
    int n_type;
    double *n_list;
} BOCO;

typedef struct   {
    int id;
    char name[33];
    int datatype;
    int units[5];
    int dataclass;
    int convtype;
    double dataconv[2];
    int exptype;
    double exponent[5];
    double *data;
} FIELD;

typedef struct   {
    int id;
    char name[33];
    int location;
    int rind[3][2];
    int size;
    int units[5];
    int dataclass;
    int nflds;
    FIELD *flds;
    int ndesc;
    DESC *desc;
} SOLUTION;

typedef struct   {
    int id;
    char name[33];
    int type;
    int idim;
    int dim[3];
    int units[5];
    int dataclass;
    int datatype;
    int vertflags;
    int nverts;
    VERTEX *verts;
    int nesets;
    ELEMSET *esets;
    int nints;
   // INTERFACE *ints;
    int nconns;
    CONNECT *conns;
    int nbocos;
    BOCO *bocos;
    int nsols;
    SOLUTION *sols;
    void *user;
    int ndesc;
    DESC *desc;
} SZONE;

#endif // CGNS_SOLUTION_STRUCT_H
