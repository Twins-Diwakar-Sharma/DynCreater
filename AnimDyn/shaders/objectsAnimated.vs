#version 400

struct Camera 
{
	vec4 spin;
	vec3 pos;
};

vec4 quatRotate(inout vec4 action, inout vec4 victim)
{
	float ar = action.w;	float br = victim.w;
	vec3 av = action.xyz;	vec3 bv = victim.xyz;
	return vec4(ar*bv + br*av + cross(av,bv), ar*br - dot(av,bv));
}


vec3 rotateWRTCam(vec3 pos, in Camera cam)
{
  vec4 spin = cam.spin;
	vec4 quatPos = vec4(pos ,0);
	vec4 spinQuat = vec4(-spin.xyz, spin.w);
	vec4 spinQuatInv = vec4(spin);

	quatPos = quatRotate(quatPos, spinQuatInv);
	quatPos = quatRotate(spinQuat, quatPos);
	
	return quatPos.xyz;
}

vec3 rotateWRTQuat(vec3 pos, in vec4 spin)
{
  vec4 spinQuat = vec4(spin);
	vec4 quatPos = vec4(pos, 0);
	vec4 spinQuatInv = vec4(-spin.xyz, spin.w);

	quatPos = quatRotate(quatPos, spinQuatInv);
	quatPos = quatRotate(spinQuat, quatPos);
	
	return quatPos.xyz;
}

/*
struct JointData
{
  vec3 translation;
  vec4 rotation;
  vec3 scale;
};
*/

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 nor;
layout (location = 3) in vec4 jointID;
layout (location = 4) in vec4 weights;

out vec2 fragTex;
out vec3 fragNor;
out float debugJoint;

uniform mat4 projection;
uniform mat4 transform;
uniform Camera cam;
uniform int totalBones;
//uniform JointData currentFrameBones[16];
uniform mat4 jointMatrices[16];

void main()
{
  
  /*
  // initialize ??
  vec3 skeletonScale = vec3(0,0,0);
  vec3 skeletonTranslation = vec3(0,0,0);
  vec4 skeletonRotation = vec4(0,0,0,0); 

  vec4 normWeights = (weights);

  for(int i=0; i<4; i++)
  {
    int boneIndex = int(jointID[i]);
    skeletonScale +=  normWeights[i] * currentFrameBones[boneIndex].scale;
    skeletonRotation += normWeights[i] * currentFrameBones[boneIndex].rotation;
    skeletonTranslation += normWeights[i] * currentFrameBones[boneIndex].translation;
  }
  
  //scale rotate translate
  vec3 objectPos = vec3(pos); 
  objectPos = skeletonScale * objectPos;
  objectPos = objectPos + skeletonTranslation;
  objectPos = rotateWRTQuat(objectPos, skeletonRotation);
  */
  
  mat4 skeletonMat = mat4(0.0);
  for(int i=0; i<4; i++)
  {
    int boneIndex = int(jointID[i]);
    skeletonMat += weights[i] * jointMatrices[boneIndex];
  }
  
  vec4 objectPos = skeletonMat * vec4(pos,1.0);

  //vec4 worldPos = transform * vec4(objectPos,1);
  vec4 worldPos = transform * objectPos;
  vec3 viewPos = worldPos.xyz - cam.pos;
  viewPos = rotateWRTCam(viewPos, cam);
  gl_Position = projection * vec4(viewPos,1);  
  fragTex = vec2(tex);
  
  vec3 objectNor = vec3(nor);
  //objectNor = rotateWRTQuat(objectNor, skeletonRotation);
  objectNor = (skeletonMat * vec4(objectNor,0.0)).xyz;
  objectNor = normalize(objectNor);  

  fragNor = normalize((transform * vec4(objectNor,0.0)).xyz);


  debugJoint = 0.0;
  for(int i=0; i<4; i++)
  {
    int boneIndex = int(jointID[i]);
    if(boneIndex == 8 && weights[i] > 0.5)
      debugJoint = 1.0;
  }
}
