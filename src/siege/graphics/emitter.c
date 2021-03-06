/*
 *  Copyright (c) 2007 SIEGE Development Team
 *  All rights reserved.
 *
 *  This file is part of libSIEGE.
 *
 *  This software is copyrighted work licensed under the terms of the
 *  2-clause BSD license. Please consult the file "license.txt" for
 *  details.
 *
 *  If you did not recieve the file with this program, please email
 *  Tim Chas <darkuranium@gmail.com>.
*/


#include <siege/graphics/draw.h>
#include <siege/graphics/texture.h>
#include <siege/graphics/emitter.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*
 * TODO:
 * implement everything in particles.h!
 * add scale in particle drawing
 * add function pointer methods and registration to allow custom update of particles
 */

void SG_EXPORT _sgParticleInit(SGParticle* particle, float x, float y, float angle, float speed, float alpha, float width, float height, float rotation)
{
	particle->x = x;
	particle->y = y;
	particle->angle = angle;
	particle->speed = speed;
	particle->age = 0.0;
	particle->alpha = alpha;
	particle->width = width;
	particle->height = height;
	particle->rotation = rotation;
}

SGParticle* SG_EXPORT _sgParticleCreate(float x, float y, float angle, float speed)
{
	SGParticle* particle = malloc(sizeof(SGParticle));
	_sgParticleInit(particle, x, y, angle, speed, 1.0, 16, 16, 0);
	return particle;
}

SGEmitter* SG_EXPORT sgEmitterCreate(
		float x,              /* initial x of particles */
		float y,              /* initial y of particles */
		float angle,          /* direction of particles */
		float delta_angle,    /* variation in direction */
		float initial_speed,  /* initial speed of particles */
		float duration,       /* lifetime of particles */
		float rate,           /* production rate of particles */
		float friction,       /* environmental friction to particles */
		size_t nb_particles,     /* size of particles pool */
		SGTexture* texture)   /* texture used by particles */
{
	size_t i;
	SGEmitter* emitter = malloc(sizeof(SGEmitter));
    if(!emitter)
        return NULL;
	emitter->x = x;
	emitter->y = y;
	emitter->angle = angle;
	emitter->delta_angle = delta_angle;
	emitter->initial_speed = initial_speed;
	emitter->duration = duration;
	emitter->rate = rate;
	emitter->friction = friction;
	emitter->particles = malloc(nb_particles * sizeof(SGParticle));
	emitter->texture = texture;
	emitter->nb_particles = nb_particles;
	emitter->time_accumulator = 0.0;
	emitter->silent = SG_FALSE;

	for(i = 0; i < emitter->nb_particles; i++)
		emitter->particles[i].age = emitter->duration + 1;

	sgEmitterSetUpdateFunc(emitter, _sgParticleUpdate);

	return emitter;
}

void SG_EXPORT _sgParticleUpdate(SGParticle* particle, float time, float friction)
{
	particle->speed -= friction * time;
	if (particle->speed < 0)
		particle->speed = 0;
	particle->x += cos(particle->angle) * particle->speed;
	particle->y += sin(particle->angle) * particle->speed;
	particle->age += time;
}

void SG_EXPORT sgEmitterUpdate(SGEmitter* emitter, float time)
{
	int i;
	SGbool condition;
	float frac = 1.0/emitter->rate;
	emitter->time_accumulator += time;

	for (i=0; i<emitter->nb_particles; i++)
	{
		if (emitter->particles[i].age < emitter->duration)
		{
			emitter->cbUpdate(&emitter->particles[i], time, emitter->friction);
			//_sgParticleUpdate(&emitter->particles[i], time, emitter->friction);
		}
	}

	while (emitter->time_accumulator >= frac)
	{
		/* use an index to start search from precedent find */
		condition = SG_FALSE;
		for (i=0; i < emitter->nb_particles; i++)
		{
			if (emitter->particles[i].age >= emitter->duration)
			{
				_sgParticleInit(&emitter->particles[i],
						emitter->x,
						emitter->y,
						emitter->angle + (rand() * 1.0 / RAND_MAX - 0.5) * emitter->delta_angle,
						emitter->initial_speed,
						1.0,
						16,
						16,
						0);
				emitter->time_accumulator -= frac;
				condition = SG_TRUE;
				break;
			}

		}
		if (condition == SG_FALSE )
		{
			if (!emitter->silent)
			{
				printf("warning, pool of particules emitter");
				printf(" full, either reduce lifetime,");
				printf(" or rate, or make pool bigger\n");
			}
			return;
		}
	}
}

void SG_EXPORT sgEmitterDraw(SGEmitter* emitter)
{
	int i;
	float angle;
	SGParticle* particle;
	for (i=0; i< emitter->nb_particles; i++)
	{
		
		particle = &emitter->particles[i];
		if (particle->age < emitter->duration)
		{
			angle = 0;
            sgDrawBeginT(SG_QUADS, emitter->texture);
			sgDrawColor4f(1.0, 1.0, 1.0, particle->alpha);
			sgDrawTexCoord2f(0.0, 0.0);
			sgDrawVertex2f(particle->x + cos(particle->rotation + angle) * particle->width / 2, particle->y + sin(particle->rotation + angle) * particle->height / 2);
			sgDrawTexCoord2f(0.0, 1.0);
			angle += 3.1415926 / 2;
			sgDrawVertex2f(particle->x + cos(particle->rotation + angle) * particle->width / 2, particle->y + sin(particle->rotation + angle) * particle->height / 2);
			sgDrawTexCoord2f(1.0, 1.0);
			angle += 3.1415926 / 2;
			sgDrawVertex2f(particle->x + cos(particle->rotation + angle) * particle->width / 2, particle->y + sin(particle->rotation + angle) * particle->height / 2);
			sgDrawTexCoord2f(1.0, 0.0);
			angle += 3.1415926 / 2;
			sgDrawVertex2f(particle->x + cos(particle->rotation + angle) * particle->width / 2, particle->y + sin(particle->rotation + angle) * particle->height / 2);
			sgDrawEnd();
		}

	}
	sgDrawColor4f(1.0, 1.0, 1.0, 1.0);
}

void SG_EXPORT sgEmitterSetUpdateFunc(SGEmitter* emitter, SGParticleUpdate* cbUpdate)
{
	emitter->cbUpdate = cbUpdate;
}
void SG_EXPORT sgEmitterSetSilent(SGEmitter* emitter, SGbool boolean)
{
	emitter->silent = boolean;
}
