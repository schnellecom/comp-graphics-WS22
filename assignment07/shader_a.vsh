#version 150

in vec4 aPosition;
in vec3 aNormal;

uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;

uniform vec3 uLightPosition;
uniform vec3 uLightColor;

uniform mat3 uAmbientMaterial;
uniform mat3 uDiffuseMaterial;
uniform mat3 uSpecularMaterial;
uniform float uSpecularityExponent;

out vec3 vColor;

float norm(vec3 v){
    float res = 0;
    res += v[0];
    res += v[1];
    res += v[2];
    res = sqrt(res);
    return res;
}

void main() {
    // Transform to eye-space
    vec4 newPosition = uModelViewMatrix * aPosition;

    // Transform normals using the correct matrix.
    // It's important to normalize in the end
    vec3 newNormal = vec3(uModelViewMatrix*vec4(aNormal, 1.0));
    newNormal = normalize(newNormal);

    vec3 wl = (uLightPosition-vec3(newPosition));
    wl = wl/norm(wl);

    vec3 wv = (-vec3(newPosition));
    wv = wv/norm(wv);

    vec3 h = (1/norm(wl+wv))*(wl + wv);

    //gl_Position = write the final vertex position into this variable
    gl_Position = uProjectionMatrix*uModelViewMatrix*aPosition;

    //diffuse term
    vec3 diffuseTerm = uLightColor*uDiffuseMaterial*((dot(newNormal, (uLightPosition - vec3(newPosition))) )*1/norm(uLightPosition- vec3(newPosition)));

    // Define the color of this vertex
    float factor = (dot(newNormal, h));
    vColor = uAmbientMaterial*uLightColor
            +diffuseTerm
            +uLightColor*uSpecularMaterial*vec3(factor);
//    vColor = vec3(1.0, 0.0, 0.0);
}
