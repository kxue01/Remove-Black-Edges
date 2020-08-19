#ifndef BIT2
#define BIT2
#define T Bit2_T

typedef struct T *T;

/*
Description: Creates a new Bit2 array of size height * width
Input: the height (int) and width (int) of the new Bit2_T
Output: a pointer to a Bit2_T array
*/
T Bit2_new(int width, int height);

/*
Description: Returns the height of the Bit2_T array pointed
        to by the bit bitarr
Input: pointer to Bit2_T bitarr
Output: (int) the height of the bit array
*/
int Bit2_height(T bitarr);

/*
Description: Returns the width of the Bit2_T array
        pointed to by bitarr
Input: pointer to Bit2_T bitarr
Output: (int) the width of the bit array
*/
int Bit2_width(T bitarr);

/*
Description: Puts the bit value specified by thisBit into
        the spot at row, col in the bit array pointed to
        by bitarr
Input: a pointer to a Bit2_T array, int row, int col,
        int bit value
Output: none
*/
int Bit2_put(T bitarr, int width, int height, int thisBit);

/*
Description: gets the bit at the specified row and
        col of the bit array pointed to by bitarr
Input: pointer to Bit2_T array, int row, int col
Output: int representing the value of the bit
*/
int Bit2_get(T bitarr, int width, int height);

/*
Description: Frees the Bit2 bitarr that is pointed to by *bitarr
Input: A pointer to a Bit2 pointer
Output: nothing
*/
void Bit2_free(T *bitarr);

/*
Description: applies an apply function to the Bit2 bitarr
        pointed to by bitarr by traversing over an entire
        row of the bitarr before moving on to the next row of the bitarr
Input: A pointer to a Bit2 bitarr, an apply function of type
        void, a pointer to a closure
Output: nothing
*/
void Bit2_map_row_major(T bitarr,
    void apply(int width, int height, T bitarr, int b, void *p1), void *cl);
/*
Description: applies an apply function to the Bit2 bitarr
        pointed to by bitarr by traversing over an entire
        col of the bitarr before moving on to the next col of the bitarr
Input: A pointer to a Bit2 bitarr, an apply function of type
        void, a pointer to a closure
Output: nothing
*/
void Bit2_map_col_major(T bitarr,
    void apply(int width, int height, T bitarr, int b, void *p1), void *cl);



#undef T
#endif
