#include "ObjectRendererAnimated.h"

ObjectRendererAnimated::ObjectRendererAnimated() : shaderProgram("objectsAnimated")
{
    shaderProgram.mapUniform("projection");
    shaderProgram.mapUniform("transform");
    shaderProgram.mapUniform("albedo");
    shaderProgram.mapCameraUniform("cam");
    shaderProgram.mapDirectionalLightUniform("sun");
    shaderProgram.mapUniform("totalBones");
    //shaderProgram.mapJointDataArrayUniform("currentFrameBones");
    shaderProgram.mapJointMatricesArrayUniform("jointMatrices");

}

ObjectRendererAnimated::~ObjectRendererAnimated()
{
}


void ObjectRendererAnimated::render(std::vector<ObjectAnimated>& objects, Camera& cam, DirectionalLight& sun)
{
  shaderProgram.use();
  shaderProgram.setUniform("projection",proj::perspective);
  shaderProgram.setUniform("albedo",0);
  shaderProgram.setUniform("cam", cam);
  shaderProgram.setUniform("sun", sun);

  for(int i=0; i<objects.size(); i++)
  {
    shaderProgram.setUniform("transform", objects[i].getTransform());
    shaderProgram.setUniform("totalBones", (int)( objects[i].getMesh().skeleton.jointTree.size() ));
    //shaderProgram.setJointDataArrayUniform("currentFrameBones", objects[i].currentFrameBones);
    shaderProgram.setJointMatricesArrayUniform("jointMatrices", objects[i].jointMatrices);
    objects[i].bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, objects[i].getTextureId());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glDrawElements(GL_TRIANGLES, objects[i].size(), GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(4);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    objects[i].unbind();
  }

  shaderProgram.unuse();
}
