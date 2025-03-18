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

void life_move(World* world, Life* life, char direction)
{
    Vector2 new_position = apply_direction((Vector2){life->x, life->y}, direction);
    if (new_position.x >= 0 && new_position.x < world->size.x && new_position.y >= 0 && new_position.y < world->size.y)
    {
        world->creature[(Int)new_position.x][(Int)new_position.y] = world->creature[life->x][life->y];
        world->creature[life->x][life->y] = -1;
        life->x = new_position.x;
        life->y = new_position.y;
    }
}

void increase_preference(PreferenceList *preferences, char type, int value)
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

void add_need(NeedList *needs, char* name, Float value)//find need with name, if found add value
{
    for (int i = 0; i < needs->size; i++)
    {
        if (strcmp(needs->data[i].name, name) == 0)
        {
            needs->data[i].value += value;
            return;
        }
    }
}

Float get_need(NeedList *needs, char* name)
{
    Float result = -1;
    for (int i = 0; i < needs->size; i++)
    {
        if (strcmp(needs->data[i].name, name) == 0)
        {
            result = needs->data[i].value;
            break;
        }
    }
    return result;
}

void set_need(NeedList *needs, char* name, Float value)
{
    for (int i = 0; i < needs->size; i++)
    {
        if (strcmp(needs->data[i].name, name) == 0)
        {
            needs->data[i].value = value;
            return;
        }
    }
}

char have_trait(CharList traits, char trait)
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

char preferred_direction(PreferenceList preferences, char default_direction)
{
    if (preferences.size == 0)
    {
        return random_int(0, 7);
    }
    else
    {
        return default_direction;
    }
}

char find_free_direction(World* world, Life* life)
{
    char direction = random_int(0, 7);
    Vector2 new_position;
    for (int i = 0; i < 8; i++)
    {
        Vector2 new_position = apply_direction((Vector2){life->x, life->y}, direction);
        if (world->creature[(Int)new_position.x][(Int)new_position.y] == -1)
        {
            return direction;
        }
    }
    return -1;
}

void _new_life(World* world, int specie, int birth_x, int birth_y, int birth_time)
{
    Life life;
    life.specie = specie;
    life.birth_x = birth_x;
    life.birth_y = birth_y;
    life.birth_time = birth_time;
    life.x = life.birth_x;
    life.y = life.birth_y;

    // make a hard copy of the preferences and needs from the specie
    life.preferences = list_init(PreferenceList);
    
    for (int i = 0; i < world->species->data[specie].preferences->size; i++)
    {
        Preference preference;
        preference.type = world->species->data[specie].preferences->data[i].type;
        preference.value = world->species->data[specie].preferences->data[i].value;
        
        list_push(*life.preferences, preference);
    }

    life.needs = list_init(NeedList);

    Need need = {name: "energy", value: 50, max: 100};

    list_push(*life.needs, need);

    world->species->data[specie].population++;

    world->creature[life.x][life.y] = world->lifes->size;

    list_push(*world->lifes, life);
}

// bruter functions

function(new_world)
{
    Float size_x = arg(0).number;
    Float size_y = arg(1).number;

    register_number(vm, "world.size.x", size_x);
    register_number(vm, "world.size.y", size_y);
    
    World* world = malloc(sizeof(World));
    world->creature = make_map16(size_x, size_y, -1);
    world->temperature = make_map8(size_x, size_y, 25);
    world->light = make_map8(size_x, size_y, 100);
    world->material = make_map8(size_x, size_y, 0);
    
    world->species = list_init(SpecieList);
    world->lifes = list_init(LifeList);
    world->time = 0;

    world->size = (Vector2){size_x, size_y};

    Int index = new_var(vm);
    data(index).pointer = world;

       
    return index;
}

function(new_specie)
{
    World* world = (World*)arg(0).pointer;

    Specie specie;
    specie.name = generate_name(1);
    specie.behaviours = list_init(BehaviourList);
    specie.preferences = list_init(PreferenceList);
    specie.traits = list_init(CharList);
    specie.needs = list_init(NeedList);
    specie.population = 0;

    Need _energy = {name: "energy", value: 50, max: 100};
    list_push(*specie.needs, _energy);

    list_push(*world->species, specie);
    
    return new_number(vm, world->species->size-1);
}

function(new_life)
{
    World* world = (World*)arg(0).pointer;

    Int specie_index = arg(1).number;
    Int birth_x = arg(2).number;
    Int birth_y = arg(3).number;

    Int birth_time = world->time;

    _new_life(world, specie_index, birth_x, birth_y, birth_time);

    return new_number(vm, world->lifes->size-1);
}

function(push_trait)
{
    World* world = (World*)arg(0).pointer;
    Int specie_index = arg(1).number;
    Int trait = arg(2).number;

    Specie* specie = &world->species->data[specie_index];
    list_push(*specie->traits, trait);

    return -1;
}

function(push_behaviour)
{
    World* world = (World*)arg(0).pointer;
    Int specie_index = arg(1).number;

    Behaviour behaviour;
    behaviour.name = arg(2).string;
    behaviour.code = arg(3).string;

    Specie* specie = &world->species->data[specie_index];
    list_push(*specie->behaviours, behaviour);

    return -1;
}

function(push_preference)
{
    World* world = (World*)arg(0).pointer;
    Int specie_index = arg(1).number;
    char type = arg(2).string[0];
    int value = arg(3).number;

    Specie* specie = &world->species->data[specie_index];
    increase_preference(specie->preferences, type, value);

    return -1;
}

function(push_need)
{
    World* world = (World*)arg(0).pointer;
    Int specie_index = arg(1).number;
    char* name = arg(2).string;
    int value = arg(3).number;
    int max = arg(4).number;

    Specie* specie = &world->species->data[specie_index];
    Need need = {name: name, value: value, max: max};
    list_push(*specie->needs, need);

    return -1;
}



function(life_photosynthesis)
{
    World* world = (World*)arg(0).pointer;
    Int life_index = arg(1).number;
    Life* life = &world->lifes->data[life_index];

    if (have_trait(*world->species->data[life->specie].traits, TRAIT_PHOTOSYNTHESIS))
    {
        // each 10 light points, 1 energy
        Float energy = world->light[life->x][life->y] / 10;
        add_need(life->needs, "energy", energy);

        // if energy is greater than 50, the plant will grow
        if (get_need(life->needs, "energy") >= 50)
        {
            // lets find a random direction
            char direction = preferred_direction(*world->species->data[life->specie].preferences, random_int(0, 7));
            if (world->creature[life->x][life->y] != -1)
            {
                direction = find_free_direction(world, life);
                if (direction == -1)
                {
                    // canf move
                    return -1;
                }
            }
            
            Vector2 new_position = apply_direction((Vector2){life->x, life->y}, direction);
            if (new_position.x >= 0 && new_position.x < world->size.x && new_position.y >= 0 && new_position.y < world->size.y)
            {
                
                _new_life(world, life->specie, new_position.x, new_position.y, world->time);
            }
        }

    }

    return -1;
}

function(game_turn)
{
    World* world = (World*)arg(0).pointer;
    Int self = hash_find(vm, "self");


    for (int i = 0; i < world->lifes->size; i++)
    {
        Life* life = &world->lifes->data[i];
        for (int j = 0; j < world->species->data[life->specie].behaviours->size; j++)
        {
            vm->stack->data[self].number = i;
            Behaviour* behaviour = &world->species->data[life->specie].behaviours->data[j];
            eval(vm, behaviour->code);
            
        }
    }

    // print map
    for (int i = 0; i < world->size.x; i++)
    {
        for (int j = 0; j < world->size.y; j++)
        {
            if (world->creature[i][j] != -1)
            {
                printf("X");
            }
            else
            {
                printf(".");
            }
        }
        printf("\n");
    }
    printf("\n");

    world->time++;
    return -1;
}

function(free_world)
{
    World* world = (World*)arg(0).pointer;
    for (int i = 0; i < world->species->size; i++)
    {
        free(world->species->data[i].name);
        list_free(*world->species->data[i].behaviours);
        list_free(*world->species->data[i].preferences);
        list_free(*world->species->data[i].traits);
        list_free(*world->species->data[i].needs);
    }
    list_free(*world->species);
    for (int i = 0; i < world->lifes->size; i++)
    {
        list_free(*world->lifes->data[i].preferences);
        list_free(*world->lifes->data[i].needs);
    }
    list_free(*world->lifes);
    for (int i = 0; i < world->size.x; i++)
    {
        free(world->creature[i]);
    }
    free(world->creature);
    for (int i = 0; i < world->size.x; i++)
    {
        free(world->temperature[i]);
    }
    free(world->temperature);
    for (int i = 0; i < world->size.x; i++)
    {
        free(world->light[i]);
    }
    free(world->light);
    for (int i = 0; i < world->size.x; i++)
    {
        free(world->material[i]);
    }
    free(world->material);
    free(world);
    return -1;
}

init(populosus)
{
    register_builtin(vm, "new_world", new_world);
    register_builtin(vm, "new_specie", new_specie);
    register_builtin(vm, "new_life", new_life);
    register_builtin(vm, "push_trait", push_trait);
    register_builtin(vm, "push_behaviour", push_behaviour);
    register_builtin(vm, "push_preference", push_preference);
    register_builtin(vm, "push_need", push_need);

    register_builtin(vm, "game_turn", game_turn);

    register_builtin(vm, "life_photosynthesis", life_photosynthesis);

    register_builtin(vm, "free_world", free_world);

    register_number(vm, "LEFT_DOWN", LEFT_DOWN);
    register_number(vm, "DOWN", DOWN);
    register_number(vm, "RIGHT_DOWN", RIGHT_DOWN);
    register_number(vm, "LEFT", LEFT);
    register_number(vm, "CENTER", CENTER);
    register_number(vm, "RIGHT", RIGHT);
    register_number(vm, "LEFT_UP", LEFT_UP);
    register_number(vm, "UP", UP);
    register_number(vm, "RIGHT_UP", RIGHT_UP);

    register_number(vm, "trait.herbivore", TRAIT_HERBIVORE);
    register_number(vm, "trait.carnivore", TRAIT_CARNIVORE);
    register_number(vm, "trait.sleep", TRAIT_SLEEP);
    register_number(vm, "trait.reproduce", TRAIT_REPRODUCE);
    register_number(vm, "trait.photosynthesis", TRAIT_PHOTOSYNTHESIS);
    register_number(vm, "trait.grow", TRAIT_GROW);
    register_number(vm, "trait.breath", TRAIT_BREATH);
    register_number(vm, "trait.eat", TRAIT_EAT);
    register_number(vm, "trait.move", TRAIT_MOVE);

    register_number(vm, "preference.temperature", PREFERENCE_TEMPERATURE);
    register_number(vm, "preference.direction", PREFERENCE_DIRECTION);
    register_number(vm, "preference.number", PREFERENCE_NUMBER);
    register_number(vm, "preference.material", PREFERENCE_MATERIAL);
    register_number(vm, "preference.light", PREFERENCE_LIGHT);

    register_number(vm, "material.salt", MATERIAL_SALT);
    register_number(vm, "material.herb", MATERIAL_HERB);
    register_number(vm, "material.meat", MATERIAL_MEAT);

    register_var(vm, "self");
    
}