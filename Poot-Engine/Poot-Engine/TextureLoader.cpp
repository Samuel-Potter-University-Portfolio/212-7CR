#include "TextureLoader.h"
#include "Logger.h"

#include <algorithm>
#include <FreeImage.h>

struct ImageData 
{
	FIBITMAP* image = nullptr;
	BYTE* bits = nullptr;
	FREE_IMAGE_FORMAT format;
	unsigned int width = 0;
	unsigned int height = 0;
};

ImageData GetImage(std::string file_name)
{
	ImageData image_data;

	//Load new texture
	image_data.format = FreeImage_GetFileType(file_name.c_str(), 0);

	//If cannot deduce format try to get from file extention
	if (image_data.format == FIF_UNKNOWN)
		image_data.format = FreeImage_GetFIFFromFilename(file_name.c_str());

	//Unable to load as cannot guess format
	if (image_data.format == FIF_UNKNOWN)
	{
		LOG(Error, "Unable to import texture '%s'", file_name.c_str());
		return image_data;
	}

	if (!FreeImage_FIFSupportsReading(image_data.format))
	{
		LOG(Error, "Unsupported file type for '%s'", file_name.c_str());
		return image_data;
	}

	image_data.image = FreeImage_Load(image_data.format, file_name.c_str());
	image_data.bits = FreeImage_GetBits(image_data.image);
	image_data.width = FreeImage_GetWidth(image_data.image);
	image_data.height = FreeImage_GetHeight(image_data.image);

	//Log any other errors
	if (!image_data.bits || !image_data.width || !image_data.height)
	{
		LOG(Error, "Unable to load '%s'", file_name.c_str());
		return image_data;
	}

	return image_data;
}

GLuint TextureLoader::operator[](std::string name) 
{
	//Ensure key is stored in lower case to prevent duplicates
	std::string key = name;
	std::transform(key.begin(), key.end(), key.begin(), ::tolower);

	if (texture_pool.find(key) != texture_pool.end())
		return texture_pool[key].texture_id;


	ImageData image_data = GetImage(name);

	GLuint texture_id;
	glGenTextures(1, &texture_id);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_data.width, image_data.height, 0, image_data.format == FIF_PNG ? GL_BGRA : GL_BGR, GL_UNSIGNED_BYTE, image_data.bits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	FreeImage_Unload(image_data.image);


	TextureData meta_data;
	meta_data.texture_id = texture_id;
	meta_data.imported = true;
	meta_data.file_path = name;


	texture_pool[key] = meta_data;
	LOG(Log, "Registered texture '%s'", key.c_str());

	return texture_id;
}

//Assumes for cubemap test.png files are stored as test_top.png, test_back.png etc.
GLuint TextureLoader::GetCubeMap(std::string name) 
{
	//Ensure key is stored in lower case to prevent duplicates
	std::string key = name;
	std::transform(key.begin(), key.end(), key.begin(), ::tolower);

	if (cube_map_pool.find(key) != cube_map_pool.end())
		return cube_map_pool[key].texture_id;


	//Attempt to load cube map
	std::string file_name, file_extention;

	//Split file name and extention
	{
		bool prefix = false;

		for (auto it = name.rbegin(); it != name.rend(); ++it)
		{
			const char c = *it;

			if (c == '.' && !prefix)
				prefix = true;
			else
			{
				if (prefix)
					file_name = c + file_name;
				else
					file_extention = c + file_extention;
			}
		}
	}


	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

	const GLuint gl_side[6]
	{
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	const std::string side_name[6]
	{
		"_right",
		"_left",
		"_top",
		"_bottom",
		"_back",
		"_front"
	};
	
	//Load each file of the cube map
	for (int i = 0; i < 6; i++)
	{
		std::string current_file = file_name + side_name[i] + "." + file_extention;
		ImageData image_data = GetImage(current_file);

		glTexImage2D(gl_side[i], 0, GL_RGBA, image_data.width, image_data.height, 0, image_data.format == FIF_PNG ? GL_BGRA : GL_BGR, GL_UNSIGNED_BYTE, image_data.bits);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		FreeImage_Unload(image_data.image);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	TextureData meta_data;
	meta_data.texture_id = texture_id;
	meta_data.imported = true;
	meta_data.file_path = name;


	cube_map_pool[key] = meta_data;
	LOG(Log, "Registered cubemap '%s'", key.c_str());

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
	
	for (auto it = cube_map_pool.begin(); it != cube_map_pool.end(); ++it)
	{
		current_texture = it->second.texture_id;
		glDeleteTextures(1, &current_texture);

		LOG(Log, "\t-Unloaded cubemap '%s'", it->first.c_str());
	}
}