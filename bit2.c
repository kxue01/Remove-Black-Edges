/*
                bit2.c

        This is the impletemenation of the bit2_t

        Authors: Kenneth Xue (kxue01)
                Alyssa Rose (arose10)
*/
#include <stdio.h>
#include <stdlib.h>
#include <except.h>
#include <uarray.h>
#include <bit2.h>
#include <bit.h>
#include <assert.h>

#define T Bit2_T

struct Bit2_T {
        UArray_T spine;
        int height;
        int width;
};

static Except_T Bad_Alloc = { "Could not allocate memory" };
static Except_T Bounds = {"Input out of bounds"};

/*
Description: Creates a new Bit2 array of size height * width
Input: the height (int) and width (int) of the new Bit2_T
Output: a pointer to a Bit2_T array
*/
T Bit2_new(int width, int height)
{
        T thisBit2 = malloc(sizeof(struct Bit2_T));
        if (thisBit2 == NULL) {
                RAISE(Bad_Alloc);
        }
        thisBit2->height = height;
        thisBit2->width = width;

        UArray_T spine = UArray_new(height, sizeof(Bit_T));
        thisBit2->spine = spine;

        Bit_T temp;
        Bit_T *temp1;

        for (int i = 0; i < height; i++) {
                temp = Bit_new(width);
                temp1 = UArray_at(thisBit2->spine, i);
                *temp1 = temp;
        }

        return thisBit2;
}

/*
Description: Returns the height of the Bit2_T array pointed
        to by the bit bitarr
Input: pointer to Bit2_T bitarr
Output: (int) the height of the bit array
*/
int Bit2_height(T bitarr)
{
        return bitarr->height;
}

/*
Description: Returns the width of the Bit2_T array
        pointed to by bitarr
Input: pointer to Bit2_T bitarr
Output: (int) the width of the bit array
*/
int Bit2_width(T bitarr)
{
        return bitarr->width;
}

/*
Description: Puts the bit value specified by thisBit into
        the spot at row, col in the bit array pointed to
        by bitarr
Input: a pointer to a Bit2_T array, int row, int col,
        int bit value
Output: none
*/
int Bit2_put(T bitarr, int width, int height, int thisBit)
{
        if (width >= bitarr->width || height >= bitarr->height) {
                RAISE(Bounds);
        }
        Bit_T *row_arr = UArray_at((bitarr->spine), height);
        int x = Bit_get(*row_arr, width);
        Bit_put(*row_arr, width, thisBit);
        return x;
}
/*
Description: gets the bit at the specified row and
        col of the bit array pointed to by bitarr
Input: pointer to Bit2_T array, int row, int col
Output: int representing the value of the bit
*/
int Bit2_get(T bitarr, int width, int height)
{
        if (width >= bitarr->width || height >= bitarr->height) {
                RAISE(Bounds);
        }
        Bit_T *row_arr = UArray_at((bitarr->spine), height);
        return Bit_get(*row_arr, width);
}

/*
Description: Frees the Bit2 bitarr that is pointed to by *bitarr
Input: A pointer to a Bit2 pointer
Output: nothing
*/
void Bit2_free(T *bitarr)
{
        for (int i = 0; i < Bit2_height(*bitarr); i++) {
                Bit_T *thisArr = UArray_at(((*bitarr)->spine), i);
                Bit_free(thisArr);
                thisArr = NULL;
        }
        UArray_free(&((*bitarr)->spine));
        free(*bitarr);
        bitarr = NULL;
}

/*
Description: applies an apply function to the Bit2 bitarr
        pointed to by bitarr by traversing over an entire
        row of the bitarr before moving on to the next row of the bitarr
Input: A pointer to a Bit2 bitarr, an apply function of type
        void, a pointer to a closure
Output: nothing
*/
void Bit2_map_row_major(T bitarr,
        void apply(int width, int height, T bitarr, int b, void *p1), void *cl)
{
        if(bitarr == NULL) {
                assert(0);
        }

        for(int i = 0; i < bitarr->height; i++) {
                for(int j = 0; j < bitarr->width; j++) {
                        int thisBit = Bit2_get(bitarr, j, i);
                        apply(j, i, bitarr, thisBit, cl);
                }
        }
}

/*
Description: applies an apply function to the Bit2 bitarr
        pointed to by bitarr by traversing over an entire
        col of the bitarr before moving on to the next col of the bitarr
Input: A pointer to a Bit2 bitarr, an apply function of type
        void, a pointer to a closure
Output: nothing
*/
void Bit2_map_col_major(T bitarr,
        void apply(int width, int height, T bitarr, int b, void *p1), void *cl)
{
        if(bitarr == NULL) {
                assert(0);
        }

        for(int i = 0; i < bitarr->width; i++) {
                for(int j = 0; j < bitarr->height; j++) {
                        int thisBit = Bit2_get(bitarr, i, j);
                        apply(i, j, bitarr, thisBit, cl);
                }
        }
}
