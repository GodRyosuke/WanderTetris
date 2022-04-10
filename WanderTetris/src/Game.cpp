#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION

#include "Game.hpp"


Game::Game()
	:mWindowWidth(1024),
	mWindowHeight(576),
	mIsRunning(true)
{

}


bool Game::Initialize()
{

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Set OpenGL attributes
	// Use the core OpenGL profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Specify version 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Request a color buffer with 8-bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);


	mWindow = SDL_CreateWindow("Wander OpenGL Tutorial", 0, 20,
		mWindowWidth, mWindowHeight, SDL_WINDOW_OPENGL);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// Create an OpenGL context
	mContext = SDL_GL_CreateContext(mWindow);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}

	auto error_code = glGetError();



	void* extraDriverData = NULL;
	Common_Init(&extraDriverData);

	mAudioSystem = NULL;
	ERRCHECK(FMOD::Studio::System::create(&mAudioSystem));

	// The example Studio project is authored for 5.1 sound, so set up the system output mode to match
	FMOD::System* coreSystem = NULL;
	ERRCHECK(mAudioSystem->getCoreSystem(&coreSystem));
	ERRCHECK(coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0));

	ERRCHECK(mAudioSystem->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData));



	//CreateSpriteVerts();

	if (!LoadData())
	{
		SDL_Log("Failed to load data.");
		return false;
	}



	mTicksCount = SDL_GetTicks();

	return true;
}




bool Game::LoadData()
{
	glm::mat4 spriteViewProj = glm::mat4(1.0f);	// view projection matrix
	spriteViewProj[0][0] = 2.0f / (float)mWindowWidth;
	spriteViewProj[1][1] = 2.0f / (float)mWindowHeight;
	spriteViewProj[3][2] = 1.0f;

	{
		std::string vert_file = "./Shaders/Sprite.vert";
		std::string frag_file = "./Shaders/Sprite.frag";
		mSpriteShader = new Shader();
		if (!mSpriteShader->CreateShaderProgram(vert_file, frag_file)) {
			return false;
		}
	}
	{
		Shader* textShader = new Shader();
		std::string vert_file = "./Shaders/Text.vert";
		std::string frag_file = "./Shaders/Text.frag";
		if (!textShader->CreateShaderProgram(vert_file, frag_file)) {
			return false;
		}
		mFont = new Font(".\\resources\\arialuni.ttf", textShader, spriteViewProj);
	}

	mSpriteShader->UseProgram();
	mSpriteShader->SetMatrixUniform("uViewProj", spriteViewProj);

	{
		Texture* tex = new Texture("./resources/forest23.jpg");
		mBackGround = new Sprite(mSpriteShader, tex, spriteViewProj);
	}
	{
		Texture* tex = new Texture("./resources/back_wall.png");
		mBackWall = new Sprite(mSpriteShader, tex, spriteViewProj);
	}

	// Mesh読み込み

	// Load Audio Bank
	return true;
}


void Game::ProcessInput()
{
	SDL_Point mouse_position = { mWindowWidth / 2, mWindowHeight / 2 };
	SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
	mMousePos.x = mouse_position.x;
	mMousePos.y = mouse_position.y;

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:	// マウスのボタンが押されたら
		{
			//if (mPhase == PHASE_IDLE) {
			//	//mLastMousePos = mMousePos;
			//	//mSwipingDropPos = mMousePos / GRID_SIZE;
			//	mPhase = PHASE_MOVE;

			//	SDL_WarpMouseInWindow(mWindow, mWindowWidth / 2, mWindowHeight / 2);
			//	mMousePos.x = mWindowWidth / 2;
			//	mMousePos.y = mWindowHeight / 2;
			//	SDL_ShowCursor(SDL_DISABLE);
			//	std::cout << "----------------------------------------------called\n";
			//}
		}
		break;
		case SDL_MOUSEBUTTONUP:		// マウスを離したら
			//if (mPhase == PHASE_MOVE) {
			//	mPhase = PHASE_IDLE;

			//	/*if (EraseDrops()) {
			//		phase = PHASE_ERASE;
			//	}
			//	else {
			//		phase = PHASE_IDLE;
			//	}*/
			//	SDL_ShowCursor(SDL_ENABLE);
			//}
			break;
		}
	}

	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_ESCAPE] || keyState[SDL_SCANCODE_Q])	// escapeキーを押下すると終了
	{
		mIsRunning = false;
	}

	//if (keyState[SDL_SCANCODE_W]) {
	//	mCameraPos += (float)mMoveSpeed * mCameraOrientation;
	//}
	//if (keyState[SDL_SCANCODE_S]) {
	//	mCameraPos -= (float)mMoveSpeed * mCameraOrientation;
	//}
	//if (keyState[SDL_SCANCODE_A]) {
	//	mCameraPos -= (float)mMoveSpeed * glm::normalize(glm::cross(mCameraOrientation, mCameraUP));
	//}
	//if (keyState[SDL_SCANCODE_D]) {
	//	mCameraPos += (float)mMoveSpeed * glm::normalize(glm::cross(mCameraOrientation, mCameraUP));
	//}
}


void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	//mTitlePos += 0.07f;
	//mMeshes["StarWarsTitle"]->SetMeshPos(glm::vec3(0.0f, mTitlePos, 0.0f));

	//mTextParam += 0.0001f;
	//mTextPos = mTextParam * mTextDir + mTextPos;

	//mCubeRotation = deltaTime * mCubeRotateVel * 10;

	//last = clock();
	//ComputeWorldTransform();	// Cubeのtransformを計算

	//if (mPhase == PHASE_MOVE) {
	//	//printf("%d %d\n", mMousePos.x, mMousePos.y);

	//	float rotX = mMoveSensitivity * (float)((float)mMousePos.y - ((float)mWindowHeight / 2.0f)) / (float)mWindowHeight;
	//	float rotY = mMoveSensitivity * (float)((float)mMousePos.x - ((float)mWindowWidth / 2.0f)) / (float)mWindowWidth;
	//	printf("rotX: %f rotY: %f\t", rotX, rotY);
	//	// Calculates upcoming vertical change in the Orientation
	//	glm::vec3 newOrientation = glm::rotate(mCameraOrientation, glm::radians(-rotX), glm::normalize(glm::cross(mCameraOrientation, mCameraUP)));

	//	// Decides whether or not the next vertical Orientation is legal or not
	//	int rad = abs(glm::angle(newOrientation, mCameraUP) - glm::radians(90.0f));
	//	std::cout << rad * 180 / M_PI << std::endl;
	//	if (abs(glm::angle(newOrientation, mCameraUP) - glm::radians(90.0f)) <= glm::radians(85.0f))
	//	{
	//		mCameraOrientation = newOrientation;
	//	}

	//	// Rotates the Orientation left and right
	//	mCameraOrientation = glm::rotate(mCameraOrientation, glm::radians(-rotY), mCameraUP);

	//	if ((mMousePos.x != mWindowWidth / 2) || (mMousePos.y != mWindowHeight / 2)) {
	//		SDL_WarpMouseInWindow(mWindow, mWindowWidth / 2, mWindowHeight / 2);
	//	}
	//}

}

void Game::Draw()
{
	glClearColor(0.2f, 0.2f, 1.0f, 1.0f);
	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw mesh components
	// Enable depth buffering/disable alpha blend
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);


	// --- draw sprites ---
	glDisable(GL_DEPTH_TEST);
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);


	// 背景
	mBackGround->Draw(glm::vec2(0.0f),0.0f, (float)mWindowWidth / 1920.0f, 1.0f);

	// フィールド
	mBackWall->Draw(glm::vec2(0.0f), 0.0f,(float)(mWindowHeight * 0.9f) / (float) mBackWall->GetHeight(), 0.9f);

	//mFont->DrawUTF(u"あいうえおhello", glm::vec3(0.0f), glm::vec3(0.2f, 1.0f, 0.2f));


	glBindVertexArray(0);

	SDL_GL_SwapWindow(mWindow);
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		Draw();
	}
}

void Game::Shutdown()
{
	delete mFont;
	delete mSpriteShader;
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}