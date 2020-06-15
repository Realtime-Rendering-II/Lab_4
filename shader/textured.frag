#version 330 core

in vec3 position;
in vec3 normal;
in vec2 tex_coord;

uniform sampler2D custom_texture_1;

out vec4 out_color;

void main(){
    vec3 diffuse_object_color = texture(custom_texture_1, tex_coord).rgb;
    out_color = vec4(diffuse_object_color, 1.0);
}