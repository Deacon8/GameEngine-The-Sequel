#pragma once
#include "stdlib.h"
//Rework along with shader system when inspiration hits
//Also what if multiple of one component

enum ComponentType
{
    CTransform,
    CModel,
    CShader
};

struct Component
{
    ComponentType type;
    void* data;
};

struct Entity
{
    Component* components = NULL;
    unsigned int componentCount = 0;

    Entity(int numComponents)
    {

    }

    void AddComponent(ComponentType type, void* data)
    {   
        Component component;
        component.type = type;
        switch(type)
        {
            case CTransform: component.data = malloc(sizeof(Transform));
                break;
            case CModel: component.data = malloc(sizeof(Model));
                break;            
            case CShader: component.data = malloc(sizeof(Shader));
                break;
        }
        component.data = data;
        componentCount++;
        components = (Component*)realloc(components, sizeof(Component) * componentCount);
        //printf("Hi %u\n", componentCount);
        components[componentCount] = component;
        //printf("Hi");
    }

    void* GetComponent(ComponentType type)
    {
        for(unsigned int i = 0; i < componentCount; i++)
        {
            if(components[i].type == type)
            {   
                switch(type)
                {   
                    case CTransform:
                        return (Transform*)components[i].data;
                    case CModel:
                        return (Model*)components[i].data;
                    case CShader:
                        return (Shader*)components[i].data;
                }
            }
        }
        return 0;
    }
};