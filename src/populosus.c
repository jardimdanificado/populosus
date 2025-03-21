#include "populosus.h"

// math utils

void randomize_seed()
{
    srand(time(NULL) + clock());
}

Float random_float(Float min, Float max)
{
    randomize_seed();
    return min + (Float)rand() / ((Float)RAND_MAX / (max - min + 1));
}

Int random_int(Int min, Int max)
{
    randomize_seed();
    return min + (Int)rand() / ((Int)RAND_MAX / (max - min + 1));
}

Int percent_chance(Float percent)
{
    randomize_seed();
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

MapFloat make_map_float(Float size_x, Float size_y, float default_value)
{
    MapFloat map = malloc(sizeof(float*) * size_x);
    for (int i = 0; i < size_x; i++)
    {
        map[i] = malloc(sizeof(float) * size_y);
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

char have_trait(ByteList traits, char trait)
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
        return default_direction;
    }
    else
    {
        // lets find the most preferred direction
        for (int i = 0; i < preferences.size; i++)
        {
            if (preferences.data[i].type == PREFERENCE_DIRECTION)
            {
                return preferences.data[i].value;
            }
        }
        return default_direction;
    }
}

char find_free_direction(World* world, Life* life)
{
    char direction = random_int(1, 9);
    Vector2 new_position;
    for (int i = 0; i < 8; i++)
    {
        Vector2 new_position = apply_direction((Vector2){life->x, life->y}, direction);
        if (new_position.x >= 0 && new_position.x < world->size.x && new_position.y >= 0 && new_position.y < world->size.y)
        {
            if (world->creature[(Int)new_position.x][(Int)new_position.y] == -1)
            {
                return direction;
            }
        }
    }
    return -1;
}

void _new_life(World* world, Specie* specie, int birth_x, int birth_y, int birth_time)
{
    Life life;
    life.specie = specie;
    life.birth_x = birth_x;
    life.birth_y = birth_y;
    life.birth_time = birth_time;
    life.x = birth_x;
    life.y = birth_y;
    life.energy = specie->limits.max.energy / 2;
    life.health = specie->limits.max.health;
    life.state = STATE_DEFAULT;

    printf("specie %s max energy %f and max health %f\n", specie->name, specie->limits.max.energy, specie->limits.max.health);
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
    world->temperature = make_map_float(size_x, size_y, 20);
    world->light = make_map_float(size_x, size_y, 1024);
    
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
    specie.traits = list_init(ByteList);
    specie.population = 0;
    specie.dna[0] = 1;
    specie.dna[1] = 1;
    specie.dna[2] = 1;
    specie.dna[3] = 1;
    specie.dna[4] = 1;
    specie.dna[5] = 1;
    specie.dna[6] = 1;
    specie.dna[7] = 1;

    specie.limits.min.temperature = 0;
    specie.limits.max.temperature = 128;
    specie.limits.min.light = 0;
    specie.limits.max.light = 2048;
    specie.limits.min.energy = 0;
    specie.limits.max.energy = 100;
    specie.limits.min.health = 0;
    specie.limits.max.health = 100;

    list_push(*world->species, specie);
    
    Int result = new_var(vm);
    data(result).pointer = &world->species->data[world->species->size-1];

    

    return result;
}

function(new_life)
{
    World* world = (World*)arg(0).pointer;

    Specie* specie = arg(1).pointer;
    Int birth_x = arg(2).number;
    Int birth_y = arg(3).number;

    Int birth_time = world->time;

    _new_life(world, specie, birth_x, birth_y, birth_time);
    printf("new life of specie \"%s\" has born at %d %d at time %d with energy %f and health %f\n", specie->name, birth_x, birth_y, birth_time, specie->limits.max.energy / 2, specie->limits.max.health);
    return new_number(vm, world->lifes->size-1);
}

function(push_trait)
{
    World* world = (World*)arg(0).pointer;
    Specie* specie = arg(1).pointer;
    Int trait = arg(2).number;

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

function(set_preference)
{
    World* world = (World*)arg(0).pointer;
    Int specie_index = arg(1).number;
    char type = arg(2).string[0];
    int value = arg(3).number;

    Specie* specie = &world->species->data[specie_index];
    increase_preference(specie->preferences, type, value);

    return -1;
}

// behaviours
// behaviours
// behaviours

function(life_burn_energy) // burn energy and increase temperature
{
    World* world = (World*)arg(0).pointer;
    Int life_index = arg(1).number;
    Life* life = &world->lifes->data[life_index];
    
    float amount = arg(2).number;
    float rate = 1;
    //if arg(3) exists use it as burn rate
    if (args->size > 3)
    {
        rate = arg(3).number;
    }
    rate *= life->specie->dna[4];
    
    life->energy -= amount * life->specie->dna[4] * life->specie->dna[1];
    life->health -= amount * life->specie->dna[3] * 0.999;
    world->temperature[life->x][life->y] += amount * rate;
    printf("life %d of specie \"%s\" burned %f energy, and %f health\n", life_index, life->specie->name, amount, amount * rate);



    

    return -1;
}

function(life_photosynthesis) // consume light and turn it into energy
{
    World* world = (World*)arg(0).pointer;
    Int life_index = arg(1).number;
    Life* life = &world->lifes->data[life_index];
    if (have_trait(*life->specie->traits, TRAIT_PHOTOSYNTHESIS))
    {
        // generate energy from light based of the amount of light avaliable, can absobing at max 10% of its max, and 5% of the world light of the current tile
        // this means in a world with 2048 light, the plant can absorb each turn 102.4 light, and 51.2 light from the tile
        float light = world->light[life->x][life->y];
        float max_light = life->specie->limits.max.light;
        float max_energy = life->specie->limits.max.energy;
        float max_light_gain = max_light * 0.05;
        float light_gain = 0;

        if (light > max_light_gain)
        {
            light_gain = max_light_gain * (0.1 * life->specie->dna[1]) * (life->health / life->specie->limits.max.health);
        }
        else
        {
            light_gain = light;
        }

        if (light_gain > max_energy * (0.1 * life->specie->dna[1]))
        {
            light_gain = max_energy * (0.1 * life->specie->dna[1]);
        }



        

        life->energy += light_gain;
        world->light[life->x][life->y] -= light_gain;
    }
    
    return -1;
}

function(life_grow)
{
    World* world = (World*)arg(0).pointer;
    Int life_index = arg(1).number;
    Life* life = &world->lifes->data[life_index];

    if (have_trait(*life->specie->traits, TRAIT_PHOTOSYNTHESIS))
    {
        // if energy is greater than 50, the plant will try to grow
        if (life->energy >= (50*life->specie->dna[1]))
        {
            // lets find a random direction
            char direction;

            if(percent_chance(90)*life->specie->dna[0])// (90*dna[0])% chance to not follow the preference
            {
                direction = random_int(1, 9);
            }
            else // follow the preference
            {
                direction = preferred_direction(*life->specie->preferences, CENTER);
            }

            Vector2 new_position = apply_direction((Vector2){life->x, life->y}, direction);
            if (new_position.x >= 0 && new_position.x < world->size.x && new_position.y >= 0 && new_position.y < world->size.y && world->creature[(Int)new_position.x][(Int)new_position.y] != -1)
            {  // if the tile is occupied, try to find a free direction
                direction = find_free_direction(world, life);
                if (direction == -1 || direction == CENTER)
                {
                    // canf move, use the same energy to increase health
                    life->health += ((50 - (life->health / 8))*life->specie->dna[2]*life->specie->dna[3]);
                    return -1;
                }
            }
            
            new_position = apply_direction((Vector2){life->x, life->y}, direction);
            if (new_position.x >= 0 && new_position.x < world->size.x && new_position.y >= 0 && new_position.y < world->size.y)
            {
                increase_preference(life->specie->preferences, PREFERENCE_DIRECTION, direction);
                world->creature[(Int)new_position.x][(Int)new_position.y] = life_index;
                life->x = new_position.x;
                life->y = new_position.y;
            }
            life->energy -= (50 - (life->health / 8));
            life->health /= 1.5;
            printf("life %d of specie \"%s\" grew to %d %d\n", life_index, life->specie->name, life->x, life->y);
            printf("life %d of specie \"%s\" now has %f energy and %f health\n", life_index, life->specie->name, life->energy, life->health);
        }
        else if (life->health < life->specie->limits.max.health) 
        {
            // if energy is less than 50 and bigger than 2, use 1 energy to gain 0.5 health
            if (life->energy > 2)
            {
                life->energy -= 1*life->specie->dna[1];
                life->health += (0.5*life->specie->dna[2]);
            }
        }
        else // if health is max, burn energy
        {
            char* code = str_format("life_burn_energy world %d %f %d", life_index, 1.0, 2*life->specie->dna[4]);
            eval(vm, code);
            free(code);
        }
    }
}

function(life_check)
{
    World* world = (World*)arg(0).pointer;
    Int life_index = arg(1).number;
    Life* life = &world->lifes->data[life_index];

    if (life->health <= life->specie->limits.min.health)
    {
        life->state = STATE_DEAD;
        printf("life %d of specie \"%s\" died at time %d from lack of health, it had %f health, and the minimum of its specie is %f\n", life_index, life->specie->name, world->time, life->health, life->specie->limits.min.health);
        return -1;
    }
    else if (life->health >= life->specie->limits.max.health)
    {
        life->health = life->specie->limits.max.health; // lets not go over the limit
        //printf("life %d of specie \"%s\" is at max health\n", life_index, life->specie->name);
    }

    if (life->energy <= life->specie->limits.min.energy)
    {
        life->state = STATE_DEAD;
        printf("life %d of specie \"%s\" died at time %d from lack of energy, it had %f energy, and the minimum of its specie is %f\n", life_index, life->specie->name, world->time, life->energy, life->specie->limits.min.energy);
    }
    else if (life->energy > life->specie->limits.max.energy) // burn extra energy immediately at a high rate of temperature generation
    {
        float extra_energy = life->energy - life->specie->limits.max.energy;
        
        char* code = str_format("life_burn_energy world %d %f 4", life_index, extra_energy);
        eval(vm, code);
        free(code);
        
        
    }
    

    return -1;
}

function(life_decay)
{
    World* world = (World*)arg(0).pointer;
    Int life_index = arg(1).number;
    Life* life = &world->lifes->data[life_index];

    life->energy -= 0.1 * life->specie->dna[4] * (life->health / life->specie->limits.max.health);
    life->health -= 0.1 * life->specie->dna[3];
    world->temperature[life->x][life->y] += 0.1;

    return -1;
}

function(game_turn)
{
    World* world = (World*)arg(0).pointer;
    Int self = hash_find(vm, "self");


    for (int i = 0; i < world->lifes->size; i++)
    {
        Life* life = &world->lifes->data[i];

        if (life->state == STATE_DEAD) // this guy cant do anything, next!
        {
            continue;
        }

        for (int j = 0; j < life->specie->behaviours->size; j++)
        {
            vm->stack->data[self].number = i;
            Behaviour* behaviour = &life->specie->behaviours->data[j];
            eval(vm, behaviour->code);
            
        }
    }
    world->time++;
    return -1;
}

function(print_world)
{
    World* world = (World*)arg(0).pointer;


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
    }
    list_free(*world->species);
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
    register_builtin(vm, "set_preference", set_preference);
    register_builtin(vm, "game_turn", game_turn);

    register_builtin(vm, "life_photosynthesis", life_photosynthesis);
    register_builtin(vm, "life_grow", life_grow);
    register_builtin(vm, "life_burn_energy", life_burn_energy);
    register_builtin(vm, "life_check", life_check);
    register_builtin(vm, "life_decay", life_decay);
    

    register_builtin(vm, "print_world", print_world);

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
    register_number(vm, "preference.light", PREFERENCE_LIGHT);


    register_var(vm, "self");
    
}