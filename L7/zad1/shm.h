#pragma once
#include <stdint.h>

int create_shm(const char*, int);
int open_shm(const char*);
char* attach_shm(const char*, int);
int detach_shm(char*);
int delete_shm(const char*);
int generate_key(const char *name);
