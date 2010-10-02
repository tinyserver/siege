/*
    Copyright (c) 2007 SIEGE Development Team
    All rights reserved.

    This file is part of libSIEGE.

    This software is copyrighted work licensed under the terms of the
    2-clause BSD license. Please consult the file "license.txt" for
    details.

    If you did not recieve the file with this program, please email
    Tim Chas <darkuranium@gmail.com>.
*/

#define SG_BUILD_LIBRARY
#include <siege/ai/astar.h>

SGAStar* SG_EXPORT SGAStarCreate(SGAStarNode* start, SGAStarNode* goal, SGAStarScore g, SGAStarScore h, SGAStarIsGoal isgoal)
{
    SGAStar* search = malloc(sizeof(SGAStar));
    search->set.open = sgLinkedListCreate();
    sgLinkedListAppend(search->set.open, start);
    search->set.closed = sgLinkedListCreate();
    search->path = sgLinkedListCreate();
    search->current = NULL;
    search->goal = goal;

    search->cb.g = g;
    search->cb.h = h;
    search->cb.isgoal = isgoal;

    start->score.g = 0;
    start->score.h = h(start, goal);
    start->score.f = start->score.g + start->score.h;

    return search;
}
void SG_EXPORT SGAStarDestroy(SGAStar* search)
{
    sgLinkedListDestroy(search->set.open);
    sgLinkedListDestroy(search->set.closed);
    sgLinkedListDestroy(search->path);
    free(search);
}

// returns TRUE if found, FALSE if not found, -1 on error
int SG_EXPORT SGAStarStep(SGAStar* search)
{
    if(!sgLinkedListLength(search->set.open))
        return -1; // FAILURE

    SGLinkedNode* finode = NULL;
    SGLinkedNode* node;
    SGAStarNode* anode;
    float minf = SG_INF;
    for(node = search->set.open->first; node != NULL; node = node->next)
    {
        anode = node->item;
        if(anode->score.f <= minf) // we allow for == in order to allow for score.f to be SG_INF
        {
            finode = node;
            minf = anode->score.f;
        }
    }
    if(finode == NULL)
        return -1; // FAILURE - some weird error just happened
    SGAStarNode* min = finode->item;
    search->current = min;

    if(search->cb.isgoal != NULL)
    {
        if(search->cb.isgoal(min, search->goal))
            return 1; // SUCCESS
    }
    else if(min == search->goal) // fallback
        return 1;

    sgLinkedListRemoveNode(search->set.open, finode);
    sgLinkedListAppend(search->set.closed, min);

    char inside;
    float teng;
    char tenbetter;
    SGLinkedNode* open;
    SGLinkedNode* closed;
    for(node = min->links->first; node != NULL; node = node->next)
    {
        anode = node->item;

        inside = 0;
        for(closed = search->set.closed->first; closed != NULL; closed = closed->next)
            if(anode == closed->item)
            {
                inside = 1;
                break;
            }
        if(inside)
            continue;

        if(search->cb.g != NULL)
            teng = search->cb.g(min, anode);
        else // FALLBACK - cost is g+1 (the number of steps)
            teng = min->score.g + 1;

        inside = 0;
        for(open = search->set.open->first; open != NULL; open = open->next)
            if(anode == open->item)
            {
                inside = 1;
                break;
            }
        if(!inside)
        {
            sgLinkedListAppend(search->set.open, anode);
            tenbetter = 1;
        }
        else if(teng < anode->score.g)
            tenbetter = 1;
        else
            tenbetter = 0;
        if(tenbetter)
        {
            anode->from = min;
            anode->score.g = teng;
            if(search->cb.h != NULL)
                anode->score.h = search->cb.h(anode, search->goal);
            else // FALLBACK - assume 0 cost, turning this into a breath-first search (feeling optimistic today, aren't we?)
                anode->score.h = 0;
            anode->score.f = anode->score.g + anode->score.h;
        }
    }

    return 0; // CONTINUE - didn't find the finish, we have to continue
}
SGLinkedList* SG_EXPORT SGAStarPath(SGAStar* search, SGuint* pathlen) // reconstruct the path from the current node to the start; current node need not be the goal
{
    // sgLinkedListClear(search->path);
    sgLinkedListDestroy(search->path);
    search->path = sgLinkedListCreate();

    if(pathlen != NULL)
        *pathlen = 0;

    SGAStarNode* current = search->current;
    while(current != NULL)
    {
        sgLinkedListAppend(search->path, current);
        current = current->from;

        if(pathlen != NULL)
            (*pathlen)++;
    }

    return search->path;
}

SGAStarNode* SG_EXPORT SGAStarNodeCreate(void* data)
{
    SGAStarNode* node = malloc(sizeof(SGAStarNode));
    node->from = NULL;
    node->links = sgLinkedListCreate();
    node->data = data;
    node->score.g = SG_INF;
    node->score.h = SG_INF;
    node->score.f = SG_INF;
    return node;
}
void SG_EXPORT SGAStarNodeDestroy(SGAStarNode* node)
{
    sgLinkedListDestroy(node->links);
    free(node);
}
void SG_EXPORT SGAStarNodeLink(SGAStarNode* from, SGAStarNode* to)
{
    SGAStarNodeUnlink(from, to); // to prevent duplication
    sgLinkedListAppend(from->links, to);
}
void SG_EXPORT SGAStarNodeDLink(SGAStarNode* one, SGAStarNode* two)
{
    SGAStarNodeLink(one, two);
    SGAStarNodeLink(two, one);
}
void SG_EXPORT SGAStarNodeUnlink(SGAStarNode* from, SGAStarNode* to)
{
    sgLinkedListRemoveItem(from->links, to);
}
void SG_EXPORT SGAStarNodeDUnlink(SGAStarNode* one, SGAStarNode* two)
{
    SGAStarNodeUnlink(one, two);
    SGAStarNodeUnlink(two, one);
}
