/*
 * Copyright (c) 2007 SIEGE Development Team
 * All rights reserved.
 *
 * This file is part of libSIEGE.
 *
 * This software is copyrighted work licensed under the terms of the
 * 2-clause BSD license. Please consult the file "license.txt" for
 * details.
 *
 * If you did not recieve the file with this program, please email
 * Tim Chas <darkuranium@gmail.com>.
 */

#define SG_BUILD_LIBRARY
#include <siege/util/list.h>

#include <stdlib.h>
#include <string.h>

SGList* SG_EXPORT sgListCreate(void)
{
	SGList* list = malloc(sizeof(SGList));
    if(!list) return NULL;
    
	list->head = NULL;
	list->tail = NULL;
	return list;
}
void SG_EXPORT sgListDestroy(SGList* list)
{
    if(!list) return;

	while(list->head != NULL)
		sgListRemoveNode(list, list->head);

	free(list);
}

size_t SG_EXPORT sgListLength(SGList* list)
{
	if(list == NULL)
		return 0;

	size_t i;
	SGListNode* node = list->head;
	for(i = 0; node != NULL; i++)
		node = node->next;
	return i;
}

SGListNode* SG_EXPORT sgListFindItem(SGList* list, void* item)
{
	if(list == NULL)
		return NULL;

	SGListNode* node;
	for(node = list->head; node != NULL; node = node->next)
		if(node->item == item)
			return node;
	return NULL;
}
SGListNode* SG_EXPORT sgListFindIndex(SGList* list, size_t index)
{
	if(list == NULL)
		return NULL;

	size_t i;
	SGListNode* node;
	for(node = list->head, i = 0; node != NULL; node = node->next, i++)
		if(i == index)
			return node;
	return NULL;
}

SGListNode* SG_EXPORT sgListInsertNode(SGList* list, SGListNode* after, void* item)
{
    if(!list) return NULL;

	SGListNode* node = malloc(sizeof(SGListNode));
    if(!node) return NULL;

	if(after == NULL) // first item
	{
		node->prev = NULL;
		node->next = list->head;
		if(list->head)
			list->head->prev = node;
		list->head = node;

		if(!list->tail)
			list->tail = node;
	}
	else // not first item
	{
		node->prev = after;
		node->next = after->next;
		if(after->next)
			after->next->prev = node;
		after->next = node;

		if(after == list->tail)
			list->tail = node;
	}

	node->item = item;
	return node;
}
SGListNode* SG_EXPORT sgListInsertItem(SGList* list, void* after, void* item)
{
	SGListNode* node = sgListFindItem(list, after);
	if(node == NULL)
		return NULL;
	return sgListInsertNode(list, node, item);
}
SGListNode* SG_EXPORT sgListInsertIndex(SGList* list, size_t after, void* item)
{
	SGListNode* node = sgListFindIndex(list, after);
	if(node == NULL)
		return NULL;
	return sgListInsertNode(list, node, item);
}
SGListNode* SG_EXPORT sgListPrepend(SGList* list, void* item)
{
	return sgListInsertNode(list, NULL, item);
}
SGListNode* SG_EXPORT sgListAppend(SGList* list, void* item)
{
	return sgListInsertNode(list, list->tail, item);
}

void SG_EXPORT sgListRemoveNode(SGList* list, SGListNode* node)
{
    if(!list) return;
    if(!node) return;

	if(list->head == node)
		list->head = node->next;
	if(list->tail == node)
		list->tail = node->prev;

	if(node->prev != NULL)
		node->prev->next = node->next;
	if(node->next != NULL)
		node->next->prev = node->prev;

	free(node);
}
void SG_EXPORT sgListRemoveItem(SGList* list, void* item)
{
	SGListNode* node = sgListFindItem(list, item);
	if(node != NULL)
		sgListRemoveNode(list, node);
}
void SG_EXPORT sgListRemoveIndex(SGList* list, size_t index)
{
	SGListNode* node = sgListFindIndex(list, index);
	if(node != NULL)
		sgListRemoveNode(list, node);
}

SGListNode* SG_EXPORT sgListGetFirst(SGList* list)
{
	if(list == NULL)
		return NULL;
	return list->head;
}
SGListNode* SG_EXPORT sgListGetLast(SGList* list)
{
	if(list == NULL)
		return NULL;
	return list->tail;
}

void* SG_EXPORT sgListPopFirst(SGList* list)
{
	if(list == NULL)
		return NULL;
	if(list->head == NULL)
		return NULL;
	void* item = list->head->item;
	sgListRemoveNode(list, list->head);
	return item;
}
void* SG_EXPORT sgListPopLast(SGList* list)
{
	if(list == NULL)
		return NULL;
	if(list->tail == NULL)
		return NULL;
	void* item = list->tail->item;
	sgListRemoveNode(list, list->tail);
	return item;
}
