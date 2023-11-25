#version 330 core
out vec4 FragColor;

void main()
{
    float grad = gl_FragCoord.z;
    FragColor = vec4(grad, grad, grad, 1.0f);
}