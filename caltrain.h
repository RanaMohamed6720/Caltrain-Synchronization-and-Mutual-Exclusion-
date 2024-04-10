#include <pthread.h>

struct station {
    int waiting;
    int available_seats;
    int passengers_walking_on_the_train;
    pthread_mutex_t lock;
    pthread_cond_t train_arrived;
    pthread_cond_t passenger_seated;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);