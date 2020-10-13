#include <iostream>
using namespace std;

//#壁 _空間 .ゴール oブロック p人
const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";
int gStageWidth = 8;
int gStageHeight = 5;
const char font[] = { ' ', '#', '.', 'o', 'O', 'p', 'P' }; //

#define OBJ_SPACE 0
#define OBJ_WALL 1
#define OBJ_GOAL 2
#define OBJ_BLOCK 3
#define OBJ_BLOCK_ON_GOAL 4
#define OBJ_MAN 5
#define OBJ_MAN_ON_GOAL 6
#define OBJ_UNKNOWN 7


//関数プロトタイプ
void initialize(int* state, int w, int h, const char* stageData);
void draw(const int* state, int w, int h);
void update(int* state, char input, int w, int h);
bool checkClear(const int* state, int w, int h);

int main() {
	//一次元配列である理由は本文参照
	int* state = new int[gStageWidth * gStageHeight]; //状態配列確保

	initialize(state, gStageWidth, gStageHeight, gStageData); //ステージ初期化
	//メインループ
	while (true) {
		//
		draw(state, gStageWidth, gStageHeight);
		//
		if (checkClear(state, gStageWidth, gStageHeight)) {
			break; //
		}
		//
		cout << "a:left s:right w:up z:down. command?" << endl; //操作説明
		char input;
		cin >> input;
		//
		update(state, input, gStageWidth, gStageHeight);
	}
	//
	cout << "Congratulation's! you won." << endl;
	//後始末
	delete[] state;
	state = 0;

	//Visual Studioから実行する人のために無限ループ。コマンドラインからはCtrl-Cで終えてください。
	while (true) {
		;
	}
	return 0;
}

//---------------------以下関数定義------------------------------------------


//いつか使う日も来るだろうと高さも渡す仕様にしたが、現状使っていないので名前だけ(height)コメントアウトしてある。
void initialize(int* state, int width, int /* height */, const char* stageData) {
	const char* d = stageData; //
	int x = 0;
	int y = 0;
	while (*d != '\0') { //
		int t; //
		switch (*d) {
		case '#': t = OBJ_WALL; break;
		case ' ': t = OBJ_SPACE; break;
		case 'o': t = OBJ_BLOCK; break;
		case 'O': t = OBJ_BLOCK_ON_GOAL; break;
		case '.': t = OBJ_GOAL; break;
		case 'p': t = OBJ_MAN; break;
		case 'P': t = OBJ_MAN_ON_GOAL; break;
		case '\n': x = 0; ++y; t = OBJ_UNKNOWN; break; //
		default: t = OBJ_UNKNOWN; break;
		}
		++d;
		if (t != OBJ_UNKNOWN) { //
			state[y * width + x] = t; //
			++x;
		}
	}
}

void draw(const int* state, int width, int height) {
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int o = state[y * width + x];
			cout << font[o];
		}
		cout << endl;
	}
}

//
//
void update(int* s, char input, int w, int h) {
	//
	int dx = 0;
	int dy = 0;
	switch (input) {
	case 'a': dx = -1; break; //
	case 's': dx = 1; break; //
	case 'w': dy = -1; break; //
	case 'z': dy = 1; break; //
	}
	//人座標を検索
	int i = -1;
	for (i = 0; i < w * h; ++i) {
		if (s[i] == OBJ_MAN) {
			break;
		}
		if ( s[i] == OBJ_MAN_ON_GOAL) {
			break;
		}
	}
	int x = i % w; //
	int y = i / w; //

	//
	//
	int tx = x + dx;
	int ty = y + dy;
	//
	if (tx < 0 || ty < 0 || tx >= w || ty >= h) {
		return;
	}
	//
	int p = y * w + x; //
	int tp = ty * w + tx; //
	if (s[tp] == OBJ_SPACE || s[tp] == OBJ_GOAL) {
		s[tp] = (s[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN; //
		s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE; //
	}
	else if (s[tp] == OBJ_BLOCK || s[tp] == OBJ_BLOCK_ON_GOAL) {
		//
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) { //
			return;
		}

		int tp2 = (ty + dy) * w + (tx + dx); //
		if (s[tp2] == OBJ_SPACE || s[tp2] == OBJ_GOAL) {
			//
			s[tp2] = (s[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			s[tp] = (s[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
		}
	}
}

//
bool checkClear(const int* s, int width, int height) {
	for (int i = 0; i < width * height; ++i) {
		if (s[i] == OBJ_BLOCK) {
			return false;
		}
	}
	return true;
}
