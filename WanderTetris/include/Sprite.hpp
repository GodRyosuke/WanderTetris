#pragma once

#include "Shader.hpp"
#include "Texture.hpp"

class Sprite {
public:
	Sprite(Shader* shader, Texture* tex, glm::mat4 viewProj);
	void Draw(glm::vec2 pos, float rot, float scale = 1.0f, float alpha = 1.0f);

	int GetWidth() { return mWidth; }
	int GetHeight() { return mHeight; }

private:
	unsigned int mVertexArray;
	unsigned int mVertexBuffer;
	unsigned int mIndexBuffer;

	Texture* mTexture;
	Shader* mSpriteShader;
	int mWidth;
	int mHeight;
};