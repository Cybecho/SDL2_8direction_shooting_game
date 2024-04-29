#include "GameClass.h"
#include "GameClassPlaying.h"
#include "Fire.h"

// sprite의 위치를 나타내는 열거형
enum Position {
	BOTTOM_LEFT = 0,
	BOTTOM_CENTER = 1,
	BOTTOM_RIGHT = 2,

	CENTER_LEFT = 3,
	CENTER_CENTER = 4,
	CENTER_RIGHT = 5,

	TOP_LEFT = 6,
	TOP_CENTER = 7,
	TOP_RIGHT = 8
};

std::vector<Fire> g_fires;

//~ 오디오
Mix_Chunk* reload_sound = nullptr;

//~ 폰트
SDL_Texture* text_score_;
SDL_Rect text_score_rect_;
TTF_Font* game_font_;

//~ 배경 이미지
SDL_Rect g_bg_source_rect;
SDL_Rect g_bg_destination_rect;
SDL_Texture* g_bg_texture;

//~ 플레이어 스프라이트 이미지
SDL_Texture* g_player_sheet_texture;
SDL_Rect g_player_source_rects[9];
SDL_Rect g_player_destination_rect;


int g_player_sprite_num;
int g_current_player_id;

int g_score; // 점수

int g_input; // 사용자의 키를 입력받는 변수
int g_maxFireCount; // 총알 최대 발사 수
int g_maxFireMagazine; // 총알 최대 장전 수

bool g_spaceKeyPressed; // 스페이스 키가 눌렸는지 여부

bool g_flag_running;

GameClassPlaying::GameClassPlaying() {
	m_flag_running = true;
	m_frame_per_sec = 30;
	m_maxFireCount = MAGAZINE_SIZE;

	cout << "***** Game Playing Phase Constructor Called *****" << endl;
}

GameClassPlaying::~GameClassPlaying() {
	ClearGame();
	ClearReloadSound();

	cout << "***** Game Playing Phase Destructor Called *****" << endl;
}

void GameClassPlaying::InitGame() {
	g_flag_running = true;
	game_font_ = TTF_OpenFont("../Resources/neodgm.ttf", 48);
	g_spaceKeyPressed = false;

	g_input = 0;
	g_score = 0;

	g_maxFireCount = MAX_FIRE_SIZE; // 총알 최대 발사 수
	g_maxFireMagazine = MAGAZINE_SIZE; // 총알 최대 장전 수

	//~ BG
	// 배경화면은 3가지 중 랜덤으로 선택됩니다
	srand(static_cast<unsigned int>(time(NULL))); // 랜덤 시드 초기화
	int randomNumber = rand() % 2; // 0, 1 중 랜덤한 숫자 생성
	string bgFilename = "../Resources/BG0" + to_string(randomNumber) + ".jpg"; // 파일 이름 생성
	SDL_Surface* bg_surface = IMG_Load(bgFilename.c_str());
	g_bg_texture = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	SDL_FreeSurface(bg_surface);

	g_bg_source_rect.x = 0;
	g_bg_source_rect.y = 0;
	g_bg_source_rect.w = WINDOW_WIDTH;
	g_bg_source_rect.h = WINDOW_HEIGHT;

	g_bg_destination_rect.x = 0;
	g_bg_destination_rect.y = 0;
	g_bg_destination_rect.w = WINDOW_WIDTH;
	g_bg_destination_rect.h = WINDOW_HEIGHT;



	// Player Character
	g_player_sprite_num = 3;
	g_current_player_id = CENTER_CENTER;

	SDL_Surface* player_sheet_surface = IMG_Load("../Resources/plane_9_direction.png");

	g_player_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, player_sheet_surface);
	SDL_FreeSurface(player_sheet_surface);
	// 중괄호로 묶은건 구조체에서 초기화한 변수에 차례대로 들어감

	g_player_source_rects[TOP_LEFT] = { 0  , 0, 200, 200 };
	g_player_source_rects[TOP_CENTER] = { 200, 0, 200, 200 };
	g_player_source_rects[TOP_RIGHT] = { 400, 0, 200, 200 };
	g_player_source_rects[CENTER_LEFT] = { 0  , 200, 200, 200 };
	g_player_source_rects[CENTER_CENTER] = { 200, 200, 200, 200 };
	g_player_source_rects[CENTER_RIGHT] = { 400, 200, 200, 200 };
	g_player_source_rects[BOTTOM_LEFT] = { 0  , 400, 200, 200 };
	g_player_source_rects[BOTTOM_CENTER] = { 200, 400, 200, 200 };
	g_player_source_rects[BOTTOM_RIGHT] = { 400, 400, 200, 200 };


	// 그려질 위치
	g_player_destination_rect.x = WINDOW_WIDTH / 2 - 100;
	g_player_destination_rect.y = WINDOW_HEIGHT / 2;
	g_player_destination_rect.w = g_player_source_rects[0].w; // 이미지의 너비
	g_player_destination_rect.h = g_player_source_rects[0].h;

	// 음악 설정
	// 중간과제에서는 인게임에서 음악 안쓰임
	// Set2Music();

	//~ init 게임에서 이미 삭제시켜주는데, 굳이 ClearGame에서 또?
	for (auto& fire : g_fires) {
		fire.destroyTexture();
		fire.Clear2Sound();
		fire.~Fire();
	}
	g_fires.clear();
}

void GameClassPlaying::HandleEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
			switch (event.key.keysym.sym) {
			case SDLK_UP:
				if (event.type == SDL_KEYDOWN) {
					g_input |= 1; // 상단 비트 설정
				}
				else {
					g_input &= ~1; // 상단 비트 해제
				}
				break;
			case SDLK_DOWN:
				if (event.type == SDL_KEYDOWN) {
					g_input |= 2; // 하단 비트 설정
				}
				else {
					g_input &= ~2; // 하단 비트 해제
				}
				break;
			case SDLK_LEFT:
				if (event.type == SDL_KEYDOWN) {
					g_input |= 4; // 좌측 비트 설정
					g_input &= ~8; // 우측 비트 해제
				}
				else {
					g_input &= ~4; // 좌측 비트 해제
					if (g_input & 8) { // 우측 비트가 설정되어 있으면
						g_input |= 8; // 우측 비트 설정 (좌측 해제 후 우측 유지)
					}
				}
				break;
			case SDLK_RIGHT:
				if (event.type == SDL_KEYDOWN) {
					g_input |= 8; // 우측 비트 설정
					g_input &= ~4; // 좌측 비트 해제
				}
				else {
					g_input &= ~8; // 우측 비트 해제
					if (g_input & 4) { // 좌측 비트가 설정되어 있으면
						g_input |= 4; // 좌측 비트 설정 (우측 해제 후 좌측 유지)
					}
				}
				break;
				//~ 재장전
			case SDLK_r:
				if (event.type == SDL_KEYDOWN) {
					g_maxFireMagazine = MAGAZINE_SIZE;
					cout << "Reloaded! MAGAZINE: " << g_maxFireMagazine << "/" << MAGAZINE_SIZE << endl;
					Set2ReloadSound();
					Mix_PlayChannel(-1, reload_sound, 0);
				}
				break;
			case SDLK_SPACE:
				if (event.type == SDL_KEYDOWN) {
					if (!g_spaceKeyPressed) { // 스페이스 키가 처음 눌렸을 때만 발사
						if ((g_fires.size() < g_maxFireCount) && (g_maxFireMagazine > 0)) {
							int directionX = 0, directionY = 0;
							switch (g_current_player_id) {
							case TOP_LEFT:      directionX = -1; directionY = -1; break;
							case TOP_CENTER:    directionX = 0; directionY = -1; break;
							case TOP_RIGHT:     directionX = 1; directionY = -1; break;
							case CENTER_LEFT:   directionX = -1; directionY = 0; break;
							case CENTER_CENTER:    directionX = 0; directionY = -1; break;
							case CENTER_RIGHT:  directionX = 1; directionY = 0; break;
							case BOTTOM_LEFT:   directionX = -1; directionY = 1; break;
							case BOTTOM_CENTER: directionX = 0; directionY = 1; break;
							case BOTTOM_RIGHT:  directionX = 1; directionY = 1; break;
							}
							g_fires.emplace_back(g_player_destination_rect.x + g_player_destination_rect.w / 2,
								g_player_destination_rect.y + g_player_destination_rect.h / 2,
								directionX, directionY, g_renderer);

							g_score += 10; // 점수 10점 증가
							if (g_maxFireMagazine > 0) { g_maxFireMagazine--; } // 장전 수가 음수가 되지 않도록 보정

							cout << "MAGAZINE : " << g_maxFireMagazine << "/" << MAGAZINE_SIZE << endl;
						}
						else {
							cout << "Maximum fire count reached! : " << g_maxFireMagazine << endl;
						}
					}
					g_spaceKeyPressed = true; // 스페이스 키가 눌렸음을 표시
				}
				else if (event.type == SDL_KEYUP) {
					g_spaceKeyPressed = false; // 스페이스 키가 떼어졌음을 표시
				}
				break;
			}
		}
		//~ 마우스 클릭 이벤트
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				int mouseX = event.button.x;
				int mouseY = event.button.y;

				if (g_maxFireMagazine == 0) {
					SDL_Color white = { 255, 255, 255, 255 };
					SDL_Surface* reload_surface = TTF_RenderUTF8_Blended(game_font_, "재충전", white);
					SDL_Rect reload_rect;
					reload_rect.x = (WINDOW_WIDTH - reload_surface->w) - 300;
					reload_rect.y = (WINDOW_HEIGHT - reload_surface->h) - 30;
					reload_rect.w = reload_surface->w;
					reload_rect.h = reload_surface->h;
					SDL_FreeSurface(reload_surface);

					if (mouseX >= reload_rect.x && mouseX <= reload_rect.x + reload_rect.w &&
						mouseY >= reload_rect.y && mouseY <= reload_rect.y + reload_rect.h) {
						g_maxFireMagazine = MAGAZINE_SIZE;
						cout << "Reloaded! MAGAZINE: " << g_maxFireMagazine << "/" << MAGAZINE_SIZE << endl;
						Set2ReloadSound();
						Mix_PlayChannel(-1, reload_sound, 0);
					}
				}
			}
			//~ 우클릭 누르면 게임 엔딩 화면으로 이동
			else if (event.button.button == SDL_BUTTON_RIGHT)
			{
				this->ClearGame();
				this->InitGame();
				Mix_HaltMusic();
				g_current_game_phase = PHASE_ENDING;
				Mix_PlayMusic(ending_music, -1);
			}
		}
	}
}

void GameClassPlaying::Update() {

	//! 캐릭터 이동
	const float speed = MOVE_SPEED; // 이동 속도
	float dx = 0, dy = 0;

	if (g_input & 1) dy -= speed; // 상단 이동
	if (g_input & 2) dy += speed; // 하단 이동
	if (g_input & 4) dx -= speed; // 좌측 이동
	if (g_input & 8) dx += speed; // 우측 이동

	//~ 대각선 이동 속도 조절
	if (dx != 0 && dy != 0) {
		dx *= 0.7071f; // sqrt(2)/2
		dy *= 0.7071f;
	}

	//~ 캐릭터 이미지 변경
	if ((g_input & 4) && (g_input & 1)) {
		g_current_player_id = TOP_LEFT; // 좌상단 이동
	}
	else if ((g_input & 4) && (g_input & 2)) {
		g_current_player_id = BOTTOM_LEFT; // 좌하단 이동
	}
	else if ((g_input & 8) && (g_input & 1)) {
		g_current_player_id = TOP_RIGHT; // 우상단 이동
	}
	else if ((g_input & 8) && (g_input & 2)) {
		g_current_player_id = BOTTOM_RIGHT; // 우하단 이동
	}
	else if (g_input & 4) {
		g_current_player_id = CENTER_LEFT; // 좌측 이동
	}
	else if (g_input & 8) {
		g_current_player_id = CENTER_RIGHT; // 우측 이동
	}
	else if (g_input & 1) {
		g_current_player_id = TOP_CENTER; // 상단 이동
	}
	else if (g_input & 2) {
		g_current_player_id = BOTTOM_CENTER; // 하단 이동
	}
	else {
		g_current_player_id = g_current_player_id; // 기본 이미지
	}

	g_player_destination_rect.x += static_cast<int>(dx);
	g_player_destination_rect.y += static_cast<int>(dy);

	//! 캐릭터 위치 경계 처리
	if (g_player_destination_rect.x < 0) {
		g_player_destination_rect.x = 0;
	}
	if (g_player_destination_rect.x > WINDOW_WIDTH - g_player_destination_rect.w) {
		g_player_destination_rect.x = WINDOW_WIDTH - g_player_destination_rect.w;
	}
	if (g_player_destination_rect.y < 0) {
		g_player_destination_rect.y = 0;
	}
	if (g_player_destination_rect.y > WINDOW_HEIGHT - g_player_destination_rect.h) {
		g_player_destination_rect.y = WINDOW_HEIGHT - g_player_destination_rect.h;
	}

	//! 총알 이동
	for (auto it = g_fires.begin(); it != g_fires.end();) {
		it->update();
		if (it->isOutOfScreen()) {
			it = g_fires.erase(it);
		}
		else {
			++it;
		}
	}
}

void GameClassPlaying::Render() {
	// Background
	SDL_RenderCopy(g_renderer, g_bg_texture, &g_bg_source_rect, &g_bg_destination_rect);

	// Fire
	for (const auto& fire : g_fires) {
		fire.render(g_renderer);
	}

	// Character
	SDL_Rect r = g_player_destination_rect;
	SDL_RenderCopy(g_renderer,
		g_player_sheet_texture,
		&g_player_source_rects[g_current_player_id],
		&r);

	// Score
	RenderScore();

	// Magazine
	RenderMagazine();

	// Reload Button
	RenderReloadButton();

	SDL_RenderPresent(g_renderer);
}

void GameClassPlaying::Set2ReloadSound() {
	if (!reload_sound) {
		reload_sound = Mix_LoadWAV("../Resources/Reload.mp3");
		if (!reload_sound) {
			printf("Failed to load reload sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		}
		else {
			Mix_VolumeChunk(reload_sound, 128);
		}
	}
}

void GameClassPlaying::RenderScore() {
	string score_str = "점수: " + string(5 - to_string(g_score).length(), '0') + to_string(g_score);
	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Surface* score_surface = TTF_RenderUTF8_Blended(game_font_, score_str.c_str(), white);
	text_score_ = SDL_CreateTextureFromSurface(g_renderer, score_surface);
	text_score_rect_.x = 40;
	text_score_rect_.y = 40;
	text_score_rect_.w = score_surface->w;
	text_score_rect_.h = score_surface->h;
	SDL_FreeSurface(score_surface);

	SDL_RenderCopy(g_renderer, text_score_, nullptr, &text_score_rect_);
	SDL_DestroyTexture(text_score_);
}

void GameClassPlaying::RenderMagazine() {
	string magazine_str = "총알: " + to_string(g_maxFireMagazine) + "/" + to_string(MAGAZINE_SIZE);
	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Surface* magazine_surface = TTF_RenderUTF8_Blended(game_font_, magazine_str.c_str(), white);
	SDL_Texture* text_magazine = SDL_CreateTextureFromSurface(g_renderer, magazine_surface);
	SDL_Rect text_magazine_rect;
	text_magazine_rect.x = WINDOW_WIDTH - magazine_surface->w - 40;
	text_magazine_rect.y = WINDOW_HEIGHT - magazine_surface->h - 40;
	text_magazine_rect.w = magazine_surface->w;
	text_magazine_rect.h = magazine_surface->h;
	SDL_FreeSurface(magazine_surface);

	SDL_RenderCopy(g_renderer, text_magazine, nullptr, &text_magazine_rect);
	SDL_DestroyTexture(text_magazine);
}

void GameClassPlaying::RenderReloadButton() {
	if (g_maxFireMagazine == 0) {
		SDL_Color white = { 255, 255, 255, 255 };
		SDL_Surface* reload_surface = TTF_RenderUTF8_Blended(game_font_, "재충전", white);
		SDL_Texture* reload_texture = SDL_CreateTextureFromSurface(g_renderer, reload_surface);
		SDL_Rect reload_rect;
		reload_rect.w = reload_surface->w + 20; // 박스의 너비를 글자 너비보다 20픽셀 크게 설정
		reload_rect.h = reload_surface->h + 20; // 박스의 높이를 글자 높이보다 20픽셀 크게 설정
		reload_rect.x = (WINDOW_WIDTH - reload_rect.w) - 300; // 박스의 x 좌표를 화면 중앙에 맞춤
		reload_rect.y = (WINDOW_HEIGHT - reload_rect.h) - 30; // 박스의 y 좌표를 화면 중앙에 맞춤

		// 주황색 배경 박스 그리기
		SDL_SetRenderDrawColor(g_renderer, 255, 165, 0, 255); // 주황색 설정 (RGB: 255, 165, 0)
		SDL_RenderFillRect(g_renderer, &reload_rect);

		// 글자 위치 조정
		SDL_Rect text_rect;
		text_rect.x = reload_rect.x + (reload_rect.w - reload_surface->w) / 2; // 글자의 x 좌표를 박스 내부 중앙으로 조정
		text_rect.y = reload_rect.y + (reload_rect.h - reload_surface->h) / 2; // 글자의 y 좌표를 박스 내부 중앙으로 조정
		text_rect.w = reload_surface->w;
		text_rect.h = reload_surface->h;

		// 글자 렌더링
		SDL_RenderCopy(g_renderer, reload_texture, nullptr, &text_rect);

		SDL_FreeSurface(reload_surface);
		SDL_DestroyTexture(reload_texture);
	}
}

void GameClassPlaying::ClearGame() {
	SDL_DestroyTexture(g_player_sheet_texture);
	SDL_DestroyTexture(g_bg_texture);

	//~ init 게임에서 이미 삭제시켜주는데, 굳이 ClearGame에서 또?
	for (auto& fire : g_fires) {
		fire.destroyTexture();
		fire.Clear2Sound();
	}
	g_fires.clear();

	TTF_CloseFont(game_font_);
}

void GameClassPlaying::ClearReloadSound()
{
	Mix_FreeChunk(reload_sound);
}