#pragma once
#include "consts.h"

int generate_key(const char* n);
int new_sem(const char* n, int st_val);
int open_sem(const char* n);
void unblock(int sem_id);
void block(int sem_id);
int del_sem(const char *n);