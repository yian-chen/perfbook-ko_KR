/*
 * locked_list.c: Sample locked iterator.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Copyright (c) 2011 Paul E. McKenney, IBM Corporation.
 */

#include "../api.h"

struct locked_list {
	spinlock_t s;
	struct list_head h;
};

struct list_head *list_next(struct locked_list *lp,
			    struct list_head *np);

struct list_head *list_start(struct locked_list *lp)
{
	spin_lock(&lp->s);
	return list_next(lp, &lp->h);
}

struct list_head *list_next(struct locked_list *lp,
			    struct list_head *np)
{
	struct list_head *ret;

	ret = np->next;
	if (ret == &lp->h) {
		spin_unlock(&lp->s);
		ret = NULL;
	}
	return ret;
}

void list_stop(struct locked_list *lp)
{
	spin_unlock(&lp->s);
}

struct list_ints {
	struct list_head n;
	int a;
};

void list_print(struct locked_list *lp)
{
	struct list_head *np;
	struct list_ints *ip;

	np = list_start(lp);
	while (np != NULL) {
		ip = list_entry(np, struct list_ints, n);
		printf("\t%d\n", ip->a);
		np = list_next(lp, np);
	}
}

struct locked_list head;

int main(int argc, char *argv[])
{
	struct list_ints n1;
	struct list_ints n2;
	struct list_ints n3;

	spin_lock_init(&head.s);
	INIT_LIST_HEAD(&head.h);
	printf("Empty list:\n");
	list_print(&head);
	n1.a = 1;
	list_add(&n1.n, &head.h);
	printf("Singleton list:\n");
	list_print(&head);
	n2.a = 2;
	list_add(&n2.n, &head.h);
	printf("Doublet list:\n");
	list_print(&head);
	n3.a = 3;
	list_add(&n3.n, head.h.prev);
	printf("Triplet list:\n");
	list_print(&head);
}
