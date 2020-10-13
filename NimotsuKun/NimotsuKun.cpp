#include <iostream>
using namespace std;

//#壁 _空間 .ゴール oブロック p人
const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";
const int gStageWidth = 8;
const int gStageHeight = 5;

enum OBJECT {
	 OBJ_SPACE,
	 OBJ_WALL,
	 OBJ_GOAL,
	 OBJ_BLOCK,
	 OBJ_BLOCK_ON_GOAL,
	 OBJ_MAN,
	 OBJ_MAN_ON_GOAL,
	 OBJ_UNKNOWN 
};

/*
#define OBJ_SPACE 0
#define OBJ_WALL 1
#define OBJ_GOAL 2
#define OBJ_BLOCK 3
#define OBJ_BLOCK_ON_GOAL 4
#define OBJ_MAN 5
#define OBJ_MAN_ON_GOAL 6
#define OBJ_UNKNOWN 7
*/


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
		//ステージの現在の状況を表示する
		draw(state, gStageWidth, gStageHeight);
		//荷物がゴールに到達しているか確認する
		if (checkClear(state, gStageWidth, gStageHeight)) {
			break; //到達していたらループを抜ける
		}
		//動かす方向を入力させる
		cout << "a:left s:right w:up z:down. command?" << endl; //操作説明
		char input;
		cin >> input;
		//オブジェクトを動かされたところにアップデートする
		update(state, input, gStageWidth, gStageHeight);
	}
	//クリア表示
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
	const char* d = stageData; //ステージの形状をｄに渡している
	int x = 0;
	int y = 0;
	while (*d != '\0') { //ステージの形状が終端にたどりつかない限りオブジェクトを配置し続ける
		OBJECT t; //オブジェクトの種類を入れておく変数
		switch (*d) {
		case '#': t = OBJ_WALL; break;
		case ' ': t = OBJ_SPACE; break;
		case 'o': t = OBJ_BLOCK; break;
		case 'O': t = OBJ_BLOCK_ON_GOAL; break;
		case '.': t = OBJ_GOAL; break;
		case 'p': t = OBJ_MAN; break;
		case 'P': t = OBJ_MAN_ON_GOAL; break;
		case '\n': x = 0; ++y; t = OBJ_UNKNOWN; break; //￥ｎまでたどり着いたら次の行へ行く
		default: t = OBJ_UNKNOWN; break;
		}
		++d;
		if (t != OBJ_UNKNOWN) { //ｔが例外でなければ
			state[y * width + x] = t; //ステージの指定した位置に指定したオブジェクトを配置する
			++x;
		}
	}
}

void draw(const int* state, int width, int height) {
	const char font[] = { ' ', '#', '.', 'o', 'O', 'p', 'P' }; //表示するものを配列にまとめている
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int o = state[y * width + x];
			cout << font[o];
		}
		cout << endl;
	}
}

//
//オブジェクトの移動処理
void update(int* s, char input, int w, int h) {
	//動く方向を指定する変数を用意する
	int dx = 0;
	int dy = 0;
	switch (input) {
	case 'a': dx = -1; break; //aが入力されていたらｘ軸にー１
	case 's': dx = 1; break; //ｓが入力されていたらx軸に１
	case 'w': dy = -1; break; //ｗが入力されていたらy軸にー１
	case 'z': dy = 1; break; //ｚが入力されていたらy軸に１
	}
	//人座標を検索
	int i = -1;
	for (i = 0; i < w * h; ++i) {
		if (s[i] == OBJ_MAN	|| s[i] == OBJ_MAN_ON_GOAL) {
			break;
		}
	}
	int x = i % w; //プレイヤーの現在のｘ座標の位置
	int y = i / w; //プレイヤーの現在のｙ座標の位置

	//ｔｘに動いた後のプレイヤーのｘ座標の位置を保存
	//ｔｙに動いた後のぷれいやーのｙ座標の位置を保存
	int tx = x + dx;
	int ty = y + dy;
	//動いた後オブジェクトがフィールドの範囲外に出ていないかのチェック
	if (tx < 0 || ty < 0 || tx >= w || ty >= h) {
		return;
	}
	//プレイヤーの移動した後の他のオブジェクトの移動処理
	int p = y * w + x; //動く前のプレイヤーの位置
	int tp = ty * w + tx; //動いた後のプレイヤーの位置
	if (s[tp] == OBJ_SPACE || s[tp] == OBJ_GOAL) {
		s[tp] = (s[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN; //移動後のプレイヤーのいる場所の状態
		s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE; //移動前にプレイヤーがいた場所の状態
	}
	else if (s[tp] == OBJ_BLOCK || s[tp] == OBJ_BLOCK_ON_GOAL) {
		//プレイヤーの移動先がブロックだった場合の処理
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) { //ブロックが範囲外に移動してしまう場合の処理
			return;
		}

		int tp2 = (ty + dy) * w + (tx + dx); //プレイヤーの移動先にあったブロックの移動処理
		if (s[tp2] == OBJ_SPACE || s[tp2] == OBJ_GOAL) {
			//ブロックが移動可能だった場合の処理
			s[tp2] = (s[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			s[tp] = (s[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
		}
	}
}

//クリアかどうかを判定する
bool checkClear(const int* s, int width, int height) {
	for (int i = 0; i < width * height; ++i) {
		if (s[i] == OBJ_BLOCK) {
			return false;
		}
	}
	return true;
}
