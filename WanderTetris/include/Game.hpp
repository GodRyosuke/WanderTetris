#include "SDL.h"
#include "glew.h"
#include <iostream>
#include <string>
#include "ft2build.h"
#include "Shader.hpp"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtx/vector_angle.hpp"
#include <map>
#include "Texture.hpp"
#include "Mesh.hpp"
#include "fmod.hpp"
#include "fmod_studio.hpp"
#include "FMOD/common.hpp"
#include "Font.hpp"
#include "Sprite.hpp"

#include FT_FREETYPE_H

class Game {
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

private:
	struct TexChar {
		GLuint texID;
		glm::ivec2 Size;
		glm::ivec2 Bearing;
		unsigned int Advance;
	};

	void ProcessInput();
	void UpdateGame();
	void Draw();
	bool LoadData();

	Font* mFont;

	bool mIsRunning;

	SDL_Window* mWindow;
	// OpenGL context
	SDL_GLContext mContext;
	int mWindowWidth;
	int mWindowHeight;

	const int mFieldWidth = 10;
	const int mFieldHeight = 20;

	Uint32 mTicksCount;
	std::map<char16_t, TexChar> mJapanTexChars;

	glm::ivec3 mMousePos;

	Sprite* mBackGround;
	Sprite* mBackWall;

	Shader* mSpriteShader;

	// ---- Sound Libraries ----
	FMOD::Studio::System* mAudioSystem;


};