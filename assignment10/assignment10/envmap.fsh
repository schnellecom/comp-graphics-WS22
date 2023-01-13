#version 150

// Get in the transformed positions and normals
// now in eye-space
in vec4 vEyePosition;
in vec3 vWorldPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vWorldNormal;

uniform sampler2D uMaterialDiffuse;
uniform mat4 uModelMatrix;

uniform vec3 uLightPosition;
uniform vec3 uLightColor;

uniform float uSpecularityExponent;
uniform vec3 uCameraPosition;

uniform bool uUseCubeMapping;
uniform bool uEnvironmentOnly;

uniform samplerCube uTextureCubeMap;
uniform sampler2D uTextureSphereMap;

out vec4 oFragColor;

/*
 * Lighting model: Phong/Blinn lighting per vertex
 * Shading Model: Average of colors of incident vertices per triangle
 * Assume attenuation = 1;
 */
vec3 ambient() {

	vec3 texColor = texture(uMaterialDiffuse, vTexCoord).rgb;
    return uLightColor * texColor * 0.02; //  to damp the ambient color
}

vec3 diffuse() {
	vec3 texColor = texture(uMaterialDiffuse, vTexCoord).rgb;

    vec3 lp = normalize(uLightPosition - vec3(vEyePosition));
    vec3 n = normalize(vNormal);
    float cosAngle = max(dot(lp, normalize(vNormal)), 0);
    return vec3((uLightColor*texColor) * cosAngle);
}

vec3 specular() {
    vec3 bisector = normalize(normalize(-vec3(vEyePosition)) + normalize((uLightPosition - vec3(vEyePosition))));

    float cosAngle = max(dot(bisector, normalize(vNormal)), 0);

	float specularMaterial = 0.5;
    return specularMaterial * uLightColor * pow(cosAngle, uSpecularityExponent);
}

void main() {
    
   vec3 normalInWorldSpace = normalize( vWorldNormal );
   vec3 cameraDir = normalize( vWorldPosition - uCameraPosition );
   vec3 reflectionVector = normalize( reflect( cameraDir, normalInWorldSpace ) );
   
   vec3 reflectionColor = vec3(0.0);

   vec3 lighingTerm = ambient() + diffuse() + specular();
      
   // =======================================================================
   // =======================================================================
   // Assignment code:
   // Create your own temporary variables here if you need to.
   // =======================================================================
   // =======================================================================

   // Environment Map:
   if (uUseCubeMapping) {
     // calculate the reflectioncolor via cube mapping
     reflectionColor = vec3(texture(uTextureCubeMap, vec3(reflectionVector)));
   } else {
     // Implement sphere mapping here by computing vec2 sphereCoord
     vec2 sphereCoord = vec2(0,0);
     reflectionColor = vec3(texture(uTextureSphereMap, sphereCoord));
   }

   if (uEnvironmentOnly) {
     vec3 fragColor = reflectionColor;
     oFragColor = vec4(fragColor, 1.0f);
   } else {
     // Part b: implement blending here
     vec3 fragColor = lighingTerm; //Replace this line
     oFragColor = vec4(fragColor, 1.0f);
   }

    // =======================================================================
    // End assignment code
    // =======================================================================
}
