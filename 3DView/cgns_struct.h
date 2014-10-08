#ifndef CGNS_STRUCT_H
#define CGNS_STRUCT_H
#include "cgnslib.h"
#include <QList>
typedef float Node[3];
#define EDGE_ANGLE 0.5
typedef struct {
    int id;
    int nodes[2];
} Edge;

typedef struct {
    int id;
    int nnodes;
    int nodes[4];
    float normal[3];
} Face;

#ifndef NO_CUTTING_PLANE

typedef struct {
    int id;
    int nodes[3];
    float ratio;
} CutNode;

typedef struct {
    int nelems;
    int *elems;
    int nedges;
    Edge *edges;
} CutData;

typedef struct {
    float plane[4];
    int nelems;
    int nedges;
    int nnodes;
    Node *nodes;
} CutPlane;

static float cutcolor[4] = {0.8, 0.4, 0.8, 0.5};
static int usecutclr = 0;
static int ignorevis = 0;
static CutPlane cutplane;
static int CutDL = 0;
static int PlaneDL = 0;

#endif

typedef struct {
    char name[33];
    int type;
    int dim;
    int data[10];
    char d_name[33];
    int nedges;
    Edge *edges;
    int nfaces;
    Face *faces;

#ifndef NO_CUTTING_PLANE
    ElementType_t elemtype;
    int nelems;
    int *elems;
    CutData cut;
#endif
    float bbox[3][2];
    int dlist;
    int mode;
    float color[4];
    char errmsg[81];
} Regn;

typedef struct {
    char name[33];
    int nnodes;
    Node *nodes;

    int nregs;
    Regn *regs;

} Zone;





enum {
    REG_MESH,
    REG_ELEM,
    REG_1TO1,
    REG_CONN,
    REG_BOCO,
    REG_BNDS
};

/* mapping from elements to faces */

static int facenodes[22][5] = {
    /* tri */
    {3, 0, 1, 2, 0},
    /* quad */
    {4, 0, 1, 2, 3},
    /* tet */
    {3, 0, 2, 1, 0},
    {3, 0, 1, 3, 0},
    {3, 1, 2, 3, 0},
    {3, 2, 0, 3, 0},
    /* pyramid */
    {4, 0, 3, 2, 1},
    {3, 0, 1, 4, 0},
    {3, 1, 2, 4, 0},
    {3, 2, 3, 4, 0},
    {3, 3, 0, 4, 0},
    /* wedge */
    {4, 0, 1, 4, 3},
    {4, 1, 2, 5, 4},
    {4, 2, 0, 3, 5},
    {3, 0, 2, 1, 0},
    {3, 3, 4, 5, 0},
    /* hex */
    {4, 0, 3, 2, 1},
    {4, 0, 1, 5, 4},
    {4, 1, 2, 6, 5},
    {4, 2, 3, 7, 6},
    {4, 0, 4, 7, 3},
    {4, 4, 5, 6, 7}
};
/*#define STATISTICS    define this for hit/seeks counts*/
/*#define REORDER       define this for reordering after find*/
typedef void *HASH;
typedef struct hash_element_ {
    void *entry;
    struct hash_element_ *next;
    struct hash_element_ **prev;
#ifdef STATISTICS
    size_t hits;
    size_t seeks;
#endif
} BUCKET;
#define HashSize(H) HashList(H,NULL,NULL)
typedef struct hash_tab_ {
    BUCKET **table;     /* pointer to hash table */
    size_t size;        /* Max number of elements in table */
    size_t numsyms;     /* number of elements currently in table */
    BUCKET *lastpos;    /* last bucket accessed by find */
    int (*compare)(     /* entry compare function */
        void *, void *
    );
    size_t (*hash)(     /* pointer to hashing routine */
        void *
    );
} HASH_TAB;
 #define NODE_INDEX(I,J,K) ((I)+dim[0]*((J)+dim[1]*(K)))









#endif // CGNS_STRUCT_H
