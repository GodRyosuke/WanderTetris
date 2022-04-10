#pragma once

#include <string>
#include <map>
#include "glew.h"
#include "glm.hpp"
#include "ft2build.h"
#include "Shader.hpp"

#include FT_FREETYPE_H

class Font {
public:
	// @param fontPath .ttfファイルのパス
	Font(std::string fontPath, Shader* textShader, glm::mat4 viewProj);
	~Font();
	void DrawUTF(std::u16string text, glm::vec3 pos, glm::vec3 color, float scale = 1.0f, float rot = 0.0f);


private:
	struct TexChar {
		GLuint texID;
		glm::ivec2 Size;
		glm::ivec2 Bearing;
		unsigned int Advance;
	};

	TexChar LoadUTFChar(char16_t c);

	FT_Face mFontFace;
	std::map<char16_t, TexChar> mJapanTexChars;

	Shader* mTextShader;
	unsigned int mTextVertexArray;
	unsigned int mTextVertexBuffer;
};