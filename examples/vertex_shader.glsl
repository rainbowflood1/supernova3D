#version 440 core
layout (location = 0) in vec3 aPos;

// Hello

void main() {
	gl_Position = vec4(aPos.xyz, 1);
}
