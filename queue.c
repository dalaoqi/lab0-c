#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q) {
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q) {
        return;
    }
    while (q->head) {
        list_ele_t *tmp = q->head;
        q->head = q->head->next;
        free(tmp->value);
        free(tmp);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    if (!q) {
        return false;
    }
    newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        return false;
    }
    size_t length = strlen(s) + 1;
    newh->value = (char *) malloc(length * sizeof(char));
    if (!newh->value) {
        free(newh);
        return false;
    }
    snprintf(newh->value, length, "%s", s);
    newh->next = q->head;
    q->head = newh;
    if (!q->tail) {
        q->tail = newh;
    }
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q) {
        return false;
    }
    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));
    if (!newt) {
        return false;
    }
    size_t length = strlen(s) + 1;
    newt->value = (char *) malloc(length * sizeof(char));
    if (!newt->value) {
        free(newt);
        return false;
    }
    snprintf(newt->value, length, "%s", s);
    newt->next = NULL;
    q->size++;
    if (!q->tail) {
        q->tail = newt;
        q->head = newt;
        return true;
    }
    q->tail->next = newt;
    q->tail = newt;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head) {
        return false;
    }
    list_ele_t *tmp = q->head;
    if (sp) {
        snprintf(sp, bufsize, "%s", tmp->value);
    }
    q->head = q->head->next;
    free(tmp->value);
    free(tmp);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q) {
        return 0;
    }
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head) {
        return;
    }
    list_ele_t *tmp = q->head->next;
    q->tail->next = q->head;
    while (tmp != q->tail) {
        q->head->next = tmp->next;
        tmp->next = q->tail->next;
        q->tail->next = tmp;
        tmp = q->head->next;
    }
    q->tail = q->head;
    q->tail->next = NULL;
    q->head = tmp;
    return;
}
list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    if (!l1) {
        return l2;
    }
    if (!l2) {
        return l1;
    }
    size_t len_l1 = strlen(l1->value), len_l2 = strlen(l2->value);
    if (len_l1 < len_l2) {
        len_l1 = len_l2;
    }
    if (strncmp(l1->value, l2->value, len_l1) <= 0) {
        l1->next = merge(l1->next, l2);
        return l1;
    } else {
        l2->next = merge(l1->next, l1);
        return l2;
    }
}
/*
 * mergeSort for q_sort()
 */
list_ele_t *merge_sort(list_ele_t *head)
{
    if (!head || !head->next) {
        return head;
    }
    list_ele_t *fast = head->next;
    list_ele_t *slow = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;
    list_ele_t *l1 = merge_sort(head);
    list_ele_t *l2 = merge_sort(fast);
    return merge(l1, l2);
}
/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head) {
        return;
    }
    q->head = merge_sort(q->head);

    while (q->tail->next) {
        q->tail = q->tail->next;
    }
}