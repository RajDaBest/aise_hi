#include "dining_philosophers.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static phil_t phil[N_PHIL];
static spoon_t spoon[N_PHIL];

static spoon_t *phil_get_right_spoon(phil_t *phil)
{
    int phil_id = phil->phil_id;
    if (phil_id == 0)
    {
        return &spoon[N_PHIL - 1];
    }

    return &spoon[phil_id - 1];
}

static spoon_t *phil_get_left_spoon(phil_t *phil)
{
    return &spoon[phil->phil_id];
}

void phil_eat(phil_t *phil)
{
    spoon_t *left_spoon = phil_get_left_spoon(phil);
    spoon_t *right_spoon = phil_get_right_spoon(phil);

    left_spoon->is_used = true;
    right_spoon->is_used = true;

    phil->eat_count++;

    fprintf(stdout, "Philosopher %d eats with the spoons [%d, %d] for the %d th time\n",
            phil->phil_id, left_spoon->spoon_id, right_spoon->spoon_id,
            phil->eat_count);
}

void phil_release_both_spoons(phil_t *phil)
{
    spoon_t *left_spoon = phil_get_left_spoon(phil);
    spoon_t *right_spoon = phil_get_right_spoon(phil);

    pthread_cond_broadcast(&left_spoon->cv);
    pthread_cond_broadcast(&right_spoon->cv);
    pthread_mutex_unlock(&right_spoon->mutex);
    pthread_mutex_unlock(&left_spoon->mutex);
}

bool phil_get_access_both_spoons(phil_t *phil)
{
    spoon_t *left_spoon = phil_get_left_spoon(phil);
    spoon_t *right_spoon = phil_get_right_spoon(phil);

    pthread_mutex_lock(&left_spoon->mutex);
    while (left_spoon->is_used)
    {
        pthread_cond_wait(&left_spoon->cv, &left_spoon->mutex);
    }
    left_spoon->is_used = true;
    pthread_mutex_lock(&right_spoon->mutex);
    if (right_spoon->is_used)
    {
        left_spoon->is_used = false;
        pthread_mutex_unlock(&right_spoon->mutex);
        pthread_mutex_unlock(&left_spoon->mutex);
        return false;
    }
    else
    {
        right_spoon->is_used = true;
        return true;
    }
}

void *phil_callback_fn(void *arg)
{
    phil_t *phil = (phil_t *)arg;
    spoon_t *left_spoon = phil_get_left_spoon(phil);
    spoon_t *right_spoon = phil_get_right_spoon(phil);

    while (true)
    {
        if (phil_get_access_both_spoons)
        {
            phil_eat(phil);
            phil_release_both_spoons(phil);
        }
        sleep(1);
    }
}

int main(int argc, char **argv)
{
    int i = 0;
    pthread_attr_t attr;

    for (; i < N_PHIL; i++)
    {
        spoon[i].spoon_id = i;
        spoon[i].is_used = false;
        pthread_mutex_init(&spoon[i].mutex, NULL);
        pthread_cond_init(&spoon[i].cv, NULL);
    }

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    for (i = 0; i < N_PHIL; i++)
    {
        phil[i].phil_id = i;
        phil[i].eat_count = 0;
        pthread_create(&phil[i].thread_handle, &attr, phil_callback_fn, (void *)&phil[i]);
    }

    pthread_attr_destroy(&attr);
    pthread_exit(EXIT_SUCCESS);
}