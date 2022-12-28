#pragma once
#include "main.h"

typedef struct err {
    struct err* next;
    float       v_err[N];
} ERR;

typedef struct {
    int     count;
    ERR*    front;
    ERR*    rear;
    ERR*    pos;
} ELIST;

typedef struct tpl {
    struct  tpl*    next;
    int             count;
    float           data[Sl+Sr];
} TPL;

typedef struct {
    int     count;
    TPL*    front;
    TPL*    rear;
    TPL*    pos;
} LLIST;

typedef struct dp {
    struct  dp*     next;
    int             count;
    int             index[N];
    float           data[N];
    float           angle[N];
} DP;

typedef struct {
    int     count;
    DP*     front;
    DP*     rear;
    DP*     pos;
} DLIST;


ELIST*      create_elist( DLIST* data, DP* std );
bool        add_err( ELIST* list, DLIST* data, DP* std );

void        print_err( ELIST* list, FILE* fout );


LLIST*      create_list();
bool        add_data( LLIST* list, float* data, FILE* fo );
bool        add_tpl( LLIST* list, TPL* tpl );
void        print_tpl_count( LLIST* list );

void        delete_list( LLIST* list );

void        print_tpl_data( FILE* file, LLIST* list, int index );
int         find_largest_tpl( LLIST* list );
bool        move_tpl_at( LLIST* from, LLIST* to, int index );
bool        divide_tpl( LLIST* from, LLIST* to );

DLIST*      create_dplist();
void        delete_dplist( DLIST* list );
bool        add_dpdata( DLIST* list, int cnt, int* index, float* data );

void        print_dp_data( FILE* file, DP* dp );
void        print_dp_count( DLIST* list );

void        bubble_sort_dp( DLIST* list );
void        swap_dp( DP* dp1, DP* dp2 );


