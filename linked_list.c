#include <stdlib.h>
#include "linked_list.h"

static list_elem_t *dummy_elem() {
    list_elem_t *ret = (list_elem_t *) malloc(sizeof(list_elem_t));
    ret->prev = ret->next = NULL;
    ret->data = NULL;

    return ret;
}

static list_elem_t *wrap_in_elem(void *data) {
    list_elem_t *ret = (list_elem_t *) malloc(sizeof(list_elem_t));
    ret->data = data;

    return ret;
}

//end static defs

list_t *new_list() {
    list_t *ret = (list_t *) malloc(sizeof(list_t));
    ret->dummy_head = dummy_elem();
    ret->dummy_tail = dummy_elem();
    ret->dummy_head->next = ret->dummy_tail;
    ret->dummy_tail->prev = ret->dummy_head;

    return ret;
}

void destroy_list(list _t *l) {
    if (!l)
        return;

    list_elem_t *cur = get_head(l);
    while (cur != l->dummy_tail) {
        list_elem_t *next = get_next(cur);
        remove_from_list(l,cur);
        if (cur->data)
            free(cur->data);
        free(cur);
        cur = next;
    }

    free(l->dummy_head);
    free(l->dummy_tail);
    free(l);
}

list_elem_t *get_head(list_t *l) {
    list_elem_t *h = l->dummy_head->next;
    if (h == l->dummy_tail)
        return NULL;
    else
        return h;
}

list_elem_t *get_tail(list_t *l) {
    list_elem_t *h = l->dummy_tail->prev;
    if (h == l->dummy_head)
        return NULL;
    else
        return h;
}


void remove_from_list(list_elem_t *elem) {
    elem->prev->next = elem->next;
    elem->next->prev = elem->prev;
}

void append_val(list_t *l, void *data) {
    list_elem_t *elem = wrap_in_elem(data);
    list_elem_t *p = l->dummy_tail->prev;
    elem->next = l->dummy_tail;
    elem->prev = p;
    p->next = elem;
    l->dummy_tail->prev = elem;
}

void append_intval (list_t *l, int data) {
    int *heap_data = (int *) malloc(sizeof(int));
    *heap_data = data;
    append_val(l,heap_data);
}

int num_elems (list_t *l) {
    int n = 0;
    for (list_elem_t *cur = get_head(l);
         cur && cur != l->dummy_tail;
         cur = cur->next)
        n++;

    return n;
}

list_elem_t *get_nth_elem (list_t *l, int index) {
    list_elem_t *cur = get_head(l);
    while (cur && index > 0) {
        cur = cur->next;
        index--;
    }

    if (!cur || cur == l->dummy_tail)
        return NULL;
    else
        return cur;
}
