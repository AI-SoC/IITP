////>`define A    uARM_DTOP.uCORE_TOP.r_PC

//>`define REG  uARM_DTOP.uCORE_TOP.uREG

/*>
initial
begin
    $display("\tTest Module: sort");
end
>*/

void swap( int* xp, int* yp )
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void bsort( int *arr, int n )
{
    int i, j;
    for( i = 0; i < n - 1; i++ )
        for( j = 0; j < n - i - 1; j++ )
            if( arr[j] > arr[j + 1] )
                swap( &arr[j], &arr[j + 1] );
}

void ssort( int *arr, int n)
{
	int i, j, tmp;
	for( i = 0; i < n - 1; i++ )
	{
		tmp = i;
		for( j = i+1; j < n; j++ )
			if( arr[tmp] >= arr[j] ) tmp = j;
		swap( &arr[j], &arr[tmp] );
	}
}

void qsort( int *arr, int l, int r ) {
	int left = l, right = r;
	int pivot = arr[(l+r)/2];

	while( left <= right ) {
		while( arr[left] < pivot ) left++;
		while( arr[right] > pivot ) right--;

		if( left <= right ) {
			swap( &arr[left], &arr[right] );
			left++;
			right--;
		}
	}
	if( l < right ) qsort( arr, l, right );
	if( r > left ) qsort( arr, left, r );
}

void heapify( int *arr, int here, int size )
{
	int left = here * 2 + 1;
	int right = here * 2 + 2;
	int max = here;
	if( left < size && arr[left] > arr[max] )
		max = left;
	if( right < size && arr[right] > arr[max] )
		max = right;
	if( max != here ) {
		swap( &arr[here], &arr[max] );
		heapify( arr, max, size );
	}
}

void buildHeap( int *arr, int size )
{
	int i, j;
	for( i = size / 2 - 1; i >= 0; i-- )
		heapify( arr, i, size );
}

void hsort( int *arr, int size ) {
	int treeSize;
	buildHeap( arr, size );
	for( treeSize = size - 1; treeSize >= 0; treeSize-- )
	{
		swap( &arr[0], &arr[treeSize] );
		heapify( arr, 0, treeSize );
	}
}

int main( void )
{
    __asm(".syntax unified\n\t");

	int n;
    int arr1[] = { 5, 1, 4, 2, 8, 10, 3, 15, 7, 20, 35 };
	n = sizeof( arr1 ) / sizeof( arr1[0] ); 
    bsort( arr1, n );
    __asm("NOP\n\t");
    //{
    //    $display(" 1. Bubble sort started!!");
    //}
    __asm("NOP\n\t");
    /*{
    `ifdef SORT_MODE
        force `REG.r_R0 = 32'h0000;
    `endif
    }*/


    int arr2[] = { 5, 1, 4, 2, 8, 10, 3, 15, 7, 20, 35 };
	n = sizeof( arr2 ) / sizeof( arr2[0] );
	ssort( arr2, n );
    __asm("NOP\n\t");
    //{
    //    $display(" 2. Selection sort started!!");
    //}


    int arr3[] = { 5, 1, 4, 2, 8, 10, 3, 15, 7, 20, 35 };
	n = sizeof( arr3 ) / sizeof( arr3[0] );
	qsort( arr3, 0, n-1 );
	__asm("NOP\n\t");
    //{
    //    $display(" 3. Quick sort started!!");
    //}


    int arr4[] = { 5, 1, 4, 2, 8, 10, 3, 15, 7, 20, 35 };
	n = sizeof( arr4 ) / sizeof( arr4[0] );
	hsort( arr4, n );
    __asm("NOP\n\t");
    //{
    //    $display( " 4. Heap sort started!!");
    //}

	return 0;
}
