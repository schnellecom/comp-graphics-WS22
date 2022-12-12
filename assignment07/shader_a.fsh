#version 150

in vec3 vColor;

out vec4 oFragColor; //write the final color into this variable

void main() {
    vec4 oFragColor = vec4(vColor, 1.0);
//    vec4 oFragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
