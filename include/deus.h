/**
 * DEUS - Defines Enums Unions Structs, "god" in Latin.
 * Why is this file called "god"?!
 * Well, this header file is included in pretty much 
 * every other file in the project, so by analogy, 
 * "all originates from God" -> all originates from this header.
 * That's why this header is DIVINE.
 * Special care shall be given to this header
 * as breaking something here will break the entire project.
 */

#ifndef DEUS_H
#define DEUS_H



#include <stdio.h>
#include <windows.h>
#include <SDL.h>
#include <SDL_mixer.h>

#define true 1
#define false 0

#define tileColorAmount 8 //7 + background
#define soundtracksAmount 3
#define soundEffectAmount 9 //left, right and down movement, drop, rotations, line clears
#define tileQueuedAmount 4
#define loggingBufferSize 1024

#define defaultFallSpeed 1000
#define defaultSpeedMultiplier 5
#define defaultGridHeight 20
#define defaultGridWidth 10

#define DEBUG //for diagnostic logging, debugging, etc.

#define TEST //for testing new stuff

typedef unsigned long long size_t;

typedef struct Point {
    int x, y;
} Point;

typedef struct Size {
    unsigned int width, height;
} Size;

typedef union int32_u {
    int integer;
    char bytes[4];
} int32_u;

typedef union Color {
    unsigned int color;
    struct {
        unsigned char red;
        unsigned char green;
        unsigned char blue;
        unsigned char alpha;
    };
} Color;

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
    MIX_OPEN_FAILURE,
    SDL_WINDOW_FAILURE,
    SDL_RENDERER_FAILURE,
    MUTEX_FAILURE,
    SEMAPHORE_FAILURE,
    THREAD_START_FAILURE
} status_t;

typedef enum LogLevel {
    LOGLEVEL_INFO,
    LOGLEVEL_WARNING,
    LOGLEVEL_ERROR,
    LOGLEVEL_DEBUG
} LogLevel;

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
    int test;
} Keymap;

/**
 * @brief Simple union to convert a Keymap struct to an array of integers.
 */
typedef union Keymap_array {
    Keymap keymap;
    int keys[sizeof(Keymap) / sizeof(int)];
} Keymap_array;

typedef enum TileColor {
    COLOR_UNKNOWN = -1,
    AQUA = 1,
    BLUE,
    GREEN,
    MAGENTA,
    ORANGE,
    RED,
    YELLOW,
    GHOST = 0x7F
} TileColor;

typedef enum TileShape {
    SHAPE_UNKNOWN,
    BASE,
    BAR,
    J,
    L,
    S,
    SQUARE,
    T,
    Z,
    BACKGROUND
} TileShape;

typedef enum TileState {
    STATE_UNKNOWN,
    BAR_HORIZONTAL_UP,
    BAR_HORIZONTAL_DOWN,
    BAR_VERTICAL_LEFT,
    BAR_VERTICAL_RIGHT,
    J_0,
    J_90,
    J_180,
    J_270,
    L_0,
    L_90,
    L_180,
    L_270,
    S_0,
    S_90,
    S_180,
    S_270,
    SQR,
    T_0,
    T_90,
    T_180,
    T_270,
    Z_0,
    Z_90,
    Z_180,
    Z_270
} TileState;

/**
 * @brief Struct for holding tiles.
 */
typedef struct Tile {
    SDL_Texture* texture;
    SDL_Rect rect;
    double angle;
    TileColor color;
    TileShape shape;
    TileState state;
    Point position;
} Tile;

typedef enum MovementSpeed {
    SPEED_NORMAL,
    SPEED_DROPSOFT, //sped up
    SPEED_HOLD //slowed down
} MovementSpeed;

typedef struct TileQueueElement {
    Tile* tile;
    struct TileQueueElement* next;
} TileQueueElement;

typedef struct TileQueue {
    size_t size;
    TileQueueElement* head;
    TileQueueElement* last;
} TileQueue;

//I guess the following 2 structs be marked as unnecesary, but eh.
typedef struct Soundtrack {
    Mix_Music* music;
} Soundtrack;

typedef struct SoundEffect {
    Mix_Chunk* sound;
} SoundEffect;

//1st bit - whether the program should continue running
//2nd bit - whether a game is played
//3rd bit - whether the game is paused
//7 bits of offset to bring the total struct size to 2 bytes
//11th and 12th bit hold the game speed
//13th and 14th bit hold the soundtrack to be played
//15th and 16th bit hold the soundtrack that is currently playing
typedef struct ProgramFlags {
    int running : 1;
    int playing : 1;
    int paused : 1;
    int __offset__: 7;
    MovementSpeed speed : 2;
    unsigned int soundtrack : 2;
    unsigned int soundtrackNowPlaying : 2;
} ProgramFlags;

/**
 * @brief Struct for holding program parameters.
 */
typedef struct ProgramParameters {
    ProgramFlags flags;
    Size screenSize;
    unsigned int fps;
    unsigned short baseFallSpeed;
    unsigned short speedMultiplier;
    unsigned short baseTileSize;
    Keymap keymap;
    //If it's > 0, scale up, if it's < 0, scale down
    short scalingFactor;
    
    LARGE_INTEGER* clockFrequency;
    
    FILE* generallog;
    FILE* errorlog;
    FILE* debugLog;
    
    SDL_Texture* baseTextures[tileColorAmount];
    SDL_Texture* digits[10];
    
    int** grid;
    Size gridSize;
    
    Tile* currentTile;
    Tile* heldTile;
    TileQueue tileQueue;
    
    Soundtrack soundtracks[soundtracksAmount];
    SoundEffect soundEffects[soundEffectAmount];
    unsigned char soundEffectsVolume; //max is 128
    unsigned char soundtracksVolume; //same here
    
    size_t score;
    unsigned int combo;
    unsigned int level;
} ProgramParameters;

typedef enum TileLoadingFlags {
    TILELOAD_NOTEXTURE = 1
} TileLoadingFlags;

/**
 * @brief Struct for holding parameters for the render thread.
 */
typedef struct renderThreadParameters {
    loopStatus_t* renderStatus;
    HANDLE renderMutex;
    HANDLE tilesMutex;
    ProgramParameters* programParameters;
    SDL_Renderer* renderer;
    Color* backgroundColor;
} renderThreadParameters;

typedef struct clockThreadParameters {
    loopStatus_t* clockStatus;
    LARGE_INTEGER* timer;
} clockThreadParameters;

typedef enum Action {
    ACTION_MOVELEFT,
    ACTION_MOVERIGHT,
    ACTION_MOVEDOWN,
    ACTION_DROPHARD,
    ACTION_ROTATECLOCKWISE,
    ACTION_ROTATECOUNTERCLOCKWISE,
    ACTION_HOLD
} Action;

typedef enum PointsPerAction {
    POINTS_HOLD = -2,
    POINTS_SOFTDROP = 1,
    POINTS_HARDDROP,
    POINTS_COMBO_MULTIPLIER = 50,
    POINTS_SINGLE = 100,
    POINTS_DOUBLE = 300,
    POINTS_TRIPLE = 500,
    POINTS_QUAD = 800
} PointsPerAction;


#endif