#ifndef LOADCGNS_H
#define LOADCGNS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <QDebug>
#include "cgns_struct.h"
#include "cgnslib.h"
#pragma comment(lib,"cgnsdll.lib")




static int cgnsfn = 0;
static int cgnsbase = 0;
static int cgnszone = 0;
static int nbases = 0;
static int nzones = 0;
static Zone *zones;
static int AxisDL = 0;

static char BaseName[33];
static int CellDim, PhyDim;
static  HASH facehash;




/*---------- HashCreate ----------------------------------------------
 * creates a hash table of indicated size.
 * Should be a prime number for best results.
 * Useful values are:
 *   47 61 89 113 127 157 193 211 257 293 359 401
 *--------------------------------------------------------------------*/

static HASH HashCreate (size_t size, int (*compare)(void *entry1, void *entry2),
                 size_t (*hashfunc)(void *entry))
{
    HASH_TAB *tabp;

    if (NULL == compare || NULL == hashfunc)
        return (NULL);
    if (size < 1) size = 11;
    tabp = (HASH_TAB *) malloc (sizeof(HASH_TAB) + size * sizeof(BUCKET *));
    if (NULL != tabp) {
        tabp->table   = (BUCKET **)(tabp + 1);
        tabp->size    = size;
        tabp->numsyms = 0;
        tabp->lastpos = NULL;
        tabp->compare = compare;
        tabp->hash    = hashfunc;
        for (size = 0; size < tabp->size; size++)
            tabp->table[size] = NULL;
    }
    return ((HASH)tabp);
}

/*---------- HashDestroy ---------------------------------------------
 * destroy a hash table
 *--------------------------------------------------------------------*/

static void HashDestroy (HASH hash, void (*freeentry)(void *entry))
{
    HASH_TAB *tabp = (HASH_TAB *)hash;
    BUCKET *p, *next;
    size_t i;

    for (i = 0; i < tabp->size; i++) {
        for (p = tabp->table[i]; p != NULL; p = next) {
            next = p->next;
            if (NULL != freeentry)
                (*freeentry) (p->entry);
            free (p);
        }
    }
    free (tabp);
}

/*---------- HashFind -------------------------------------------------
 * finds symbol in hash table or NULL if no match
 * If more than one such entry is in the table, the most
 * recently added one is found (which will be first in the list).
 *---------------------------------------------------------------------*/

static void *HashFind (HASH hash, void *entry)
{
    BUCKET *p, **pf;
    HASH_TAB *tabp = (HASH_TAB *)hash;

    pf = &(tabp->table)[((*tabp->hash) (entry)) % tabp->size];
    p = *pf;

    while (NULL != p && (*tabp->compare) (entry, p->entry)) {
#ifdef STATISTICS
        p->seeks++;
#endif
        p = p->next;
    }
    if (NULL == p)
        return (NULL);

#ifdef STATISTICS
    p->hits++;
#endif
    tabp->lastpos = p;

    /*
     * this mod places the symbol at the beginning of the list
     * if not already there. This tends to cluster the most often
     * accessed symbols at the beginning of the table, thus
     * reducing the length of the search path
     */

#ifdef REORDER
    if (p != *pf) {
        BUCKET *sym = *pf;
        if ((*(p->prev) = p->next) != NULL)
            p->next->prev = p->prev;
        *pf = p;
        p->prev = pf;
        p->next = sym;
        sym->prev = &p->next;
    }
#endif

    return (p->entry);
}

/*---------- HashAdd -------------------------------------------------
 * add a symbol to the hash table
 * The new symbol is placed first in the bucket.
 *--------------------------------------------------------------------*/

static void *HashAdd (HASH hash, void *entry)
{
    BUCKET **p, *tmp;
    BUCKET *sym;
    HASH_TAB *tabp = (HASH_TAB *)hash;

    if ((sym = (BUCKET *) malloc (sizeof(BUCKET))) == NULL)
        return (NULL);

    p = &(tabp->table)[(*tabp->hash) (entry) % tabp->size];

    tmp = *p;
    *p = sym;
    sym->entry = entry;
    sym->prev = p;
    sym->next = tmp;

    if (NULL != tmp)
        tmp->prev = &sym->next;

    tabp->numsyms++;
    tabp->lastpos = NULL;
    return (entry);
}

/*---------- HashDelete ----------------------------------------------
 * removes a symbol from the hash table
 *--------------------------------------------------------------------*/

void *HashDelete (HASH hash, void *entry)
{
    BUCKET *p;
    HASH_TAB *tabp = (HASH_TAB *)hash;

    if (NULL == tabp->lastpos || entry != tabp->lastpos->entry) {
        if (NULL == HashFind (hash, entry))
            return (NULL);
    }
    p = tabp->lastpos;
    tabp->numsyms--;
    if ((*(p->prev) = p->next) != NULL)
        p->next->prev = p->prev;
    free (p);
    return (entry);
}

/*---------- HashList ------------------------------------------------
 * output the hash table
 *--------------------------------------------------------------------*/

static size_t HashList (HASH hash, size_t (*listentry)(void *entry, void *userdata),
                 void *userdata)
{
    HASH_TAB *tabp = (HASH_TAB *)hash;
    BUCKET *p;
    size_t i, cnt = 0;

    if (NULL == listentry)
        return (tabp->numsyms);
    for (i = 0; i < tabp->size; i++) {
        for (p = tabp->table[i]; p != NULL; p = p->next)
            cnt += (*listentry) (p->entry, userdata);
    }
    return cnt;
}

/*---------- HashStats ------------------------------------------------
 * print a variety of statistics about the hash table
 *---------------------------------------------------------------------*/

#define MAXINT  ((size_t) ~ 0)
#define MAXLEN  128

static void HastStats (HASH hash)
{
    HASH_TAB *tabp = (HASH_TAB *)hash;
    BUCKET *p;
    size_t i;
    size_t chain_len;
    size_t chain_avg;
    size_t maxlen = 0;
    size_t minlen = MAXINT;
    size_t lengths[MAXLEN];
    size_t longer = 0;
#ifdef STATISTICS
    size_t hits = 0;
    size_t seeks = 0;
#endif
    int deviation = 0;

    chain_avg = tabp->numsyms / tabp->size;
    memset (lengths, 0, sizeof(lengths));

    for (i = 0; i < tabp->size; i++) {
        chain_len = 0;
        for (p = tabp->table[i]; p; p = p->next) {
#ifdef STATISTICS
            hits += p->hits;
            seeks += p->seeks;
#endif
            chain_len++;
        }

        if (chain_len >= MAXLEN)
            longer++;
        else
            ++lengths[chain_len];

        if (minlen > chain_len) minlen = chain_len;
        if (maxlen < chain_len) maxlen = chain_len;

        deviation += abs ((int)(chain_len - chain_avg));
    }

    printf ("%ld entries in %ld element hash table, ",
        (long)tabp->numsyms, (long)tabp->size);
    printf ("%ld (%1.0f%%) empty.\n",
        (long)lengths[0], ((double)lengths[0] / tabp->size) * 100.0);
    printf ("Mean chain length = %ld, min = %ld, max = %ld, dev = %ld\n",
        (long)chain_avg, (long)minlen, (long)maxlen,
        (long)(deviation / tabp->size));
#ifdef STATISTICS
    printf ("Table searchs = %ld, found = %ld, percent = %.2f\n",
        (long)seeks, (long)hits,
        (seeks ? (double)hits / seeks * 100.0 : (double)0));
#endif

    for (i = 0; i < MAXLEN; i++)
        if (lengths[i])
            printf ("%ld chains of length %d\n", (long)lengths[i], (int)i);

    if (longer)
        printf ("%ld chains of length %d or longer\n", (long)longer, MAXLEN);
}



static void freeall(void)
{
     int nz, nr;

    if (!nzones) return;
    for (nz = 0; nz < nzones; nz++) {
        for (nr = 0; nr < zones[nz].nregs; nr++) {
            /*if (zones[nz].regs[nr].dlist)
                glDeleteLists (zones[nz].regs[nr].dlist, 1);*/
            if (zones[nz].regs[nr].nedges)
                free (zones[nz].regs[nr].edges);
            if (zones[nz].regs[nr].nfaces)
                free (zones[nz].regs[nr].faces);
#ifndef NO_CUTTING_PLANE
            if (zones[nz].regs[nr].nelems)
                free (zones[nz].regs[nr].elems);
            if (zones[nz].regs[nr].cut.nelems)
                free (zones[nz].regs[nr].cut.elems);
            if (zones[nz].regs[nr].cut.nedges)
                free (zones[nz].regs[nr].cut.edges);
#endif
        }
        if (zones[nz].nregs) free (zones[nz].regs);
        if (zones[nz].nnodes) free(zones[nz].nodes);
    }
    free (zones);
    nzones = 0;
#ifndef NO_CUTTING_PLANE
    if (cutplane.nnodes)
        free (cutplane.nodes);
    cutplane.nelems = 0;
    cutplane.nedges = 0;
    cutplane.nnodes = 0;
#endif
}

static void  FATAL(char *errmsg){
     char cmd[129];
     sprintf (cmd, "error_exit {%s}", errmsg);

}

static void * MALLOC(char *funcname, int bytes){
     void *data = calloc (bytes, 1);
    if (NULL == data) {
        char msg[128];
        if (funcname != NULL)
            sprintf (msg, "%s:malloc failed for %d bytes", funcname, bytes);
        else
            sprintf (msg, "malloc failed for %d bytes", bytes);
        FATAL (msg);
    }
    return data;
}


static void *  REALLOC(char *funcname, int bytes, void *old_data){
  void *new_data;

    if (NULL == old_data)
        return MALLOC (funcname, bytes);
    new_data = realloc (old_data, bytes);
    if (NULL == new_data) {
        char msg[128];
        if (funcname != NULL)
            sprintf (msg, "%s:realloc failed for %d bytes", funcname, bytes);
        else
            sprintf (msg, "realloc failed for %d bytes", bytes);
        FATAL (msg);
    }
    return new_data;
}
static void free_all(){
         int nz, nr;

        if (!nzones) return;
        for (nz = 0; nz < nzones; nz++) {
            for (nr = 0; nr < zones[nz].nregs; nr++) {
                /*if (zones[nz].regs[nr].dlist)
                    glDeleteLists (zones[nz].regs[nr].dlist, 1);*/
                if (zones[nz].regs[nr].nedges)
                    free (zones[nz].regs[nr].edges);
                if (zones[nz].regs[nr].nfaces)
                    free (zones[nz].regs[nr].faces);
    #ifndef NO_CUTTING_PLANE
                if (zones[nz].regs[nr].nelems)
                    free (zones[nz].regs[nr].elems);
                if (zones[nz].regs[nr].cut.nelems)
                    free (zones[nz].regs[nr].cut.elems);
                if (zones[nz].regs[nr].cut.nedges)
                    free (zones[nz].regs[nr].cut.edges);
    #endif
            }
            if (zones[nz].nregs) free (zones[nz].regs);
            if (zones[nz].nnodes) free(zones[nz].nodes);
        }
        free (zones);
        nzones = 0;

}
static int  compare_ints (const void *v1, const void *v2)
{
    return (*((int *)v1) - *((int *)v2));
}

static int  find_int (int value, int nlist, int *list)
{
    int lo = 0, hi = nlist - 1, mid;

    if (!nlist || value < list[0]) return 0;
    if (value == list[0]) return 1;
    if (!hi || value > list[hi]) return 0;
    if (value == list[hi]) return 1;

    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        if (value == list[mid]) return 1;
        if (value < list[mid])
            hi = mid - 1;
        else
            lo = mid + 1;
    }
    return 0;
}

static int  structured_range ( Regn *reg, int *dim, int *ptrng, GridLocation_t location)
{
    int n, i, j, k, nfaces, rng[3][2];
    Face *f;
    static char *funcname = "structured_range";

    if (location != Vertex && location != IFaceCenter &&
        location != JFaceCenter && location != KFaceCenter) {
        i = j = 0;
        for (n = 0; n < 3; n++) {
            if (ptrng[n] == ptrng[n+3] &&
               (ptrng[n] == 1 || ptrng[n] == dim[n])) {
                if (ptrng[n] == 1)
                    i++;
                else if (j) {
                    j = 4;
                    break;
                }
                else
                    j = n + 1;
            }
        }
        if (!j && i == 1) {
            for (n = 0; n < 3; n++) {
                if (ptrng[n] == ptrng[n+3] && ptrng[n] == 1) {
                    j = n + 1;
                    break;
                }
            }
        }
        if (j == 1)
            location = IFaceCenter;
        else if (j == 2)
            location = JFaceCenter;
        else if (j == 3)
            location = KFaceCenter;
        else {
            strcpy (reg->errmsg,
                "unable to determine boundary - use [IJK]FaceCenter");
            return 0;
        }
    }

    nfaces = 1;
    if (location == Vertex) {
        for (n = 0, i = 0; i < 3; i++) {
            if (ptrng[i] < 1 || ptrng[i] > dim[i]) return 0;
            if (ptrng[i] == ptrng[i+3]) {
                if (n || (ptrng[i] != 1 && ptrng[i] != dim[i]))
                    return 0;
                n = i + 1;
                rng[i][0] = rng[i][1] = ptrng[i] - 1;
            } else {
                if (ptrng[i] < ptrng[i+3]) {
                    rng[i][0] = ptrng[i] - 1;
                    rng[i][1] = ptrng[i+3] - 1;
                }
                else {
                    rng[i][0] = ptrng[i+3] - 1;
                    rng[i][1] = ptrng[i] - 1;
                }
                nfaces *= (rng[i][1] - rng[i][0]);
            }
        }
    }
    else {
        if (location == IFaceCenter)
            n = 0;
        else if (location == JFaceCenter)
            n = 1;
        else
            n = 2;
        for (i = 0; i < 3; i++) {
            if (i == n) {
                if (ptrng[i] != ptrng[i+3] ||
                   (ptrng[i] != 1 && ptrng[i] != dim[i])) return 0;
                rng[i][0] = rng[i][1] = ptrng[i] - 1;
            }
            else {
                if (ptrng[i] < ptrng[i+3]) {
                    rng[i][0] = ptrng[i] - 1;
                    rng[i][1] = ptrng[i+3];
                }
                else {
                    rng[i][0] = ptrng[i+3] - 1;
                    rng[i][1] = ptrng[i];
                }
                if (rng[i][0] < 0 || rng[i][1] >= dim[i]) return 0;
                nfaces *= (rng[i][1] - rng[i][0]);
            }
        }
        n++;
    }
    if (!nfaces || !n) {
        strcpy (reg->errmsg, "couldn't find any exterior faces");
        return 0;
    }

    reg->nfaces = nfaces;
    reg->faces = f = (Face *) MALLOC (funcname, nfaces * sizeof(Face));

    if (n == 1) {
        if ((i = rng[0][0]) == 0) {
            for (k = rng[2][0]; k < rng[2][1]; k++) {
                for (j = rng[1][0]; j < rng[1][1]; j++) {
                    f->nnodes = 4;
                    f->nodes[0] = NODE_INDEX (i, j, k);
                    f->nodes[1] = NODE_INDEX (i, j, k+1);
                    f->nodes[2] = NODE_INDEX (i, j+1, k+1);
                    f->nodes[3] = NODE_INDEX (i, j+1, k);
                    f++;
                }
            }
        }
        else {
            for (k = rng[2][0]; k < rng[2][1]; k++) {
                for (j = rng[1][0]; j < rng[1][1]; j++) {
                    f->nnodes = 4;
                    f->nodes[0] = NODE_INDEX (i, j, k);
                    f->nodes[1] = NODE_INDEX (i, j+1, k);
                    f->nodes[2] = NODE_INDEX (i, j+1, k+1);
                    f->nodes[3] = NODE_INDEX (i, j, k+1);
                    f++;
                }
            }
        }
    }
    else if (n == 2) {
        if ((j = rng[1][0]) == 0) {
            for (k = rng[2][0]; k < rng[2][1]; k++) {
                for (i = rng[0][0]; i < rng[0][1]; i++) {
                    f->nnodes = 4;
                    f->nodes[0] = NODE_INDEX (i, j, k);
                    f->nodes[1] = NODE_INDEX (i+1, j, k);
                    f->nodes[2] = NODE_INDEX (i+1, j, k+1);
                    f->nodes[3] = NODE_INDEX (i, j, k+1);
                    f++;
                }
            }
        }
        else {
            for (k = rng[2][0]; k < rng[2][1]; k++) {
                for (i = rng[0][0]; i < rng[0][1]; i++) {
                    f->nnodes = 4;
                    f->nodes[0] = NODE_INDEX (i, j, k);
                    f->nodes[1] = NODE_INDEX (i, j, k+1);
                    f->nodes[2] = NODE_INDEX (i+1, j, k+1);
                    f->nodes[3] = NODE_INDEX (i+1, j, k);
                    f++;
                }
            }
        }
    }
    else {
        if ((k = rng[2][0]) == 0) {
            for (j = rng[1][0]; j < rng[1][1]; j++) {
                for (i = rng[0][0]; i < rng[0][1]; i++) {
                    f->nnodes = 4;
                    f->nodes[0] = NODE_INDEX (i, j, k);
                    f->nodes[1] = NODE_INDEX (i, j+1, k);
                    f->nodes[2] = NODE_INDEX (i+1, j+1, k);
                    f->nodes[3] = NODE_INDEX (i+1, j, k);
                    f++;
                }
            }
        }
        else {
            for (j = rng[1][0]; j < rng[1][1]; j++) {
                for (i = rng[0][0]; i < rng[0][1]; i++) {
                    f->nnodes = 4;
                    f->nodes[0] = NODE_INDEX (i, j, k);
                    f->nodes[1] = NODE_INDEX (i+1, j, k);
                    f->nodes[2] = NODE_INDEX (i+1, j+1, k);
                    f->nodes[3] = NODE_INDEX (i, j+1, k);
                    f++;
                }
            }
        }
    }
    return nfaces;
}



static int  structured_list ( Regn *mesh, Regn *reg, int *dim, int npts, int *pts, GridLocation_t location)
{
    int n, nn, nf, nfaces = 0;
    Face **faces, *f;
    static char *funcname = "structured_list";

    if (location != Vertex && location != IFaceCenter &&
        location != JFaceCenter && location != KFaceCenter) {
        int rng[3][2];
        for (n = 0; n < 3; n++)
            rng[n][0] = rng[n][1] = pts[n];
        for (nf = 1; nf < npts; nf++) {
            nn = nf * 3;
            for (n = 0; n < 3; n++) {
                if (rng[n][0] > pts[nn+n]) rng[n][0] = pts[nn+n];
                if (rng[n][1] < pts[nn+n]) rng[n][1] = pts[nn+n];
            }
        }
        nf = nn = 0;
        for (n = 0; n < 3; n++) {
            if (rng[n][0] == rng[n][1] &&
                (rng[n][0] == 1 || rng[n][0] == dim[n])) {
                if (rng[n][0] == 1)
                    nf++;
                else if (nn) {
                    nn = 4;
                    break;
                }
                else
                    nn = n + 1;
            }
        }
        if (!nn && nf == 1) {
            for (n = 0; n < 3; n++) {
                if (rng[n][0] == rng[n][1] && rng[n][0] == 1) {
                    nn = n + 1;
                    break;
                }
            }
        }
        if (nn == 1)
            location = IFaceCenter;
        else if (nn == 2)
            location = JFaceCenter;
        else if (nn == 3)
            location = KFaceCenter;
        else {
            strcpy (reg->errmsg,
                "unable to determine boundary - use [IJK]FaceCenter");
            return 0;
        }
    }

    faces = (Face **) MALLOC (funcname, npts * sizeof(Face *));

    if (location == Vertex) {
        for (n = 0; n < npts; n++) {
            nn = 3 * n;
            pts[n] = NODE_INDEX (pts[nn]-1, pts[nn+1]-1, pts[nn+2]-1);
        }
        for (n = 1; n < npts; n++) {
            if (pts[n] < pts[n-1]) {
                qsort (pts, npts, sizeof(int), compare_ints);
                break;
            }
        }

        for (f = mesh->faces, nf = 0; nf < mesh->nfaces; nf++, f++) {
            for (nn = 0; nn < f->nnodes; nn++) {
                if (!find_int (f->nodes[nn], npts, pts))
                    break;
            }
            if (nn == f->nnodes) {
                if (nfaces == npts) {
                    npts += 100;
                    faces = (Face **) REALLOC (funcname,
                        npts * sizeof(Face *), faces);
                }
                faces[nfaces++] = f;
            }
        }
    }

    else if (location == IFaceCenter) {
        for (n = 0; n < npts; n++) {
            nn = 3 * n;
            if ((pts[nn] == 1 || pts[nn] == dim[0]) &&
                pts[nn+1] > 0 && pts[nn+1] < dim[1] &&
                pts[nn+2] > 0 && pts[nn+2] < dim[2]) {
                nf = pts[nn+1]-1 + (pts[nn+2]-1) * (dim[1]-1);
                if (pts[nn] == dim[0])
                    nf += (dim[1]-1) * (dim[2]-1);
                faces[nfaces++] = &(mesh->faces[nf]);
            }
        }
    }

    else if (location == JFaceCenter) {
        int noff = 2 * (dim[1]-1) * (dim[2]-1);
        for (n = 0; n < npts; n++) {
            nn = 3 * n;
            if ((pts[nn+1] == 1 || pts[nn+1] == dim[1]) &&
                pts[nn] > 0 && pts[nn] < dim[0] &&
                pts[nn+2] > 0 && pts[nn+2] < dim[2]) {
                nf = noff + pts[nn]-1 + (pts[nn+2]-1) * (dim[0]-1);
                if (pts[nn+1] == dim[1])
                    nf += (dim[0]-1) * (dim[2]-1);
                faces[nfaces++] = &(mesh->faces[nf]);
            }
        }
    }

    else  {
        int noff = 2 * ((dim[1]-1) * (dim[2]-1) + (dim[0]-1) * (dim[2]-1));
        for (n = 0; n < npts; n++) {
            nn = 3 * n;
            if ((pts[nn+2] == 1 || pts[nn+2] == dim[2]) &&
                pts[nn] > 0 && pts[nn] < dim[0] &&
                pts[nn+1] > 0 && pts[nn+1] < dim[1]) {
                nf = noff + pts[nn]-1 + (pts[nn+1]-1) * (dim[0]-1);
                if (pts[nn+2] == dim[2])
                    nf += (dim[0]-1) * (dim[1]-1);
                faces[nfaces++] = &(mesh->faces[nf]);
            }
        }
    }

    if (nfaces) {
        reg->nfaces = nfaces;
        reg->faces = (Face *) MALLOC (funcname, nfaces * sizeof(Face));
        for (nf = 0; nf < nfaces; nf++) {
            reg->faces[nf].nnodes = faces[nf]->nnodes;
            for (nn = 0; nn < faces[nf]->nnodes; nn++)
                reg->faces[nf].nodes[nn] = faces[nf]->nodes[nn];
        }
    }
    else
        strcpy (reg->errmsg, "couldn't find any exterior faces");
    free (faces);
    return nfaces;
}


static int  structured_zone (int *dim)
{
    char name[33], d_name[33];
    int ni, nj, nk, nr, nf, n, i, j, k;
    int nints, nconns, nbocos, *pts, *d_pts;
    int range[6], d_range[6], transform[3];
    GridLocation_t location;
    GridConnectivityType_t type;
    PointSetType_t ptype, d_ptype;
    ZoneType_t d_ztype;
    DataType_t datatype;
    BCType_t bctype;
    Face *f;
    Zone *z = &zones[cgnszone-1];
    static char *funcname = "structured_zone";

    //zone_message ("finding exterior faces", NULL);
    if (CellDim == 3) {
        if (cg_n1to1 (cgnsfn, cgnsbase, cgnszone, &nints) ||
            cg_nconns (cgnsfn, cgnsbase, cgnszone, &nconns) ||
            cg_nbocos (cgnsfn, cgnsbase, cgnszone, &nbocos)) {

            return 1;
        }
#ifndef NO_MESH_BOUNDARIES
        z->nregs = nints + nconns + nbocos + 7;
#else
        z->nregs = nints + nconns + nbocos + 1;
#endif
    }
    else {
        nints = nconns = nbocos = 0;
        z->nregs = 1;
    }
    z->regs = (Regn *) MALLOC (funcname, z->nregs * sizeof(Regn));
    ni = dim[0] - 1;
    nj = dim[1] - 1;
    nk = dim[2] - 1;
    nr = 1;

    /* volume mesh boundaries */

    strcpy (z->regs[0].name, "<mesh>");
    z->regs[0].type = REG_MESH;
    for (n = 0; n < 3; n++)
        z->regs[0].data[n] = dim[n];

    if (CellDim == 2) {
        z->regs[0].dim = 2;
        z->regs[0].nfaces = ni * nj;
        z->regs[0].faces = f =
            (Face *) MALLOC (funcname, z->regs[0].nfaces * sizeof(Face));
        for (k = 0, j = 0; j < nj; j++) {
            for (i = 0; i < ni; i++) {
                f->nnodes = 4;
                f->nodes[0] = NODE_INDEX (i, j, k);
                f->nodes[1] = NODE_INDEX (i, j+1, k);
                f->nodes[2] = NODE_INDEX (i+1, j+1, k);
                f->nodes[3] = NODE_INDEX (i+1, j, k);
                f++;
            }
        }
#ifndef NO_CUTTING_PLANE
        nf = z->regs[0].nfaces;
        f = z->regs[0].faces;
        z->regs[0].elemtype = QUAD_4;
        z->regs[0].nelems = nf;
        z->regs[0].elems = (int *) MALLOC (funcname, 4 * nf * sizeof(int));
        for (n = 0, j = 0; j < nf; j++, f++) {
            for (i = 0; i < 4; i++)
                z->regs[0].elems[n++] = f->nodes[i];
        }
#endif
        return 0;
    }

    z->regs[0].dim = 3;

#ifndef NO_CUTTING_PLANE
    z->regs[0].elemtype = HEXA_8;
    z->regs[0].nelems = ni * nj * nk;
    z->regs[0].elems = pts =
        (int *) MALLOC (funcname, 8 * z->regs[0].nelems * sizeof(int));

    for (n = 0, k = 0; k < nk; k++) {
        for (j = 0; j < nj; j++) {
            for (i = 0; i < ni; i++) {
                pts[n++] = NODE_INDEX (i,   j,   k);
                pts[n++] = NODE_INDEX (i+1, j,   k);
                pts[n++] = NODE_INDEX (i+1, j+1, k);
                pts[n++] = NODE_INDEX (i,   j+1, k);
                pts[n++] = NODE_INDEX (i,   j,   k+1);
                pts[n++] = NODE_INDEX (i+1, j,   k+1);
                pts[n++] = NODE_INDEX (i+1, j+1, k+1);
                pts[n++] = NODE_INDEX (i,   j+1, k+1);
            }
        }
    }
#endif

    z->regs[0].nfaces = 2 * (nj * nk + ni * nk + ni * nj);
    z->regs[0].faces = f =
        (Face *) MALLOC (funcname, z->regs[0].nfaces * sizeof(Face));

    for (i = 0, k = 0; k < nk; k++) {
        for (j = 0; j < nj; j++) {
            f->nnodes = 4;
            f->nodes[0] = NODE_INDEX (i, j, k);
            f->nodes[1] = NODE_INDEX (i, j, k+1);
            f->nodes[2] = NODE_INDEX (i, j+1, k+1);
            f->nodes[3] = NODE_INDEX (i, j+1, k);
            f++;
        }
    }
    for (i = ni, k = 0; k < nk; k++) {
        for (j = 0; j < nj; j++) {
            f->nnodes = 4;
            f->nodes[0] = NODE_INDEX (i, j, k);
            f->nodes[1] = NODE_INDEX (i, j+1, k);
            f->nodes[2] = NODE_INDEX (i, j+1, k+1);
            f->nodes[3] = NODE_INDEX (i, j, k+1);
            f++;
        }
    }
    for (j = 0, k = 0; k < nk; k++) {
        for (i = 0; i < ni; i++) {
            f->nnodes = 4;
            f->nodes[0] = NODE_INDEX (i, j, k);
            f->nodes[1] = NODE_INDEX (i+1, j, k);
            f->nodes[2] = NODE_INDEX (i+1, j, k+1);
            f->nodes[3] = NODE_INDEX (i, j, k+1);
            f++;
        }
    }
    for (j = nj, k = 0; k < nk; k++) {
        for (i = 0; i < ni; i++) {
            f->nnodes = 4;
            f->nodes[0] = NODE_INDEX (i, j, k);
            f->nodes[1] = NODE_INDEX (i, j, k+1);
            f->nodes[2] = NODE_INDEX (i+1, j, k+1);
            f->nodes[3] = NODE_INDEX (i+1, j, k);
            f++;
        }
    }
    for (k = 0, j = 0; j < nj; j++) {
        for (i = 0; i < ni; i++) {
            f->nnodes = 4;
            f->nodes[0] = NODE_INDEX (i, j, k);
            f->nodes[1] = NODE_INDEX (i, j+1, k);
            f->nodes[2] = NODE_INDEX (i+1, j+1, k);
            f->nodes[3] = NODE_INDEX (i+1, j, k);
            f++;
        }
    }
    for (k = nk, j = 0; j < nj; j++) {
        for (i = 0; i < ni; i++) {
            f->nnodes = 4;
            f->nodes[0] = NODE_INDEX (i, j, k);
            f->nodes[1] = NODE_INDEX (i+1, j, k);
            f->nodes[2] = NODE_INDEX (i+1, j+1, k);
            f->nodes[3] = NODE_INDEX (i, j+1, k);
            f++;
        }
    }

#ifndef NO_MESH_BOUNDARIES
    for (n = 0; n < 3; n++) {
        range[n] = 1;
        range[n+3] = dim[n];
    }

    f = z->regs->faces;
    nf = nj * nk;
    strcpy (z->regs[nr].name, "<imin>");
    z->regs[nr].dim = 2;
    z->regs[nr].type = REG_BNDS;
    for (n = 0; n < 6; n++)
        z->regs[nr].data[n] = range[n];
    z->regs[nr].data[3] = 1;
    z->regs[nr].nfaces = nf;
    z->regs[nr].faces = (Face *) MALLOC (funcname, nf * sizeof(Face));
    for (n = 0; n < nf; n++, f++) {
        z->regs[nr].faces[n].nnodes = 4;
        for (i = 0; i < 4; i++)
            z->regs[nr].faces[n].nodes[i] = f->nodes[i];
    }
    nr++;

    strcpy (z->regs[nr].name, "<imax>");
    z->regs[nr].dim = 2;
    z->regs[nr].type = REG_BNDS;
    for (n = 0; n < 6; n++)
        z->regs[nr].data[n] = range[n];
    z->regs[nr].data[0] = dim[0];
    z->regs[nr].nfaces = nf;
    z->regs[nr].faces = (Face *) MALLOC (funcname, nf * sizeof(Face));
    for (n = 0; n < nf; n++, f++) {
        z->regs[nr].faces[n].nnodes = 4;
        for (i = 0; i < 4; i++)
            z->regs[nr].faces[n].nodes[i] = f->nodes[i];
    }
    nr++;

    nf = ni * nk;
    strcpy (z->regs[nr].name, "<jmin>");
    z->regs[nr].dim = 2;
    z->regs[nr].type = REG_BNDS;
    for (n = 0; n < 6; n++)
        z->regs[nr].data[n] = range[n];
    z->regs[nr].data[4] = 1;
    z->regs[nr].nfaces = nf;
    z->regs[nr].faces = (Face *) MALLOC (funcname, nf * sizeof(Face));
    for (n = 0; n < nf; n++, f++) {
        z->regs[nr].faces[n].nnodes = 4;
        for (i = 0; i < 4; i++)
            z->regs[nr].faces[n].nodes[i] = f->nodes[i];
    }
    nr++;

    strcpy (z->regs[nr].name, "<jmax>");
    z->regs[nr].dim = 2;
    z->regs[nr].type = REG_BNDS;
    for (n = 0; n < 6; n++)
        z->regs[nr].data[n] = range[n];
    z->regs[nr].data[1] = dim[1];
    z->regs[nr].nfaces = nf;
    z->regs[nr].faces = (Face *) MALLOC (funcname, nf * sizeof(Face));
    for (n = 0; n < nf; n++, f++) {
        z->regs[nr].faces[n].nnodes = 4;
        for (i = 0; i < 4; i++)
            z->regs[nr].faces[n].nodes[i] = f->nodes[i];
    }
    nr++;

    nf = ni * nj;
    strcpy (z->regs[nr].name, "<kmin>");
    z->regs[nr].dim = 2;
    z->regs[nr].type = REG_BNDS;
    for (n = 0; n < 6; n++)
        z->regs[nr].data[n] = range[n];
    z->regs[nr].data[5] = 1;
    z->regs[nr].nfaces = nf;
    z->regs[nr].faces = (Face *) MALLOC (funcname, nf * sizeof(Face));
    for (n = 0; n < nf; n++, f++) {
        z->regs[nr].faces[n].nnodes = 4;
        for (i = 0; i < 4; i++)
            z->regs[nr].faces[n].nodes[i] = f->nodes[i];
    }
    nr++;

    strcpy (z->regs[nr].name, "<kmax>");
    z->regs[nr].dim = 2;
    z->regs[nr].type = REG_BNDS;
    for (n = 0; n < 6; n++)
        z->regs[nr].data[n] = range[n];
    z->regs[nr].data[2] = dim[2];
    z->regs[nr].nfaces = nf;
    z->regs[nr].faces = (Face *) MALLOC (funcname, nf * sizeof(Face));
    for (n = 0; n < nf; n++, f++) {
        z->regs[nr].faces[n].nnodes = 4;
        for (i = 0; i < 4; i++)
            z->regs[nr].faces[n].nodes[i] = f->nodes[i];
    }
    nr++;
#endif

    /* 1 to 1 interfaces */

    for (n = 1; n <= nints; n++) {
        if (cg_1to1_read (cgnsfn, cgnsbase, cgnszone, n,
            name, d_name, range, d_range, transform)) {

            return 1;
        }
        strcpy (z->regs[nr].name, name);
        z->regs[nr].type = REG_1TO1;
        z->regs[nr].data[0] = 6;
        for (i = 0; i < 6; i++)
            z->regs[nr].data[i+1] = range[i];
        strcpy (z->regs[nr].d_name, d_name);
        if (structured_range (&z->regs[nr], dim, range, Vertex))
            z->regs[nr].dim = 2;
        nr++;
    }

    /* general connectivities */

    for (n = 1; n <= nconns; n++) {
        if (cg_conn_info (cgnsfn, cgnsbase, cgnszone, n, name,
            &location, &type, &ptype, &i, d_name, &d_ztype,
            &d_ptype, &datatype, &j)) {

            return 1;
        }
        pts = (int *) MALLOC (funcname, 3 * i * sizeof(int));
        d_pts = (int *) MALLOC (funcname, 3 * j * sizeof(int));
        k = cg_conn_read (cgnsfn, cgnsbase, cgnszone, n, pts, Integer, d_pts);
        free (d_pts);
        if (k) {
            free (pts);

            return 1;
        }
        strcpy (z->regs[nr].name, name);
        z->regs[nr].type = REG_CONN;
        z->regs[nr].data[0] = type;
        z->regs[nr].data[1] = location;
        z->regs[nr].data[2] = ptype;
        z->regs[nr].data[3] = i;
        if (ptype == PointRange) {
            z->regs[nr].data[3] = 6;
            for (j = 0; j < 6; j++)
                z->regs[nr].data[j+4] = pts[j];
        }
        strcpy (z->regs[nr].d_name, d_name);

        if ((type == Abutting1to1 || type == Abutting) &&
            (ptype == PointList || ptype == PointRange)) {
            if (ptype == PointRange)
                k = structured_range (&z->regs[nr], dim, pts, location);
            else
                k = structured_list (z->regs, &z->regs[nr], dim, i, pts,
                    location);
            if (k) z->regs[nr].dim = 2;
        }
        else
            strcpy (z->regs[nr].errmsg,
                "not Abutting or Abutting1to1 with PointList or PointRange");
        free (pts);
        nr++;
    }

    /* boundary conditions */

    for (n = 1; n <= nbocos; n++) {
        if (cg_boco_info (cgnsfn, cgnsbase, cgnszone, n, name,
            &bctype, &ptype, &i, transform, &j, &datatype, &k)) {

            return 1;
        }
        if (cg_goto (cgnsfn, cgnsbase, "Zone_t", cgnszone,
           "ZoneBC_t", 1, "BC_t", n, "end")) {

            return 1;
        }
        if (cg_gridlocation_read (&location))
            location = Vertex;
        pts = (int *) MALLOC (funcname, 3 * i * sizeof(int));
        d_pts = NULL;
        if (j) {
            k = datatype == RealSingle ? sizeof(float) : sizeof(double);
            d_pts = (int *) MALLOC (funcname, j * k);
        }
        k = cg_boco_read (cgnsfn, cgnsbase, cgnszone, n, pts, d_pts);
        if (NULL != d_pts) free (d_pts);
        if (k) {
            free (pts);

            return 1;
        }
        strcpy (z->regs[nr].name, name);
        z->regs[nr].type = REG_BOCO;
        z->regs[nr].data[0] = bctype;
        z->regs[nr].data[1] = location;
        z->regs[nr].data[2] = ptype;
        z->regs[nr].data[3] = i;
        if (ptype == PointRange || ptype == ElementRange) {
            z->regs[nr].data[3] = 6;
            for (j = 0; j < 6; j++)
                z->regs[nr].data[j+4] = pts[j];
        }

        if (ptype == PointRange || ptype == ElementRange)
            k = structured_range (&z->regs[nr], dim, pts, location);
        else if (ptype == PointList || ptype == ElementList)
            k = structured_list (z->regs, &z->regs[nr], dim, i, pts,
                location);
        else {
            k = 0;
            strcpy (z->regs[nr].errmsg, "invalid point set type");
        }
        if (k) z->regs[nr].dim = 2;
        free (pts);
        nr++;
    }

    z->nregs = nr;

#ifndef NO_CUTTING_PLANE
    for (nr = 0; nr < z->nregs; nr++) {
        if (z->regs[nr].dim == 2 && z->regs[nr].nfaces) {
            nf = z->regs[nr].nfaces;
            f = z->regs[nr].faces;
            z->regs[nr].elemtype = QUAD_4;
            z->regs[nr].nelems = nf;
            z->regs[nr].elems = (int *) MALLOC (funcname, 4 * nf * sizeof(int));
            for (n = 0, j = 0; j < nf; j++, f++) {
                for (i = 0; i < 4; i++)
                    z->regs[nr].elems[n++] = f->nodes[i];
            }
        }
    }
#endif

    return 0;
}


/*====================================================================
 * unstructured grid regions
 *====================================================================*/
static int  compare_faces (void *v1, void *v2)
{
    Face *f1 = (Face *)v1;
    Face *f2 = (Face *)v2;
    int i, id, k, nn, n1[4], n2[4];

    if (f1->nnodes != f2->nnodes)
        return (f1->nnodes - f2->nnodes);

    for (i = 0; i < f1->nnodes; i++) {
        id = f1->nodes[i];
        for (k = 0; k < i; k++) {
            if (n1[k] > id) {
                nn = n1[k];
                n1[k] = id;
                id = nn;
            }
        }
        n1[i] = id;
    }
    for (i = 0; i < f2->nnodes; i++) {
        id = f2->nodes[i];
        for (k = 0; k < i; k++) {
            if (n2[k] > id) {
                nn = n2[k];
                n2[k] = id;
                id = nn;
            }
        }
        n2[i] = id;
    }

    for (i = 0; i < f1->nnodes; i++) {
        if (n1[i] != n2[i])
            return (n1[i] - n2[i]);
    }
    return (0);
}

/*-------------------------------------------------------------------*/

static unsigned  hash_face ( void *v)
{
    Face *f = (Face *)v;
    int n;
    unsigned hash = 0;

    for (n = 0; n < f->nnodes; n++)
        hash += (size_t)f->nodes[n];
    return (hash);
}

/*-------------------------------------------------------------------*/

static size_t  get_faces (void *vf, void *vr)
{
    Face *f = (Face *)vf;
    Regn *r = (Regn *)vr;
    int n;

    r->faces[r->nfaces].id = f->id;
    r->faces[r->nfaces].nnodes = f->nnodes;
    for (n = 0; n < f->nnodes; n++)
        r->faces[r->nfaces].nodes[n] = f->nodes[n];
    free (f);
    (r->nfaces)++;
    return 1;
}

/*-------------------------------------------------------------------*/

static void  exterior_faces ( int nelems, ElementType_t elemtype, int *conn, int rind[2])
{
    int i, j, ne, nn, nf, ip, type = elemtype;
    Face face, *pf;

    for (nn = 0, ne = 0; ne < nelems - rind[1]; ne++) {
        if (elemtype == MIXED) type = conn[nn++];
        switch (type) {
            case TETRA_4:
            case TETRA_10:
                ip = 2;
                nf = 4;
                break;
            case PYRA_5:
            case PYRA_14:
                ip = 6;
                nf = 5;
                break;
            case PENTA_6:
            case PENTA_15:
            case PENTA_18:
                ip = 11;
                nf = 5;
                break;
            case HEXA_8:
            case HEXA_20:
            case HEXA_27:
                ip = 16;
                nf = 6;
                break;
            default:
                if (type < NODE || type > HEXA_27)
                    FATAL ("unknown element type int exterior_faces");
                ip = 0;
                nf = 0;
                break;
        }
        if (ne >= rind[0]) {
            for (j = 0; j < nf; j++) {
                face.nnodes = facenodes[ip+j][0];
                for (i = 0; i < face.nnodes; i++)
                    face.nodes[i] = conn[nn + facenodes[ip+j][i+1]] - 1;
                if (NULL == (pf = (Face *) HashFind (facehash, &face))) {
                    pf = (Face *) MALLOC ("exterior_faces", sizeof(Face));
                    pf->id = 0;
                    pf->nnodes = face.nnodes;
                    for (i = 0; i < face.nnodes; i++)
                        pf->nodes[i] = face.nodes[i];
                    (void) HashAdd (facehash, pf);
                }
                else {
                    HashDelete (facehash, pf);
                    free (pf);
                }
            }
        }
        cg_npe ((ElementType_t)type, &j);
        nn += j;
    }
}

/*-------------------------------------------------------------------*/
static void  element_faces (int istart, int nelems, ElementType_t elemtype, int *conn, int rind[2])

{
    int i, ne, nn, ip, type = elemtype;
    Face face, *pf;

    for (nn = 0, ne = 0; ne < nelems - rind[1]; ne++) {
        if (elemtype == MIXED) type = conn[nn++];
        switch (type) {
            case TRI_3:
            case TRI_6:
                ip = 3;
                break;
            case QUAD_4:
            case QUAD_8:
            case QUAD_9:
                ip = 4;
                break;
            default:
                if (type < NODE || type > HEXA_27)
                    FATAL ("unknown element type int element_faces");
                ip = 0;
                break;
        }
        if (ip && ne >= rind[0]) {
            face.nnodes = ip;
            for (i = 0; i < ip; i++)
                face.nodes[i] = conn[nn+i] - 1;

            if (NULL == (pf = (Face *) HashFind (facehash, &face))) {
                pf = (Face *) MALLOC ("element_faces", sizeof(Face));
                pf->id = istart + ne;
                pf->nnodes = face.nnodes;
                for (i = 0; i < face.nnodes; i++)
                    pf->nodes[i] = face.nodes[i];
                (void) HashAdd (facehash, pf);
            }
        }
        cg_npe ((ElementType_t)type, &i);
        nn += i;
    }
}

/*-------------------------------------------------------------------*/

static int  unstructured_region (int nregs, Regn *regs, Regn *r, PointSetType_t ptype, int nlist, int *list)

{
    int nf, nr, nn, maxfaces, nfaces;
    Face **faces, *f;
    static char *funcname = "unstructured_region";

    if (ptype == PointList || ptype == ElementList) {
        for (nn = 1; nn < nlist; nn++) {
            if (list[nn] < list[nn-1]) {
                qsort (list, nlist, sizeof(int), compare_ints);
                break;
            }
        }
        maxfaces = nlist;
    }
    else if (ptype == PointRange || ptype == ElementRange) {
        if (list[0] > list[1]) {
            nn = list[0];
            list[0] = list[1];
            list[1] = nn;
        }
        maxfaces = list[1] - list[0] + 1;
    }
    else {
        strcpy (r->errmsg, "invalid point set type");
        maxfaces = 0;
    }

    if (maxfaces < 1) return 0;
    faces = (Face **) MALLOC (funcname, maxfaces * sizeof(Face *));

    nfaces = 0;
    for (nr = 0; nr < nregs; nr++) {
        if (!regs[nr].nfaces) continue;
        f = regs[nr].faces;
        for (nf = 0; nf < regs[nr].nfaces; nf++, f++) {
            switch (ptype) {
                case PointList:
                    if (f->id) continue;
                    for (nn = 0; nn < f->nnodes; nn++) {
                        if (!find_int (f->nodes[nn]+1, nlist, list))
                            break;
                    }
                    if (nn == f->nnodes) break;
                    continue;
                case PointRange:
                    if (f->id) continue;
                    for (nn = 0; nn < f->nnodes; nn++) {
                        if (f->nodes[nn]+1 < list[0] ||
                            f->nodes[nn]+1 > list[1])
                            break;
                    }
                    if (nn == f->nnodes) break;
                    continue;
                case ElementList:
                    if (find_int (f->id, nlist, list)) break;
                    continue;
                case ElementRange:
                    if (f->id >= list[0] && f->id <= list[1]) break;
                    continue;
                default:
                    continue;
            }
            if (nfaces == maxfaces) {
                maxfaces += 100;
                faces = (Face **) REALLOC (funcname,
                    maxfaces * sizeof(Face *), faces);
            }
            faces[nfaces++] = f;
        }
    }
    if (nfaces) {
        r->nfaces = nfaces;
        r->faces = (Face *) MALLOC (funcname, nfaces * sizeof(Face));
        for (nf = 0; nf < nfaces; nf++) {
            r->faces[nf].id = faces[nf]->id;
            r->faces[nf].nnodes = faces[nf]->nnodes;
            for (nn = 0; nn < faces[nf]->nnodes; nn++)
                r->faces[nf].nodes[nn] = faces[nf]->nodes[nn];
        }
    }
    else
        strcpy (r->errmsg, "couldn't find any exterior faces");
    free (faces);
    return nfaces;
}


static int  unstructured_zone ()
{
    int ns, is, ie, nb, ip, nr, hasvol;
    int nsect, nints, nconns, nbocos, nrmlindex[3];
    int nelem, elemsize, *conn, *par;
    int range[6], d_range[6], transform[3], rind[2];
    GridLocation_t location;
    GridConnectivityType_t type;
    PointSetType_t ptype, d_ptype;
    ZoneType_t d_ztype;
    DataType_t datatype;
    BCType_t bctype;
    ElementType_t elemtype;
    char name[33], d_name[33];
    Zone *z = &zones[cgnszone-1];
    static char *funcname = "unstructured_zone";
    static char *dspmsg = "finding exterior faces for";

    if (cg_nsections (cgnsfn, cgnsbase, cgnszone, &nsect) ||
        cg_n1to1 (cgnsfn, cgnsbase, cgnszone, &nints) ||
        cg_nconns (cgnsfn, cgnsbase, cgnszone, &nconns) ||
        cg_nbocos (cgnsfn, cgnsbase, cgnszone, &nbocos)) {
         cg_get_error();
        return 1;
    }
    z->nregs = nsect + nints + nconns + nbocos;
    z->regs = (Regn *) MALLOC (funcname, z->nregs * sizeof(Regn));
    nr = 0;

    /* element sets */

    for (ns = 1; ns <= nsect; ns++) {
        if (cg_section_read (cgnsfn, cgnsbase, cgnszone, ns,
            name, &elemtype, &is, &ie, &nb, &ip) ||
            cg_ElementDataSize (cgnsfn, cgnsbase, cgnszone, ns, &elemsize)) {
                cg_get_error();
            return 1;
        }

        strcpy (z->regs[nr].name, name);
        z->regs[nr].type = REG_ELEM;
        z->regs[nr].data[0] = elemtype;
        z->regs[nr].data[1] = is;
        z->regs[nr].data[2] = ie;
        z->regs[nr].data[3] = 0;
        z->regs[nr].data[4] = 0;
        if (elemtype < TRI_3 || elemtype > MIXED)
            strcpy (z->regs[nr].errmsg, "invalid element type");
        else {
            int ne, n, i;
            nelem = ie - is + 1;
            conn = (int *) MALLOC (funcname, elemsize * sizeof(int));
            par = (int *) MALLOC (funcname, 4 * nelem * sizeof(int));
            if (cg_elements_read (cgnsfn, cgnsbase, cgnszone, ns, conn, par)) {
                free (conn);
                free (par);
                 cg_get_error() ;
                return 1;
            }
            free (par);

            if (cg_goto (cgnsfn, cgnsbase, "Zone_t", cgnszone,
                    "Elements_t", ns, "end") ||
                cg_rind_read (rind)) {
                rind[0] = rind[1] = 0;
            }
            z->regs[nr].data[3] = rind[0];
            z->regs[nr].data[4] = rind[1];

            /* check element indices */

            cg_npe (elemtype, &ip);
            for (n = 0, ne = 0; ne < nelem; ne++) {
                if (elemtype == MIXED) {
                    nb = conn[n++];
                    if (cg_npe ((ElementType_t)nb, &ip) || ip <= 0) {
                        strcpy(z->regs[nr].errmsg,
                            "unhandled element type found in MIXED");
                        break;
                    }
                }
                for (i = 0; i < ip; i++) {
                    if (conn[n] < 1 || conn[n] > z->nnodes) {
                        strcpy(z->regs[nr].errmsg, "invalid element index");
                        break;
                    }
                    n++;
                }
                if (i < ip) break;
            }
            hasvol = 0;
            if (ne == nelem) {
           /*     HASH face =   HashCreate (nelem > 1024 ? nelem / 3 : 127,compare_faces, hash_face);
           */   /*  facehash = &face;*/
                /*  facehash = face;*/
             facehash =   HashCreate (2047,compare_faces, hash_face);

                if (NULL == facehash)
                    FATAL ("face hash table creation failed");

                exterior_faces (nelem, elemtype, conn, rind);
                hasvol = HashSize (facehash);
                element_faces (is, nelem, elemtype, conn, rind);

                nb = HashSize (facehash);
                if (nb) {
                    z->regs[nr].dim = hasvol ? 3 : 2;
                    z->regs[nr].faces = (Face *) MALLOC (funcname,
                        nb * sizeof(Face));
                    HashList (facehash, get_faces, &z->regs[nr]);
                }
                else {
                    strcpy (z->regs[nr].errmsg,
                        "couldn't find any exterior faces");
                }
                HashDestroy (facehash, NULL);
            }
#ifndef NO_CUTTING_PLANE
            if (z->regs[nr].dim > 1) {
                z->regs[nr].elemtype = elemtype;
                z->regs[nr].nelems = nelem;
                z->regs[nr].elems = conn;
                /* fix element indexing */
                cg_npe (elemtype, &ip);
                for (n = 0, ne = 0; ne < nelem; ne++) {
                    if (elemtype == MIXED) {
                        nb = conn[n++];
                        cg_npe ((ElementType_t)nb, &ip);
                    }
                    for (i = 0; i < ip; i++) {
                        (conn[n])--;
                        n++;
                    }
                }
            }
            else
#endif
                free (conn);
        }
        nr++;
    }

    /* 1to1 connectivities */

    for (ns = 1; ns <= nints; ns++) {
        if (cg_1to1_read (cgnsfn, cgnsbase, cgnszone, ns,
            name, d_name, range, d_range, transform)) {
           cg_get_error();
            return 1;
        }

        strcpy (z->regs[nr].name, name);
        z->regs[nr].type = REG_1TO1;
        z->regs[nr].data[0] = 2;
        z->regs[nr].data[1] = range[0];
        z->regs[nr].data[2] = range[1];
        strcpy (z->regs[nr].d_name, d_name);
        if (unstructured_region (nsect, z->regs, &z->regs[nr],
            PointRange, 2, range)) z->regs[nr].dim = 2;
        nr++;
    }

    /* general connectivities */

    for (ns = 1; ns <= nconns; ns++) {
        if (cg_conn_info (cgnsfn, cgnsbase, cgnszone, ns, name,
            &location, &type, &ptype, &ip, d_name, &d_ztype,
            &d_ptype, &datatype, &is)) {
            cg_get_error();
            return 1;
        }

        conn = (int *) MALLOC (funcname, ip * sizeof(int));
        /* this should be donor index dim but a bug in versions prior to 2.2
           use cell dim instead */
        par = (int *) MALLOC (funcname, CellDim * is * sizeof(int));
        ie = cg_conn_read (cgnsfn, cgnsbase, cgnszone, ns, conn, Integer, par);
        free (par);
        if (ie) {
            free (conn);
            cg_get_error();
            return 1;
        }
        strcpy (z->regs[nr].name, name);
        z->regs[nr].type = REG_CONN;
        z->regs[nr].data[0] = type;
        z->regs[nr].data[1] = location;
        z->regs[nr].data[2] = ptype;
        z->regs[nr].data[3] = ip;
        if (ptype == PointRange) {
            z->regs[nr].data[4] = conn[0];
            z->regs[nr].data[5] = conn[1];
        }
        strcpy (z->regs[nr].d_name, d_name);

        if (type != Abutting && type != Abutting1to1) {
            strcpy(z->regs[nr].errmsg,
               "can only handle Abutting or Abutting1to1 currently");
        }
        else if (ptype != PointList && ptype != PointRange) {
            strcpy(z->regs[nr].errmsg,
               "point set type not PointList or PointRange");
        }
        else if (location != Vertex && location != CellCenter &&
                 location != FaceCenter) {
            strcpy(z->regs[nr].errmsg,
               "location not Vertex, CellCenter or FaceCenter");
        }
        else {
            if (location != Vertex)
                ptype = (ptype == PointRange ? ElementRange : ElementList);
            if (unstructured_region (nsect, z->regs, &z->regs[nr],
                ptype, ip, conn)) z->regs[nr].dim = 2;
        }
        free (conn);
        nr++;
    }

    /* boundary conditions */

    for (ns = 1; ns <= nbocos; ns++) {
        if (cg_boco_info (cgnsfn, cgnsbase, cgnszone, ns, name,
            &bctype, &ptype, &ip, nrmlindex, &is, &datatype, &ie)) {
       /*     cg_get_error();*/
            return 1;
        }

        if (cg_goto (cgnsfn, cgnsbase, "Zone_t", cgnszone,
           "ZoneBC_t", 1, "BC_t", ns, "end")) {
           /* cg_get_error();*/
            return 1;
        }
        if (cg_gridlocation_read (&location))
            location = Vertex;
        conn = (int *) MALLOC (funcname, ip * sizeof(int));
        par = NULL;
        if (is) {
            ie = datatype == RealSingle ? sizeof(float) : sizeof(double);
            par = (int *) MALLOC (funcname, is * ie);
        }
        ie = cg_boco_read (cgnsfn, cgnsbase, cgnszone, ns, conn, par);
        if (is) free (par);
        if (ie) {
            free (conn);
              cg_get_error();
            return 1;
        }
        strcpy (z->regs[nr].name, name);
        z->regs[nr].type = REG_BOCO;
        z->regs[nr].data[0] = bctype;
        z->regs[nr].data[1] = location;
        z->regs[nr].data[2] = ptype;
        z->regs[nr].data[3] = ip;
        if (ptype == PointRange || ptype == ElementRange) {
            z->regs[nr].data[4] = conn[0];
            z->regs[nr].data[5] = conn[1];
        }

        if ((ptype == PointRange || ptype == PointList) &&
            (location == FaceCenter || location == CellCenter))
            ptype = (ptype == PointRange ? ElementRange : ElementList);
        if (unstructured_region (nsect, z->regs, &z->regs[nr],
            ptype, ip, conn)) z->regs[nr].dim = 2;
        free (conn);
        nr++;
    }

    z->nregs = nr;
    return 0;
}


/*====================================================================
 * find region edges
 *====================================================================*/

/*-------------------------------------------------------------------*/

 static int  compare_edges ( void *v1, void *v2)

{
    Edge *e1 = (Edge *)v1;
    Edge *e2 = (Edge *)v2;

    if (e1->nodes[0] < e1->nodes[1]) {
        if (e2->nodes[0] < e2->nodes[1])
            return (e1->nodes[0] - e2->nodes[0]);
        return (e1->nodes[0] - e2->nodes[1]);
    }
    if (e2->nodes[0] < e2->nodes[1])
        return (e1->nodes[1] - e2->nodes[0]);
    return (e1->nodes[1] - e2->nodes[1]);
}

/*-------------------------------------------------------------------*/

  static unsigned  hash_edge ( void *v)
{
    Edge *e = (Edge *)v;

    return ((unsigned)(e->nodes[0] + e->nodes[1]));
}

/*-------------------------------------------------------------------*/

 static size_t  get_edges ( void *ve, void *vr)

{
    Edge *e = (Edge *)ve;
    Regn *r = (Regn *)vr;

    r->edges[r->nedges].nodes[0] = e->nodes[0];
    r->edges[r->nedges].nodes[1] = e->nodes[1];
    (r->nedges)++;
    return 1;
}

/*-------------------------------------------------------------------*/

 static void  extract_edges ( Regn *r)
{
    int n, i, j, k;
    Face *f;
    Edge edge, *ep;
    HASH edgehash;
    float dot;
    static char *funcname = "extract_edges";

    if (!r->nfaces) return;
     edgehash = HashCreate (r->nfaces, compare_edges, hash_edge);

    if (NULL == edgehash)
        FATAL ("edge hash table creation failed");
    for (f = r->faces, j = 0; j < r->nfaces; j++, f++) {
        for (i = 0, k = f->nnodes-1; i < f->nnodes; k = i++) {
            if (f->nodes[i] == f->nodes[k]) continue;
            if (f->nodes[i] < f->nodes[k]) {
                edge.nodes[0] = f->nodes[i];
                edge.nodes[1] = f->nodes[k];
            }
            else {
                edge.nodes[0] = f->nodes[k];
                edge.nodes[1] = f->nodes[i];
            }
            ep = (Edge *) HashFind (edgehash, &edge);
            if (NULL == ep) {
                ep = (Edge *) MALLOC (funcname, sizeof(Edge));
                ep->nodes[0] = edge.nodes[0];
                ep->nodes[1] = edge.nodes[1];
                ep->id = j;
                (void) HashAdd (edgehash, ep);
            }
            else {
                n = ep->id;
                dot = r->faces[n].normal[0] * f->normal[0] +
                      r->faces[n].normal[1] * f->normal[1] +
                      r->faces[n].normal[2] * f->normal[2];
                if (dot > EDGE_ANGLE) {
                    HashDelete (edgehash, ep);
                    free (ep);
                }
            }
        }
    }

    n = HashSize (edgehash);
    if (n) {
        r->nedges = 0;
        r->edges = (Edge *) MALLOC (funcname, n * sizeof(Edge));
        HashList (edgehash, get_edges, r);
    }
    HashDestroy (edgehash, NULL);
}


/*===================================================================
 * region manipulation
 *===================================================================*/

/*-------------------------------------------------------------------*/

  static float * compute_normal (Node n0, Node n1, Node n2, Node n3)
{
    int j;
    double xn, yn, zn, sn;
    double d1[3], d2[3];
    static float norm[3];

    /* triangle */

    if (NULL == n3) {
        for (j = 0; j < 3; j++) {
            d1[j] = n1[j] - n0[j];
            d2[j] = n2[j] - n0[j];
        }
        sn = 0.5;
    }

    /* quadrilateral */

    else {
        for (j = 0; j < 3; j++) {
            d1[j] = n2[j] - n0[j];
            d2[j] = n3[j] - n1[j];
        }
        sn = 1.0;
    }
    xn = sn * (d1[1] * d2[2] - d2[1] * d1[2]);
    yn = sn * (d1[2] * d2[0] - d2[2] * d1[0]);
    zn = sn * (d1[0] * d2[1] - d2[0] * d1[1]);
    sn = sqrt (xn * xn + yn * yn + zn * zn);
    if (sn == 0.0) sn = 1.0;
    norm[0] = (float)(xn / sn);
    norm[1] = (float)(yn / sn);
    norm[2] = (float)(zn / sn);
    return norm;
}

/*-------------------------------------------------------------------*/

static void  face_normals ( Zone *z, Regn *r)
{
    int i, j;
    Face *f;
    float *norm;

    for (f = r->faces, j = 0; j < r->nfaces; j++, f++) {
        norm = compute_normal(z->nodes[f->nodes[0]],
            z->nodes[f->nodes[1]], z->nodes[f->nodes[2]],
            f->nnodes == 4 ? z->nodes[f->nodes[3]] : NULL);
        for (i = 0; i < 3; i++)
            f->normal[i] = norm[i];
    }
}

/*-------------------------------------------------------------------*/

static void  bounding_box ( Zone *z, Regn *r)
{
    int i, j, n;

    if (r->nfaces) {
        Face *f = r->faces;
        for (j = 0; j < 3; j++)
            r->bbox[j][0] = r->bbox[j][1] = z->nodes[f->nodes[0]][j];
        for (n = 0; n < r->nfaces; n++, f++) {
            for (i = 0; i < f->nnodes; i++) {
                for (j = 0; j < 3; j++) {
                    if (r->bbox[j][0] > z->nodes[f->nodes[i]][j])
                        r->bbox[j][0] = z->nodes[f->nodes[i]][j];
                    if (r->bbox[j][1] < z->nodes[f->nodes[i]][j])
                        r->bbox[j][1] = z->nodes[f->nodes[i]][j];
                }
            }
        }
    }
    else if (r->nedges) {
        Edge *e = r->edges;
        for (j = 0; j < 3; j++)
            r->bbox[j][0] = r->bbox[j][1] = z->nodes[e->nodes[0]][j];
        for (n = 0; n < r->nedges; n++, e++) {
            for (i = 0; i < 2; i++) {
                for (j = 0; j < 3; j++) {
                    if (r->bbox[j][0] > z->nodes[e->nodes[i]][j])
                        r->bbox[j][0] = z->nodes[e->nodes[i]][j];
                    if (r->bbox[j][1] < z->nodes[e->nodes[i]][j])
                        r->bbox[j][1] = z->nodes[e->nodes[i]][j];
                }
            }
        }
    }
    else {
        for (j = 0; j < 3; j++)
            r->bbox[j][0] = r->bbox[j][1] = 0.0;
    }
}
/*-------------------------------------------------------------------*/

static void  get_bounds ( int all, float bbox[3][2])
{
    int nz, nr, n, first = 1;

    for (nz = 0; nz < nzones; nz++) {
        for (nr = 0; nr < zones[nz].nregs; nr++) {
            if (zones[nz].regs[nr].nfaces &&
               (all || zones[nz].regs[nr].mode)) {
                if (first) {
                    for (n = 0; n < 3; n++) {
                        bbox[n][0] = zones[nz].regs[nr].bbox[n][0];
                        bbox[n][1] = zones[nz].regs[nr].bbox[n][1];
                    }
                    first = 0;
                }
                else {
                    for (n = 0; n < 3; n++) {
                        if (bbox[n][0] > zones[nz].regs[nr].bbox[n][0])
                            bbox[n][0] = zones[nz].regs[nr].bbox[n][0];
                        if (bbox[n][1] < zones[nz].regs[nr].bbox[n][1])
                            bbox[n][1] = zones[nz].regs[nr].bbox[n][1];
                    }
                }
            }
        }
    }
    if (first) {
        for (n = 0; n < 3; n++) {
            bbox[n][0] = 0.0;
            bbox[n][1] = 1.0;
        }
    }
}




static int  CGNSopen (char *argv)

{
    int fn, nb, idum;
    static char buff[33];

    if (cgnsfn) {
        cg_close (cgnsfn);
        cgnsfn = 0;
    }
    if (cg_open (argv, MODE_READ, &fn)) {
           cg_get_error() ;
        return 0;
    }
    if (cg_nbases (fn, &nb)) {
           cg_get_error() ;
           cg_close (fn);
          return 0;
    }
    if (nb < 1) {
       qDebug()<< "no bases defined"<< "";
        cg_close (fn);
        return 0;
    }
    nbases = nb;
    for (nb = 1; nb <= nbases; nb++) {
        if (cg_base_read (fn, nb, buff, &idum, &idum)) {
               cg_get_error();
            cg_close (fn);
            return 0;
        }

    }
   qDebug()<<"base number"<<nb<< "";
    cgnsfn = fn;
    return 1;
}


static int  CGNSclose ()
{
    if (cgnsfn && cg_close (cgnsfn)) {
           cg_get_error();
        return 0;
    }
    cgnsfn = 0;
    free_all();

    return 1;
}



/*---------- CGNSbase ----------------------------------------------
 * set the CGNS base - return zones
 *------------------------------------------------------------------*/

static int  CGNSbase (int argv)
{
    int base, nz, sizes[9];
    char buff[33];

    if (!cgnsfn) {
       qDebug()<< "CGNS file not open"<< "";
        return 0;
    }

    base = argv + 1;
    if (base < 1 || base > nbases) {
       qDebug()<< "base number out of range"<< "";
        return 0;
    }

    cgnsbase = base;
    if (cg_base_read (cgnsfn, cgnsbase, BaseName, &CellDim, &PhyDim) ||
        cg_nzones (cgnsfn, cgnsbase, &nz)) {
        cg_get_error();
        return 0;
    }
     free_all ();
    if (CellDim < 2 || CellDim > 3 || PhyDim < 2 || PhyDim > 3) {
       qDebug()<<"CellDim and Phydim not 2 or 3"<< "";
        return 0;
    }
    nzones = nz;
   qDebug()<<"zone number"<<nz<< "";
    zones = (Zone *) MALLOC ("CGNSbase", nzones * sizeof(Zone));
    for (nz = 1; nz <= nzones; nz++) {
        if (cg_zone_read (cgnsfn, cgnsbase, nz, buff, sizes)) {
            cg_get_error();
            return 0;
        }
        strcpy (zones[nz-1].name, buff);

    }
    return 1;
}

/*---------- CGNSzone ----------------------------------------------
 * set the CGNS zone - return regions
 *------------------------------------------------------------------*/

static int  CGNSzone (  int argv)
{
    int i, j, n, zone, nr, nf, nn, ncoords;
    int sizes[9], rng[2][3], rind[6], nnodes;
//    int *tag;
    DataType_t datatype;
    ZoneType_t zonetype;
    Node *nodes;
    float *xyz;
    double rad, theta, phi;
    char buff[65], coordtype[4];
    Zone *z;
    static char *funcname = "CGNSzone";

    if (!cgnsfn) {
       qDebug()<<"CGNS file not open"<< "";
        return 0;
    }

    zone = argv + 1;
    if (zone < 1 || zone > nzones) {
       qDebug()<<"zone number out of range"<< "";
        return 0;
    }

    if (cg_zone_read (cgnsfn, cgnsbase, zone, buff, sizes) ||
        cg_zone_type (cgnsfn, cgnsbase, zone, &zonetype)) {
        cg_get_error();
        return 0;
    }
    if (zonetype == Structured) {
        for (n = 0; n < CellDim; n++) {
            if (sizes[n] < 2) {
               qDebug()<<"zone dimension < 2"<< "";
                return 0;
            }
        }
    }
    else if (zonetype != Unstructured) {
       qDebug()<<"invalid zone type"<< "";
        return 0;
    }
    cgnszone = zone;
    z = &zones[zone-1];

    /* get number of coordinates */

    if (cg_ncoords (cgnsfn, cgnsbase, cgnszone, &ncoords)) {
        cg_get_error();
        return 0;
    }
    if (ncoords < PhyDim) {
       qDebug()<<"less than PhyDim coordinates"<< "";
        return 0;
    }

    /* check for rind */

    if (cg_goto (cgnsfn, cgnsbase, "Zone_t", zone,"GridCoordinates_t", 1, "end")) {
        cg_get_error();
        return 0;
    }
    if ((i = cg_rind_read (rind)) != CG_OK) {
        if (i != CG_NODE_NOT_FOUND) {
            cg_get_error();
            return 0;
        }
        for (n = 0; n < 6; n++)
            rind[n] = 0;
    }

    /* get grid coordinate range */

    if (zonetype == Structured) {
        for (n = 0; n < 3; n++) {
            rng[0][n] = 1;
            rng[1][n] = 1;
        }
        nnodes = 1;
        for (n = 0; n < CellDim; n++) {
            rng[0][n] = rind[2*n] + 1;
            rng[1][n] = rind[2*n] + sizes[n];
            nnodes *= sizes[n];
        }
    }
    else {
        nnodes = sizes[0] + rind[0] + rind[1];
        rng[0][0] = 1;
        rng[1][0] = nnodes;
    }

    /* read the nodes */

    strcpy (coordtype, "   ");

    xyz = (float *) MALLOC (funcname, nnodes * sizeof(float));
    nodes = (Node *) MALLOC (funcname, nnodes * sizeof(Node));
    for (i = 1; i <= ncoords; i++) {
        if (cg_coord_info (cgnsfn, cgnsbase, cgnszone, i, &datatype, buff) ||
            cg_coord_read (cgnsfn, cgnsbase, cgnszone, buff,
                RealSingle, rng[0], rng[1], xyz)) {
            free (xyz);
            free (nodes);
            cg_get_error();
            return 0;
        }
        if (0 == strcmp (buff, "CoordinateX") ||
            0 == strcmp (buff, "CoordinateR"))
            j = 0;
        else if (0 == strcmp (buff, "CoordinateY") ||
                 0 == strcmp (buff, "CoordinateTheta"))
            j = 1;
        else if (0 == strcmp (buff, "CoordinateZ") ||
                 0 == strcmp (buff, "CoordinatePhi"))
            j = 2;
        else
            continue;
        if (coordtype[j] == ' ' || strchr ("XYZ", buff[10]) != NULL)
            coordtype[j] = buff[10];
        for (n = 0; n < nnodes; n++)
            nodes[n][j] = xyz[n];
    }
    free (xyz);
    if (0 == strncmp (coordtype, "RTZ", PhyDim)) {
        for (n = 0; n < nnodes; n++) {
            rad = nodes[n][0];
            theta = nodes[n][1];
            nodes[n][0] = (float)(rad * cos (theta));
            nodes[n][1] = (float)(rad * sin (theta));
        }
    }
    else if (0 == strcmp (coordtype, "RTP")) {
        for (n = 0; n < nnodes; n++) {
            rad = nodes[n][0];
            theta = nodes[n][1];
            phi = nodes[n][2];
            nodes[n][0] = (float)(rad * sin (theta) * cos (phi));
            nodes[n][1] = (float)(rad * sin (theta) * sin (phi));
            nodes[n][2] = (float)(rad * cos (theta));
        }
    }
    else if (strncmp (coordtype, "XYZ", PhyDim)) {
        free (nodes);
       qDebug()<<"unknown coordinate system"<< "";
        return 0;
    }

    z->nnodes = nnodes;
    z->nodes = nodes;

    /* build regions */

    if (zonetype == Structured) {
        if (structured_zone (  sizes))
            return 0;
    }
    else {
        if (unstructured_zone ())
            return 0;
    }

#ifdef NO_CUTTING_PLANE

    tag = (int *) MALLOC (funcname, nnodes * sizeof(int));
    for (n = 0; n < nnodes; n++)
        tag[n] = -1;

    /* tag nodes which are actually used */

    for (nn = 0, nr = 0; nr < z->nregs; nr++) {
        for (nf = 0; nf < z->regs[nr].nfaces; nf++) {
            for (n = 0; n < z->regs[nr].faces[nf].nnodes; n++) {
                i = z->regs[nr].faces[nf].nodes[n];
                if (tag[i] < 0)
                    tag[i] = nn++;
            }
        }
    }

    nodes = (Node *) MALLOC (funcname, nn * sizeof(Node));
    for (n = 0; n < nnodes; n++) {
        if (tag[n] >= 0) {
            j = tag[n];
            for (i = 0; i < 3; i++)
                nodes[j][i] = z->nodes[n][i];
        }
    }

    free(z->nodes);
    z->nodes = nodes;
    z->nnodes = nn;

    /* re-index region faces */

    for (nr = 0; nr < z->nregs; nr++) {
        for (nf = 0; nf < z->regs[nr].nfaces; nf++) {
            for (n = 0; n < z->regs[nr].faces[nf].nnodes; n++) {
                i = z->regs[nr].faces[nf].nodes[n];
                z->regs[nr].faces[nf].nodes[n] = tag[i];
            }
        }
    }

    free(tag);

#endif

    /* find region bounding box, edges and normals */


    for (nr = 0; nr < z->nregs; nr++) {
        if (z->regs[nr].nfaces) {
            bounding_box (z, &z->regs[nr]);
            face_normals (z, &z->regs[nr]);
            extract_edges (&z->regs[nr]);
        }
    }


    for (nr = 0; nr < z->nregs; nr++) {
        switch (z->regs[nr].type) {
            case REG_MESH:
                strcpy(buff, z->regs[nr].name);
                break;
            case REG_ELEM:
                sprintf(buff, "<Element Sections>/%s", z->regs[nr].name);
                break;
            case REG_1TO1:
                sprintf(buff, "<1to1 Connections>/%s", z->regs[nr].name);
                break;
            case REG_CONN:
                sprintf(buff, "<General Connections>/%s", z->regs[nr].name);
                break;
            case REG_BOCO:
                sprintf(buff, "<Boundary Conditions>/%s", z->regs[nr].name);
                break;
            case REG_BNDS:
                sprintf(buff, "<Mesh Boundaries>/%s", z->regs[nr].name);
                break;
        }

    }
    return 1;
}

/*---------- CGNSsummary -------------------------------------------
 * return info summary string
 *------------------------------------------------------------------*/

static int  CGNSsummary (int argc,int zone_n,int rz)
{
    int n, nz;
    char *p, buff[128];
    Regn *r;

    if (!cgnsfn) {
       qDebug()<<"CGNS file not open"<< "";
        return 0;
    }
    if (argc < 1 || argc > 3) {
       qDebug()<<"usage: CGNSsummary [zone [reg]]"<< "";
        return 0;
    }
    if (argc == 1) {
        sprintf (buff, "Physical Dim = %d, Cell Dim = %d", PhyDim, CellDim);
       qDebug()<<buff<< "";
        return 1;
    }

    nz = zone_n;
    if (nz < 0 || nz >= nzones) {
       qDebug()<<"zone number out of range"<< "";
        return 0;
    }


    if (argc == 2) {
        int sizes[9];
        ZoneType_t zonetype;
        if (cg_zone_read (cgnsfn, cgnsbase, nz+1, buff, sizes) ||
            cg_zone_type (cgnsfn, cgnsbase, nz+1, &zonetype)) {
            cg_get_error();
            return 0;
        }
//       qDebug()<< cg_ZoneTypeName(zonetype)<<"Zone"<< "";
     /*   Tcl_AppendResult (interp, cg_ZoneTypeName(zonetype),
            " Zone : ", NULL);*/
        if (zonetype == Unstructured) {
            sprintf (buff, "%d vertices, %d elements", sizes[0], sizes[1]);
//           qDebug()<<sizes[0]<<"vertices   and  "<<sizes[1]<<"elements"<< "";

        }
        else {
            sprintf (buff, "%d", sizes[0]);
            for (n = 1; n < CellDim; n++) {
                p = buff + strlen(buff);
                sprintf (p, " x %d", sizes[n]);
            }
//           qDebug()<<buff<<"vertices"<< "";

        }
        return 1;
    }

    n = rz;
    if (n < 0 || n >= zones[nz].nregs) {
       qDebug()<<"region number out of range"<< "";
        return 0;
    }
    r = &zones[nz].regs[n];

    switch (r->type) {
        case REG_MESH:
            if (CellDim == 2)
                sprintf (buff, "%d x %d", r->data[0], r->data[1]);
            else
                sprintf (buff, "%d x %d x %d", r->data[0], r->data[1],
                    r->data[2]);
       //    qDebug()<<"Structured Mesh : "<<buff<<"vertices"<< "";
          /*  Tcl_AppendResult (interp, "Structured Mesh : ",
                buff, " vertices", NULL);*/
            break;
        case REG_ELEM:
            sprintf (buff, "%d", r->data[2] - r->data[1] + 1);
         /*   Tcl_AppendResult (interp, cg_ElementTypeName(r->data[0]),
                " Element Set : ", buff, " elements", NULL);*/
            break;
        case REG_1TO1:
            if (r->data[0] == 2)
                sprintf (buff, "%d", r->data[2] - r->data[1] + 1);
            else
                sprintf (buff, "%d x %d x %d",
                    r->data[4] - r->data[1] + 1,
                    r->data[5] - r->data[2] + 1,
                    r->data[6] - r->data[3] + 1);
        /*    Tcl_AppendResult (interp, "1to1 Connection : PointRange ",
                buff, " -> ", r->d_name, NULL);*/
            break;
        case REG_CONN:
            if (r->data[2] == PointList || r->data[2] == ElementList)
                sprintf (buff, " %d", r->data[3]);
            else if (r->data[3] == 2)
                sprintf (buff, " %d", r->data[5] - r->data[4] + 1);
            else
                sprintf (buff, " %d x %d x %d",
                    r->data[7] - r->data[4] + 1,
                    r->data[8] - r->data[5] + 1,
                    r->data[9] - r->data[6] + 1);
          /*qDebug()<<
                cg_GridConnectivityTypeName(r->data[0])
               << " Connection : "<< cg_PointSetTypeName(r->data[2])
               << buff<< " -> "<<r->d_name ;*/
            break;
        case REG_BOCO:
            if (r->data[2] == PointList || r->data[2] == ElementList)
                sprintf (buff, " %d", r->data[3]);
            else if (r->data[3] == 2)
                sprintf (buff, " %d", r->data[5] - r->data[4] + 1);
            else
                sprintf (buff, " %d x %d x %d",
                    r->data[7] - r->data[4] + 1,
                    r->data[8] - r->data[5] + 1,
                    r->data[9] - r->data[6] + 1);
            /*Tcl_AppendResult (interp, cg_BCTypeName(r->data[0]),
                " Boundary Condition : ", cg_PointSetTypeName(r->data[2]),
                buff, NULL);*/
            break;
        case REG_BNDS:
            sprintf (buff, "%d x %d x %d",
                r->data[3] - r->data[0] + 1,
                r->data[4] - r->data[1] + 1,
                r->data[5] - r->data[2] + 1);
        //  qDebug()<< "Mesh Boundary : "<<buff<<
//                " vertices" << "";
            break;
    }
    return 1;
}


/*---------- CGNSgetbase -------------------------------------------
 * get base properties
 *------------------------------------------------------------------*/

static int  CGNSgetbase ()
{
   qDebug()<<"get base properties"<< "";
    char cd[16], pd[16], nz[16];

    if (!cgnsfn) {
       qDebug()<< "CGNS file not qDebug()"<< "";
        return 0;
    }
   /* if (argc != 1) {
       qDebug()<< "usage: CGNSgetbase"<< "";
        return 0;
    }*/


    sprintf (pd, "%d", PhyDim);
    sprintf (cd, "%d", CellDim);
    sprintf (nz, "%d", nzones);
   qDebug()<<"Base Name "<<BaseName
        <<"Physical Dim "<<pd
        <<"Cell Dim     "<<cd
            <<"Number Zones "<<nz<< "";
  /*  Tcl_AppendResult (interp,
          "Base Name   : ", BaseName,
        "\nPhysical Dim: ", pd,
        "\nCell Dim    : ", cd,
        "\nNumber Zones: ", nz, NULL);*/
    return 1;
}

/*---------- CGNSgetzone -------------------------------------------
 * get zone properties
 *------------------------------------------------------------------*/

 static int  CGNSgetzone (int argv)
{
    int n, ndim, zone, sizes[9], cnts[4];
    ZoneType_t zonetype;
    char *p, buff[65];
    static char *cntname[] = {
        "Element Sections",
        "1to1 Connections",
        "General Connections",
        "Boundary Conditions"
    };

    if (!cgnsfn) {
       qDebug()<< "CGNS file not qDebug()"<< "";
        return 0;
    }

    zone = argv + 1;
    if (zone < 1 || zone > nzones) {
       qDebug()<< "zone number out of range"<< "";
        return 0;
    }

    if (cg_zone_read (cgnsfn, cgnsbase, zone, buff, sizes) ||
        cg_zone_type (cgnsfn, cgnsbase, zone, &zonetype) ||
        cg_nsections (cgnsfn, cgnsbase, zone, &cnts[0]) ||
        cg_n1to1 (cgnsfn, cgnsbase, zone, &cnts[1]) ||
        cg_nconns (cgnsfn, cgnsbase, zone, &cnts[2]) ||
        cg_nbocos (cgnsfn, cgnsbase, zone, &cnts[3])) {
       qDebug()<< (char *)cg_get_error()<< "";
        return 0;
    }


 /*   Tcl_AppendResult (interp, "Zone Name          : ", buff,
        "\nType of Zone       : ", cg_ZoneTypeName(zonetype),
        "\nVertex Dimensions  : ", NULL);*/
   qDebug()<<"Zone Name          : "<<buff
        <<"\nType of Zone       : "<<cg_ZoneTypeName(zonetype)
        << "";

    ndim = zonetype == Unstructured ? 1 : CellDim;
    sprintf (buff, "%d", sizes[0]);
    for (n = 1; n < ndim; n++) {
        p = buff + strlen(buff);
        sprintf (p, " x %d", sizes[n]);
    }
 /*   Tcl_AppendResult (interp, buff, "\nCell Dimensions    : ", NULL);*/

    sprintf (buff, "%d", sizes[ndim]);
    for (n = 1; n < ndim; n++) {
        p = buff + strlen(buff);
        sprintf (p, " x %d", sizes[n+CellDim]);
    }
  qDebug()<< buff<< "";

    for (n = 0; n < 4; n++) {
        sprintf (buff, "\n%-19s: %d", cntname[n], cnts[n]);
       qDebug()<<buff<< "";
    }
    return 1;
}


/*---------- CGNSgetregion -----------------------------------------
 * get region properties
 *------------------------------------------------------------------*/

static int  CGNSgetregion (int zone_n ,int reg_n)

{
    int n;
    char  buff[128];
    Zone *z;
    Regn *r;

    if (!cgnsfn) {
      qDebug()<< "CGNS file not qDebug()"<< "";
        return 0;
    }

    n = zone_n;
    if (n < 0 || n >= nzones) {
      qDebug()<< "zone number out of range"<< "";
        return 0;
    }
    z = &zones[n];
    n = reg_n;
    if (n < 0 || n >= z->nregs) {
      qDebug()<< "region number out of range"<< "";
        return 0;
    }
    r = &z->regs[n];

    switch (r->type) {
        case REG_MESH:
            if (CellDim == 2)
                sprintf (buff, "%d x %d", r->data[0], r->data[1]);
            else
                sprintf (buff, "%d x %d x %d", r->data[0], r->data[1],
                    r->data[2]);
//          qDebug()<<
//                  "Region Name    : "<< r->name <<
//                "\nType of Region : Structured Mesh" <<
//                "\nMesh Dimensions: "<<buff<< "";

            break;
        case REG_ELEM:
            sprintf (buff, "%d -> %d", r->data[1], r->data[2]);
//         qDebug()<<
//                  "Region Name     : "<< r->name
//               << "\nType of Region  : Element Set"
//        /*       << "\nElement Set Type: "<<cg_ElementTypeName(r->data[0])*/
//                <<"\nElement Range   : "<<buff<< "";

            if (r->data[3] || r->data[4]) {
                sprintf (buff, "%d %d", r->data[3], r->data[4]);
              qDebug()<<
                    "\nRind Elements   : "<<buff<< "";

            }
            break;
        case REG_1TO1:
          qDebug()<<
                  "Region Name   : "<<r->name<<
                "\nType of Region: 1to1 Connectivity"<<
                "\nPoint Set Type: PointRange"<< "";
            if (r->data[0] == 2) {
                sprintf (buff, "%d -> %d", r->data[1], r->data[2]);
              qDebug()<<
                    "\nIndex Range   : "<<buff<< "";

            }
            else {
                sprintf (buff, "%d -> %d", r->data[1], r->data[4]);
              qDebug()<<
                    "\nI Index Range : "<<buff<< "";
                sprintf (buff, "%d -> %d", r->data[2], r->data[5]);
              qDebug()<<
                    "\nJ Index Range : "<<buff<< "";
                sprintf (buff, "%d -> %d", r->data[3], r->data[6]);
               qDebug()<<
                    "\nK Index Range : "<<buff<< "";
            }
            qDebug()<< "\nDonor Zone    : "<< r->d_name<< "";
            break;
        case REG_CONN:
          qDebug()<<
                  "Region Name      : "<< r->name
               << "\nType of Region   : General Connectivity"
         /*      << "\nConnectivity Type: "
               <<     cg_GridConnectivityTypeName(r->data[0]) */
               /* <<"\nGrid Location    : "<< cg_GridLocationName(r->data[1])
                <<"\nPoint Set Type   : "<< cg_PointSetTypeName(r->data[2])*/
                << "";

            if (r->data[2] == PointList || r->data[2] == ElementList) {
                sprintf (buff, "%d", r->data[3]);
               qDebug()<< "\nNumber of Points : "<<buff<< "";
            }
            else if (r->data[3] == 2) {
                sprintf (buff, "%d -> %d", r->data[4], r->data[5]);
              qDebug()<< "\nIndex Range      : "<<buff<< "";
            }
            else {
                sprintf (buff, "%d -> %d", r->data[4], r->data[7]);
               qDebug()<<
                    "\nI Index Range    : "<<buff<< "";
                sprintf (buff, "%d -> %d", r->data[5], r->data[8]);
              qDebug()<<
                    "\nJ Index Range    : "<<buff<< "";
                sprintf (buff, "%d -> %d", r->data[6], r->data[9]);
              qDebug()<<
                    "\nK Index Range    : "<<buff<< "";
            }
       qDebug()<< "\nDonor Zone       : "<< r->d_name<< "";
            break;
        case REG_BOCO:
          qDebug()<<
                  "Region Name     : "<<r->name<<
                "\nType of Region  : Boundary Condition" << "";
              /*  "\nType of BC      : ", cg_BCTypeName(r->data[0]),
                "\nGrid Location   : ", cg_GridLocationName(r->data[1]),
                "\nPoint Set Type  : ", cg_PointSetTypeName(r->data[2]),*/

            if (r->data[2] == PointList || r->data[2] == ElementList) {
                sprintf (buff, "%d", r->data[3]);
              qDebug()<< "\nNumber of Points: "<<buff<< "";
            }
            else if (r->data[3] == 2) {
                sprintf (buff, "%d -> %d", r->data[4], r->data[5]);
              qDebug()<< "\nIndex Range     : "<<buff<< "";
            }
            else {
                sprintf (buff, "%d -> %d", r->data[4], r->data[7]);
              qDebug()<<
                    "\nI Index Range   : "<< buff<< "";
                sprintf (buff, "%d -> %d", r->data[5], r->data[8]);
             qDebug()<<
                    "\nJ Index Range   : "<<buff<< "";
                sprintf (buff, "%d -> %d", r->data[6], r->data[9]);
              qDebug()<<
                    "\nK Index Range   : "<<buff<< "";
            }
            break;
        case REG_BNDS:
            strcpy (buff, r->name);
          qDebug()<<
                  "Region Name   : "<<r->name<<
                "\nType of Region: Mesh Boundary"<< "";
            sprintf (buff, "%d -> %d", r->data[0], r->data[3]);
         qDebug()<<
                "\nI Index Range : "<<buff<< "";
            sprintf (buff, "%d -> %d", r->data[1], r->data[4]);
        qDebug()<<
                "\nJ Index Range : "<<buff<< "";
            sprintf (buff, "%d -> %d", r->data[2], r->data[5]);
          qDebug()<<
                "\nK Index Range : "<<buff<< "";
            break;
    }
    return 1;
}

/*---------- CGNSregiondim -----------------------------------------
 * return dimension of a region
 *------------------------------------------------------------------*/

 static int  CGNSregiondim (  int zone_n,int reg_n )
{
   qDebug()<<"---------- dimension of a region---------"<< "";
    int n;
    char buff[16];
    Zone *z;

    if (!cgnsfn) {
      qDebug()<< "CGNS file not qDebug()"<< "";
        return 0;
    }

    n = zone_n;
    if (n < 0 || n >= nzones) {
      qDebug()<< "zone number out of range"<< "";
        return 0;
    }
    z = &zones[n];
    n = reg_n;
    if (n < 0 || n >= z->nregs) {
      qDebug()<< "region number out of range"<< "";
        return 0;
    }

    if (!z->regs[n].dim) {
        if (*(z->regs[n].errmsg))
          qDebug()<< z->regs[n].errmsg<< "";
        return 0;
    }
    sprintf (buff, "%d", z->regs[n].dim);
  qDebug()<< buff << "";
    return 1;
}

/*---------- CGNSbounds --------------------------------------------
 * get bounding box
 *------------------------------------------------------------------*/

 static void  transform_bounds (float m[16], float bb[3][2])
{
    int i, j;
    float x, y, z, bbox[3][2];

    x = m[0] * bb[0][0] + m[4] * bb[1][0] +  m[8] * bb[2][0] + m[12];
    y = m[1] * bb[0][0] + m[5] * bb[1][0] +  m[9] * bb[2][0] + m[13];
    z = m[2] * bb[0][0] + m[6] * bb[1][0] + m[10] * bb[2][0] + m[14];
    bbox[0][0] = bbox[0][1] = x;
    bbox[1][0] = bbox[1][1] = y;
    bbox[2][0] = bbox[2][1] = z;

    x = m[0] * bb[0][1] + m[4] * bb[1][0] +  m[8] * bb[2][0] + m[12];
    y = m[1] * bb[0][1] + m[5] * bb[1][0] +  m[9] * bb[2][0] + m[13];
    z = m[2] * bb[0][1] + m[6] * bb[1][0] + m[10] * bb[2][0] + m[14];
    if (bbox[0][0] > x) bbox[0][0] = x;
    if (bbox[0][1] < x) bbox[0][1] = x;
    if (bbox[1][0] > y) bbox[1][0] = y;
    if (bbox[1][1] < y) bbox[1][1] = y;
    if (bbox[2][0] > z) bbox[2][0] = z;
    if (bbox[2][1] < z) bbox[2][1] = z;

    x = m[0] * bb[0][0] + m[4] * bb[1][1] +  m[8] * bb[2][0] + m[12];
    y = m[1] * bb[0][0] + m[5] * bb[1][1] +  m[9] * bb[2][0] + m[13];
    z = m[2] * bb[0][0] + m[6] * bb[1][1] + m[10] * bb[2][0] + m[14];
    if (bbox[0][0] > x) bbox[0][0] = x;
    if (bbox[0][1] < x) bbox[0][1] = x;
    if (bbox[1][0] > y) bbox[1][0] = y;
    if (bbox[1][1] < y) bbox[1][1] = y;
    if (bbox[2][0] > z) bbox[2][0] = z;
    if (bbox[2][1] < z) bbox[2][1] = z;

    x = m[0] * bb[0][1] + m[4] * bb[1][1] +  m[8] * bb[2][0] + m[12];
    y = m[1] * bb[0][1] + m[5] * bb[1][1] +  m[9] * bb[2][0] + m[13];
    z = m[2] * bb[0][1] + m[6] * bb[1][1] + m[10] * bb[2][0] + m[14];
    if (bbox[0][0] > x) bbox[0][0] = x;
    if (bbox[0][1] < x) bbox[0][1] = x;
    if (bbox[1][0] > y) bbox[1][0] = y;
    if (bbox[1][1] < y) bbox[1][1] = y;
    if (bbox[2][0] > z) bbox[2][0] = z;
    if (bbox[2][1] < z) bbox[2][1] = z;

    x = m[0] * bb[0][0] + m[4] * bb[1][0] +  m[8] * bb[2][1] + m[12];
    y = m[1] * bb[0][0] + m[5] * bb[1][0] +  m[9] * bb[2][1] + m[13];
    z = m[2] * bb[0][0] + m[6] * bb[1][0] + m[10] * bb[2][1] + m[14];
    if (bbox[0][0] > x) bbox[0][0] = x;
    if (bbox[0][1] < x) bbox[0][1] = x;
    if (bbox[1][0] > y) bbox[1][0] = y;
    if (bbox[1][1] < y) bbox[1][1] = y;
    if (bbox[2][0] > z) bbox[2][0] = z;
    if (bbox[2][1] < z) bbox[2][1] = z;

    x = m[0] * bb[0][1] + m[4] * bb[1][0] +  m[8] * bb[2][1] + m[12];
    y = m[1] * bb[0][1] + m[5] * bb[1][0] +  m[9] * bb[2][1] + m[13];
    z = m[2] * bb[0][1] + m[6] * bb[1][0] + m[10] * bb[2][1] + m[14];
    if (bbox[0][0] > x) bbox[0][0] = x;
    if (bbox[0][1] < x) bbox[0][1] = x;
    if (bbox[1][0] > y) bbox[1][0] = y;
    if (bbox[1][1] < y) bbox[1][1] = y;
    if (bbox[2][0] > z) bbox[2][0] = z;
    if (bbox[2][1] < z) bbox[2][1] = z;

    x = m[0] * bb[0][0] + m[4] * bb[1][1] +  m[8] * bb[2][1] + m[12];
    y = m[1] * bb[0][0] + m[5] * bb[1][1] +  m[9] * bb[2][1] + m[13];
    z = m[2] * bb[0][0] + m[6] * bb[1][1] + m[10] * bb[2][1] + m[14];
    if (bbox[0][0] > x) bbox[0][0] = x;
    if (bbox[0][1] < x) bbox[0][1] = x;
    if (bbox[1][0] > y) bbox[1][0] = y;
    if (bbox[1][1] < y) bbox[1][1] = y;
    if (bbox[2][0] > z) bbox[2][0] = z;
    if (bbox[2][1] < z) bbox[2][1] = z;

    x = m[0] * bb[0][1] + m[4] * bb[1][1] +  m[8] * bb[2][1] + m[12];
    y = m[1] * bb[0][1] + m[5] * bb[1][1] +  m[9] * bb[2][1] + m[13];
    z = m[2] * bb[0][1] + m[6] * bb[1][1] + m[10] * bb[2][1] + m[14];
    if (bbox[0][0] > x) bbox[0][0] = x;
    if (bbox[0][1] < x) bbox[0][1] = x;
    if (bbox[1][0] > y) bbox[1][0] = y;
    if (bbox[1][1] < y) bbox[1][1] = y;
    if (bbox[2][0] > z) bbox[2][0] = z;
    if (bbox[2][1] < z) bbox[2][1] = z;

    for (i = 0; i < 3; i++)
        for (j = 0; j < 2; j++)
            bb[i][j] = bbox[i][j];
}


static int  CGNSbounds (int argc, char **argv)
{
    float bbox[3][2], matrix[16];
    int n, all = 0;
    char **args, sbb[65];

    if (argc > 1) all = atoi(argv[1]);
    get_bounds (all, bbox);
    if (argc > 2) {
       /* if (1 != Tcl_SplitList (interp, argv[2], &n, &args))
            return 0;*/
        for (n = 0; n < 16; n++)
            matrix[n] = (float) atof (args[n]);
     /*   Tcl_Free ((char *)args);*/
        transform_bounds (matrix, bbox);
    }
  /*  Tcl_ResetResult (interp);*/
    for (n = 0; n < 3; n++) {
        sprintf (sbb, "%f %f", bbox[n][0], bbox[n][1]);
       /* Tcl_AppendElement (interp, sbb);*/
    }
    return 1;
}




#endif // LOADCGNS_H
