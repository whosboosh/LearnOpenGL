#pragma once
#include "Light.h"

namespace opengl {
    class DirectionalLight :
        public Light
    {
    public:
        DirectionalLight();

        DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight,
            GLfloat red, GLfloat green, GLfloat blue,
            GLfloat aIntensity, GLfloat dIntensity,
            GLfloat xDir, GLfloat yDir, GLfloat zDir);

        void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
            GLuint diffuseIntensityLocation, GLuint directionLocation);

        void UpdatePosition(GLfloat xDir, GLfloat yDir, GLfloat zDir);

        glm::mat4 CalculateLightTransform();

        ~DirectionalLight();

    private:
        glm::vec3 direction;
    };
}