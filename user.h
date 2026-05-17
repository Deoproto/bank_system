#ifndef USER_H
#define USER_H

#define MAX_USERS    20
#define MAX_ACC_USER 10

typedef struct {
    int  userId;
    char name[64];
    unsigned int pinHash;
    int  accountIds[MAX_ACC_USER];
    int  accountCount;
    int  active;
} User;

unsigned int hash_pin(const char *pin);
int  user_authenticate(const User *u, const char *pin);
void user_print(const User *u);

#endif
