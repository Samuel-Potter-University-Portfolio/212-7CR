#include "TextureLoader.h"
#include "Logger.h"

#include <algorithm>
#include <FreeImage.h>


GLuint TextureLoader::operator[](std::string name) 
{
	//Ensure key is stored in lower case to prevent duplicates
	std::string key = name;
	std::transform(key.begin(), key.end(), key.begin(), ::tolower);

	if (texture_pool.find(key) != texture_pool.end())
		return texture_pool[key].texture_id;

	//Load new texture
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(name.c_str(), 0);

	//If cannot deduce format try to get from file extention
	if (format == FIF_UNKNOWN)
		format = FreeImage_GetFIFFromFilename(name.c_str());

	//Unable to load as cannot guess format
	if (format == FIF_UNKNOWN)
	{
		LOG(Error, "Unable to import texture '%s'", name.c_str());
		return 0;
	}

	if (!FreeImage_FIFSupportsReading(format))
	{
		LOG(Error, "Unsupported file type for '%s'", name.c_str());
		return 0;
	}
	
	FIBITMAP* image = FreeImage_Load(format, name.c_str());
	BYTE* bits = FreeImage_GetBits(image);
	unsigned int width = FreeImage_GetWidth(image);
	unsigned int height = FreeImage_GetHeight(image);

	if (!bits || !width || !height)
	{
		LOG(Error, "Unable to load '%s'", name.c_str());
		return 0;
	}

	GLuint texture_id;
	glGenTextures(1, &texture_id);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, GL_FALSE, GL_BGR, GL_UNSIGNED_BYTE, bits);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
	FreeImage_Unload(image);

	TextureData meta_data;
	meta_data.texture_id = texture_id;
	meta_data.imported = true;
	meta_data.file_path = name;

	texture_pool[key] = meta_data;
	LOG(Log, "Registered texture '%s'", key.c_str());
	return texture_id;
}

void TextureLoader::CleanUp() 
{
	LOG(Log, "Cleaning up textures:");

	GLuint current_texture;
	for (auto it = texture_pool.begin(); it != texture_pool.end(); ++it)
	{
		current_texture = it->second.texture_id;
		glDeleteTextures(1, &current_texture);

		LOG(Log, "\t-Unloaded texture '%s'", it->first.c_str());
	}
}