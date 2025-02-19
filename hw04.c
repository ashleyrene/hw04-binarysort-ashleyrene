#include <stdlib.h>
#include <stdio.h>
#include "hw04.h"

/* The first function you write counts the numbers of vectors
(that is, the number of triads of 32-bit integers) found in the
file with name 'filename'.  It assigns this value to the location
pointed to by 'vector_count'.  If it is unable to open the file,
OR if the file cannot be interpreted as a binary file of vectors
(that is, it does not contain a multiple of three integers' worth of data), it returns
EXIT_FAILURE.  Else, it returns EXIT_SUCCESS.  */
int count_vectors_in_file(char * filename, int * vector_count) {
    // Open the file in "rb" (read binary) mode
    FILE *file = fopen(filename, "rb");
    if (!file) {
        return EXIT_FAILURE;
    }
    
    // Set up a buffer of three integers to read to,
    // a count of how many integers are read in on the last pass,
    // and a count of how many times three integers have been read in
    int buffer[3];
    int count = 0;
    size_t read_count;
    
    // While not encountering the end of the file or receiving an error,
    // call fread with the buffer to retrieve three integers.  Update
    // the counts.
    while ((read_count = fread(buffer, sizeof(int), 3, file)) == 3) {
        count++;
    }
    
    // If the last pass reached the end of the file and retrieved no integers
    // (rather than a stray one or two) and didn't throw an error, return EXIT_SUCCESS
    if (read_count != 0) {
        fclose(file);
        return EXIT_FAILURE;
    }
    
    *vector_count = count;
    fclose(file);
    return EXIT_SUCCESS;
}

/* The second function you write allocates space for the vectors
counted before - there are vector_count vectors stored in the file at
filename.  It reads in those vector_count vectors from the file 'filename'
to the space it allocates and returns a pointer to that space.

That space will need to be freed - though, of course, not before
the program is done using it.  Free it in an appropriate place (hint: not in this function.)

In the event of a file read failure or memory allocation failure (both
are unlikely), you can return NULL.*/
struct vector * read_vectors_from_file(char * filename, int vector_count) {
    // Open the file in "rb" (read binary) mode
    FILE *file = fopen(filename, "rb");
    if (!file) {
        return NULL;
    }
    
    // Allocate sufficient space for all of the vector structs
    struct vector *vector_list = malloc(vector_count * sizeof(struct vector));
    if (!vector_list) {
        fclose(file);
        return NULL;
    }
    
    // Read in all the vectors from the file
    if (fread(vector_list, sizeof(struct vector), vector_count, file) != (size_t) vector_count) {
        free(vector_list);
        fclose(file);
        return NULL;
    }
    
    fclose(file);
    return vector_list;
}

/* The third function you write takes in two pointers of type
const void*, casts them to access the attributes of the 
structs to which they point, and returns a positive value if the
first is greater, a negative value if the second is greater,
and 0 if they are equal.  Being greater is determined by the 
x-coordinate's being greater, or, those being equal, by the
y-coordinate's being greater, or, those also being equal, by 
the z-coordinate's being greater.

This function is meant to be sent to qsort - see 
https://en.cppreference.com/w/c/algorithm/qsort for an example
of using qsort and a warning about comparison functions.*/
int compare_vectors(const void* a, const void* b) {
    struct vector *vec1 = (struct vector *)a;
    struct vector *vec2 = (struct vector *)b;
    
    // Compare x-coordinates first
    if (vec1->x > vec2->x) return 1;
    if (vec1->x < vec2->x) return -1;
    
    // If x-coordinates are equal, compare y-coordinates
    if (vec1->y > vec2->y) return 1;
    if (vec1->y < vec2->y) return -1;
    
    // If both x- and y-coordinates are equal, compare z-coordinates
    if (vec1->z > vec2->z) return 1;
    if (vec1->z < vec2->z) return -1;

    return 0;
}

/* The fourth function calls qsort with the appropriate
parameters, including the function above. */
void call_qsort(struct vector * vector_list, int vector_count) {
    // Call qsort to sort the vector list
    qsort(vector_list, vector_count, sizeof(struct vector), compare_vectors);
}

/* The fifth function writes the vectors in 'vector_list' (which
has length 'vector_count') to a binary file at 'filename'.  If the
file cannot be opened, it returns EXIT_FAILURE. */
int write_vectors_to_file(char * filename, struct vector * vector_list, int vector_count) {
    // Open the file in "wb" (write binary) mode
    FILE *file = fopen(filename, "wb");
    if (!file) {
        return EXIT_FAILURE;
    }
    
    // Write the coordinates of each vector to the binary file
    if (fwrite(vector_list, sizeof(struct vector), vector_count, file) != (size_t) vector_count) {
        fclose(file);
        return EXIT_FAILURE;
    }
    
    fclose(file);
    return EXIT_SUCCESS;
}
