/*
 * Copyright (c) 2007 SIEGE Development Team
 * All rights reserved.
 *
 * This file is part of libSIEGE.
 *
 * This software is copyrighted work licensed under the terms of the
 * 2-clause BSD license. Please consult the file "COPYING.txt" for
 * details.
 *
 * If you did not recieve the file with this program, please email
 * Tim Chas <darkuranium@gmail.com>.
 */

#define SG_BUILD_LIBRARY
#include <siege/core/entity.h>
#include <siege/audio/source.h>
#include <siege/graphics/sprite.h>
#include <siege/graphics/mask.h>
#include <siege/physics/body.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>

SGbool SG_EXPORT _sgEntityInit(void)
{
    _sg_entList = sgListCreate();
    if(!_sg_entList)
        return SG_FALSE;
    _sg_entSMap = sgSMapCreate();
    return SG_TRUE;
}
SGbool SG_EXPORT _sgEntityDeinit(void)
{
    sgEntityDestroyAll();
    sgListDestroy(_sg_entList);
    sgSMapDestroy(_sg_entSMap);
    return SG_TRUE;
}

static void SG_EXPORT _sg_evCall(SGEntity* entity, size_t num, va_list args)
{
    if(!entity->active) return;

    SGenum type;

    size_t siz[2];
    SGuint val[2];
    void* ptr[2];

	SGuint i;
	for(i = 0; i < num; i++)
	{
        type = va_arg(args, SGenum);
		switch(type)
		{
			case SG_EVF_INIT:
				if(entity->evInit != NULL)
					entity->evInit(entity);
				break;
			case SG_EVF_DEINIT:
				if(entity->evDeinit != NULL)
					entity->evDeinit(entity);
				break;

			case SG_EVF_START:
				if(entity->evStart != NULL)
					entity->evStart(entity);
				break;
			case SG_EVF_EXIT:
				if(entity->evExit != NULL)
					entity->evExit(entity);
				break;
			case SG_EVF_TICK:
				if(entity->evTick != NULL)
					entity->evTick(entity);
				break;
			case SG_EVF_TICKB:
				if(entity->evTickBegin != NULL)
					entity->evTickBegin(entity);
				break;
			case SG_EVF_TICKE:
				if(entity->evTickEnd != NULL)
					entity->evTickEnd(entity);
				break;
			case SG_EVF_DRAW:
				if(entity->evDraw != NULL)
					entity->evDraw(entity);
				break;

			case SG_EVF_WINOPEN:
				if(entity->evWindowOpen != NULL)
					entity->evWindowOpen(entity);
				break;
			case SG_EVF_WINCLOSE:
				if(entity->evWindowClose != NULL)
					entity->evWindowClose(entity);
				break;
			case SG_EVF_WINRESIZE:
				val[0] = va_arg(args, SGuint);
				val[1] = va_arg(args, SGuint);
				if(entity->evWindowResize != NULL)
					entity->evWindowResize(entity, val[0], val[1]);
				break;

			case SG_EVF_MOUSEBUTH:
				val[0] = va_arg(args, SGuint);
				if(entity->evMouseButton != NULL)
					entity->evMouseButton(entity, val[0]);
				break;
			case SG_EVF_MOUSEBUTP:
				val[0] = va_arg(args, SGuint);
				if(entity->evMouseButtonPress != NULL)
					entity->evMouseButtonPress(entity, val[0]);
				break;
			case SG_EVF_MOUSEBUTR:
				val[0] = va_arg(args, SGuint);
				if(entity->evMouseButtonRelease != NULL)
					entity->evMouseButtonRelease(entity, val[0]);
				break;

			case SG_EVF_MOUSEBUTLH:
				if(entity->evMouseButtonLeft != NULL)
					entity->evMouseButtonLeft(entity);
				break;
			case SG_EVF_MOUSEBUTLP:
				if(entity->evMouseButtonLeftPress != NULL)
					entity->evMouseButtonLeftPress(entity);
				break;
			case SG_EVF_MOUSEBUTLR:
				if(entity->evMouseButtonLeftRelease != NULL)
					entity->evMouseButtonLeftRelease(entity);
				break;

			case SG_EVF_MOUSEBUTRH:
				if(entity->evMouseButtonRight != NULL)
					entity->evMouseButtonRight(entity);
				break;
			case SG_EVF_MOUSEBUTRP:
				if(entity->evMouseButtonRightPress != NULL)
					entity->evMouseButtonRightPress(entity);
				break;
			case SG_EVF_MOUSEBUTRR:
				if(entity->evMouseButtonRightRelease != NULL)
					entity->evMouseButtonRightRelease(entity);
				break;

			case SG_EVF_MOUSEBUTMH:
				if(entity->evMouseButtonMiddle != NULL)
					entity->evMouseButtonMiddle(entity);
				break;
			case SG_EVF_MOUSEBUTMP:
				if(entity->evMouseButtonMiddlePress != NULL)
					entity->evMouseButtonMiddlePress(entity);
				break;
			case SG_EVF_MOUSEBUTMR:
				if(entity->evMouseButtonMiddleRelease != NULL)
					entity->evMouseButtonMiddleRelease(entity);
				break;

			case SG_EVF_MOUSEMOVE:
				val[0] = va_arg(args, SGint);
				val[1] = va_arg(args, SGint);
				if(entity->evMouseMove != NULL)
					entity->evMouseMove(entity, val[0], val[1]);
				break;
			case SG_EVF_MOUSEWHEEL:
				val[0] = va_arg(args, SGuint);
				if(entity->evMouseWheel != NULL)
					entity->evMouseWheel(entity, val[0]);
				break;

			case SG_EVF_KEYKEYH:
				val[0] = va_arg(args, SGuint);
				if(entity->evKeyboardKey != NULL)
					entity->evKeyboardKey(entity, val[0]);
				break;
			case SG_EVF_KEYKEYP:
				val[0] = va_arg(args, SGuint);
				if(entity->evKeyboardKeyPress != NULL)
					entity->evKeyboardKeyPress(entity, val[0]);
				break;
			case SG_EVF_KEYKEYR:
				val[0] = va_arg(args, SGuint);
				if(entity->evKeyboardKeyRelease != NULL)
					entity->evKeyboardKeyRelease(entity, val[0]);
				break;
			case SG_EVF_KEYKEYA:
				val[0] = va_arg(args, SGuint);
				if(entity->evKeyboardKeyRepeat != NULL)
					entity->evKeyboardKeyRepeat(entity, val[0]);
				break;

			case SG_EVF_KEYCHARP:
                val[0] = va_arg(args, SGenum);
				if(entity->evKeyboardCharPress != NULL)
					entity->evKeyboardCharPress(entity, val[0]);
				break;

			case SG_EVF_JOYSTICKBUTH:
				val[0] = va_arg(args, SGuint);
				val[1] = va_arg(args, SGuint);
				if(entity->evJoystickButton != NULL)
					entity->evJoystickButton(entity, val[0], val[1]);
				break;
			case SG_EVF_JOYSTICKBUTP:
				val[0] = va_arg(args, SGuint);
				val[1] = va_arg(args, SGuint);
				if(entity->evJoystickButtonPress != NULL)
					entity->evJoystickButtonPress(entity, val[0], val[1]);
				break;
			case SG_EVF_JOYSTICKBUTR:
				val[0] = va_arg(args, SGuint);
				val[1] = va_arg(args, SGuint);
				if(entity->evJoystickButtonRelease != NULL)
					entity->evJoystickButtonRelease(entity, val[0], val[1]);
				break;
			case SG_EVF_JOYSTICKMOVE:
				val[0] = va_arg(args, SGuint);
				ptr[0] = va_arg(args, float*);
				siz[0] = va_arg(args, size_t);
				if(entity->evJoystickMove != NULL)
					entity->evJoystickMove(entity, val[0], ptr[0], siz[0]);
				break;

			// networking goes here (SG_EVF_NET)

			case SG_EVF_LVLSTART:
				if(entity->evLevelStart != NULL)
					entity->evLevelStart(entity);
				break;

			case SG_EVF_LVLEND:
				if(entity->evLevelEnd != NULL)
					entity->evLevelEnd(entity);
            
            default:
                if(type & SG_EVT_USER)
                {
                    if(entity->evUser)
                        entity->evUser(entity, type & SG_EV_USERMASK, args);
                }
		}
	}
}
static void SG_EXPORT _sg_evDraw(SGEntity* entity)
{
	if(entity->visible)
		sgEntityDraw(entity);
}

SGEntity* SG_EXPORT sgEntityCreate(void)
{
    static const SGEntity DefaultEntity = { 0 };

	SGEntity* entity = malloc(sizeof(SGEntity));
    *entity = DefaultEntity;
	entity->active = SG_TRUE;
	entity->pausable = SG_TRUE;

	entity->visible = SG_TRUE;
	entity->x = 0.0;
	entity->y = 0.0;
	entity->depth = 0.0;
	entity->angle = 0.0;

	entity->minside = SG_FALSE;

	entity->evDraw = _sg_evDraw;

	entity->node = sgListAppend(_sg_entList, entity);
    entity->mnode = NULL;
	return entity;
}
void SG_EXPORT sgEntityDestroy(SGEntity* entity)
{
	if(entity == NULL)
		return;

	if(entity->lcDestroy != NULL)
		entity->lcDestroy(entity);

    sgListRemoveNode(_sg_entList, entity->node);
    sgEntitySetName(entity, NULL);

	free(entity);
}
void SG_EXPORT sgEntityDestroyAll(void)
{
    while(_sg_entList->head)
        sgEntityDestroy(_sg_entList->head->item);
}

void SG_EXPORT sgEntitySetName(SGEntity* entity, const char* name)
{
    SGList* list;
    if(entity->mnode)
    {
        list = entity->mnode->val;
        sgListRemoveNode(list, entity->mlnode);
        if(!list->head)
        {
            sgSMapRemove(_sg_entSMap, entity->mnode->key);
            sgListDestroy(list);
        }
        entity->mnode = NULL;
        entity->mlnode = NULL;
    }
    if(!name)
        return;

    SGSMapNode* mnode = sgSMapFind(_sg_entSMap, name);
    if(!mnode)
        mnode = sgSMapAssignNode(_sg_entSMap, name, sgListCreate());

    entity->mnode = mnode;
    entity->mlnode = sgListAppend(mnode->val, entity);
}
char* SG_EXPORT sgEntityGetName(SGEntity* entity)
{
    if(!entity->mnode) return NULL;

    return entity->mnode->key;
}

void SG_EXPORT sgEntitySetSprite(SGEntity* entity, SGSprite* sprite)
{
	if(entity == NULL)
		return;

	entity->sprite = sprite;
}
SGSprite* SG_EXPORT sgEntityGetSprite(SGEntity* entity)
{
	if(entity == NULL)
		return NULL;

	return entity->sprite;
}

void SG_EXPORT sgEntitySetMask(SGEntity* entity, SGMask* mask)
{
	if(entity == NULL)
		return;

	entity->mask = mask;
}
SGMask* SG_EXPORT sgEntityGetMask(SGEntity* entity)
{
	if(entity == NULL)
		return NULL;

	return entity->mask;
}

void SG_EXPORT sgEntitySetPhysicsBody(SGEntity* entity, SGPhysicsBody* body)
{
	if(entity == NULL)
		return;
    if(entity->body)
        entity->body->entity = NULL;
    if(body)
        body->entity = entity;
	entity->body = body;
}
SGPhysicsBody* SG_EXPORT sgEntityGetPhysicsBody(SGEntity* entity)
{
	if(entity == NULL)
		return NULL;

	return entity->body;
}

void SG_EXPORT sgEntitySetAudioSource(SGEntity* entity, SGAudioSource* source)
{
	if(entity == NULL)
		return;

	entity->source = source;
}
SGAudioSource* SG_EXPORT sgEntityGetAudioSource(SGEntity* entity)
{
	if(entity == NULL)
		return NULL;

	return entity->source;
}

void SG_EXPORT sgEntitySetPos(SGEntity* entity, float x, float y)
{
	if(entity == NULL)
		return;

	entity->x = x;
	entity->y = y;
	if(entity->body != NULL)
		sgPhysicsBodySetPos(entity->body, x, y);
}
void SG_EXPORT sgEntityGetPos(SGEntity* entity, float* x, float* y)
{
	if(entity == NULL)
		return;

	if(x != NULL)
		*x = entity->x;
	if(y != NULL)
		*y = entity->y;
	if(entity->body != NULL)
		sgPhysicsBodyGetPos(entity->body, x, y);
}

void SG_EXPORT sgEntitySetPosX(SGEntity* entity, float x)
{
	if(entity == NULL)
		return;

	entity->x = x;
	if(entity->body != NULL)
		sgPhysicsBodySetPosX(entity->body, x);
}
float SG_EXPORT sgEntityGetPosX(SGEntity* entity)
{
	if(entity == NULL)
		return SG_NAN;

	if(entity->body != NULL)
		return sgPhysicsBodyGetPosX(entity->body);
	return entity->x;
}
void SG_EXPORT sgEntitySetPosY(SGEntity* entity, float y)
{
	if(entity == NULL)
		return;

	entity->y = y;
	if(entity->body != NULL)
		sgPhysicsBodySetPosY(entity->body, y);
}
float SG_EXPORT sgEntityGetPosY(SGEntity* entity)
{
	if(entity == NULL)
		return SG_NAN;

	if(entity->body != NULL)
		return sgPhysicsBodyGetPosY(entity->body);
	return entity->y;
}

void SG_EXPORT sgEntitySetDepth(SGEntity* entity, float depth)
{
	if(entity == NULL)
		return;

	entity->depth = depth;
}
float SG_EXPORT sgEntityGetDepth(SGEntity* entity)
{
	if(entity == NULL)
		return SG_NAN;

	return entity->depth;
}

void SG_EXPORT sgEntitySetAngleRads(SGEntity* entity, float rads)
{
	if(entity == NULL)
		return;

	entity->angle = rads;
	if(entity->body != NULL)
		sgPhysicsBodySetAngleRads(entity->body, rads);
}
float SG_EXPORT sgEntityGetAngleRads(SGEntity* entity)
{
	if(entity == NULL)
		return SG_NAN;

	if(entity->body != NULL)
		return sgPhysicsBodyGetAngleRads(entity->body);
	return entity->angle;
}
void SG_EXPORT sgEntitySetAngleDegs(SGEntity* entity, float degs)
{
	sgEntitySetAngleRads(entity, degs * SG_PI / 180.0);
}
float SG_EXPORT sgEntityGetAngleDegs(SGEntity* entity)
{
	return sgEntityGetAngleRads(entity) * 180.0 / SG_PI;
}

void SG_EXPORT sgEntityDraw(SGEntity* entity)
{
	if(entity == NULL)
		return;
	if(entity->sprite == NULL)
		return;

	if(entity->body != NULL)
	{
		sgPhysicsBodyGetPos(entity->body, &entity->x, &entity->y);
		entity->angle = sgPhysicsBodyGetAngleRads(entity->body);
	}

	sgSpriteDrawRads3f1f(entity->sprite, entity->x, entity->y, entity->depth, entity->angle);
}

SGList* SG_EXPORT sgEntityFind(const char* name)
{
    return sgSMapFind(_sg_entSMap, name);
}

void SG_EXPORT sgEntityEventSignalv(size_t num, va_list args)
{
    _sg_entStop = SG_FALSE;
    SGEntity* entity;
    SGListNode* node;
    SGListNode* next;
    va_list curarg;
    for(node = _sg_entList->head; node; node = next)
    {
        next = node->next;

        if(_sg_entStop)
            break;
        entity = node->item;
        
        va_copy(curarg, args);
        _sg_evCall(entity, num, curarg);
        va_end(curarg);
    }
}
void SG_EXPORT sgEntityEventSignal(size_t num, ...)
{
    va_list args;
    va_start(args, num);
    sgEntityEventSignalv(num, args);
    va_end(args);
}
void SG_EXPORT sgEntityEventStop(void)
{
    _sg_entStop = SG_TRUE;
}
