#pragma once

#define CGLTF_IMPLEMENTATION
#include "cgltf.h"

#include <stdio.h>
struct Model
{
    float* vertices;
    unsigned int verticesize;

    unsigned int* indices;
    unsigned int indicesize;
};

Model LoadModel()
{
    Model model;
    cgltf_options options = {};
    cgltf_data* data = NULL;
    cgltf_result result = cgltf_parse_file(&options, "res/models/duck.gltf", &data);
	printf("Result: %d\n", result);
    if (result == cgltf_result_success)
    {
		result = cgltf_load_buffers(&options, data, "res/models/duck.gltf");
		result = cgltf_validate(data);

	    printf("Result: %d\n", result);
		printf("Type: %u\n", data->file_type);
		printf("Meshes: %u\n", (unsigned)data->meshes_count);
        model.verticesize = data->buffers->size;
        model.vertices = (float*)malloc(model.verticesize * sizeof(float));
        model.indices = (unsigned int*)malloc(model.indicesize * sizeof(unsigned int));
	    cgltf_free(data);
        return model;
    }
    return model;
}
