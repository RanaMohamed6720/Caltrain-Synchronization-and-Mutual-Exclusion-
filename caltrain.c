#include <pthread.h>
#include "caltrain.h"

void station_init(struct station *station)
{
    station->waiting = 0;
    station->available_seats = 0;
    station->passengers_walking_on_the_train = 0;
    pthread_mutex_init(&station->lock, NULL);
    pthread_cond_init(&station->train_arrived, NULL);
    pthread_cond_init(&station->passenger_seated, NULL);
}

void station_load_train(struct station *station, int count)
{
    pthread_mutex_lock(&station->lock);

    station->available_seats = count;

    // Signal waiting passengers that train has arrived
    pthread_cond_broadcast(&station->train_arrived);
    // Wait until all passengers are seated 
    while (station->waiting > 0 && station->available_seats > 0)
    {
        pthread_cond_wait(&station->passenger_seated, &station->lock);
    }

    // reset the remaining seats
    station->available_seats = 0;

    pthread_mutex_unlock(&station->lock);
}

void station_wait_for_train(struct station *station)
{
    pthread_mutex_lock(&station->lock);

    station->waiting++;

    // wait until there is at least one available seat
    while (station->available_seats == 0)
    {
        pthread_cond_wait(&station->train_arrived, &station->lock);
    }

    station->waiting--;
    station->available_seats--;
    station->passengers_walking_on_the_train++;

    pthread_mutex_unlock(&station->lock);
}

void station_on_board(struct station *station)
{
    pthread_mutex_lock(&station->lock);

    station->passengers_walking_on_the_train--;

    // signal the train if all passengers are seated and it is either full or no one is waiting
    if (station->passengers_walking_on_the_train == 0 && (station->available_seats == 0 || station->waiting == 0))
    {
        pthread_cond_signal(&station->passenger_seated);
    }

    pthread_mutex_unlock(&station->lock);
}