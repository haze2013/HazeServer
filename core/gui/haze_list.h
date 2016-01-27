#ifndef LIST_H
#define LIST_H

#include <stddef.h>

struct list_head {
	struct list_head *prev, *next;
};

 #define LIST_HEAD_INIT(name) { &(name), &(name) }

#define INIT_LIST_HEAD(name_ptr)        do {    (name_ptr)->next = (name_ptr); \
	(name_ptr)->prev = (name_ptr); \
}while (0)

#define OFFSET(type, member)            (char *)&(((type *)0x0)->member)
#define container_of(ptr, type, member) ({                                      \
		(type *)((char * )ptr - OFFSET(type, member)); });

#define list_for_each(pos, head)        for (pos = (head)->next; pos != (head); pos = pos->next)
#define list_for_each_prev(pos, head)   for (pos = (head)->prev; pos != (head); pos = pos->prev)
#define list_entry(ptr, type, member)   container_of(ptr, type, member)

/**
 *  * list_for_each_safe - iterate over a list safe against removal of list entry
 *   * @pos:	the &struct list_head to use as a loop cursor.
 *    * @n:		another &struct list_head to use as temporary storage
 *     * @head:	the head for your list.
 *      */
#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
			pos = n, n = pos->next)

/*
 *  * Insert a new entry between two known consecutive entries.
 *   *
 *    * This is only for internal list manipulation where we know
 *     * the prev/next entries already!
 *      */
static inline void __list_add(struct list_head *new,
		struct list_head *prev,
		struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}

static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}

static inline void list_del(struct list_head *p)
{
	p->prev->next = p->next;
	p->next->prev = p->prev;
}

/**
 *  * list_replace - replace old entry by new one
 *   * @old : the element to be replaced
 *    * @new : the new element to insert
 *     * Note: if 'old' was empty, it will be overwritten.
 *      */
static inline void list_replace(struct list_head *old,
		struct list_head *new)
{
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;
}

/**
 *  * list_empty - tests whether a list is empty
 *   * @head: the list to test.
 *    */
static inline int list_empty(const struct list_head *head)
{
	return head->next == head;
}

#endif


