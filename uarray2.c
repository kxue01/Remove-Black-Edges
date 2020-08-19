/*
                uarray2.c

        This is the impletemenation of the UArray_T

        Authors: Kenneth Xue (kxue01)
                Alyssa Rose (arose10)
*/
#include <stdio.h>
#include <stdlib.h>
#include <except.h>
#include "uarray2.h"
#include <uarray.h>
#include <assert.h>
#include <except.h>

#define T UArray2_T

struct UArray2_T {
        UArray_T spine;
        int col;
        int row;
        int size;
};

static Except_T Bad_Alloc = { "Could not allocate memory" };
static Except_T Bounds = {"Input out of bounds"};


// Does: makes a new UArray2_T with the specified col, row, and size.
// Expects: a positive non-zero col, row, and size.
//          Expects an output of non-null UArray2 pointer.
T UArray2_new(int col, int row, int size)
{
        T thisArr2 = malloc(sizeof(struct UArray2_T));
        if (thisArr2 == NULL) {
                RAISE(Bad_Alloc);
        }
        UArray_T spine = UArray_new(row, sizeof(UArray_T));

        thisArr2->spine = spine;
        thisArr2->row = row;
        thisArr2->col = col;
        thisArr2->size = size;

        UArray_T temp;
        UArray_T *temp1;
        for (int i = 0; i < row; i++) {
                temp = UArray_new(col, size);
                temp1 = UArray_at(thisArr2->spine, i);
                assert(sizeof(*temp1) == UArray_size(thisArr2->spine));
                *temp1 = temp;
        }
        return thisArr2;
}

// Does: Frees the a given UArray2
// Expects: a non-null pointer to a UArray2. No Output
void UArray2_free(T *UArray2)
{
  // overall pointer to the entire UArray2 is the spine UArray
 // iterate through the spine
        for (int i = 0; i < UArray2_height(*UArray2); i++) {
    // call UArray_free(ith elem of spine)
                UArray_T *thisArr = UArray_at(((*UArray2)->spine), i);
                assert(sizeof(*thisArr) == UArray_size((*UArray2)->spine));
                UArray_free(thisArr);
                thisArr = NULL;
        }
  // cal UArray free on the spine
        UArray_free(&((*UArray2)->spine));
  //*UArray2 = NULL;
        free(*UArray2);
}
// Does: Returns the width of given UArray2
// Expects: a non-null pointer to a UArray2. Outputs a Int greater than 0
int UArray2_width(T UArray2)
{
        return (UArray2->col);

}

// Does: Returns the height of given UArray2
// Expects: a non-null pointer to a UArray2. Outputs a Int greater than 0
int UArray2_height(T UArray2)
{
        return (UArray2->row);
}

// Does: Returns the total size of given UArray2
// Expects: a non-null pointer to a UArray2. Outputs a Int greater than 0
int UArray2_size(T UArray2)
{
        return (UArray2->size);
}

// Does: Returns a void* pointer to the element at given row and col
// Expects: a non-null pointer to a UArray2, and non-zero postive row and col
//          Outputs a non-null void*
void *UArray2_at(T UArray2, int col, int row)
{
        if (col >= UArray2->col || row >= UArray2->row) {
                RAISE(Bounds);
        }
        UArray_T *row_arr = UArray_at((UArray2->spine), row);
        assert(sizeof(*row_arr) == UArray_size(UArray2->spine));
        void* result = UArray_at(*row_arr, col);
        return result;
}

/*
Description: applies an apply function to the Uarray2_t
    pointed to by uarray2 by traversing over an entire
    col of the uarray2 before moving on to the next col of the arr
Input: A pointer to a UArray2_T, an apply function of type
    void, a pointer to a closure
Output: nothing
*/
void UArray2_map_col_major(T UArray2,
        void apply(int col, int row, T UArray2, void *p1, void *p2), void *cl)
{
        if (UArray2 == NULL) {
                assert(0);
        }

        for (int i = 0; i < UArray2->col; i++) {
                for (int j = 0; j < UArray2->row; j++) {
                        void* thisElem = UArray2_at(UArray2, i, j);
                        if (thisElem == NULL) {
                                assert(0);
                        }
                        apply(i, j, UArray2, thisElem, cl);
                }
        }
}

/*
Description: applies an apply function to the Uarray2_t
    pointed to by uarray2 by traversing over an entire
    row of the uarray2 before moving on to the next row of the arr
Input: A pointer to a UArray2_T, an apply function of type
    void, a pointer to a closure
Output: nothing
*/
void UArray2_map_row_major(T UArray2,
        void apply(int col, int row, T UArray2, void *p1, void *p2), void *cl)
{
        if (UArray2 == NULL) {
                assert(0);
        }

        for (int i = 0; i < UArray2->row; i++) {
                for (int j = 0; j < UArray2->col; j++) {
                        void* thisElem = UArray2_at(UArray2, j, i);
                        if (thisElem == NULL) {
                                assert(0);
                        }
                        apply(j, i, UArray2, thisElem, cl);
                }
        }
}
