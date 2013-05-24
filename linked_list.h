#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct list_elem_t list_elem_t;
struct list_elem_t {
    list_elem_t *next,
                *prev;
    void *data;
};

typedef struct list_t {
    list_elem_t *dummy_head,
                *dummy_tail;
} list_t;

list_t *new_list();
void destroy_list(list_t *l);

list_elem_t *get_head(list_t *l);
list_elem_t *get_tail(list_t *l);
void remove_from_list(list_elem_t *elem);
void append_val(list_t *l, void *data);
void append_intval(list_t *l, int data);

list_elem_t *get_nth_elem(list_t *l, int index);
int num_elems(list_t *l);

#endif
