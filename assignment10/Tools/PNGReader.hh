
#pragma once

#include <fstream>
#include <iostream>

#include "TextureData.hh"
#include "lodepng.hh"

class PNGReader
{
public:
    PNGReader() {}
    virtual ~PNGReader() {}

    TextureData* readFile(const std::string& fileName)
    {
        TextureData* texData = new TextureData;
        unsigned error;
        unsigned char* image;
        unsigned width, height;

        error = lodepng_decode32_file(&image, &width, &height, fileName.c_str());
        if (error)
        {
            std::cerr << "error loading file " << fileName << std::endl;
            return texData;
        }

        GLubyte* pImageData = new GLubyte[width * height * 4];

        memcpy(pImageData, image, width * height * 4);
        free(image);

        texData->setData(pImageData);
        texData->setWidth(width);
        texData->setHeight(height);
        texData->setDepth(1); // 2D so, depth is 1
        texData->setFormat(GL_RGBA);
        texData->setType(GL_UNSIGNED_BYTE); // == unsigned char

        return texData;
    }
};
