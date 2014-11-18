/* Logan Keig and Cory Nettnin
 * COSC 301 Project 4
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <strings.h>
#include <string.h>
#include <ucontext.h>

#include "threadsalive.h"

/* Linked List Functions (adapted for this project from solution code given on Moodle) */

void list_clear(struct node *list) {
    while (list != NULL) {
        struct node *tmp = list;
        list = list->next;
		free(tmp->nodeCTX);
        free(tmp);
    }
}

void list_deleteHead(struct node **head) {
    struct node *dead = *head;
    *head = (*head)->next;
	free(dead->nodeCTX);
    free(dead);
}

void list_append(ucontext_t newCTX, struct node **head) {
    struct node *new = malloc(sizeof(struct node));
	new->nodeCTX = malloc(sizeof(newCTX));
	new->nodeCTX = &newCTX;
    new->next = NULL;

    if (*head == NULL) {
        *head = new;
        return;
    }

    struct node *tmp = *head;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }
    tmp->next = new;
}

/* ***************************** 
     stage 1 library functions
   ***************************** */

static struct node *head = NULL;
static ucontext_t mainContext;

void ta_libinit(void) {
    return;
}

void ta_create(void (*func)(void *), void *arg) {
//much of this is adapted from ctxtest.c
#define STACKSIZE 131072 //128 KB
    unsigned char *stack = (unsigned char *)malloc(STACKSIZE);
    assert(stack);

	ucontext_t ctx;
    getcontext(&ctx);

    ctx.uc_stack.ss_sp = stack;
    ctx.uc_stack.ss_size = STACKSIZE;
	
    ctx.uc_link = &mainContext;

	list_append(ctx,&head);

    makecontext(&ctx, (void (*)(void)) func, 1, arg);

    return;
}

void ta_yield(void) {
	ucontext_t current;
	ucontext_t *temp = head->nodeCTX; //temp to be able to delete and then refer to context for swapcontext afterward
	list_deleteHead(&head);
	list_append(current,&head);
    swapcontext(&current,temp);
    return;
}

int ta_waitall(void) {	
	while(1){ //using a while loop until we have the funcitonality of the locks from stage 2
		ucontext_t *temp = head->nodeCTX;
		list_deleteHead(&head);
		list_append(mainContext,&head);
		swapcontext(&mainContext,temp);
		if(head->next == NULL){ //if the linkedlist is empty
			return 0;
		}
	}
    return -1;
}
//still need to de-allocate memory used by threads for stack and context


/* ***************************** 
     stage 2 library functions
   ***************************** */

void ta_sem_init(tasem_t *sem, int value) {
}

void ta_sem_destroy(tasem_t *sem) {
}

void ta_sem_post(tasem_t *sem) {
}

void ta_sem_wait(tasem_t *sem) {
}

void ta_lock_init(talock_t *mutex) {
}

void ta_lock_destroy(talock_t *mutex) {
}

void ta_lock(talock_t *mutex) {
}

void ta_unlock(talock_t *mutex) {
}


/* ***************************** 
     stage 3 library functions
   ***************************** */

void ta_cond_init(tacond_t *cond) {
}

void ta_cond_destroy(tacond_t *cond) {
}

void ta_wait(talock_t *mutex, tacond_t *cond) {
}

void ta_signal(tacond_t *cond) {
}

