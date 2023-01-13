/*===========================================================================*/
/*                                                                           *
 * Copyright (c) 2020, Computer Graphics Group RWTH Aachen University        *
 *                            All rights reserved                            *
 *                                                                           *
 * Basic Techniques in Computer Graphics Exercise                            *
 *                                                                           *
 *                                                                           */
/*===========================================================================*/

#include "assignment.hh"

#include "Tools/ABReader.hh"
#include "Tools/HelperFunctions.hh"
#include "Tools/PNGReader.hh"
#include "Tools/TextureData.hh"


extern glm::mat4 projectionMatrix;

namespace
{
GLuint bunnyTexture;
GLuint earthTexture;

GLuint cubeMapTexture;
GLuint cubeMapTextureDebug;

GLuint sphereMapTexture;
GLuint sphereMapTextureDebug;

//
// gets a filename and creates an OpenGL texture with MipMaps.
//
GLuint readTexture2D(const std::string& fileName)
{
    PNGReader pngreader;
    TextureData* texture;
    GLuint gltexture;
    //
    // you will need:
    //
    // texture->getWidth()   <- image width
    // texture->getHeight()  <- image height
    // texture->getFormat()  <- format, e.g. GL_RGB
    // texture->getType()    <- data type, e.g. GL_UNSIGNED_BYTE
    // texture->getData()    <- a pointer to the data
    //

    texture = pngreader.readFile(fileName);
    glGenTextures(1, &gltexture);
    glBindTexture(GL_TEXTURE_2D, gltexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,
                 0, // MipMap level
                 GL_SRGB, texture->getWidth(), texture->getHeight(),
                 0, // no border !
                 texture->getFormat(), texture->getType(), texture->getData());

    glGenerateMipmap(GL_TEXTURE_2D);
    delete texture; // Not needed anymore!

    return gltexture;
}

}

//
// Control the app by pressing:
// R: model rotation on/off
// X: switch between debug and normal environment map / skytexture                  (debugTexture)
// E: only show the environment map or add the environment * 0.1 to the phong model (environmentOnly)
// C: switch between cubemapping and sphere mapping                                 (cubeMapping)
// M: switch between the meshes                                                     (meshNumber)
//
void task::drawScene(bool environmentOnly, int meshNumber, bool cubeMapping, bool debugTexture)
{
    //
    // When entering here, the correct shader is already active and the skybox is already rendered.
    // You need to bind the cubemap or spheremap texture based on 'cubeMapping' and use the
    // debug or non-debug texture based on 'debugTexture'.
    //
    // Set additional uniforms with
    //  g_shader->setUniform("name", <value> );
    //


    if (debugTexture)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureDebug);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, sphereMapTextureDebug);
    }
    else
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, sphereMapTexture);
    }

    g_shader->setUniform("uUseCubeMapping", cubeMapping ? 1 : 0);
    // set color mixing vs pure environment reflection:
    g_shader->setUniform("uEnvironmentOnly", environmentOnly ? 1 : 0);


    // render the correct mesh:
    if (meshNumber == 0)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, earthTexture);
        g_vaoEarth->render();
    }
    else
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, bunnyTexture);
        g_vaoBunny->render();
    }
}


void task::initCustomResources()
{
    // activate nicer sampling at seams of the cube map.
    // try to uncomment this and you will notice aliasing at the color edges
    // of the debug cubemap on the sphere mesh (the aliasing is much more subtle
    // on the bunny):
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // Create your resources here, e.g. shaders, buffers,...

    ////////////////////////////////////////////////////////////////////////////
    // Shader:

    // you have to edit the shader for texture access, so rename them and read your shader
    // files here:
    g_shader = new ShaderProgram(std::string(CURR_DIR) + "/assignment10/envmap.vsh", std::string(CURR_DIR) + "/assignment10/envmap.fsh");
    if (!g_shader->link())
        exit(0);

    g_shader->use();
    g_shader->setUniform("uMaterialDiffuse", (int)0);
    g_shader->setUniform("uTextureCubeMap", (int)1);
    g_shader->setUniform("uTextureSphereMap", (int)2);
    g_shader->setUniform("uMaterialSpecular", (int)3);

    ////////////////////////////////////////////////////////////////////////////
    // Read textures:

    //
    // Simple 2D textures can be loaded with this little helper function.
    // You need to write your own code for the cubemaps.
    //
    bunnyTexture = readTexture2D(std::string(CURR_DIR) + "/assignment10/assets/clownfishBunny.png");
    earthTexture = readTexture2D(std::string(CURR_DIR) + "/assignment10/assets/blueMarble.png");
    sphereMapTexture = readTexture2D(std::string(CURR_DIR) + "/assignment10/assets/sphereMap.png");
    sphereMapTextureDebug = readTexture2D(std::string(CURR_DIR) + "/assignment10/assets/sphereMapDebug.png");

    PNGReader pngreader;
    TextureData* texture;


    glGenTextures(1, &cubeMapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

    texture = pngreader.readFile(std::string(CURR_DIR) + "/assignment10/assets/posx.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_SRGB, texture->getWidth(), texture->getHeight(), 0, texture->getFormat(), texture->getType(),
                 texture->getData());
    delete texture;

    texture = pngreader.readFile(std::string(CURR_DIR) + "/assignment10/assets/negx.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_SRGB, texture->getWidth(), texture->getHeight(), 0, texture->getFormat(), texture->getType(),
                 texture->getData());
    delete texture;

    texture = pngreader.readFile(std::string(CURR_DIR) + "/assignment10/assets/posy.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_SRGB, texture->getWidth(), texture->getHeight(), 0, texture->getFormat(), texture->getType(),
                 texture->getData());
    delete texture;

    texture = pngreader.readFile(std::string(CURR_DIR) + "/assignment10/assets/negy.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_SRGB, texture->getWidth(), texture->getHeight(), 0, texture->getFormat(), texture->getType(),
                 texture->getData());
    delete texture;

    texture = pngreader.readFile(std::string(CURR_DIR) + "/assignment10/assets/posz.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_SRGB, texture->getWidth(), texture->getHeight(), 0, texture->getFormat(), texture->getType(),
                 texture->getData());
    delete texture;

    texture = pngreader.readFile(std::string(CURR_DIR) + "/assignment10/assets/negz.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_SRGB, texture->getWidth(), texture->getHeight(), 0, texture->getFormat(), texture->getType(),
                 texture->getData());
    delete texture;

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP); // will fail if not all 6 faces were given!


    // debug cubemap:

    glGenTextures(1, &cubeMapTextureDebug);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureDebug);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

    texture = pngreader.readFile(std::string(CURR_DIR) + "/assignment10/assets/posxd.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_SRGB, texture->getWidth(), texture->getHeight(), 0, texture->getFormat(), texture->getType(),
                 texture->getData());
    delete texture;

    texture = pngreader.readFile(std::string(CURR_DIR) + "/assignment10/assets/negxd.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_SRGB, texture->getWidth(), texture->getHeight(), 0, texture->getFormat(), texture->getType(),
                 texture->getData());
    delete texture;

    texture = pngreader.readFile(std::string(CURR_DIR) + "/assignment10/assets/posyd.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_SRGB, texture->getWidth(), texture->getHeight(), 0, texture->getFormat(), texture->getType(),
                 texture->getData());
    delete texture;

    texture = pngreader.readFile(std::string(CURR_DIR) + "/assignment10/assets/negyd.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_SRGB, texture->getWidth(), texture->getHeight(), 0, texture->getFormat(), texture->getType(),
                 texture->getData());
    delete texture;

    texture = pngreader.readFile(std::string(CURR_DIR) + "/assignment10/assets/poszd.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_SRGB, texture->getWidth(), texture->getHeight(), 0, texture->getFormat(), texture->getType(),
                 texture->getData());
    delete texture;

    texture = pngreader.readFile(std::string(CURR_DIR) + "/assignment10/assets/negzd.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_SRGB, texture->getWidth(), texture->getHeight(), 0, texture->getFormat(), texture->getType(),
                 texture->getData());
    delete texture;

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP); // will fail if not all 6 faces were given!
}

void task::deleteCustomResources()
{
    // Don't forget to delete your OpenGL resources (shaders, buffers, etc.)!

    glDeleteTextures(1, &earthTexture);
    glDeleteTextures(1, &bunnyTexture);
    glDeleteTextures(1, &cubeMapTexture);
    glDeleteTextures(1, &cubeMapTextureDebug);
}
