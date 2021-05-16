// Exemple de threads sense protegir amb semafors

// Includes propios
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semaphore.h"

int SEM_constructor_with_name(semaphore *sem, key_t key) {

    // IPC_CREAT: if this is specified, and a semaphore with the given key does not exist, it is created, otherwise the call returns with -1, setting the appropriate errno value.
    sem->shmid = semget(key, 1, IPC_CREAT | 0666);
    if (sem->shmid < 0)
        return sem->shmid;
    return 0;
}

int SEM_constructor(semaphore *sem) {
    assert(sem != NULL);
    sem->shmid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    if (sem->shmid < 0)
        return sem->shmid;
    return 0;
}

int SEM_init(const semaphore *sem, const int v) {
    unsigned short _v[1] = {v};
    assert(sem != NULL);
    return semctl(sem->shmid, 0, SETALL, _v);
}

int SEM_destructor(const semaphore *sem) {
    assert(sem != NULL);
    return semctl(sem->shmid, 0, IPC_RMID, NULL);
}

int SEM_wait(const semaphore *sem) {
    struct sembuf o = {0, -1, SEM_UNDO};
    assert(sem != NULL);
    return semop(sem->shmid, &o, 1);
}

int SEM_signal(const semaphore *sem) {
    struct sembuf o = {0, 1, SEM_UNDO};
    assert(sem != NULL);
    return semop(sem->shmid, &o, 1);
}