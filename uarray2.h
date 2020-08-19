#ifndef UARRAY2
#define UARRAY2
#define T UArray2_T
typedef struct T *T;

// Does: makes a new UArray2_T with the specified width, height, and size.
// Expects: a positive non-zero width, height, and size.
//          Expects an output of non-null UArray2 pointer.
T UArray2_new(int col, int row, int size);

// Does: Frees the a given UARRAY2
// Expects: a non-null pointer to a UArray2. No Output
void UArray2_free(T *UArray2);

// Does: Returns the width of given UArray2
// Expects: a non-null pointer to a UArray2. Outputs a Int greater than 0
int UArray2_width(T UArray2);

// Does: Returns the height of given UArray2
// Expects: a non-null pointer to a UArray2. Outputs a Int greater than 0
int UArray2_height(T UArray2);

// Does: Returns the total size of given UArray2
// Expects: a non-null pointer to a UArray2. Outputs a Int greater than 0
int UArray2_size(T UArray2);

// Does: Returns a void* pointer to the element at given row and col
// Expects: a non-null pointer to a UArray2, and non-zero postive row and col
//          Outputs a non-null void*
void *UArray2_at(T UArray2, int col, int row);

void UArray2_map_col_major(T UArray2,
        void apply(int col, int row, T UArray2, void *p1, void *p2), void *cl);

// Does: Returns
void UArray2_map_row_major(T UArray2,
        void apply(int col, int row, T UArray2, void *p1, void *p2), void *cl);

//TEST FUNCTIONS
void print_int(T to_print);

#undef T
#endif
