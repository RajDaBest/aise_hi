#ifndef _DIN_PHIL_
#define _DIN_PHIL_

#include <pthread.h>
#include <stdbool.h>

#define N_PHIL 5

typedef struct phil_
{
    pthread_t thread_handle;
    int phil_id;
    int eat_count;
} phil_t;

typedef struct spoon_
{
    int spoon_id;
    bool is_used;          // bool to indicate if the spoon is being used or not
    pthread_mutex_t mutex; // for mutual exclusion
    pthread_cond_t cv;     // for coordinating threads competing for this resource
} spoon_t;

static spoon_t *phil_get_right_spoon(phil_t *phil);
static spoon_t *phil_get_left_spoon(phil_t *phil);
static void phil_eat(phil_t *phil);
static void phil_release_both_spoons(phil_t *phil);
static bool philosopher_get_access_both_spoons(phil_t *phil);

#endif
