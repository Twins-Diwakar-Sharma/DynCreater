#version 400

struct Camera 
{
	vec4 spin;
	vec3 pos;
};


struct DirectionalLight
{
	vec3 dir;
	vec3 col;
};


in vec2 fragTex;
in vec3 fragNor;
in float debugJoint;

out vec4 outColor;

uniform sampler2D albedo;
uniform DirectionalLight sun;


void main()
{
  vec4 color = texture(albedo, fragTex);

	if(color.a < 0.1)
		discard;

	vec3 toLight = normalize(-1*sun.dir);	
	float diffuse = max(dot(toLight,fragNor),0.15); // 0.1
	vec3 diffuseColor = diffuse*sun.col;
	
	outColor = vec4(diffuseColor,1.0)  * color; // * color

  if(debugJoint > 0.8)
  {
    outColor.rgb = vec3(1,0,0);
  }


}
