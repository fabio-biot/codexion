#include "codexion.h"


pthread_mutex_t mutex;

int get_time_in_ms(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

/*
number_of_coders
time_to_burnout
time_to_compile
time_to_debug
time_to_refactor
number_of_compiles_required
dongle_cooldown scheduler
*/

void  print_state(t_simulation* sim, t_coder* coder, char* message)
{
    pthread_mutex_lock(&sim->print_mutex);
    printf("%ld : Coder %d %s\n", get_time_in_ms(), &coder->id, message);
    pthread_mutex_unlock(&sim->print_mutex);
    return;
}

int main(int argc, char *argv[])
{
    int *args;
    t_simulation sim;
    t_coder coder_one;
    t_coder coder_two;
    t_dongle dongle_one;
    args = parsing_args(argc, argv);
    if (argc == 8 && args != NULL)
    {
        sim = init_sim(argc, args);
        coder_one = init_coders(&sim)[0];
        coder_two = init_coders(&sim)[1];
        dongle_one = init_dongles(&sim)[0];
        printf("%d\n", sim.number_of_coders);
        printf("%ld\n", sim.time_to_burnout);
        printf("CODER ID: %d\n", coder_one.id);
        printf("CODER COMPILE COUNT: %d\n", coder_one.compile_count);
        printf("CODER LAST COMPILE: %ld\n", coder_one.last_compile);
        printf("CODER ID: %d\n", coder_two.id);
        printf("CODER COMPILE COUNT: %d\n", coder_two.compile_count);
        printf("CODER LAST COMPILE: %ld\n", coder_two.last_compile);
        printf("LAST UDSED DONGLE: %ld\n", dongle_one.last_used);
        printf("TIME TO COMPILE: %d\n", get_time_in_ms());
    }
    else
    {
        printf("Failed to parse args.\n");
        return 1;
    }
    // thread_test(NULL);
    return 0;
    
}
