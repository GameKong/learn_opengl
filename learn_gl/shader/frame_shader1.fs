#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

// 灰化：计算rgb的平均值，然后设置每个颜色的分量为该值；
void main()
{
    FragColor = texture(screenTexture, TexCoords);
    float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    FragColor = vec4(average, average, average, 1.0);
}

/*void main()
{
	vec3 col = texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(col, 1.0);
}*/ 

// 反相：1-原本的颜色  
/*void main()
{
	vec3 col = vec3(1 - texture(screenTexture, TexCoords));
    FragColor = vec4(col, 1.0);
} */ 



// 卷积矩阵 做后期： 卷积矩阵是一个3*3的矩阵，每一个值可以看成一个权重值，然后把当前像素和周围的8个像素分别乘以对应的权重值，
//然后求和，作为当前值。 卷积矩阵的所有值的和一般为1，不为1的话最终的和可能使颜色更亮或更暗
/*const float offset = 1.0 / 300.0; */

/*void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );

	// 模糊
	/*float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
	);*/

	// 锐化
    /*float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );*/ 

	// 边缘检测
    float kernel[9] = float[](
        1, 1, 1,
        1,  -8, 1,
        1, 1, 1
    ); 

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
	
		col += sampleTex[i] * kernel[i];

    FragColor = vec4(col, 1.0);
}*/