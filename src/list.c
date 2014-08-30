#include "list.h"

/* Pushes a link onto the top of the list. May traverse the list until at the head,
 * if a pointer to the head of the list is not passed in.
 *
 * @param list A pointer to a link in the list, preferably the head.
 * @param link The link to be added to the top of the list.
 * @return A pointer to the new head of the list.
 */
struct double_link *push_link_top(struct double_link *list, struct double_link *link) {
	while (list->prev != NULL) {
		list = list->prev;
	}
	link->next = list;
	list->prev = link;
	return link;
}

/* Sticks a link to the end of a list. May traverse list until tail.
 * 
 * @param list A pointer to a link in the list, preferably the tail.
 * @param link A pointer to the link that should be added to the list.
 * @return A pointer to the new tail of the list.
 */
struct double_link *push_link(struct double_link *list, struct double_link *link) {
	while (list->next != NULL) {
		list = list->next;
	}
	link->prev = list;
	list->next = link;
	return link;
}

/* Get the head of a list.
 *
 * @param list A pointer to an element of a list.
 * @return A painter to the head of the list.
 */
struct double_link *get_list_head(struct double_link *list) {
	while (list->prev != NULL) {
		list = list->prev;
	}
	return list;
}