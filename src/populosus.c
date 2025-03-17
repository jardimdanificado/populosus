#include "populosus.h"

// math utils

void randomize_seed()
{
    srand(time(NULL) + clock());
}

Float random_float(Float min, Float max)
{
    return min + (Float)rand() / ((Float)RAND_MAX / (max - min));
}

Int random_int(Int min, Int max)
{
    return min + (Int)rand() / ((Int)RAND_MAX / (max - min));
}

Int percent_chance(Float percent)
{
    return random_float(0, 100) < percent;
}

char* generate_name(char compounds)
{
    randomize_seed();
    char* name = malloc(sizeof(char) * 100);
    name[0] = '\0';
    for (int i = 0; i < compounds; i++)
    {
        char* compound = vocabulary[random_int(0, sizeof(vocabulary) / sizeof(char*) - 1)];
        strcat(name, compound);
        if (i == compounds - 1)
        {
            break;  // whatever
        }
        else if (percent_chance(10))
        {
            strcat(name, " ");
        }
        else if (percent_chance(10))
        {
            strcat(name, "-");
        }
        else if (percent_chance(10))
        {
            switch (random_int(0, 5))
            {
                case 0:
                    strcat(name, " o ");
                    break;
                case 1:
                    strcat(name, " a ");
                    break;
                case 2:
                    strcat(name, " de ");
                    break;
                case 3:
                    strcat(name, " da ");
                    break;
                case 4:
                    strcat(name, " do ");
                    break;
            }
        }
    }
    // lets reallocate the name to the correct size
    name = realloc(name, sizeof(char) * strlen(name) + 1);
    return name;
}

char* scramble_name(const char* name) 
{
    size_t len = strlen(name);
    if (len < 8) {
        return strdup(name); // Retorna uma cópia para evitar modificar a string original
    }

    int parts = random_int(2, 3); // Ao menos 2 partes
    size_t part_size = len / parts; // Tamanho base de cada parte

    char** name_parts = malloc(parts * sizeof(char*));
    char* new_name = calloc(len + 1, sizeof(char)); // Inicializa zerado

    // Dividir o nome em partes
    for (int i = 0; i < parts; i++) {
        size_t actual_size = (i == parts - 1) ? (len - i * part_size) : part_size;
        name_parts[i] = malloc(actual_size + 1);
        strncpy(name_parts[i], name + i * part_size, actual_size);
        name_parts[i][actual_size] = '\0';
    }

    // Embaralhar as partes usando Fisher-Yates Shuffle
    for (int i = parts - 1; i > 0; i--) {
        int j = random_int(0, i);
        char* temp = name_parts[i];
        name_parts[i] = name_parts[j];
        name_parts[j] = temp;
    }

    // Concatenar as partes embaralhadas
    for (int i = 0; i < parts; i++) {
        strcat(new_name, name_parts[i]);
        free(name_parts[i]); // Liberar a parte após copiar
    }

    free(name_parts);
    // if first char is - or space, remove it
    if (new_name[0] == '-' || new_name[0] == ' ')
    {
        memmove(new_name, new_name + 1, strlen(new_name));
    }
    return new_name;
}

char* fuse_names(char* name1, char* name2)
{
    switch (random_int(0, 8))// parts name will be divided
    {
        case 0:
            return str_format    ("%s%s", name1, name2);
        case 1:
            return str_format    ("%s-%s", name1, name2);
        case 2:
            return str_format    ("%s %s", name1, name2);
        case 3: // the first half of 1 and the second half of 2
            return str_format    ("%.*s%.*s", strlen(name1) / 2, name1, strlen(name2) / 2, name2 + strlen(name2) / 2);
        case 4: // the first half of 2 and the second half of 1
            return str_format    ("%.*s%.*s", strlen(name2) / 2, name2, strlen(name1) / 2, name1 + strlen(name1) / 2);
        case 5: // the first half of 1 and the first half of 2
            return str_format    ("%.*s%.*s", strlen(name1) / 2, name1, strlen(name2) / 2, name2);
        case 6: // the second half of 1 and the second half of 2
            return str_format    ("%.*s%.*s", strlen(name1) / 2, name1 + strlen(name1) / 2, strlen(name2) / 2, name2 + strlen(name2) / 2);
        case 7: // the second half of 1 and the first half of 2
            return str_format    ("%.*s%.*s", strlen(name1) / 2, name1 + strlen(name1) / 2, strlen(name2) / 2, name2);
        case 8: // the first half of 2 and the first half of 1
            return str_format    ("%.*s%.*s", strlen(name2) / 2, name2, strlen(name1) / 2, name1);
    }
}

char* evolve_name(char* name) // add a new word to the name, or, if the name is less or equals to 99 chars remove the amount of char of the new word from a random position between 0 and max-wordsize
{
    char* new_name = malloc(sizeof(char) * 256);
    strcpy(new_name, name);
    if (strlen(new_name) <= 255)
    {
        char* word = vocabulary[random_int(0, sizeof(vocabulary) / sizeof(char*) - 1)];
        if (strlen(new_name) + strlen(word) + 6 <= 99)
        {
            switch (random_int(0, 8))
            {
                case 0:
                    strcat(new_name, " o ");
                    break;
                case 1:
                    strcat(new_name, " a ");
                    break;
                case 2:
                    strcat(new_name, " de ");
                    break;
                case 3:
                    strcat(new_name, " da ");
                    break;
                case 4:
                    strcat(new_name, " do ");
                    break;
                case 5:
                    strcat(new_name, " dos ");
                    break;
                case 6:
                    strcat(new_name, " das ");
                    break;
                case 7:
                    strcat(new_name, " ");
                    break;
                case 8:
                    strcat(new_name, "-");
                    break;
            }
            

            strcat(new_name, word);
        }
        else
        {
            int position = random_int(0, strlen(new_name) - strlen(word));
            memmove(new_name + position, new_name + position + strlen(word), strlen(new_name) - position - strlen(word));
        }
    }
    return new_name;
}


Map8 make_map8(Float size_x, Float size_y, char default_value)
{
    Map8 map = malloc(sizeof(char*) * size_x);
    for (int i = 0; i < size_x; i++)
    {
        map[i] = malloc(sizeof(char) * size_y);
        for (int j = 0; j < size_y; j++)
        {
            map[i][j] = default_value;
        }
    }
    return map;
}

Map16 make_map16(Float size_x, Float size_y, short default_value)
{
    Map16 map = malloc(sizeof(short*) * size_x);
    for (int i = 0; i < size_x; i++)
    {
        map[i] = malloc(sizeof(short) * size_y);
        for (int j = 0; j < size_y; j++)
        {
            map[i][j] = default_value;
        }
    }
    return map;
}

Map32 make_map32(Float size_x, Float size_y, int default_value)
{
    Map32 map = malloc(sizeof(int*) * size_x);
    for (int i = 0; i < size_x; i++)
    {
        map[i] = malloc(sizeof(int) * size_y);
        for (int j = 0; j < size_y; j++)
        {
            map[i][j] = default_value;
        }
    }
    return map;
}


Vector2 apply_direction(Vector2 vector, char direction)
{
    switch (direction)
    {
        case LEFT_DOWN:
            vector.x--;
            vector.y++;
            break;
        case DOWN:
            vector.y++;
            break;
        case RIGHT_DOWN:
            vector.x++;
            vector.y++;
            break;
        case LEFT:
            vector.x--;
            break;
        case CENTER:
            break;
        case RIGHT:
            vector.x++;
            break;
        case LEFT_UP:
            vector.x--;
            vector.y--;
            break;
        case UP:
            vector.y--;
            break;
        case RIGHT_UP:
            vector.x++;
            vector.y--;
            break;
    }
    return vector;
}

// game functions

void increaseoradd_preference(PreferenceList *preferences, char type, int value)
{
    int index = -1;
    for (int i = 0; i < preferences->size; i++)
    {
        if (preferences->data[i].type == type)
        {
            index = i;
            break;
        }
    }
    
    if (index != -1)
    {
        // lets swap this reference with the one before it, if its not zero
        if (index > 0)
        {
            list_swap(*preferences, index, index - 1);
        }
        
    }
    else
    {
        Preference preference;
        preference.type = type;
        preference.value = value;
        list_push(*preferences, preference);
    }
}

char is_trait(CharList traits, char trait)
{
    for (int i = 0; i < traits.size; i++)
    {
        if (traits.data[i] == trait)
        {
            return 1;
        }
    }
    return 0;
}

// bruter functions

function(new_world)
{
    Float size_x = arg(0).number;
    Float size_y = arg(1).number;

    register_number(vm, "world.size.x", size_x);
    register_number(vm, "world.size.y", size_y);
    
    World* world = malloc(sizeof(World));
    world->creature = make_map16(size_x, size_y, 0);
    world->temperature = make_map8(size_x, size_y, 25);
    world->quality = make_map8(size_x, size_y, 100);
    world->material = make_map8(size_x, size_y, 0);
    
    world->species = list_init(SpecieList);
    world->lifes = list_init(LifeList);

    Int index = new_var(vm);
    data(index).pointer = world;

       
    return index;
}

function(new_specie)
{
    Int world_index = arg_i(0);
    World* world = data(world_index).pointer;

    Specie specie;
    specie.name = arg(1).string;
    specie.behaviours = list_init(BehaviourList);
    specie.preferences = list_init(PreferenceList);
    specie.traits = list_init(CharList);
    specie.needs = list_init(NeedList);

    list_push(*world->species, specie);

    return new_number(vm, world->species->size);
}

function(new_life)
{
    Int world_index = arg_i(0);
    World* world = data(world_index).pointer;

    Life life;
    life.name = arg(1).string;
    life.specie = arg(2).number;
    life.type = arg(3).number;
    life.birth_x = arg(4).number;
    life.birth_y = arg(5).number;
    life.birth_time = arg(6).number;
    life.x = life.birth_x;
    life.y = life.birth_y;
    life.needs = list_init(NeedList);

    list_push(*world->lifes, life);

    return new_number(vm, world->lifes->size);
}

function(creature_move)
{
    Int world_index = arg_i(0);
    World* world = data(world_index).pointer;

    Int life_index = arg_i(1);
    Life* life = &world->lifes->data[life_index];

    int x = life->x;
    int y = life->y;

    char direction = arg(2).number;
    Vector2 position = {x, y};
    position = apply_direction(position, direction);
    if (position.x < 0 || position.x >= data(hash_find(vm, "world.size.x")).number || position.y < 0 || position.y >= data(hash_find(vm, "world.size.y")).number)
    {
        return -1;
    }
    else
    {
        if (world->creature[(Int)position.x][(Int)position.y] == 0)
        {
            world->creature[(Int)position.x][(Int)position.y] = world->creature[x][y];
            world->creature[x][y] = 0;
            life->x = position.x;
            life->y = position.y;
            return 1;
        }
        else
        {
            // maybe merge, maybe consume
            // TODO
        }
        return 1;
    }
    
    return -1;
}

function(creature_grow)
{
    Int world_index = arg_i(0);
    World* world = data(world_index).pointer;

    Int life_index = arg_i(1);
    Life* life = &world->lifes->data[life_index];

    CharList *traits = world->species->data[life->specie].traits;

    if (is_trait(*traits, TRAIT_PHOTOSYNTHESIS))
    {
        // lets find a direction preference
        int direction = -1;

        for (int i = 0; i < world->species->data[life->specie].preferences->size; i++)
        {
            if (world->species->data[life->specie].preferences->data[i].type == PREFERENCE_DIRECTION)
            {
                direction = world->species->data[life->specie].preferences->data[i].value;
                break;
            }
        }

        if (direction == -1)
        {
            while (direction == -1 || direction == CENTER)
            {
                direction = random_int(0, 8);
            }
            // add direction preference
            increaseoradd_preference(world->species->data[life->specie].preferences, PREFERENCE_DIRECTION, direction);
        }

        // lets check if there is a creature in the direction
        Vector2 position = {life->x, life->y};
        position = apply_direction(position, direction);
        if (position.x < 0 || position.x >= data(hash_find(vm, "world.size.x")).number || position.y < 0 || position.y >= data(hash_find(vm, "world.size.y")).number)
        {
            position = (Vector2){life->x, life->y};
            for (int i = 1; i < 9; i++)
            {
                if ( i != CENTER)
                {
                    position = apply_direction(position, i);
                    if (position.x >= 0 && position.x < data(hash_find(vm, "world.size.x")).number && position.y >= 0 && position.y < data(hash_find(vm, "world.size.y")).number)
                    {
                        if (world->creature[(Int)position.x][(Int)position.y] == 0)
                        {
                            world->creature[(Int)position.x][(Int)position.y] = life_index;
                            // add direction preference
                            Preference preference;
                            preference.type = PREFERENCE_DIRECTION;
                            preference.value = direction;
                            list_push(*world->species->data[life->specie].preferences, preference);
                            return 1;
                        }
                    }
                }
            }
        }
        else
        {
            if (world->creature[(Int)position.x][(Int)position.y] == 0)
            {
                world->creature[(Int)position.x][(Int)position.y] = life_index;
                return 1;
            }
            else
            {
                // maybe merge, maybe consume
                // TODO
            }
        }

        // photosynthesis
        for (int i = 1; i < 9; i++)
        {

        }
    }
    else
    {
        // animals cant grow(not in this way)
    }
    

    return 1;
}

function(creature_consume)
{
    Int world_index = arg_i(0);
    World* world = data(world_index).pointer;

    Int life_index = arg_i(1);
    Life* life = &world->lifes->data[life_index];



    CharList *traits = world->species->data[life->specie].traits;
    
    if(list_find(*traits, TRAIT_PHOTOSYNTHESIS) != -1)
    {
        // photosynthesis
    }
    else
    {

        char direction = arg(2).number;
        Vector2 position = {life->x, life->y};
        position = apply_direction(position, direction);
        if (position.x < 0 || position.x >= data(hash_find(vm, "world.size.x")).number || position.y < 0 || position.y >= data(hash_find(vm, "world.size.y")).number)
        {
            return -1;
        }
        else
        {
            if (world->material[(Int)position.x][(Int)position.y] == MATERIAL_MEAT)
            {
                world->material[(Int)position.x][(Int)position.y] = 0;
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
}

function(print_random_name)
{
    char* name = generate_name(2);
    printf("%s\n", name);
    free(name);
    return -1;
}

function(print_scrambled_name)
{
    char* name = generate_name(2);
    name = scramble_name(name);
    printf("%s\n", name);
    free(name);
    return -1;
}

function(print_evolved_name)
{
    char* name = generate_name(2);
    name = evolve_name(name);
    name = evolve_name(name);
    name = evolve_name(name);
    printf("%s\n", name);
    free(name);
    return -1;
}

init(populosus)
{
    register_builtin(vm, "new_world", new_world);
    register_builtin(vm, "new_specie", new_specie);
    register_builtin(vm, "new_life", new_life);

    register_builtin(vm, "print_random_name", print_random_name);
    register_builtin(vm, "print_scrambled_name", print_scrambled_name);
    register_builtin(vm, "print_evolved_name", print_evolved_name);
}