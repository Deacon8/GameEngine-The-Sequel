#pragma once

#define CGLTF_IMPLEMENTATION
#include "cgltf.h"

#include <stdio.h>

struct Model
{
    float* vertices;
    size_t verticesize;

    unsigned short* indices;
    size_t indicesize; 

    float* normals;
    size_t normalsize;

    float* texcoords;
    size_t texcoordsize;
};

Model LoadModel()
{
    Model model = {};
    cgltf_options options = {};
    cgltf_data* data = NULL;
    cgltf_result result = cgltf_parse_file(&options, "res/models/duck.gltf", &data);
	printf("Result: %d\n", result);
    if (result == cgltf_result_success)
    {
		result = cgltf_load_buffers(&options, data, "res/models/duck.gltf");
		result = cgltf_validate(data);
        
		printf("Type: %u\n", data->file_type);
		printf("Meshes: %u\n", (unsigned)data->meshes_count);
        //0 ???Indices
        //1 Normal
        //2 Position
        //3 TexCoords
        char* file = "res/models/";
        strcat(file, data->buffers->uri);
        FILE *buffer = fopen(file, "rb");
        model.indicesize = data->accessors[0].count;
        model.indices = (unsigned short*)malloc(model.indicesize * sizeof(unsigned short));
        fseek(buffer, data->accessors[0].offset, SEEK_SET);
        fread(model.indices, sizeof(unsigned short), model.indicesize, buffer);

        model.verticesize = data->accessors[1].count;
        model.vertices = (float*)malloc(model.verticesize * sizeof(float));
        fseek(buffer, data->accessors[1].offset, SEEK_SET);
        fread(model.vertices, sizeof(unsigned short), model.verticesize, buffer);

        model.normalsize = data->accessors[2].count;
        model.normals = (float*)malloc(model.normalsize * sizeof(float));
        fseek(buffer, data->accessors[2].offset, SEEK_SET);
        fread(model.normals, sizeof(unsigned short), model.normalsize, buffer);

        model.texcoordsize = data->accessors[3].count;
        model.texcoords = (float*)malloc(model.texcoordsize * sizeof(float));
        fseek(buffer, data->accessors[3].offset, SEEK_SET);
        fread(model.texcoords, sizeof(unsigned short), model.texcoordsize, buffer);

        printf("%u", model.verticesize);

	    cgltf_free(data);
        return model;
    }
    return model;
}
