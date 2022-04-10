#include "Sprite.hpp"

Sprite::Sprite(Shader* shader, Texture* tex, glm::mat4 viewProj)
{
	mTexture = tex;
	mWidth = tex->getWidth();
	mHeight = tex->getHeight();

	float SpriteVertices[] =
	{ //     COORDINATES     /        COLORS      /   TexCoord  //
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f, // top left
		0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f,	// top right
		0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f,	// bottom right
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f	// bottom left
	};

	unsigned int SpriteIndices[] =
	{
		0, 1, 2,
		2, 3, 0
	};


	// view projection matrix‚ÌÝ’è
	mSpriteShader = shader;
	mSpriteShader->UseProgram();
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	// VAO‚ÌÝ’è
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteVertices), SpriteVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SpriteIndices), SpriteIndices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// unbind cube vertex arrays
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Sprite::Draw(glm::vec2 pos, float rot, float scale, float alpha)
{
	// Sprite‚Ì•`‰æˆÊ’u‚ðÝ’è
	mSpriteShader->UseProgram();
	// Sprite Translation Matrix
	glm::vec3 spritePos = glm::vec3(pos, 0.0f);
	glm::mat4 SpriteTrans = glm::translate(glm::mat4(1.0f), spritePos);

	// sprite‚Ìscaling matrix
	glm::vec3 sprite_scale_vec = glm::vec3((float)mTexture->getWidth(), (float)mTexture->getHeight(), 1.0f);
	sprite_scale_vec *= scale;
	glm::mat4 SpriteScaling = glm::scale(glm::mat4(1.0f), sprite_scale_vec);

	// sprite‚Ìrotation matrix
	glm::mat4 SpriteRotate = glm::rotate(glm::mat4(1.0f), rot, glm::vec3(0, 0, 1.0f));
	//glm::mat4 SpriteRotate = glm::rotate(glm::mat4(1.0f), (float)M_PI, glm::vec3(0, 0, 1.0f));

	// Texture‚Ìalpha’l‚ðÝ’è
	mSpriteShader->SetFloatUniform("uSpriteAlpha", alpha);
	mSpriteShader->SetMatrixUniform("uTranslate", SpriteTrans);	// cube‚ÌÀ•W‚ð”½‰f
	mSpriteShader->SetMatrixUniform("uScaling", SpriteScaling);	// cube‚ÌÀ•W‚ð”½‰f
	mSpriteShader->SetMatrixUniform("uRotate", SpriteRotate);	// cube‚ÌÀ•W‚ð”½‰f

	// •`‰æ
	glBindVertexArray(mVertexArray);
	mTexture->BindTexture();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	mTexture->UnBindTexture();


}
