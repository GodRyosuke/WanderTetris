#version 330 core


// Uniforms for world transform and view-proj
uniform mat4 uTranslate;
uniform mat4 uScaling;
uniform mat4 uRotate;
uniform mat4 uViewProj;

// Attribute 0 is position, 1 is normal, 2 is tex coords.
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// Any vertex outputs (other than position)
out vec2 fragTexCoord;

void main()
{
	// Convert position to homogeneous coordinates
	vec4 pos = vec4(inPosition, 1.0);
	// Transform to position world space, then clip space
	// gl_Position = pos * uWorldTransform * uViewProj;
	// gl_Position = pos;
	// gl_Position = uViewProj * uWorldTransform * pos;
	gl_Position = uViewProj * uTranslate * uRotate * uScaling * pos;
	// gl_Position = vec4(inPosition.x, inPosition.y, inPosition.z, 1.0);

	// Pass along the texture coordinate to frag shader
	fragTexCoord = inTexCoord;
}