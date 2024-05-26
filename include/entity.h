#pragma once

#include <stdlib.h>
#include <stack>
#include <stdint.h>

#include "transform.h"
#include "renderer.h"
#include "shader.h"

#define MAX_ENTITIES 100

typedef uint32_t Entity;
typedef uint8_t ComponentType;
typedef unsigned char Signature; //Manages which components are contained

//Bit fiddling
const unsigned char bExists     = 0b10000000;
const unsigned char bTransform  = 0b01000000;
const unsigned char bModel      = 0b00100000;
const unsigned char bShader     = 0b00010000;

//Components
static Transform transforms[MAX_ENTITIES];
static Model models[MAX_ENTITIES];
static Shader shaders[MAX_ENTITIES];

struct EntityManager
{       
    // Add stack/queue to manage destruction
    unsigned int currentEntityCount = 0;

    //Entity
    bool isAlive[MAX_ENTITIES] = { 0 };
    std::stack<Entity> availableEntities{};
    Signature signatures[MAX_ENTITIES] = { 0 };

    EntityManager()
    {
        for(int i = MAX_ENTITIES-1; i >= 0; i--)
        {
            availableEntities.push(i);
        }
    }

    Entity CreateEntity()
    {   
        if(currentEntityCount <= MAX_ENTITIES)
        {
            currentEntityCount++;
            Entity e = availableEntities.top();
            availableEntities.pop();
            isAlive[e] = 1;
            return e;
        } 
        else 
        {
            printf("Out of space\n");
            return 0;
        }
    }

    void DestroyEntity(Entity entity)
    {   
        currentEntityCount--;
        isAlive[entity] = 0;
        availableEntities.push(entity);
        //We don't destroy data -> will probably cause annoying bugs
    }
};

EntityManager entityManager;