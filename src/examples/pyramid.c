#include <siege/siege.h>

#include <stdlib.h>
#include <stdio.h>

SGPhysicsSpace* space;

size_t numboxes = 0;
SGEntity** boxes = NULL;
SGbool overlay = SG_FALSE;

SGEntity* controller;

// 1280x720
#define WIDTH 1280
#define HEIGHT 720

#define BWIDTH 1   // 4
#define BLENGTH 8 // 32

#define FLOORHEIGHT 8

#define ISIZE (((BWIDTH << 1) + (BLENGTH >> 1)) >> 1)

#define MAXCOLS (WIDTH * 2 / BLENGTH / 3)
#define MAXROWS (HEIGHT - FLOORHEIGHT) / (BLENGTH + BWIDTH + BWIDTH)

#define NCOLS MAXCOLS
#define NROWS MAXROWS

#define NEXPAR 32

typedef struct Box
{
    SGPhysicsShape* shape;
    float length;
    float width;
} Box;

void destroyBox(SGEntity* entity)
{
    if(entity == NULL)
        return;
    
    sgPhysicsBodyDestroy(entity->body);
    
    Box* box = entity->data;
    sgPhysicsShapeDestroy(box->shape);
    
    free(box);
}
void drawBox(SGEntity* entity)
{
    if(overlay)
        return;
    
    Box* box = entity->data;
    
    size_t i;
    SGVec2 verts[4];
    verts[0] = sgVec2f(-box->length / 2.0, -box->width / 2.0);
    verts[1] = sgVec2f(-box->length / 2.0,  box->width / 2.0);
    verts[2] = sgVec2f( box->length / 2.0,  box->width / 2.0);
    verts[3] = sgVec2f( box->length / 2.0, -box->width / 2.0);
    
    float x, y;
    sgEntityGetPos(entity, &x, &y);
    float angle = sgEntityGetAngleRads(entity);
    for(i = 0; i < 4; i++)
    {
        verts[i] = sgVec2SetAngleRads(verts[i], sgVec2GetAngleRads(verts[i]) + angle);
        verts[i].x += x;
        verts[i].y += y;
    }
    
    sgDrawColor4f(0.75, 0.75, 0.75, 1.0);
    sgDrawBegin(SG_GRAPHICS_PRIMITIVE_QUADS);
        for(i = 0; i < 4; i++)
            sgDrawVertex2fv(&verts[i].x);
    sgDrawEnd();
    sgDrawColor4f(0.0, 0.0, 0.0, 1.0);
    sgDrawBegin(SG_GRAPHICS_PRIMITIVE_LINE_LOOP);
        for(i = 0; i < 4; i++)
            sgDrawVertex2fv(&verts[i].x);        
    sgDrawEnd();
    sgDrawColor4f(1.0, 1.0, 1.0, 1.0);
}
SGEntity* createBox(float x, float y, float angle, float length, float width, float density, SGbool stat)
{
    SGuint type = SG_PHYSICS_BODY_NORMAL;
    if(stat)
        type = (density == SG_INF) ? SG_PHYSICS_BODY_STATIC : SG_PHYSICS_BODY_SEMISTATIC;

    SGPhysicsBody* body = sgPhysicsBodyCreate(space, type);

    float w = length;
    float h = width;

    float verts[8] = {-w / 2.0, -h / 2.0,
                      -w / 2.0,  h / 2.0,
                       w / 2.0,  h / 2.0,
                       w / 2.0, -h / 2.0};

    SGEntity* entity = sgEntityCreate(0.0);

    Box* box = malloc(sizeof(Box));
    box->length = length;
    box->width = width;
    entity->data = box;
    
    sgEntitySetPhysicsBody(entity, body);
    sgEntitySetPos(entity, x, y);
    sgEntitySetAngleDegs(entity, angle);

    box->shape = sgPhysicsShapeCreatePoly(body, 0.0, 0.0, verts, 4);
    sgPhysicsShapeSetRestitution(box->shape, 0.25);
    sgPhysicsShapeSetFriction(box->shape, 1.0);
    sgPhysicsBodySetMass(body, sgPhysicsShapeGetMass(box->shape, density));
    sgPhysicsBodySetMoment(body, sgPhysicsShapeGetMomentDensity(box->shape, density));

    entity->lcDestroy = destroyBox;
    entity->evDraw = drawBox;

    numboxes++;
    boxes = realloc(boxes, numboxes * sizeof(SGEntity*));
    boxes[numboxes - 1] = entity;
    
    if(!(numboxes % 10000))
        printf("NUM %u\n", (unsigned int)numboxes);
    
    return entity;
}
SGEntity* createTile(float x, float y, float angle, float length, float width)
{
    SGEntity* entity = createBox(x, y, angle, length, width, 1.0, SG_FALSE);
    sgPhysicsBodySetSleeping(sgEntityGetPhysicsBody(entity), SG_TRUE);
    return entity;
}
SGEntity* createFloor(float x, float y, float length, float width)
{
    return createBox(x, y, 0.0, length, width, SG_INF, SG_TRUE);
}
void boxDrawDBG(SGEntity* entity)
{
    if(entity == NULL)
        return;

    Box* box = entity->data;
    sgPhysicsShapeDrawDBG(box->shape);
}

void evKeyboardKeyPress(SGEntity* entity, SGenum key)
{
    if(key == SG_KEYBOARD_KEY_F1)
        overlay = !overlay;
}
void evKeyboardKeyRepeat(SGEntity* entity, SGenum key)
{
    evKeyboardKeyPress(entity, key);
}
void evMouseButtonLeftPress(SGEntity* entity)
{
    SGint mx, my;
    sgMouseGetPos(&mx, &my);
    
    createBox(mx, my, 0.0, ISIZE, ISIZE, 1.0, SG_FALSE);
}
void evMouseButtonRightPress(SGEntity* entity)
{
    SGint mx, my;
    sgMouseGetPos(&mx, &my);
    
    SGVec2 mouse = sgVec2f(mx, my);
    
    SGVec2 pos;
    pos.x = mx < WIDTH / 2.0 ? 0 : WIDTH;
    pos.y = HEIGHT / 2.0;
    
    SGVec2 vel = sgVec2SetLength(sgVec2Sub(mouse, pos), 100.0);
    
    SGEntity* bentity = createBox(pos.x, pos.y, 0.0, ISIZE, ISIZE, 1.0, SG_FALSE);
    SGPhysicsBody* body = sgEntityGetPhysicsBody(bentity);
    sgPhysicsBodySetVel(body, vel.x, vel.y);
}
void evMouseButtonMiddlePress(SGEntity* entity)
{
    SGint mx, my;
    sgMouseGetPos(&mx, &my);
    
    SGEntity* bentity;
    SGPhysicsBody* body;
    Box* box;
    size_t i;
    
    SGVec2 vel;
    
    // we do this to ensure that each explosion doesn't interact with itself!
    size_t nboxes = numboxes;
    for(i = 0; i < NEXPAR; i++)
    {
        bentity = createBox(mx, my, 0.0, ISIZE, ISIZE, 1.0, SG_FALSE);
        body = sgEntityGetPhysicsBody(bentity);
        box = bentity->data;
        
        sgPhysicsShapeSetGroup(box->shape, nboxes);
        
        vel = sgVec2PolarRads(rand() / (float)RAND_MAX * SG_PI * 2, 100.0);
        sgPhysicsBodySetVel(body, vel.x, vel.y);
    }
}

int main(void)
{
    srand(1);
    
    size_t i, j;
    float x;
    
    sgLoadModule("SDL");
    sgLoadModule("OpenGL");
    sgLoadModule("Chipmunk");
    sgInit(WIDTH, HEIGHT, 32, 0);
    sgWindowSetTitle("SIEGE Pyramid Demo - Press F1 for debug overlay");

    space = sgPhysicsSpaceCreate();
	//space = sgPhysicsSpaceGetDefault();
	sgPhysicsSpaceSetIterations(space, 100);
	sgPhysicsSpaceSetDamping(space, 1.0);
    sgPhysicsSpaceSetGravity(space, 0.0, /*9.81*/0.5);

    controller = sgEntityCreate(0.0);
    controller->evMouseButtonLeftPress = evMouseButtonLeftPress;
    controller->evMouseButtonRightPress = evMouseButtonRightPress;
    controller->evMouseButtonMiddlePress = evMouseButtonMiddlePress;
    controller->evKeyboardKeyPress = evKeyboardKeyPress;
    controller->evKeyboardKeyRepeat = evKeyboardKeyRepeat;

    createFloor(WIDTH / 2.0, HEIGHT - FLOORHEIGHT / 2.0, WIDTH, FLOORHEIGHT);

    float thspace = BLENGTH * 1.5;
    float tvspace = BLENGTH + BWIDTH + BWIDTH;
    float twidth = NCOLS * thspace;
    float tstart = (WIDTH - twidth) / 2.0 + thspace * 0.5;
    
    printf("%u cols, %u rows\n", NCOLS, NROWS);
    
    for(j = 0; j < NROWS; j++)
    {
        for(i = j; i < NCOLS; i++)
        {
            x = tstart + i * thspace;
            
            createTile(x, HEIGHT - FLOORHEIGHT - tvspace * j - BLENGTH / 2.0, 90, BLENGTH, BWIDTH);
            createTile(x, HEIGHT - FLOORHEIGHT - tvspace * j - BLENGTH - BWIDTH / 2.0, 0, BLENGTH, BWIDTH);
            if(i != j)
                createTile(x - thspace / 2.0, HEIGHT - FLOORHEIGHT - tvspace * (j + 1) + BWIDTH / 2.0, 0, BLENGTH, BWIDTH);
            
            if(i == j)
                createTile(x - BLENGTH / 2.0 + BWIDTH / 2.0, HEIGHT - FLOORHEIGHT - tvspace * (j + 1) + BWIDTH - BLENGTH / 2.0, 90, BLENGTH, BWIDTH);
            if(i == NCOLS - 1)
                createTile(x + BLENGTH / 2.0 - BWIDTH / 2.0, HEIGHT - FLOORHEIGHT - tvspace * (j + 1) + BWIDTH - BLENGTH / 2.0, 90, BLENGTH, BWIDTH);
        }
        tstart -= thspace * 0.5;
    }

    printf("Number of shapes: %u\n", (unsigned int)numboxes);

    while(sgLoop(NULL))
    {
        if(overlay)
            for(i = 0; i < numboxes; i++)
                boxDrawDBG(boxes[i]);

        sgWindowSwapBuffers();
        sgDrawClear4f(1.0, 1.0, 1.0, 1.0);
        
        sgPhysicsSpaceStep(space, 1.0/16.0);
    }
    
    sgDeinit();

    free(boxes);

    return 0;
}
