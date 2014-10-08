#ifndef CGNSIO_H
#define CGNSIO_H
/* Structure Declarations */

/* Structure for Descriptor */
typedef struct _DESC {
    int id;
    char name[33];
    char *desc;
} DESC;

/* Structure for Vertex */
typedef struct _VERTEX {
    int id;
    double x, y, z, w;
} VERTEX;

/* Structure for Element Set */
typedef struct _ELEMSET {
    int id;
    char name[33];
    int type;
    int start;
    int end;
    int nbndry;
    int *conn;
    int *parent;
} ELEMSET;

/* Structure for Interface */
typedef struct _INTERFACE {
    int id;
    char name[33];
    int range[3][2];
    char d_name[33];
    int d_range[3][2];
    int transform[3];
    int d_zone;
} INTERFACES;

/* Structure for Grid Connectivity */
typedef struct _CONNECT {
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

/* Structure for Boundary Condition */
typedef struct _BOCO {
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

/* Structure for Field */
typedef struct _FIELD {
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

/* Structure for Solution */
typedef struct _SOLUTION {
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

/* Structure for Zone */
typedef struct _ZONE {
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
    INTERFACES *ints;
    int nconns;
    CONNECT *conns;
    int nbocos;
    BOCO *bocos;
    int nsols;
    SOLUTION *sols;
    void *user;
    int ndesc;
    DESC *desc;
} ZONE;

/* Variable Declarations */

extern float Version;
extern int nZones;
extern ZONE *Zones;
extern int baseunits[5];
extern int baseclass;
extern int cgnsfn;
extern int cgnsbase;
extern int element_node_counts[];

/* Function Declarations */
int Write(const char *file);
int convert(const char *file);

/* CGNSIO Functions */
void FATAL(char *procname, char *errmsg);
int file_exists(const char *file);
char *temporary_file(void);
void copy_file(char *oldfile, char *newfile);
ZONE *new_zone(int count);
VERTEX *new_vertex(int nverts);
ELEMSET *new_elemset(int nesets);
INTERFACES *new_interface(int nints);
CONNECT *new_connect(int nconns);
BOCO *new_boco(int nbocos);
SOLUTION *new_solution(int nsols);
FIELD *new_field(int nflds, int size);
DESC *new_desc(int ndesc);
int vertex_index(ZONE *z, int i, int j, int k);
int cell_index(ZONE *z, int i, int j, int k);
int solution_index(ZONE *z, SOLUTION *s, int i, int j, int k);
int open_cgns(const char *cgnsfile, int mode);
int find_base(char *basename);
void read_cgns(void);
int read_zones(void);
void read_zone_data(int nz);
int read_zone_grid(int nz);
int read_zone_element(int nz);
int structured_elements(int nz);
int read_zone_interface(int nz);
int read_zone_connect(int nz);
int read_zone_boco(int nz);
int read_zone_solution(int nz);
int read_solution_field(int nz, int ns, int nf);
int read_units(int units[5]);

void print_interface(ZONE *zone);
void print_connect(ZONE *zone);
void print_solution(ZONE *zone);
void print_GridLocation(int location);
void print_BCDataType(int type);
void print_GridConnectivityType(int type);
void print_PointSetType(int type);
void print_BCType(int type);
void print_DataType(int type);
void print_ZoneType(int type);
#endif // CGNSIO_H
