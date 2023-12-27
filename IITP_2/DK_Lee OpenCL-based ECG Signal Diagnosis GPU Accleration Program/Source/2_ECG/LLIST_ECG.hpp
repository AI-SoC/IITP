#pragma once

#include "main.hpp"

typedef struct tpl {
    tpl( int    cnt, float* array ): count(cnt) {
        for( int i = 0; i < DLENGTH; i++ ) {
            data[i] = array[i];
        }
    }
    struct  tpl*    next;
    int             count;
    float           data[DLENGTH];
} TPL;

class   LLIST {
    public:
        // Constructor, destructor
        LLIST();
        virtual ~LLIST();

        // Modifier
        virtual void    clear();
        virtual bool    train_data( float** data );
        virtual bool    divide_tpl( LLIST& to );

        virtual bool    add_tpl( TPL* tpl );

        // Selector
        virtual void    printList( std::ostream& fout );
        virtual TPL*    get_front();
        virtual TPL*    get_rear();

    private:
        virtual int     find_largest_tpl();
        virtual bool    move_tpl_at( LLIST& to, int index );

        int     count;
        TPL*    front;
        TPL*    rear;
        TPL*    pos;
};





typedef struct dp {
    struct  dp* next;
    int         count;
    int         index[NN];
    float       data[NN];
} DP;

class   DLIST : public LLIST {
    public:
        // Constructor
        DLIST();
        ~DLIST();

        // Modifier
        virtual void    clear();
        virtual bool    dynamic_programming( LLIST& list );
        virtual void    bubble_sort_dp();

        // Selector
        virtual void    printList( std::ostream& fout );
        int             get_count() {   return count;   };
        virtual void    get_index( int* array );
        virtual void    get_data( float* array );

        
    private:
        virtual bool    add_dpdata( int cnt, int* index, float* data );
        virtual void    swap_dp( DP* dp1, DP* dp2 );

        int     count;
        DP*     front;
        DP*     rear;
        DP*     pos;
};



