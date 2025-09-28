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
    charj*** data;
    int width;
    int height;
} Sprite;                                                               // Sprites are groups of pixels


// Main functions

static inline void clear();                                             // Iterates through the back buffer and clears every set pixel 
static inline void set_pixel(char* pixel, int width, int height);       // Sets a pixel in the back buffer to a particular colour in a particular position
static inline void set_sprite(Sprite sprite, int width, int height);    // Sets a sprite in a particular position
static inline void set_window_size(int width, int height);              // Sets the window size - allocating the buffers and setting seqpixel_width and _height variables
static inline void draw();                                              // Draws what is in the front buffer to the screen
static inline void seqpixel_deinit();                                   // Frees the seqpixel buffer


// Color functions

static inline const char* colour_inner(...);                            // Inside logic for colour()     
#define colour(...) colour_inner(__VA_ARGS__, NULL)                     // Takes either 1 or 3 chars (1 for 8, 16 and 256 colour - 3 for true RGB)

                            
// Variables

char*** seqpixel_front_buffer = NULL;
char*** seqpixel_back_buffer = NULL;
int seqpixel_width = 0;
int seqpixel_height = 0;


///////////////////////////////// Functions /////////////////////////////////// 

static inline void clear() {
    // Set every character in the buffer to a blank string
    for (int y = 0; y < seqpixel_height; y++)
        for (int x = 0; x < seqpixel_width; x++)
            seqpixel_back_buffer[y][x] = "";
}


static inline void set_pixel(char* pixel, int width, int height) {
    if (seqpixel_width >= width && seqpixel_height >= height)
        seqpixel_back_buffer[height][width] = pixel;
}


static inline void set_sprite(Sprite* sprite, int width, int height) {
    // Iterate through pixels and call set_pixel
    for (int y = 0; y < sprite->height; y++)
        for (int x = 0; x < sprite->width; x++) {
            int target_x = x + width;
            int target_y = y + height;
            char* target_pixel = sprite->data[y + height][x + width];
            set_pixel(target_pixel, target_x, target_y);
        }
}


static inline void set_window_size(int width, int height) {
    // Allocating memory for buffers
    seqpixel_front_buffer = realloc(sizeof(char*) * width * height + 1);
    seqpixel_back_buffer = realloc(sizeof(char*) * width * height + 1);

    if (seqpixel_front_buffer == NULL || seqpixel_back_buffer == NULL)
        return NULL;

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
    fputs(seqpixel_back_buffer, stdout);
    fflush(stdout);

    // Swapping buffers
    char*** temp = seqpixel_front_buffer;
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

static inline const char* colour_inner(...) {
    char r, g, b = 0;
    int arg_count = 0;

    // Populate RGB with values
    va_list args;
    va_start(args, arg_count);

    char current_arg;
    while (current_arg = va_arg(args, char)) {
        if (current_arg == NULL)
            break;

        switch (arg_count) {
            case 2:
                r = current_arg;
                break;
            case 1:
                g = current_arg;
                break;
            default:
                b = current_arg;
                break;
        }

        arg_count++;
    }

    va_end(args);


    // Return based on values
    if (arg_count == 3)         // 3 will be rgb
        return;
    else                        // Else is one of the other colour types
        if (b < 8)
            return;             // TODO
        if (b < 16)
            return;
        else
            return;
}

