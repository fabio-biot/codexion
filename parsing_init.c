#include "codexion.h"

int *parsing_args(int argc, char *argv[]) {
    int *args;
    int i;

    i = 0;
    args = malloc(sizeof(int) * 7);
    printf("Nombre d args: %d\n", argc);

    if (argc != 9) {
        printf("Usage: %s <number_of_coders> <time_to_burnout>", argv[0]);
        printf("<time_to_compile> <time_to_debug> ");
        printf("<time_to_refactor> <number_of_compiles_required>");
        printf("<dongle_cooldown>\n");
        printf("<dongle_cooldown>\n");
        return NULL;
    }

    while (i < argc)
    {
        if (!is_number(argv[i]) && i > 0 && i < 8)
        {
            printf("Argument %d should be an int\n", i);
            return (NULL);
        }
        i++;
        if (i == 8 && (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0))
        {
            printf("Argument %d should be 'fifo' or 'edf'\n", i);
            return (NULL);
        }
        i++;
    }
    i = 0;
    printf("number_of_coders: %d\n", atoi(argv[1]));
    printf("time_to_burnout: %d\n", atoi(argv[2]));
    printf("time_to_compile: %d\n", atoi(argv[3]));
    printf("time_to_debug: %d\n", atoi(argv[4]));
    printf("time_to_refactor: %d\n", atoi(argv[5]));
    printf("number_of_compiles_required: %d\n", atoi(argv[6]));
    printf("dongle_cooldown: %d\n", atoi(argv[7]));
    printf("Method used %s\n", argv[8]);
    while(i < 7) {
        args[i] = atoi(argv[i + 1]);
        i++;
    }
    return args;
}

t_simulation *init_sim(int *args, char* scheduler)
{
    t_simulation *sim;

    sim = malloc(sizeof(t_simulation));
    if (!sim)
        return NULL;

    sim->number_of_coders = args[0];
    sim->time_to_burnout = args[1];
    sim->time_to_compile = args[2];
    sim->time_to_debug = args[3];
    sim->time_to_refactor = args[4];
    sim->number_of_compiles_required = args[5];
    sim->dongle_cooldown = args[6];
    sim->scheduler = scheduler;

    sim->stop = 0;
    sim->start_time = get_time_in_ms();

    pthread_mutex_init(&sim->print_mutex, NULL);
    pthread_mutex_init(&sim->stop_mutex, NULL);

    sim->dongles = init_dongles(sim);
    sim->coders = init_coders(sim);

    return sim;
}

t_coder *init_coders(t_simulation *sim)
{
    t_coder *coders;
    int i;

    coders = malloc(sizeof(t_coder) * sim->number_of_coders);
    if (!coders)
        return NULL;

    i = 0;
    while (i < sim->number_of_coders)
    {
        coders[i].id = i + 1;
        coders[i].compile_count = 0;
        coders[i].last_compile_start = sim->start_time;
        coders[i].sim = sim;
        coders[i].left = &sim->dongles[i];
        coders[i].right = &sim->dongles[(i + 1) % sim->number_of_coders];
        coders[i].thread = 0;
        pthread_mutex_init(&coders[i].lock, NULL);

        i++;
    }
    return coders;
}

t_dongle *init_dongles(t_simulation *sim)
{
    t_dongle *dongles;
    int i;

    dongles = malloc(sizeof(t_dongle) * sim->number_of_coders);
    if (!dongles)
        return NULL;

    i = 0;
    while (i < sim->number_of_coders)
    {
        pthread_mutex_init(&dongles[i].mutex, NULL);
        dongles[i].available_at = 0;
        dongles[i].heap = malloc(sizeof(t_request) * sim->number_of_coders);
        i++;
    }
    return dongles;
}

t_request *init_request()
{
    t_request *request;

    request = malloc(sizeof(t_request));
    if (!request)
        return NULL;
    return (request);
}

// int main(int argc, char *argv[])
// {
//     int *args;
//     t_simulation *sim;

//     args = parsing_args(argc, argv);
//     if (!args)
//         return (1);

//     sim = init_sim(argc, args);
//     if (!sim)
//         return (1);

//     printf("Coders: %d\n", sim->number_of_coders);
//     printf("Burnout: %ld\n", sim->time_to_burnout);

//     return 0;
// }