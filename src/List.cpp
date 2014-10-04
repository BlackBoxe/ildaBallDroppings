/*
 * Copyright (C) 2014 by Nicolas Thill <nico@openwrt.org>
 * 
 * This is free software, licensed under Creative Commons Attribution-ShareAlike 3.0 Unported License (CC BY-SA 3.0)
 * See /LICENSE for more information.
 */

#include "List.h"

Link::Link(void *data) {
	next = prev = 0;
	this->data = data;
}

List::List() {
	head = tail = 0;
	count = 0;
}

void List::add_head(Link *link)
{
	if (head) {
		link->next = head;
		head->prev = link;
		head = link;
	} else {
		head = tail = link;
	}
}

void List::add_tail(Link *link)
{
	if (tail) {
		link->prev = tail;
		tail->next = link;
		tail = link;
	} else {
		tail = head = link;
	}
}

Link *List::remove_head()
{
	Link *link = head;

	if (link) {
		if (link->next) {
			link->next->prev = 0;
			head = link->next;
		} else {
			head = tail = 0;
		}
	}

	return link;
}

Link *List::remove_tail()
{
	Link *link = tail;

	if (link) {
		if (link->prev) {
			link->prev->next = 0;
			tail = link->prev;
		} else {
			tail = head = 0;
		}
	}

	return link;
}

bool List::enqueue(void *data) {
	Link *link;

	link = new Link(data);
	if (link) {
		add_tail(link);
		count++;
		return 0;
	}

	return -1;
}

void *List::dequeue() {
	Link *link;
	void *data;

	link = remove_head();
	if (link) {
		data = link->data;
		delete link;
		count--;
		return data;
	}

	return 0;
}

bool List::empty()
{
	return (!head);
}

void *List::get(int n)
{
	Link *link = head;
	int i = 0;

	while (link) {
		if (i == n) {
			return link->data;
		}
		link = link->next;
		i++;
	}

	return 0;
}

void List::remove(Link *link) {
    if (link->next) {
        link->next->prev = link->prev;
    } else {
        tail = link->prev;
    }
    if (link->prev) {
        link->prev->next = link->next;
    } else {
        head = link->next;
    }
    delete link;
    count--;
}

