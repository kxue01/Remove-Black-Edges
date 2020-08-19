/*
        unblackedges.c

        This program removes black edges from a
        PBM (plain bit map) file (PNM with magic number
        1) using the Bit2 data type

        Authors: Kenneth Xue (kxue01)
                Alyssa Rose (arose10)

*/
#include "bit2.h"
#include <pnmrdr.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stack.h>
#include <stdbool.h>
#include <except.h>


/* Error messages */
static Except_T Args = {"More Than One Argument"};
static Except_T No_PBM = {"PBM Not Provided"};
static Except_T Malloc_Fail = {"Memory Allocation Failed"};
static Except_T Bad_Pointer = {"File Pointer NULL"};

/* Functions */
Bit2_T pbmread(FILE *inputfp);
void pbmwrite(FILE *outputfp, Bit2_T bitarr);
void traverse_edges(Bit2_T *image);
void BFS(Bit2_T *bit, int x, int y);
bool valid_edge(Bit2_T bit, int x, int y);
struct index *make_coord(int x, int y);
void visit_neighbor(struct index *new_ind, Stack_T *Primary, Bit2_T bit);

/*
Struct to hold coordinates of a black
edge pixel
*/
struct index {
        int x, y;
};


int main(int argc, char *argv[])
{
        if (argc > 2) {
                RAISE(Args);
        }
        if (argc == 2) {
                FILE *fp = fopen(argv[1], "r");
                if (fp == NULL) {
                        RAISE(No_PBM);
                } else {
                        Bit2_T pbm = pbmread(fp);
                        fclose(fp);
                        traverse_edges(&pbm);
                        pbmwrite(stdout, pbm);
                        Bit2_free(&pbm);
                }
        }
        if (argc == 1) {
                FILE *fp = stdin;
                if (fp == NULL) {
                        RAISE(No_PBM);
                } else {
                        Bit2_T pbm = pbmread(fp);
                        fclose(fp);
                        traverse_edges(&pbm);
                        pbmwrite(stdout, pbm);
                        Bit2_free(&pbm);
                }
        }
        exit(0);
}

/*
Description: reads pixels from a PBM file pointed
        to by inputfp and stores into a Bit2_T map
Input: file pointer (either file or stdin)
Output: Bit2_T map
*/
Bit2_T pbmread(FILE *inputfp)
{
        Pnmrdr_T pgm = Pnmrdr_new(inputfp);
        Pnmrdr_mapdata map_data = Pnmrdr_data(pgm);
        int width = map_data.width;
        int height = map_data.height;
        if ((width == 0) || (height == 0) ||
        map_data.type != 1) {
                Pnmrdr_free(&pgm);
                fclose(inputfp);
                RAISE(No_PBM);
        }
        Bit2_T image = Bit2_new(width, height);
        int this_pix;
        for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                        this_pix = Pnmrdr_get(pgm);
                        Bit2_put(image, j, i, this_pix);
                }
        }
        Pnmrdr_free(&pgm);
        return image;
}

/*
Description: Loops through the edge pixels of the image and calls
        the BFS function if they are black.
Input: A pointer to a Bit2_T map
Output: None
*/
void traverse_edges(Bit2_T *image)
{
        for (int y = 0; y < Bit2_height(*image); y++) {
            if (Bit2_get(*image, 0, y)) {
                BFS(image, 0, y);
            }

            if (Bit2_get(*image, Bit2_width(*image) - 1, y)) {
                BFS(image, Bit2_width(*image) - 1, y);
            }
        }
        for (int x = 0; x < Bit2_width(*image); x++) {
            if (Bit2_get(*image, x, 0)) {
                BFS(image, x, 0);
            }

            if (Bit2_get(*image, x, Bit2_height(*image) - 1)) {
                BFS(image, x, Bit2_height(*image) - 1);
            }
        }
}

/*
Description: Implements a breadth first search on the Bit2_T map to find
        the black edge pixels and their neighbors that are also black
        edges (defined inductively)
Input: A pointer to a Bit2_T map, integers x and y (representing coordinates)
Output: none
*/
void BFS(Bit2_T *bit, int x, int y)
{
        Stack_T Primary = Stack_new();
        if (Primary == NULL) {
                Bit2_free(bit);
                RAISE(Malloc_Fail);
        }
        /* secondary check that bit is black edge pixel */
        if (valid_edge(*bit, x, y)) {
            struct index *coord = make_coord(x, y);
            if (coord == NULL){
                    Bit2_free(bit);
                    Stack_free(&Primary);
                    RAISE(Malloc_Fail);
            }
                Stack_push(Primary, coord);
                while (!Stack_empty(Primary)) {
                        struct index *new_ind = Stack_pop(Primary);
                        if (new_ind == NULL) {
                                Bit2_free(bit);
                                Stack_free(&Primary);
                                RAISE(Malloc_Fail);
                        }
                        Bit2_put(*bit, new_ind->x, new_ind->y, 0);
                        visit_neighbor(new_ind, &Primary, *bit);
                        free(new_ind);
                }
                Stack_free(&Primary);
        }
}

/*
Description: writes pixels of the Bit2_T map pointed
        to by bitarr to the file pointed to by outputfp
Input: a pointer to a file, a pointer to a
        Bit2_T array
Output: nothing
*/
void pbmwrite(FILE *outputfp, Bit2_T bitarr)
{
        if (outputfp == NULL) {
                Bit2_free(&bitarr);
                RAISE(Bad_Pointer);
        }
        fprintf(outputfp, "P1\n");
        fprintf(outputfp, "# Have Mercy\n");
        fprintf(outputfp, "%d %d\n", Bit2_width(bitarr), Bit2_height(bitarr));
        for (int y = 0; y < Bit2_height(bitarr); y++) {
          for (int x = 0; x < Bit2_width(bitarr); x++) {
                  /* Prevents space from being printed after the last
                  character in the row */
                  if (x == (Bit2_width(bitarr) - 1)) {
                          fprintf(outputfp, "%d", Bit2_get(bitarr, x, y));
                  } else {
                          fprintf(outputfp, "%d ", Bit2_get(bitarr, x, y));
                  }
          }
        fprintf(outputfp, "\n");
        }
}

/*
Description: Checks if a set of x, y coordinates are valid indices
        and if they are, if the pixel at that point in the map
        is a black pixel
Input: Bit2_T map, integers x and y (representing coordinates)
*/
bool valid_edge(Bit2_T bit, int x, int y)
{
        if ((0 <= x) && (x < Bit2_width(bit))) {
            if ((0 <= y) && (y < Bit2_height(bit))){
                if (Bit2_get(bit, x, y)){
                    return(true);
                }
            }

        }
        return(false);
}

/*
Description: Makes an index struct for the x and y coordinates passed in
        by mallocing space for an index struct and returning it
Input: integers x and y representing coordinates
Output: a pointer to an index struct
*/
struct index *make_coord(int x, int y)
{
        struct index *new = malloc(sizeof(struct index));
        if (new == NULL) {
                return(new);
        }
        new->x = x;
        new->y = y;
        return(new);
}

/*
Description: Checks if the neighboring pixels of the pixel at the
        new_ind coordinates qualify as black edge pixels and if so, pushes
        them onto the primary stack for the BFS funtion.
Input: pointer to index struct (coordinates of the pixel whose neighbors
        will be checked), pointer to a Stack_T holding pixels to be changed
        to white, and a Bit2_T map
Output: none
*/
void visit_neighbor(struct index *new_ind, Stack_T *Primary, Bit2_T bit)
{
        if (new_ind == NULL || Primary == NULL) {
                Bit2_free(&bit);
                RAISE(Bad_Pointer);
        }
        if (valid_edge(bit, new_ind->x, (new_ind->y)-1)) {
                struct index *cord = make_coord(new_ind->x, (new_ind->y)-1);
                Stack_push(*Primary, cord);
        }
        if (valid_edge(bit, (new_ind->x)-1, new_ind->y)) {
                struct index *cord2 = make_coord((new_ind->x)-1, new_ind->y);
                Stack_push(*Primary, cord2);
        }
        if (valid_edge(bit, (new_ind->x)+1, new_ind->y)) {
                struct index *cord3 = make_coord((new_ind->x)+1, new_ind->y);
                Stack_push(*Primary, cord3);
        }
        if (valid_edge(bit, new_ind->x, (new_ind->y)+1)) {
                struct index *cord4 = make_coord(new_ind->x, (new_ind->y)+1);
                Stack_push(*Primary, cord4);
        }
}
