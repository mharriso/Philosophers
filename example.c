#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
//#include <conio.h>
#include <string.h>

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define BAD_MESSAGE         -13
#define SUCCESS               0

typedef struct someArgs_tag
{
    int id;
    const char *msg;
    int out;
} someArgs_t;

void* helloWorld(void *args)
{
    someArgs_t *arg = (someArgs_t*) args;
    int len;

    if (arg->msg == NULL) {
        return BAD_MESSAGE;
    }

    len = strlen(arg->msg);
    printf("%s\n", arg->msg);
    arg->out = len;

    return SUCCESS;
}

#define NUM_THREADS 4

int main()
{
    pthread_t threads[NUM_THREADS];
    int status;
    int i;
    int status_addr;
    someArgs_t args[NUM_THREADS];
    const char *messages[] =
	{
        "First",
        NULL,
        "Third Message",
        "Fourth Message"
    };

    for (i = 0; i < NUM_THREADS; i++)
	{
        args[i].id = i;
        args[i].msg = messages[i];
    }

    for (i = 0; i < NUM_THREADS; i++)
	{
        status = pthread_create(&threads[i], NULL, helloWorld, (void*) &args[i]);
        if (status != 0)
		{
            printf("main error: can't create thread, status = %d\n", status);
            exit(ERROR_CREATE_THREAD);
        }
    }

    printf("Main Message\n");

    for (i = 0; i < NUM_THREADS; i++)
	{
        status = pthread_join(threads[i], (void**)&status_addr);
        if (status != SUCCESS)
		{
            printf("main error: can't join thread, status = %d\n", status);
            exit(ERROR_JOIN_THREAD);
        }
        printf("joined with address %d\n", status_addr);
    }

    for (i = 0; i < NUM_THREADS; i++)
	{
        printf("thread %d arg.out = %d\n", i, args[i].out);
    }

    //_getch();
    return 0;
}
