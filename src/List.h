/*
 * Copyright (C) 2014 by Nicolas Thill <nico@openwrt.org>
 * 
 * This is free software, licensed under Creative Commons Attribution-ShareAlike 3.0 Unported License (CC BY-SA 3.0)
 * See /LICENSE for more information.
 */

#ifndef _LIST_H
#define _LIST_H

class Link {
	public:
		Link *prev;
		Link *next;
		void *data;
		Link(void *);
};

class List {
	public:
		Link *head;
		Link *tail;
		int count;

		List();
		bool empty();
		void *get(int);
		bool enqueue(void *);
		void *dequeue();

		void remove(Link *);
		void add_head(Link *);
		void add_tail(Link *);
		Link *remove_head();
		Link *remove_tail();
};

#define List_for_each(list, link) \
	for ((link) = (list)->head; (link); (link) = (link)->next)

#define List_for_each_safe(list, link, temp) \
	for ((link) = (list)->head, (temp) = (link) ? (link)->next : 0; (link); (link) = (temp), (temp) = (link) ? (link)->next : 0)

#endif /* _LIST_H */
