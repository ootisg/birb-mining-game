#ifndef TEXTURE_MAPPING_H
#define TEXTURE_MAPPING_H

/// A struct representing an image mapping on a texture
struct texture_mapping {
	unsigned int tex_id;
	rectangle bounds;
};

typedef struct texture_mapping texture_mapping;

#endif