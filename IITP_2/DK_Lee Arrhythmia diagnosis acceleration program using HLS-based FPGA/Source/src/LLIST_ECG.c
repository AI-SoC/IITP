#include "LLIST_ECG.h"

void    copyArray( float* src, float* dst, int n );
float   pearson( float* src1, float* src2, int n );

///////////////////////////////////////////////////
LLIST* create_list() {
    LLIST*  list    = (LLIST*)malloc(sizeof(LLIST));

    if( list ) {
        list->front = NULL;
        list->rear  = NULL;
        list->pos   = NULL;
        list->count = 0;

        return list;
    }

    return NULL;
}

bool add_data( LLIST* list, float* data, FILE* fo ) {
    // data offset
    int     i;
    float   mean = 0;
    for( i = 0; i < Sl+Sr; i++ ) {
        mean += data[i];
    }
    mean /= Sl+Sr;
    for( i = 0; i < Sl+Sr; i++ ) {
        data[i] -= mean;
    }
    for( i = 0; i < Sl+Sr; i++ ) {
        fprintf( fo, "%f, ", data[i] );
    }
    fprintf( fo, "\n" );

    if( list->count == 0 ) {
        // Create new tpl
        TPL*    new_tpl = (TPL*)malloc(sizeof(TPL));
        if( !new_tpl ) {
            return false;
        }

        new_tpl->next   = NULL;
        new_tpl->count  = 1;
        copyArray( data, new_tpl->data, Sl+Sr );

        list->front     = new_tpl;
        list->rear      = new_tpl;
        (list->count)++;

        return true;
    }
    else {
        // Compare with other template
        float   diff_max    = 0;
        int     diff_pos    = 0;

        float   diff_sig;    // signal difference
        float   diff_p;      // P wave difference
        float   diff;
        int     iter_i;
        list->pos = list->front;
        for( i = 0; i < list->count; i++ ) {
            diff_sig    = pearson( data, list->pos->data, Sl+Sr );
            diff_p      = pearson( data, list->pos->data, PLength );
            diff        = (diff_sig + diff_p) / 2;

            if( diff > diff_max ) {
                diff_max    = diff;
                diff_pos    = i;
            }
            list->pos = list->pos->next;
        }

        // Threshold check
        if( diff_max > THR_UPD ) {  // Update template
            iter_i = 0;
            list->pos = list->front;
            while( iter_i != diff_pos ) {
                list->pos = list->pos->next;
                iter_i++;
            }

            // weight average
            for( i = 0; i < Sl+Sr; i++ ) {
                list->pos->data[i]  = (list->pos->count * list->pos->data[i] + data[i])
                                    / (list->pos->count + 1);
            }
            (list->pos->count)++;

            return true;
        }
        else {  // Create new template
            TPL*    new_tpl = (TPL*)malloc(sizeof(TPL));
            if( !new_tpl ) {
                printf( "Create TPL failed\n" );
                return false;
            }

            new_tpl->next   = NULL;
            new_tpl->count  = 1;
            copyArray( data, new_tpl->data, Sl+Sr );

            // insert rear
            iter_i = 1;
            list->pos = list->front;
            while( iter_i != (list->count) ) {
                list->pos = list->pos->next;
                iter_i++;
            }

            list->pos->next = new_tpl;
            list->rear      = new_tpl;
            (list->count)++;

            return true;
        }
        
        return false;
    }
    return false;
}

bool add_tpl( LLIST* list, TPL* tpl ) {
    if( list->count == 0 ) {
        list->front = tpl;
        list->rear  = tpl;
        tpl->next   = NULL;
        (list->count)++;

        return true;
    }
    else {
        list->rear->next    = tpl;
        list->rear          = tpl;
        tpl->next           = NULL;
        (list->count)++;

        return true;
    }

    return false;
}

void print_tpl_count( LLIST* list ) {
    list->pos = list->front;
    int i = 0;
    while( list->pos != NULL ) {
        printf( "tpl %d's count = %d\n",i, list->pos->count );
        list->pos = list->pos->next;
        i++;
    }
}

void delete_list( LLIST* list ) {
    TPL*    del = list->front;
    TPL*    next;

    while( del != NULL ) {
        next    = del->next;
        free( del );
        del  = next;
    }

    free( list );
}

void print_tpl_data( FILE* file, LLIST* list, int index ) {
    int i;
#ifdef  ONE
    int iter_i = 0;
    list->pos = list->front;
    while( iter_i != index ) {
        list->pos   = list->pos->next;
        iter_i++;
    }

    for( i = 0; i < Sl+Sr; i++ ) {
        fprintf( file, "%d %f\n", i, list->pos->data[i] );
    }
#else
    for( i = 0; i < Sl+Sr; i++ ) {
        list->pos = list->front;
        fprintf( file, "%d ", i );
        while( list->pos != NULL ) {
            fprintf( file, "%f ", list->pos->data[i] );
            list->pos = list->pos->next;
        }
        fprintf( file, "\n" );
    }
#endif
}


int find_largest_tpl( LLIST* list ) {
    int i;
    int plargest = 0;
    int count = 0;
    list->pos = list->front;
    
    for( i = 0; i < list->count; i++ ) {
        if( list->pos->count > count ) {
            count = list->pos->count;
            plargest = i;
        }
        list->pos = list->pos->next;
    }

    return plargest;
}

bool move_tpl_at( LLIST* from, LLIST* to, int index ) {
    TPL* target = NULL;
    if( index == 0 ) {
        // Delete node
        target = from->front;
        from->front = target->next;
        (from->count)--;

        // Add tpl
        if( !add_tpl( to, target ) ) {
            printf( "Add tpl failed\n" );
            return false;
        }

        return true;
    }
    else if( index == (from->count - 1) ) { // rear
        int iter_i = 0;
        from->pos = from->front;
        while( iter_i != index ) {
            target = from->pos;             // prev
            from->pos = from->pos->next;    // target
            iter_i++;
        }
        from->rear      = target;
        target->next    = NULL;
        (from->count)--;

        if( !add_tpl( to, from->pos ) ) {
            printf( "Add tpl failed\n" );
            return false;
        }

        return true;
    }
    else {
        int iter_i = 0;
        from->pos = from->front;
        while( iter_i != index ) {
            target = from->pos;             // prev
            from->pos = from->pos->next;    // target
            iter_i++;
        }

        target->next    = from->pos->next;
        (from->count)--;

        if( !add_tpl( to, from->pos ) ) {
            printf( "Add tpl failed\n" );
            return false;
        }

        return true;
    }
}

bool divide_tpl( LLIST* from, LLIST* to ) {
    if( !move_tpl_at( from, to, find_largest_tpl( from ) ) ) {
        printf( "TPL move error\n" );
        return false;
    }

    int iter_i = 0;
    float* cmp_data = to->front->data;
    TPL*    next_tpl = from->front;
    
    float   diff;

    while( next_tpl != NULL ) {
        diff    = (fabs(pearson( cmp_data, next_tpl->data, Sl+Sr )) +
                    fabs(pearson( cmp_data, next_tpl->data, PLength ))) / 2;

        printf( "diff %f\n", diff );
        if( diff > THR_SEP ) {
            next_tpl = next_tpl->next;
            if( !move_tpl_at( from, to, iter_i ) ) {
                printf( "TPL move error\n" );
                return false;
            }
        }
        else {
            next_tpl = next_tpl->next;
            iter_i++;
        }
    }


    return true;
}


///////////////////////////////////////////////////
void copyArray( float* src, float* dst, int n ) {
    int i;
    for( i = 0; i < n; i++ ) {
        dst[i] = src[i];
    }
}


float   pearson( float* src1, float* src2, int n ) {
    int     i;
    float   mean1   = 0;
    float   mean2   = 0;

    // Calculate mean
    for( i = 0; i < n; i++ ) {
        mean1 += src1[i];
        mean2 += src2[i];
    }
    mean1 /= n;
    mean2 /= n;

    float   pearson = 0;
    float   std1    = 0;
    float   std2    = 0;
    for( i = 0; i < n; i++ ) {
        pearson = pearson + ((src1[i] - mean1) * (src2[i] - mean2));
        std1   += powf( src1[i] - mean1, 2 );
        std2   += powf( src2[i] - mean2, 2 );
    }
    pearson = pearson / (sqrtf(std1) * sqrtf(std2));

    return pearson;
}

///////////////////////////////////////////////////////

DLIST* create_dplist() {
    DLIST*  list    = (DLIST*)malloc(sizeof(DLIST));

    if( list ) {
        list->front = NULL;
        list->rear  = NULL;
        list->pos   = NULL;
        list->count = 0;

        return list;
    }

    return NULL;
}

void delete_dplist( DLIST* list ) {
    DP*     del  = list->front;
    DP*     next;

    while( del != NULL ) {
        next    = del->next;
        free( del );
        del  = next;
    }

    free( list );
}

bool add_dpdata( DLIST* list, int cnt, int* index, float* data ) {
    int     i;

    DP* new_dp  = (DP*)malloc(sizeof(DP));
    if( !new_dp )
        return false;

    new_dp->next    = NULL;
    new_dp->count   = cnt;
    for( i = 0; i < N; i++ ) {
        new_dp->index[i] = index[i];
        new_dp->angle[i] = INF;
    }
    copyArray( data, new_dp->data, N );
    
    if( list->count == 0 ) {
        list->front = new_dp;
        list->rear  = new_dp;
        (list->count)++;

        return true;
    }
    else {
        i = 1;
        list->pos = list->front;
        while( i != (list->count) ) {
            list->pos = list->pos->next;
            i++;
        }

        list->pos->next = new_dp;
        list->rear      = new_dp;
        (list->count)++;

        return true;
    }

    return false;
}


void print_dp_data( FILE* file, DP* dp ) {
    int i;
    
    for( i = 0; i < N; i++ ) {
        fprintf( file, "%d %f\n", dp->index[i], dp->data[i] );
    }
}

void print_dp_count( DLIST* list ) {
    list->pos = list->front;
    int i = 0;
    while( list->pos != NULL ) {
        printf( "dp %d's count = %d\n",i, list->pos->count );
        list->pos = list->pos->next;
        i++;
    }
}

void swap_dp( DP* dp1, DP* dp2 ) {
    int     i;
    int     count;
    int     index;
    float   data;
    float   angle;

    count       = dp1->count;
    dp1->count  = dp2->count;
    dp2->count  = count;

    for( i = 0; i < N; i++ ) {
        index           = dp1->index[i];
        dp1->index[i]   = dp2->index[i];
        dp2->index[i]   = index;
        data            = dp1->data [i];
        dp1->data [i]   = dp2->data [i];
        dp2->data [i]   = data;
        angle           = dp1->angle[i];
        dp1->angle[i]   = dp2->angle[i];
        dp2->angle[i]   = angle;
    }
}

void bubble_sort_dp( DLIST* list ) {
    DP* pos;
    DP* nxt;
    int i, j;

    for( i = (list->count)-1; i > 0; i-- ) {
        pos = list->front;
        nxt = pos->next;
        
        for( j = 0; j < i; j++ ) {
            if( pos->count < nxt->count ) {
                swap_dp( pos, nxt );
            }
            pos = nxt;
            nxt = nxt->next;
        }
    }
}

/*
void insertion_sort( DLIST* list ) {
    DP*     tmp;
    DP*     cmp;
    int     i, j;
    for( i = 1; i < list->count; i++ ) {
        tmp = get_dp_at( list, i );

        cmp = list->front;
        for( j = 0; j < i; j++ ) {
            if( cmp->count < tmp->count ) {
                set_dp_at( list, j, tmp );
                break;
            }
            cmp = cmp->next;
        }
        if( j == i ) {
            set_dp_at( list, j, tmp );
        }
    }
}

void set_dp_at( DLIST* list, int index, DP* dp ) {
    int i = 0;
    DP* pre;

    if( index == 0 ) {
        dp->next = list->front;
        list->front = dp;
        (list->count)++;
    }
    else if( index == (list->count) ) {
        list->rear->next = dp;
        list->rear       = dp;
        (list->count)++;
    }
    else {
        list->pos = list->front;
        while( i != index ) {
            pre       = list->pos;
            list->pos = list->pos->next;
        }

        dp->next = list->pos;
        pre->next = dp;
        (list->count)++;
    }
}

DP* get_dp_at( DLIST* list, int index ) {
    DP* tmp;
    DP* pre;
    int i;

    tmp = list->front;
    for( i = 0; i < index; i++ ) {
        pre = tmp;
        tmp = tmp->next;
    }

    if( index == 0 ) {
        list->front = tmp->next;
        (list->count)--;
    }
    else if( index == (list->count) ) {
        list->rear = pre;
        pre->next = NULL;
        (list->count)--;
    }
    else {
        pre->next = tmp->next;
        (list->count)--;
    }

    tmp->next = NULL;
}
*/


///////////////////////////////////////////////////
ELIST*  create_elist( DLIST* data, DP* std ) {
    ELIST*  list    = (ELIST*)malloc(sizeof(ELIST));

    if( list ) {
        list->front = NULL;
        list->rear  = NULL;
        list->pos   = NULL;
        list->count = 0;

        add_err( list, data, std );

        return list;
    }

    return NULL;
}

void calc_v_err( ERR* err, float* std, float* data ) {
    int i;
    for( i = 0; i < N; i++ ) {
        err->v_err[i]   = std[i] - data[i];
    }
}

bool add_err( ELIST* list, DLIST* data, DP* std ) {
    
    data->pos       = data->front;
    while( data->pos != NULL ) {
        ERR*    new_err = (ERR*) malloc(sizeof(ERR));
        if( !new_err )
            return false;

        new_err->next   = NULL;
        calc_v_err( new_err, std->data, data->pos->data );

        if( list->count == 0 ) {
            list->front = new_err;
            list->rear  = new_err;
            (list->count)++;
        }
        else {
            list->rear->next    = new_err;
            list->rear          = new_err;
            (list->count)++;
        }

        data->pos = data->pos->next;
    }

    return true;
}

void print_err( ELIST* list, FILE* fout ) {
    int i;


    for( i = 0; i < N; i++ ) {
        list->pos = list->front;
        fprintf( fout, "%d ", i );
        while( list->pos != NULL ) {
            fprintf( fout, "%f ", list->pos->v_err[i] );
            list->pos = list->pos->next;
        }
        fprintf( fout, "\n" );
    }
    /*
    while( list->pos != NULL ) {
        for( i = 0; i < N; i++ ) {
            printf( "%f ", list->pos->v_err[i] );
        }
        printf( "\n" );

        list->pos = list->pos->next;
    }
    */
}

