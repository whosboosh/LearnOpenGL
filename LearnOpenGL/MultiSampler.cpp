#include "MultiSampler.h"

MultiSampler::MultiSampler()
{
    framebuffer = 0;
    screenWidth = 0;
    screenHeight = 0;
}

MultiSampler::MultiSampler(GLuint width, GLuint height)
{
    framebuffer = 0;
    screenWidth = width;
    screenHeight = height;
}

void MultiSampler::init()
{
    // framebuffer configuration
    // -------------------------
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MultiSampler::BlitFrameBuffer()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
    //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
    glBlitFramebuffer(0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void MultiSampler::BindFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}


void MultiSampler::Read()
{
    //glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
}

MultiSampler::~MultiSampler()
{
}
