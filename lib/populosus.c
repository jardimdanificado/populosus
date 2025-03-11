#include "bruter.h"

enum 
{
    item_air,
    item_water,
    item_plant
};

typedef char** World;

typedef struct
{
    World world;
    World temperature;
} ComplexWorld;

World make_world(Float size_x, Float size_y, char default_value)
{
    World world = malloc(sizeof(char*) * size_x);
    for (int i = 0; i < size_x; i++)
    {
        world[i] = malloc(sizeof(char) * size_y);
        for (int j = 0; j < size_y; j++)
        {
            world[i][j] = default_value;
        }
    }
    return world;
}

function(new_world)
{
    Float size_x = arg(0).number;
    Float size_y = arg(1).number;
    ComplexWorld* world = malloc(sizeof(ComplexWorld));
    world->world = make_world(size_x, size_y, item_air);
    world->temperature = make_world(size_x, size_y, 25);
    Int index = new_var(vm);
    data(index).pointer = world;
    return index;
}

init(populosus)
{
    register_builtin(vm, "new_world", new_world);
}