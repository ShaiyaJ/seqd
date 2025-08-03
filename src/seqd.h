// Seqd header-only implementation 
// Seqd (sequence-display) aims to be a simple library to display text with 
// graphics using only ANSI escape sequences.

#ifndef SEQD_H
#define SEQD_H 

///////////////////////////////// Dependencies //////////////////////////////// 
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else 
    #include <sys/poll.h>
    #include <termios.h>
    #include <unistd.h>
#endif

///////////////////////////////////// Docs //////////////////////////////////// 
// Some reference documentation for you. Most of the functions do exactly as //
// they say on the tin.                                                      //
//                                                                           //
// Functions marked with a "*" at the start of their comment have platform-  //
// specific behaviour. They make use of #ifdef _WIN32 to distinguish         //
// environments that are running Windows.                                    //
//                                                                           //
// Below this are two sets of definitions, these are also useful to know.    //
// They were placed under the docs for clarity in code. However, from the    //
// perspective of a reference it makes more sense to put them above the      //
// functions.                                                                //
///////////////////////////////////////////////////////////////////////////////

// Utility functions (intended for library use, not user use)
static inline char* ctos(char c);                                       // Turns a single char into a null terminated char*, returns NULL on failure
static inline void deinit();                                            // Frees memory related to buffers, and input buffers
static inline const char* ansi_argd_seq(const char* fmt, ...);          // "Registers" a new SEQD_ command that takes args

// Global variables                                                     // If you do not use seqd for your entire program, you may want to free() these at some point, deinit() achieves this
char* seqdbuf = NULL;                                                   // For use in display and buffered commands
unsigned int seqdbuf_size = 0;                                          // For use in display and buffered commands
char* seqdibuf = NULL;                                                  // For use in input buffers 
bool seqdraw = false;                                                   // For use in set/unset raw mode and keypress

#ifdef _WIN32                                                           // These are for use in set/unset_raw_mode, they are platform specific
    DWORD seqdmode;
#else
    struct termios seqdterm;
#endif


// Input
static inline char keypress();                                          // *Reads a single character from the keyboard
static inline char* get_input(int max_size);                            // Get line of input from the user (until you hit '\n') and return upto the maximum amount of characters


// Output
static inline void display();                                           // Display everything stored in the buffer
static inline char* buffer(const char* sequence);                       // Buffered commands until display is called - sequence must be null terminated
static inline void null_terminated_buffers(const char* first, ...);     // Variable arguments that are NULL terminated
/* MACRO queue(x)      null_terminated_buffers(##x, NULL) */            // Macro to call null_termianted_buffers with trailing NULL

static inline void immediate(const char* sequence);                     // Immediate flushing
static inline void null_terminated_immediates(const char* first, ...);  // Variable arguments that are NULL terminated
/* MACRO execute(...) */                                                // Macro to call null_termianted_buffers with trailing NULL


// Cursor manipulation
static inline void get_terminal_size(int* width, int* height);          // Returns the width of the terminal in characters, requires raw mode
    
// Setting terminal "raw mode"
static inline void set_raw_mode();                                      // *Turns on terminal raw mode - in this mode you can perform non-blocking reads on the keyboard
static inline void unset_raw_mode();                                    // *Turns off terminal raw mode - in this mode the user can exit their text before sending it

///////////////////////// Preprocessor config options ///////////////////////// 
// Some options that you can #define to alter the behaviour of seqd.         //
///////////////////////////////////////////////////////////////////////////////
#ifndef SEQD_MAX_GET_LINE_MAXIMUM_ITERATION 
#define SEQD_MAX_GET_LINE_MAXIMUM_ITERATION 1024                        // Maximum iteration count for get_line_until
#endif

#ifndef SEQD_MAX_BUFFER_SIZE
#define SEQD_MAX_BUFFER_SIZE 1024                                       // Decides maximum input size for certain functions
#endif

#ifndef SEQD_STATIC_BUFFER_SIZE                                         // Static buffer used for ansi_argd_seq (any function starting with SEQD_ that takes a value)
#define SEQD_STATIC_BUFFER_SIZE 32
#endif

#ifndef SEQD_STATIC_BUFFER_COUNT                                        // Static buffer used for ansi_argd_seq (any function starting with SEQD_ that takes a value)
#define SEQD_STATIC_BUFFER_COUNT 8
#endif

#ifndef SEQD_KEYBOARD_TIMEOUT
#define SEQD_KEYBOARD_TIMEOUT 100                                       // Maxmimum milliseconds that nonblocking keypress() polls for
#endif

//////////////////////////////// ANSI constants /////////////////////////////// 

#define SEQD_ESC                    "\033["
#define SEQD_RESET                  SEQD_ESC "0m"


// Cursor commands
#define SEQD_CURPOS                 SEQD_ESC "6n"
#define SEQD_HIDECUR                SEQD_ESC "?25l" 
#define SEQD_SHOWCUR                SEQD_ESC "?25h" 

static inline const char* SEQD_SETCUR(int row, int col ) {return ansi_argd_seq("\033[%d;%dH", row, col);}
static inline const char* SEQD_CUR_UP(int n)             {return ansi_argd_seq("\033[%dA",    n       );}
static inline const char* SEQD_CUR_DOWN(int n)           {return ansi_argd_seq("\033[%dB",    n       );}
static inline const char* SEQD_CUR_FORWARD(int n)        {return ansi_argd_seq("\033[%dC",    n       );}
static inline const char* SEQD_CUR_BACKWARD(int n)       {return ansi_argd_seq("\033[%dD",    n       );}
static inline const char* SEQD_CUR_NEXT_LINE(int n)      {return ansi_argd_seq("\033[%dE",    n       );}
static inline const char* SEQD_CUR_PREV_LINE(int n)      {return ansi_argd_seq("\033[%dF",    n       );}
static inline const char* SEQD_CUR_HORIZONTAL(int n)     {return ansi_argd_seq("\033[%dG",    n       );}

// Console commands
#define SEQD_CLEAR                  SEQD_ESC "2J"

static inline const char* SEQD_SCROLL_UP(int n)          {return ansi_argd_seq("\033[%dS",    n       );}
static inline const char* SEQD_SCROLL_DOWN(int n)        {return ansi_argd_seq("\033[%dT",    n       );}

static inline const char* SEQD_ERASE_DISPLAY(int n)      {return ansi_argd_seq("\033[%dJ",    n       );}
static inline const char* SEQD_ERASE_LINE(int n)         {return ansi_argd_seq("\033[%dK",    n       );}

// Text styles
#define SEQD_BOLD                   SEQD_ESC "1m"
#define SEQD_FAINT                  SEQD_ESC "2m"
#define SEQD_ITALIC                 SEQD_ESC "3m"
#define SEQD_UNDERLINE              SEQD_ESC "4m"
#define SEQD_BLINK_SLOW             SEQD_ESC "5m"
#define SEQD_BLINK_FAST             SEQD_ESC "6m"
#define SEQD_REVERSE                SEQD_ESC "7m"
#define SEQD_CONCEAL                SEQD_ESC "8m"
#define SEQD_CROSSED_OUT            SEQD_ESC "9m"

#define SEQD_RESET_BOLD             SEQD_ESC "22m"
#define SEQD_RESET_ITALIC           SEQD_ESC "23m"
#define SEQD_RESET_UNDERLINE        SEQD_ESC "24m"
#define SEQD_RESET_BLINK            SEQD_ESC "25m"
#define SEQD_RESET_REVERSE          SEQD_ESC "27m"
#define SEQD_RESET_CONCEAL          SEQD_ESC "28m"
#define SEQD_RESET_CROSSED_OUT      SEQD_ESC "29m"

// Colour functions
static inline const char* SEQD_FG_7(int col)                { return ansi_argd_seq("\033[3%dm", col); }
static inline const char* SEQD_BG_7(int col)                { return ansi_argd_seq("\033[4%dm", col); }
static inline const char* SEQD_FG_B7(int col)               { return ansi_argd_seq("\033[9%dm", col); }
static inline const char* SEQD_BG_B7(int col)               { return ansi_argd_seq("\033[10%dm", col); }

static inline const char* SEQD_FG_256(int col)              { return ansi_argd_seq("\033[38;5;%dm", col); }
static inline const char* SEQD_BG_256(int col)              { return ansi_argd_seq("\033[48;5;%dm", col); }

static inline const char* SEQD_FG_RGB(int r, int g, int b)  { return ansi_argd_seq("\033[38;2;%d;%d;%dm", r, g, b); }
static inline const char* SEQD_BG_RGB(int r, int g, int b)  { return ansi_argd_seq("\033[48;2;%d;%d;%dm", r, g, b); }
// Colour constants
#define SEQD_FG_BLACK               SEQD_ESC "30m"
#define SEQD_FG_RED                 SEQD_ESC "31m"
#define SEQD_FG_GREEN               SEQD_ESC "32m"
#define SEQD_FG_YELLOW              SEQD_ESC "33m"
#define SEQD_FG_BLUE                SEQD_ESC "34m"
#define SEQD_FG_MAGENTA             SEQD_ESC "35m"
#define SEQD_FG_CYAN                SEQD_ESC "36m"
#define SEQD_FG_WHITE               SEQD_ESC "37m"

#define SEQD_FG_BRIGHT_BLACK        SEQD_ESC "90m"
#define SEQD_FG_BRIGHT_RED          SEQD_ESC "91m"
#define SEQD_FG_BRIGHT_GREEN        SEQD_ESC "92m"
#define SEQD_FG_BRIGHT_YELLOW       SEQD_ESC "93m"
#define SEQD_FG_BRIGHT_BLUE         SEQD_ESC "94m"
#define SEQD_FG_BRIGHT_MAGENTA      SEQD_ESC "95m"
#define SEQD_FG_BRIGHT_CYAN         SEQD_ESC "96m"
#define SEQD_FG_BRIGHT_WHITE        SEQD_ESC "97m"

#define SEQD_BG_BLACK               SEQD_ESC "40m"
#define SEQD_BG_RED                 SEQD_ESC "41m"
#define SEQD_BG_GREEN               SEQD_ESC "42m"
#define SEQD_BG_YELLOW              SEQD_ESC "43m"
#define SEQD_BG_BLUE                SEQD_ESC "44m"
#define SEQD_BG_MAGENTA             SEQD_ESC "45m"
#define SEQD_BG_CYAN                SEQD_ESC "46m"
#define SEQD_BG_WHITE               SEQD_ESC "47m"

#define SEQD_BG_BRIGHT_BLACK        SEQD_ESC "100m"
#define SEQD_BG_BRIGHT_RED          SEQD_ESC "101m"
#define SEQD_BG_BRIGHT_GREEN        SEQD_ESC "102m"
#define SEQD_BG_BRIGHT_YELLOW       SEQD_ESC "103m"
#define SEQD_BG_BRIGHT_BLUE         SEQD_ESC "104m"
#define SEQD_BG_BRIGHT_MAGENTA      SEQD_ESC "105m"
#define SEQD_BG_BRIGHT_CYAN         SEQD_ESC "106m"
#define SEQD_BG_BRIGHT_WHITE        SEQD_ESC "107m"

///////////////////////////// Useful key constants //////////////////////////// 

#define SEQD_KEY_CTRL_PLUS_(k)      ((k) & 0x1F)        // Macro function for "Ctrl + key` - in raw mode this shows up as "key-64" (only for a-z)   // TODO: ansi_argd_seq

#define SEQD_KEY_SHIFT_PLUS_(k)     ((k) ^ 0x20)        // Macro function for "Shift + key` - in raw mode this shows up as uppercase, doesn't work for some keys // TODO: ansi_argd_seq

#define SEQD_KEY_ALT_PLUS_(k)       SEQD_ESC (k)        // TODO: implement keypress_ex that can detect this

#define SEQD_KEY_ESC                '\x1B'
#define SEQD_KEY_BACKSPACE          '\x7F'
#define SEQD_KEY_TAB                '\t'
#define SEQD_KEY_ENTER              '\n'
#define SEQD_KEY_RETURN             '\n'

#define SEQD_KEY_UP                 SEQD_ESC "A"        // TODO: implement keypress_ex that can detect this
#define SEQD_KEY_DOWN               SEQD_ESC "B"        // TODO: implement keypress_ex that can detect this
#define SEQD_KEY_RIGHT              SEQD_ESC "C"        // TODO: implement keypress_ex that can detect this
#define SEQD_KEY_LEFT               SEQD_ESC "D"        // TODO: implement keypress_ex that can detect this

#define SEQD_KEY_INSERT             SEQD_ESC "2~"       // TODO: implement keypress_ex that can detect this
#define SEQD_KEY_DELETE             SEQD_ESC "3~"       // TODO: implement keypress_ex that can detect this
#define SEQD_KEY_HOME               SEQD_ESC "H"        // TODO: implement keypress_ex that can detect this
#define SEQD_KEY_END                SEQD_ESC "F"        // TODO: implement keypress_ex that can detect this
#define SEQD_KEY_PAGE_UP            SEQD_ESC "5~"       // TODO: implement keypress_ex that can detect this
#define SEQD_KEY_PAGE_DOWN          SEQD_ESC "6~"       // TODO: implement keypress_ex that can detect this

////////////////////////////// Utility functions //////////////////////////////
static inline char* ctos(char c) {      // char to string (null terminated char*)
    char* s = (char*) malloc(2);

    if (s == NULL)
        return NULL;
    
    s[0] = c;
    s[1] = '\0';
    return s;
}

static inline void deinit() {
    seqdbuf_size = 0;
    if (seqdbuf != NULL) {
        free(seqdbuf);
        seqdbuf = NULL;
    }
    
    if (seqdibuf != NULL) {
        free(seqdibuf);
        seqdibuf = NULL; 
    }
}

static inline const char* ansi_argd_seq(const char* fmt, ...) {   // Takes a format and argument list to match to an escape sequence
    // Get target buffer 
    static char bufs[SEQD_STATIC_BUFFER_COUNT][SEQD_STATIC_BUFFER_SIZE]; // Avoids a bug where execute("...", "...") will rewrite the same buffer 
    static int index = 0;
    
    char* buf = bufs[index];
    index = (index + 1) % SEQD_STATIC_BUFFER_COUNT;

    // Get args and write to the buffer
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, 32, fmt, args);   
    va_end(args);
    
    return buf;
}

////////////////////////// Cross platform functions /////////////////////////// 



// Buffering sequences to be executed later (when display() is called)

static inline void display() {
    if (seqdbuf == NULL)
        return;

    fputs(seqdbuf, stdout);
    fflush(stdout);
}

static inline char* buffer(const char* sequence) {
    if (seqdbuf == NULL) {                  // If seqdbuf hasn't been allocated, allocate it.
        seqdbuf = (char*) malloc(1);

        if (seqdbuf == NULL) {
            seqdbuf_size = 0;
            return NULL;
        }

        seqdbuf_size = 1;
        seqdbuf[0] = '\0';
    }


    // Calculate sizes
    int sequence_size = strnlen(sequence, SEQD_MAX_BUFFER_SIZE);
    
    // Buffer reallocation
    seqdbuf = realloc(seqdbuf, seqdbuf_size + sequence_size);

    if (seqdbuf == NULL) {
        seqdbuf_size = 0;
        return NULL;
    }

    seqdbuf_size += sequence_size;          // If allocation succeeds then add it to the seqdbuf_size

    strncat(seqdbuf, sequence, seqdbuf_size);
    return seqdbuf;
}

static inline void null_terminated_buffers(const char* first, ...) { 
    va_list args;
    va_start(args, first);

    const char* sequence = first;

    do {
        buffer(sequence);
        sequence = va_arg(args, const char*);
    } while (sequence != NULL);
}

#define queue(...) null_terminated_buffers(__VA_ARGS__, NULL)          // Asserts NULL termination



// Immediately displaying sequences

static inline void immediate(const char* sequence) {                    // Immediate flushing
    fputs(sequence, stdout);
    fflush(stdout);
} 


static inline void null_terminated_immediates(const char* first, ...) { 
    va_list args;
    va_start(args, first);

    const char* sequence = first;

    do {
        immediate(sequence);
        sequence = va_arg(args, const char*);
    } while (sequence != NULL);
}

#define execute(...) null_terminated_immediates(__VA_ARGS__, NULL)     // Asserts NULL termination




// Input (cross platform)

void clear_seqd_input() {
    if (seqdibuf != NULL)
        if (!strchr(seqdibuf, '\n'))
            while(fgetc(stdin)!='\n');                                  //discard until newline
}

static inline char* get_input(int max_size) {
    clear_seqd_input();
    seqdibuf = (char*) realloc(seqdibuf, max_size+1);
    memset(seqdibuf, 0, max_size+1);                    // Clearing array
    
    // Read from stdin
    fgets(seqdibuf, max_size+1, stdin);

    return seqdibuf;
}


// Cursor/console commands

static inline void get_terminal_size(int* width, int* height) {
    if (!seqdraw)                           // Only works in raw mode, this variable is defined 
        return;                             // later on next to the functions for setting raw


    // Send escape sequence to request cursor position
    immediate(SEQD_SETCUR(999,999));
    immediate(SEQD_CURPOS);

    /*
    // Read the response
    char buf[32];
    if (fgets(buf, sizeof(buf), stdin) != NULL) {
        // The format will be like: ESC[row;colR
        // Example: ESC[5;10R means row 5, column 10
        int row, col;
        if (sscanf(buf, SEQD_ESC "[%d;%dR", &row, &col) == 2) {
            return row;
        }
    }

    return -1;
    */

    char buf[11];
    ssize_t len = read(STDIN_FILENO, buf, sizeof(buf) - 1);

    if (len > 0) {
        buf[len] = '\0';  // Null-terminate so sscanf works
        int row, col;
        if (sscanf(buf, "\033[%d;%dR", &row, &col) == 2) {
            *width = col;
            *height = row;
        }
    }
}

///////////////////////// Platform specific functions ///////////////////////// 
// These are a little harder to read. Dependencies and any globals are       //
// included at the top of this section. Globals are prefixed with seqd_.     //
//                                                                           //
// There's a comment with whitespace to indicate what each sub-section is    //
// responsible for.                                                          //
///////////////////////////////////////////////////////////////////////////////

// Raw mode
 
static inline void set_raw_mode() {

    #ifdef _WIN32                           // WINDOWS implementation
                
            // Disable input buffering and character echo
            HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);

            // Get current console input mode
            if (!GetConsoleMode(hstdin, &seqdmode)) {
                perror("GetConsoleMode");
                exit(EXIT_FAILURE);
            }

            // Disable line input and echo
            DWORD new_mode = seqdmode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
            if (!SetConsoleMode(hstdin, new_mode)) {
                perror("SetConsoleMode");
                exit(EXIT_FAILURE);
            }

    #else                                       // POSIX implementation

            // Get the current terminal settings
            //struct termios term = { 0 };
            fflush(stdout);

            if (tcgetattr(STDIN_FILENO, &seqdterm) < 0)
                perror("tcgetattr");

            // Save the original terminal settings for later restoration
            //seqdterm = term;
            struct termios term = seqdterm;

            // Set the terminal to raw mode
            term.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
            term.c_cc[VMIN] = 1;               // Minimum number of bytes for non-canonical mode
            term.c_cc[VTIME] = 0;              // Timeout for non-canonical mode

            if (tcsetattr(STDIN_FILENO, TCSANOW, &term) < 0)
                perror("tcsetattr");

    #endif
    
    seqdraw = true;
}


static inline void unset_raw_mode() {

    #ifdef _WIN32

        HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);

        if (!SetConsoleMode(hstdin, seqdmode)) {
            perror("SetConsoleMode");
            exit(EXIT_FAILURE);
        }

    #else       // POSIX implementation

        if (tcsetattr(STDIN_FILENO, TCSANOW, &seqdterm) < 0)
            perror("tcsetattr");

    #endif

    seqdraw = false;

}



// Input (non-blocking)

static inline char keypress() {

    #ifdef _WIN32                   // WINDOWS implementation

        if (_kbhit())
            return _getch();
        return 0;

    #else                           // POSIX implementation
        // Non-blocking read only works in raw mode, for blocking reads there is a separate function
        if (!seqdraw)
            return 0;

        // Check if there is char in stdin
        if (feof(stdin) != 0) {
            clearerr(stdin);
            return 0;
        }

        // Read char from stdin
        struct pollfd fds;
        fds.fd = STDIN_FILENO; 
        fds.events = POLLIN;

        if (poll(&fds, 1, SEQD_KEYBOARD_TIMEOUT)) {
            char c;
            ssize_t n = read(STDIN_FILENO, &c, 1);

            if (n == 1)
                return (char) c;
        }

        return 0;

    #endif

}

/////////////////////////////////////////////////////////////////////////////// 
// There was once a complicated macro definition in the docs    //   |\_     //
// section and I wanted it to maintain readability. So I drew a //  /.  \/|  //
// small ASCII cat - which I thought was quite fitting          // <  w` />  //
// considering what this library is about. But, now that weird  //  \    /   //
// macro definition is gone I didn't need the padding. I really //  #UvUv#   //
// didn't want to delete the little guy though, so here he is.  //  ######   //
///////////////////////////////////////////////////////////////////////////////
//   |\_     //
//  /.  \/|  //
// <  w` />  //
//  \v v/    //
//  #UvU##   //
//  ######   //
///////////////
#endif
