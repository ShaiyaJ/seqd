/////////////////////////////// Dependencies ////////////////////////////////// 
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>

///////////////////////// Preprocessor config options ///////////////////////// 
// Some options that you can #define to alter the behaviour of seqpixel.     //
///////////////////////////////////////////////////////////////////////////////

// Rendering modes
#define RECTANGLE_PIXEL 0
#define HALF_BLOCK_PIXEL 1

#define RENDERING_MODE RECTANGLE_PIXEL


// Scaling modes
#define NO_SCALING 0 
#define NEAREST_NEIGHBOUR 1

#define SCALING_MODE NO_SCALING


///////////////////////////////////// Docs ////////////////////////////////////
// Some reference documentation for seqpixel.                                //
///////////////////////////////////////////////////////////////////////////////


// Structs

typedef struct {
    uint32_t data;
    
    enum {
        COL_8,
        COL_16,
        COL_256,
        COL_TRUE
    } color_type;
} Color;


typedef struct {
    Color** data;
    int width;
    int height;
} Sprite;                                                               // Sprites are groups of pixels


// Main functions

static inline void clear(Color col);                                    // Iterates through the back buffer and clears every set pixel 
static inline void set_pixel(Color col, int width, int height);         // Sets a pixel in the back buffer to a particular colour in a particular position
static inline void set_sprite(Sprite* sprite, int width, int height);   // Sets a sprite in a particular position
static inline void set_window_size(int width, int height);              // Sets the window size - allocating the buffers and setting seqpixel_width and _height variables
static inline void draw();                                              // Draws what is in the front buffer to the screen
static inline void seqpixel_deinit();                                   // Frees the seqpixel buffer

                            
// Variables

Color** seqpixel_front_buffer = NULL;
Color** seqpixel_back_buffer = NULL;
int seqpixel_width = 0;
int seqpixel_height = 0;


///////////////////////////////// Functions /////////////////////////////////// 

static inline void clear(Color col) {
    for (int y = 0; y < seqpixel_height; y++)
        for (int x = 0; x < seqpixel_width; x++)
            seqpixel_back_buffer[y][x] = col; 
}


static inline void set_pixel(Color col, int width, int height) {
    if (seqpixel_width >= width && seqpixel_height >= height)
        seqpixel_back_buffer[height][width] = col;
}


static inline void set_sprite(Sprite* sprite, int width, int height) {
    // Iterate through pixels and call set_pixel
    for (int y = 0; y < sprite->height; y++)
        for (int x = 0; x < sprite->width; x++) {
            int target_x = x + width;
            int target_y = y + height;
            Color target_pixel = sprite->data[y + height][x + width];
            set_pixel(target_pixel, target_x, target_y);
        }
}


static inline void set_window_size(int width, int height) {
    // Allocating memory for buffers
    seqpixel_front_buffer = realloc(seqpixel_front_buffer, sizeof(Color) * width * height + 1);
    seqpixel_back_buffer = realloc(seqpixel_back_buffer, sizeof(Color) * width * height + 1);

    if (seqpixel_front_buffer == NULL || seqpixel_back_buffer == NULL)
        exit(1);

    // Setting width and height
    seqpixel_width = width;
    seqpixel_height = height;

    // Setting last character of buffers to \0 so that printf works correctly
    seqpixel_front_buffer[width + height] = '\0';
    seqpixel_back_buffer[width + height] = '\0';
}


static inline void draw() {
    // Setting cursor to 0,0
    // TODO
    
    // Displaying buffer to stdout
    if (seqpixel_width >= 0 && seqpixel_height >= 0)
        for (int y = 0; y < seqpixel_height; y++)
            for (int x = 0; x < seqpixel_width; x++) {
                Color c = seqpixel_back_buffer[y][x];
                
                switch (c.color_type) {
                    case COL_8:
                        //fputs();
                        continue;
                    case COL_16:
                        //fputs();
                        continue;
                    case COL_256:
                        //fputs();
                        continue;
                    case COL_TRUE:
                        //fputs();
                        continue;
                }
            }

    fflush(stdout);

    // Swapping buffers
    Color** temp = seqpixel_front_buffer;
    seqpixel_front_buffer = seqpixel_back_buffer;
    seqpixel_back_buffer = temp;
}


static inline void seqpixel_deinit() {
    if (seqpixel_front_buffer != NULL) {
        free(seqpixel_front_buffer);
        seqpixel_front_buffer = NULL;
    }

    if (seqpixel_back_buffer != NULL) {
        free(seqpixel_back_buffer);
        seqpixel_back_buffer = NULL;
    }

    seqpixel_width = 0;
    seqpixel_height = 0;
}

