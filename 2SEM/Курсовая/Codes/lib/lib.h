#ifndef LIB_H
#define LIB_H

const int MAX_PLANES = 100;
const int TIME_LEN = 6;
const int MODEL_LEN = 20;
const int BORT_LEN = 7;
const int AIRPORT_LEN = 4;

struct Plane {
    char time[TIME_LEN];     // HH:MM
    int minutes;            // время в минутах
    char model[MODEL_LEN];  // модель самолета
    char bort[BORT_LEN];    // бортовой номер
    char airport[AIRPORT_LEN]; // аэродром
};

// ПРОТОТИПЫ ФУНКЦИЙ
int read_data(const char *filename, Plane *planes, int *count);
void process_airport(Plane *planes, int count, const char *airport);
bool is_time_valid(const char *time);
bool is_bort_valid(const char *bort);
bool is_airport_valid(const char *airport);
void bubble_sort(int *indices, int size, Plane *data);
void print_table(int *indices, int size, Plane *data, const char *airport);

#endif