#ifndef STRUCTS_UNIONS_H
#define STRUCTS_UNIONS_H


#include <windows.h>
#include <SDL.h>

typedef unsigned long long size_t;

typedef struct Point {
    int x, y;
} Point;
typedef union int32_u {
    int integer;
    char bytes[4];
} int32_u;

typedef struct QueueElement {
    SDL_Event event;
    struct QueueElement* next;
} QueueElement;

typedef struct Queue {
    size_t size;
    QueueElement* head;
    QueueElement* last;
} Queue;

/**
 * @brief Simple enum for a status of an operation. Used in otherwise 'void' functions.
 */
typedef enum status_t {
    SUCCESS,
    FAILURE,
    BASEOUTOFRANGE,
    BUFFERTOOSMALL,
    MEMORY_FAILURE,
    FILEOPEN_FAILURE,
    LOADCONFIG_FAILURE,
    SDL_INIT_FAILURE,
    IMG_INIT_FAILURE,
    SDL_WINDOW_FAILURE,
    SDL_RENDERER_FAILURE,
    MUTEX_FAILURE,
    SEMAPHORE_FAILURE,
    THREAD_START_FAILURE
} status_t;

/**
 * @brief Loop status enum. Used for controlling execution of loops outside the main thread.
 */
typedef enum loopStatus_t {
    CONTINUE,
    STOP
} loopStatus_t;

/**
 * @brief Keymap struct to hold all keybinds. Maps to SDL's keycode.
 */
typedef struct Keymap {
    int movePieceLeft;
    int movePieceRight;
    int rotateClockwise;
    int rotateCounterClockwise;
    int dropSoft;
    int dropHard;
    int hold; //7 keys

    int pause;
} Keymap;

/**
 * @brief Simple union to convert a Keymap struct to an array of integers.
 */
typedef union Keymap_array {
    Keymap keymap;
    int keys[8];
} Keymap_array;

/**
 * @brief Struct for holding program parameters.
 */
typedef struct ProgramParameters {
    int screen_width;
    int screen_height;
    int fps;
    Keymap keymap;
    LARGE_INTEGER* clockFrequency;
    LARGE_INTEGER* timer;
    FILE* generallog;
    FILE* errorlog;
    FILE* debugLog;
} ProgramParameters;

/**
 * @brief Struct for holding tiles.
 */
typedef struct Tile {
    SDL_Texture* texture;
    SDL_Rect rect;
} Tile;

/**
 * @brief Struct for holding parameters for the render thread.
 */
typedef struct renderThreadParameters {
    loopStatus_t* renderStatus;
    HANDLE renderMutex;
    HANDLE tilesMutex;
    ProgramParameters* programParameters;
    SDL_Renderer* renderer;
    Tile** tiles;
    size_t tilesAmount;
} renderThreadParameters;

typedef struct logicLoopParameters {
    loopStatus_t* logicStatus;
    HANDLE logicMutex;
    HANDLE tilesMutex;
    ProgramParameters* programParameters;
    Queue* eventQueue;
    Tile** tiles;
    size_t tilesAmount;
} logicLoopParameters;

typedef enum TileColor{
    COLOR_UNKNOWN,
    AQUA,
    BLUE,
    GREEN,
    MAGENTA,
    RED,
    YELLOW
} TileColor;

typedef enum TileShape {
    SHAPE_UNKNOWN,
    BASE,
    BAR,
    L,
    S,
    SQUARE,
    T,
    BACKGROUND
} TileShape;

typedef struct clockThreadParameters {
    loopStatus_t* clockStatus;
    LARGE_INTEGER* timer;
} clockThreadParameters;


#endif