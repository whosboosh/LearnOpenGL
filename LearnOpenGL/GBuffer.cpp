#include "GBuffer.h"

GBuffer::GBuffer()
{
    screenWidth = 0;
    screenHeight = 0;

    gBuffer = 0;
    rboDepth = 0;

    gPosition = 0;
    gNormal = 0;
    gAlbedoSpec = 0;
}

GBuffer::GBuffer(GLuint width, GLuint height)
{
	gBuffer = 0;
    rboDepth = 0;

	gPosition = 0;
	gNormal = 0;
	gAlbedoSpec = 0;

    screenWidth = width;
    screenHeight = height;
}

void GBuffer::init()
{
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    // Position color buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    // normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    // color + specular color buffer
    glGenTextures(1, &gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

    // Tell opengl which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    // Create and attach depth buffer (render buffer)
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::updateScreenSize(GLuint width, GLuint height)
{
    screenHeight = height;
    screenWidth = width;
}

void GBuffer::Write()
{
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
}

void GBuffer::Read(GLenum textureUnit, char* type)
{
    glActiveTexture(textureUnit);
    if (strcmp(type,"gPosition")) {
        glBindTexture(GL_TEXTURE_2D, gPosition);
    }
    else if (strcmp(type,"gNormal")) {
        glBindTexture(GL_TEXTURE_2D, gNormal);
    }
    else if (strcmp(type, "gAlbedoSpec")) {
        glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    }
}

GBuffer::~GBuffer()
{
    if (gBuffer)
    {
        glDeleteFramebuffers(1, &gBuffer);
    }
    if (rboDepth)
    {
        glDeleteRenderbuffers(1, &rboDepth);
    }
    if (gPosition)
    {
        glDeleteTextures(1, &gPosition);
    }
    if (gNormal)
    {
        glDeleteTextures(1, &gNormal);
    }
    if (gAlbedoSpec)
    {
        glDeleteTextures(1, &gAlbedoSpec);
    }
}
