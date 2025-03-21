#include <buxu.h>
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
    STATE_DEAD, // the dead state, no behaviours will be done
    STATE_DEFAULT, // the default state, will do all the behaviours
    STATE_SLEEP, // the sleep state, some behaviours will not be done
    STATE_HIBERNATE, // the hibernate state, just decay behaviour will be done
};

enum 
{
    PREFERENCE_TEMPERATURE = 1,
    PREFERENCE_DIRECTION,
    PREFERENCE_LIGHT,
    MIN_TEMPERATURE,
    MAX_TEMPERATURE,
    MIN_LIGHT,
    MAX_LIGHT,
    MIN_ENERGY,// MAYBE AWALYS 0
    MAX_ENERGY,
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
    TRAIT_CARNIVORE
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
    float temperature;
    float light;
    float energy;
    float health;
} SpecieLimitBasic;

typedef struct 
{
    SpecieLimitBasic min;
    SpecieLimitBasic max;
} SpecieLimits;

typedef struct 
{
    char* name;
    BehaviourList *behaviours;
    PreferenceList *preferences;
    ByteList *traits;
    SpecieLimits limits;
    Int population;
    float dna[8];
} Specie;
typedef List(Specie) SpecieList;

typedef struct
{
    //char* name;
    Specie* specie;

    // birth
    int birth_x;
    int birth_y;
    int birth_time;

    // position
    int x;
    int y;

    // needs
    float energy;
    float health;
    
    char state;

} Life;
typedef List(Life) LifeList;


typedef char** Map8;
typedef short** Map16;
typedef int** Map32;
typedef float** MapFloat;

typedef struct
{
    SpecieList *species;
    LifeList *lifes;

    Map16 creature;
    MapFloat temperature;
    MapFloat light;
    Vector2 size;

    Int time;
} World;

