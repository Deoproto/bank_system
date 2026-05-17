#include "user.h"
#include <stdio.h>
#include <string.h>

/* djb2 hash */
unsigned int hash_pin(const char *pin) {
    unsigned int h = 5381;
    while (*pin) { h = ((h << 5) + h) + (unsigned char)*pin++; }
    return h;
}

int user_authenticate(const User *u, const char *pin) {
    return u->active && (hash_pin(pin) == u->pinHash);
}

void user_print(const User *u) {
    printf("  Потребител #%d: %s  (сметки: %d)\n",
           u->userId, u->name, u->accountCount);
}
