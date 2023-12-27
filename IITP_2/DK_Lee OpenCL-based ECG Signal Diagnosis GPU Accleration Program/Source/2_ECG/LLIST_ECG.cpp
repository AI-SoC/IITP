#include "LLIST_ECG.hpp"

using namespace std;


void    copyArray( float* src, float* dst, int n ) {
/*{{{*/
    int i;
    for( i = 0; i < n; i++ ) {
        dst[i] = src[i];
    }
/*}}}*/
}

float   pearson( float* src1, float* src2, int n ) {
/*{{{*/
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
/*}}}*/
}


LLIST::LLIST() {
/*{{{*/
    //cout << "Creating LLIST.." << endl;

    count   = 0;
    front   = NULL;
    rear    = NULL;
    pos     = NULL;
/*}}}*/
}

LLIST::~LLIST() {
/*{{{*/
    //cout << "Deleting LLIST.." << endl;
    clear();
/*}}}*/
}

void    LLIST::clear() {
/*{{{*/
    //cout << "Clearing LLIST.." << endl;

    TPL*    delNode = front;
    TPL*    next;

    while( delNode != NULL ) {
        next    = delNode->next;
        delete delNode;
        delNode = next;
    }
/*}}}*/
}


bool    LLIST::train_data( float** data ) {
/*{{{*/
    float*  array   = (*data);

    if( count == 0 ) {
        // Create new tpl
        TPL*    new_tpl = (TPL*)malloc(sizeof(TPL));
        if( !new_tpl ) {
            return false;
        }

        new_tpl->next   = NULL;
        new_tpl->count  = 1;
        copyArray( array, new_tpl->data, SL+SR );

        front     = new_tpl;
        rear      = new_tpl;
        count++;

        return true;
    }
    else {
        // Compare with other template
        float   diff_max    = 0;
        int     diff_pos    = 0;

        float   diff_sig;       // signal difference
        float   diff_p;         // P wave difference
        float   diff;
        int     iter_i;

        pos = front;
        for( int i = 0; i < count; i++ ) {
            diff_sig    = pearson( array, pos->data, SL+SR );
            diff_p      = pearson( array, pos->data, PLENGTH );
            diff        = (diff_sig + diff_p) / 2;

            if( diff > diff_max ) {
                diff_max    = diff;
                diff_pos    = i;
            }
            pos = pos->next;
        }

        // Threshold check
        if( diff_max > THR_UPD ) {  // Update template
            iter_i = 0;
            pos = front;
            while( iter_i != diff_pos ) {
                pos = pos->next;
                iter_i++;
            }

            // weight average
            for( int i = 0; i < SL+SR; i++ ) {
                pos->data[i]  = (pos->count * pos->data[i] + array[i])
                                    / (pos->count + 1);
            }
            (pos->count)++;

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
            copyArray( array, new_tpl->data, SL+SR );

            // insert rear
            iter_i = 1;
            pos = front;
            while( iter_i != (count) ) {
                pos = pos->next;
                iter_i++;
            }

            pos->next = new_tpl;
            rear      = new_tpl;
            (count)++;

            return true;
        }
    }

    return false;
/*}}}*/
}

bool    LLIST::move_tpl_at( LLIST& to, int index ) {
/*{{{*/
    TPL*    target  = NULL;

    if( index == 0 ) {
        // Del node at 'this'
        target  = front;
        front   = front->next;
        count--;

        // Add node at 'to'
        if( !to.add_tpl( target ) ) {
            cout << "Add TPL error" << endl;
            return false;
        }

        return true;
    }
    else if( index == (count - 1) ) {   // rear
        int     iter_i  = 0;
        pos = front;

        while( iter_i != index ) {
            target  = pos;
            pos     = pos->next;
            iter_i++;
        }
        rear            = target;
        target->next    = NULL;
        count--;

        if( !to.add_tpl( pos ) ) {
            cout << "Add TPL error" << endl;
            return false;
        }

        return true;
    }
    else {
        int iter_i = 0;
        pos = front;
        while( iter_i != index ) {
            target  = pos;             // prev
            pos     = pos->next;    // target
            iter_i++;
        }

        target->next    = pos->next;
        count--;

        if( !to.add_tpl( pos ) ) {
            cout << "Add TPL error" << endl;
            return false;
        }

        return true;
    }

    return false;
/*}}}*/
}

int     LLIST::find_largest_tpl() {
/*{{{*/
    int pLar    = 0;
    int pCnt    = 0;

    pos = front;

    for( int i = 0; i < count; i++ ) {
        if( pos->count > pCnt ) {
            pCnt    = pos->count;
            pLar    = i;
        }
        pos = pos->next;
    }

    return pLar;
/*}}}*/
}

bool    LLIST::divide_tpl( LLIST& to ) {
/*{{{*/
    if( !move_tpl_at( to, find_largest_tpl() ) ) {
        cout << "TPL move error" << endl;
        return false;
    }

    int     iter_i      = 0;
    float*  cmp_data    = to.front->data;
    TPL*    next_tpl    = front;

    float   diff;

    while( next_tpl != NULL ) {
        diff    = (fabs(pearson( cmp_data, next_tpl->data, SL+SR )) +
                    fabs(pearson( cmp_data, next_tpl->data, PLENGTH ))) / 2;

        if( diff > THR_SEP ) {
            next_tpl    = next_tpl->next;
            if( !move_tpl_at( to, iter_i ) ) {
                cout << "TPL move error" << endl;
                return false;
            }
        }
        else {
            next_tpl    = next_tpl->next;
            iter_i++;
        }
    }

    return true;
/*}}}*/
}

bool    LLIST::add_tpl( TPL* tpl ) {
/*{{{*/
    if( count == 0 ) {
        front       = tpl;
        rear        = tpl;
        tpl->next   = NULL;
        count++;

        return true;
    }
    else {
        rear->next  = tpl;
        rear        = tpl;
        tpl->next   = NULL;

        return true;
    }

    return false;
/*}}}*/
}


void  LLIST::printList( std::ostream& fout ) {
/*{{{*/
    pos = front;
    
    fout.setf(ios::fixed);
    fout.precision(2);
    while( pos != NULL ) {
        for( int i = 0; i < DLENGTH; i++ ) {
            fout << i << " " << pos->data[i] << endl;
        }
        fout << endl;
        pos = pos->next;
    }
    fout.unsetf(ios::fixed);
/*}}}*/
}

TPL*    LLIST::get_front() {
/*{{{*/
    return  front;
/*}}}*/
}

TPL*    LLIST::get_rear() {
/*{{{*/
    return  rear;
/*}}}*/
}


float p2l_dist( float x1, float x2, float xp, float y1, float y2, float yp ) {
/*{{{*/
    float   lx1 = x1/FS;
    float   lx2 = x2/FS;
    float   lxp = xp/FS;
    float   dist;
    
    dist = fabs( (lx2-lx1)*(y1-yp) - (lx1-lxp)*(y2-y1) )
            / sqrtf( powf((lx2-lx1),2) + powf((y2-y1),2) );

    return dist;
/*}}}*/
}

float func_d( float* data, int from, int to, float dist_weight ) {
/*{{{*/
    int     i;
    float   temp = 0;

    for( i = from; i <= to; i++ ) {
        temp += fabs( p2l_dist( from, to, i, data[from], data[to], data[i] ) );
    }

    return dist_weight * temp;
/*}}}*/
}


DLIST::DLIST() {
/*{{{*/
    count   = 0;
    front   = NULL;
    rear    = NULL;
    pos     = NULL;
/*}}}*/
}

DLIST::~DLIST() {
/*{{{*/
    clear();
/*}}}*/
}

void    DLIST::clear() {
/*{{{*/
    DP*     delNode = front;
    DP*     next;

    while( delNode != NULL ) {
        next    = delNode->next;
        delete delNode;
        delNode = next;
    }
/*}}}*/
}

bool    DLIST::dynamic_programming( LLIST& list ) {
/*{{{*/
    int     len     = SL+SR;
    int     vertex  = NN - 2;
    int     i, j, dep, col, tmp_pos;

    float*  data;
    float   tmp, tmp_min;

    float*  base_mat    = (float*) malloc(sizeof(float)*N_BIT);
    float*  cost_tmp    = (float*) malloc(sizeof(float)*N_BIT);

    float   cost_mat[vertex][len];
    int     way_mat [vertex][len];

    float   output  [NN];
    int     way     [NN];

    for( i = 0; i < vertex; i++ ) {
        for( j = 0; j < len; j++ ) {
            cost_mat[i][j] = INF;
            way_mat [i][j] = 0;
        }
    }

    float   diff_max, diff_min;
    float   dist_max, dist_weight;

    TPL*    pos = list.get_front();
    
    while( pos->next != NULL ) {
        data    = pos->data;

        // Set distance weight
        diff_max    = -1;
        diff_min    = INF;

        for( i = 0; i < len; i++ ) {
            tmp = p2l_dist( 0, len-1, i, data[0], data[len-1], data[i] );
            if( diff_max < tmp )
                diff_max = tmp;
            else if( diff_min > tmp )
                diff_min = tmp;
        }

        dist_max = (diff_max - diff_min) *
            sqrtf( powf(len-1, 2) + powf(data[len-1] - data[0], 2) );
        dist_weight = floorf(65535/dist_max);

        // Cost temp matrix
        cost_tmp[0] = 0;
        for( i = 1; i < N_BIT; i++ ) {
            cost_tmp[i] = func_d( data, 0, i+1, dist_weight );
        }

        for( col = 3; col < len; col++ ) {
            // Update base matrix
            for( i = max(1, col-N_BIT); i < col; i++ ) {
                base_mat[i-col+N_BIT] = func_d( data, i-1, col-1, dist_weight );
                //printf( "col = %d, i = %d, base[%d] = %f\n", col, i, i-col+N_BIT, base_mat[i-col+N_BIT] );
            }

            for( dep = max(1, col-len+vertex); dep < min(vertex, col-1); dep++ ) {
                tmp_min = INF;
                tmp_pos = dep+1;

                if( dep == 1 ) {
                    for( i = max(dep+1, col-N_BIT); i < col; i++ ) {
                        if( i < 34 ) {
                            tmp = cost_tmp[i-2] + base_mat[i-col+N_BIT];

                            if( tmp < tmp_min ) {
                                tmp_min = tmp;
                                tmp_pos = i-1;
                            }
                        }
                    }
                }
                else {
                    for( i = max(dep+1, col-N_BIT); i < col; i++ ) {
                        tmp = cost_mat[dep-2][i-1] + base_mat[i-col+N_BIT];

                        if( tmp < tmp_min ) {
                            tmp_min = tmp;
                            tmp_pos = i-1;
                        }
                    }
                }

                cost_mat[dep-1][col-1] = tmp_min;
                way_mat [dep-1][col-1] = tmp_pos;
            } // dep loop
        } // col loop
        
        col = len;
        dep = vertex;

        for( i = max(1, col-N_BIT); i < col; i++ ) {
            base_mat[i-col+N_BIT] = func_d( data, i, col, dist_weight );
        }

        tmp_min = INF;
        tmp_pos = dep+1;
        for( i = max(dep+1, col-N_BIT); i < col; i++ ) {
            tmp = cost_mat[dep-2][i-1] + base_mat[i-col+N_BIT];

            if( tmp < tmp_min ) {
                tmp_min = tmp;
                tmp_pos = i-1;
            }            
        }
        way_mat[dep-1][col-1] = tmp_pos;
    
        // Select shortest way
        way[0] = 0;
        way[NN-1] = len-1;

        for( i = vertex; i > 0; i-- )
            way[i] = way_mat[i-1][way[i+1]];

        for( i = 0; i < NN; i++ )
            output[i] = data[ way[i] ];

        if( !add_dpdata( pos->count, way, output ) )
            return -1;

        pos = pos->next;
    }

    return true;
/*}}}*/
}



bool    DLIST::add_dpdata( int cnt, int* index, float* data ) {
/*{{{*/
    int     i;
    DP*     new_dp  = (DP*)malloc(sizeof(DP));
    if( !new_dp )    return false;

    new_dp->next    = NULL;
    new_dp->count   = cnt;
    for( i = 0; i < NN; i++ ) {
        new_dp->index[i] = index[i];
    }
    copyArray( data, new_dp->data, NN );

    if( count == 0 ) {
        front   = new_dp;
        rear    = new_dp;
        count++;

        return true;
    }
    else {
        i   = 1;

        pos = front;
        while( i != count ) {
            pos = pos->next;
            i++;
        }

        pos->next   = new_dp;
        rear        = new_dp;
        count++;

        return true;
    }

    return false;
/*}}}*/
}

void    DLIST::swap_dp( DP* dp1, DP* dp2 ) {
/*{{{*/
    int     i;
    int     count;
    int     index;
    float   data;

    count       = dp1->count;
    dp1->count  = dp2->count;
    dp2->count  = count;

    for( i = 0; i < NN; i++ ) {
        index           = dp1->index[i];
        dp1->index[i]   = dp2->index[i];
        dp2->index[i]   = index;
        data            = dp1->data [i];
        dp1->data [i]   = dp2->data [i];
        dp2->data [i]   = data;
    }
/*}}}*/
}

void    DLIST::bubble_sort_dp() {
/*{{{*/
    DP* nxt;
    int i, j;

    for( i = (count-1); i > 0; i-- ) {
        pos = front;
        nxt = pos->next;

        for( j = 0; j < i; j++ ) {
            if( pos->count < nxt->count ) {
                swap_dp( pos, nxt );
            }
            pos = nxt;
            nxt = nxt->next;
        }
    }
/*}}}*/
}

void    DLIST::printList( std::ostream& fout ) {
/*{{{*/
    pos = front;
    
    fout.setf(ios::fixed);
    fout.precision(2);
    while( pos != NULL ) {
        for( int i = 0; i < NN; i++ ) {
            fout << pos->index[i] << " " << pos->data[i] << endl;
        }
        fout << endl;
        pos = pos->next;
    }
    fout.unsetf(ios::fixed);
/*}}}*/
}

void    DLIST::get_index( int* array ) {
/*{{{*/
    int     iter_i  = 0;

    pos = front;
    
    while( pos != NULL ) {
        for( int i = 0; i < NN; i++ ) {
            array[iter_i] = pos->index[i];
            iter_i++;
        }
        pos = pos->next;
    }
/*}}}*/
}

void    DLIST::get_data( float* array ) {
/*{{{*/
    int     iter_i  = 0;
    pos = front;

    while( pos != NULL ) {
        for( int i = 0; i < NN; i++ ) {
            array[iter_i] = pos->data[i];
            iter_i++;
        }
        pos = pos->next;
    }
/*}}}*/
}
