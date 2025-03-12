#include "bruter.h"
#include "vocabulary.h"

// math basics
#ifndef Vector2
typedef struct
{
    float x;
    float y;
} Vector2;
#endif


// enums

enum 
{
    MATERIAL_SALT = 1,
    MATERIAL_HERB,
    MATERIAL_MEAT,

};

enum 
{
    PREFERENCE_TEMPERATURE = 1,
    PREFERENCE_DIRECTION,
    PREFERENCE_NUMBER,
    PREFERENCE_FOOD
};

enum 
{
    TRAIT_MOVE = 1,
    TRAIT_EAT,
    TRAIT_PHOTOSYNTHESIS,
    TRAIT_BREATH,
    TRAIT_SLEEP,
    TRAIT_REPRODUCE,
    TRAIT_GROW,
    TRAIT_HERBIVORE,
    TRAIT_CARNIVORE,

};

enum 
{
    LEFT_DOWN = 1,
    DOWN,
    RIGHT_DOWN,
    LEFT,
    CENTER,
    RIGHT,
    LEFT_UP,
    UP,
    RIGHT_UP
};

typedef List(short) ShortList;

typedef struct
{
    char* name;
    char* code;
} Behaviour;
typedef List(Behaviour) BehaviourList;

typedef struct
{
    char type;
    int value;
} Preference;
typedef List(Preference) PreferenceList;

typedef struct 
{
    char* name;
    short value;
    short max;
} Need;
typedef List(Need) NeedList;

typedef struct 
{
    char* name;
    BehaviourList *behaviours;
    PreferenceList *preferences;
    CharList *traits;
    NeedList *needs;
} Specie;
typedef List(Specie) SpecieList;

typedef struct
{
    char* name;
    short specie;
    short type;

    // birth
    int birth_x;
    int birth_y;
    int birth_time;

    // position
    int x;
    int y;

    // needs
    NeedList *needs;
} Life;
typedef List(Life) LifeList;


typedef char** Map8;
typedef short** Map16;
typedef int** Map32;

typedef struct
{
    SpecieList *species;
    LifeList *lifes;

    Map16 creature;
    Map8 temperature;
    Map8 quality; // water quality
    Map8 material;
} World;

