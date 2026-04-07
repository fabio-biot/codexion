#include "codexion.h"

int *parsing_args(int argc, char *argv[]) {
    int *args;
    int i;

    i = 0;
    args = malloc(sizeof(int) * 7);
    printf("Nombre d args: %d\n", argc);

    if (argc != 8) {
        printf("Usage: %s <number_of_coders> <time_to_burnout>", argv[0]);
        printf("<time_to_compile> <time_to_debug> ");
        printf("<time_to_refactor> <number_of_compiles_required>");
        printf("<dongle_cooldown>\n");
        return NULL;
    }

    printf("number_of_coders: %d\n", atoi(argv[1]));
    printf("time_to_burnout: %d\n", atoi(argv[2]));
    printf("time_to_compile: %d\n", atoi(argv[3]));
    printf("time_to_debug: %d\n", atoi(argv[4]));
    printf("time_to_refactor: %d\n", atoi(argv[5]));
    printf("number_of_compiles_required: %d\n", atoi(argv[6]));
    printf("dongle_cooldown: %d\n", atoi(argv[7]));
    while(i < 7) {
        args[i] = atoi(argv[i + 1]);
        i++;
    }
    return args;
}

t_simulation init_sim(int argc, int* args) {
    t_simulation sim;

    sim.number_of_coders = args[0];
    sim.time_to_burnout = args[1];
    sim.time_to_compile = args[2];
    sim.time_to_debug = args[3];
    sim.time_to_refactor = args[4];
    sim.number_of_compiles_required = args[5];
    sim.dongle_cooldown = args[6];
    sim.coders = init_coders(&sim);
    sim.dongles = init_dongles(&sim);
    pthread_mutex_init(&sim.print_mutex, NULL);
    pthread_mutex_init(&sim.stop_mutex, NULL);

    return sim;
}

t_coder *init_coders(t_simulation *sim) {
    t_coder *coders;
    int i;

    coders = malloc(sizeof(t_coder) * sim->number_of_coders);
    i = 0;
    while (i < sim->number_of_coders) {
        coders[i].id = i + 1;
        coders[i].compile_count = 0;
        coders[i].last_compile = 0;
        coders[i].left = NULL;
        coders[i].right = NULL;
        coders[i].sim = sim;
        i++;
    }
    return coders;
}

t_dongle *init_dongles(t_simulation *sim) {
    t_dongle *dongles;
    int i;

    dongles = malloc(sizeof(t_dongle) * sim->number_of_coders);
    i = 0;
    while (i < sim->number_of_coders) {
        pthread_mutex_init(&dongles[i].mutex, NULL);
        dongles[i].last_used = 0;
        i++;        
    }
    return dongles;
}

// int main(int argc, char *argv[])
// {
//     int *args;
//     t_simulation sim;
//     args = parsing_args(argc, argv);
//     if (argc == 8 && args != NULL)
//     {
//         sim = init_sim(argc, args);
//         printf("%d\n", sim.number_of_coders);
//         printf("%ld\n", sim.time_to_burnout);
//     }
//     else
//     {
//         printf("Failed to parse args.\n");
//         return 1;
//     }
//     return 0;
// }