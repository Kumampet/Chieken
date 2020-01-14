#include "DxLib.h"
#include "chicken.h"
#include "car.h"
#include <vector>
#include <time.h>
using namespace std;

void Stage_Mng();

inline void InitRand(){
	srand((unsigned int)time(NULL));
}

// 画面モード
typedef enum {
	eScene_Title,    //タイトル画面
	eScene_Game,    //ゲーム画面
	eScene_Stage,  //ステージ選択画面
	eScene_Pause, //ポーズ画面
	eScene_Gameover, // GameOver
	eScene_Gameclear, // Gameclear
} eScene;

// 難易度選択
typedef enum {
	Stage_1,//Stage1
	Stage_2,//Stage2
	Stage_3,//Stage3
	Stage_4,//Stage4

	eMenu_Num,        //本項目の数
} eMenu;

// タイトル画面メニュー
typedef enum {
	To_Game,//game
	To_Exit,//exit

	eTitle_Menu_Num,        //本項目の数
} eTitle;

// ポーズメニュー
typedef enum {
	Resume,//Resume
	Stage_ReSelect,//Stage_ReSelect
	Back_Title,//Back to Title

	ePause_Num,        //本項目の数
} ePause;

static int Scene = eScene_Title;    //現在の画面(シーン)
static int StageSelect = Stage_1;    //現在の選択状態(初期はStage1)
static int PauseSelect = Resume; //ポーズメニューの初期選択項目
static int TitleSelect = To_Game; // タイトル画面初期選択項目
static 	int car_num;	// ステージ中の車の数

//グラフィックハンドラー
static int graph_title; //タイトル背景イメージ
static int graph_title_name; //タイトル文字
static int jiki;// 自機のグラフィックハンドラ
static int graph_right_car;//右向き車
static int graph_right_bus;//右向きバス
static int graph_right_truck;//右向きトラック
static int graph_right_bike;//右向きトラック
static int graph_left_car;//左向き車
static int graph_left_bus;//左向きバス
static int graph_left_truck;//左向きトラック
static int graph_left_bike;//左向きトラック
static int graph_start_1;
static int graph_start_2;
static int graph_quit_1;
static int graph_quit_2;
static int graph_stage_select_background;//ステージ選択画面の背景
static int graph_stage1_selected;
static int graph_stage1_no_selected;
static int graph_stage2_selected;
static int graph_stage2_no_selected;
static int graph_stage3_selected;
static int graph_stage3_no_selected;
static int graph_stage4_selected;
static int graph_stage4_no_selected;
static int graph_stage1_explanation; //ステージの説明
static int graph_stage2_explanation; //ステージの説明
static int graph_stage3_explanation; //ステージの説明
static int graph_stage4_explanation; //ステージの説明
static int graph_road;//ステージの背景
static int graph_pause_background; //ポーズ画面の背景
static int graph_resume_selected;
static int graph_stage_reselect_selected;
static int graph_back_to_title_selected;
static int graph_resume_no_selected;
static int graph_stage_reselect_no_selected;
static int graph_back_to_title_no_selected;
static int graph_clear_back;//クリア画面

//サウンドハンドラ
static int sound_title;//タイトル画面のBGM
static int sound_stage;//ゲームプレイ画面のBGM
static int sound_clear;//クリアした時の神グル
static int sound_pause;//ポーズ画面時のボタン音
static int sound_cursor;//カーソル音
static int sound_decition;//決定音
static int sound_walk;//歩行音
static int sound_cry;//鳴き声

bool exit_flag = false;

int counter = 0, FpsTime[2] = { 0, }, FpsTime_i = 0;
double Fps = 0.0;
int key[256];
static chicken ch;
static 	Car* car;

int GetHitKeyStateAll_2(int KeyStateBuf[]) {
	char GetHitKeyStateAll_Key[256];
	GetHitKeyStateAll(GetHitKeyStateAll_Key);
	for (int i = 0; i < 256; i++) {
		if (GetHitKeyStateAll_Key[i] == 1) KeyStateBuf[i]++;
		else                            KeyStateBuf[i] = 0;
	}
	return 0;
}

void FpsTimeFanction() {
	if (FpsTime_i == 0)
		FpsTime[0] = GetNowCount();               //1周目の時間取得
	if (FpsTime_i == 49) {
		FpsTime[1] = GetNowCount();               //50周目の時間取得
		Fps = 1000.0f / ((FpsTime[1] - FpsTime[0]) / 50.0f);//測定した値からfpsを計算
		FpsTime_i = 0;//カウントを初期化
	}
	else
		FpsTime_i++;//現在何周目かカウント
	if (Fps != 0)
		DrawFormatString(0, 0, GetColor(255, 255, 255), "FPS %.1f", Fps); //fpsを表示
	return;
}

//タイトル画面
void Title() {
	if (CheckSoundMem(sound_clear == 1)) {// クリア画面のBGMを止める
		StopSoundMem(sound_clear);
	}
	if (CheckSoundMem(sound_title) == 0) {//タイトル画面のBGM
		PlaySoundMem(sound_title, DX_PLAYTYPE_LOOP, true);
	}
	DrawExtendGraph(0, 0, 640, 480, graph_title, TRUE);
	DrawExtendGraph(320, 240, 640, 480, graph_title_name, true);
	if (key[KEY_INPUT_DOWN] == 1) {//下キーが押されていたら
		PlaySoundMem(sound_cursor, DX_PLAYTYPE_BACK, true);
		TitleSelect = (TitleSelect + 1) % eTitle_Menu_Num;//選択状態を一つ下げる
	}
	if (key[KEY_INPUT_UP] == 1) {//上キーが押されていたら
		PlaySoundMem(sound_cursor, DX_PLAYTYPE_BACK, true);
		TitleSelect = (TitleSelect + (eTitle_Menu_Num - 1)) % eTitle_Menu_Num;//選択状態を一つ上げる
	}
	if (key[KEY_INPUT_RETURN] == 1) {//エンターキーが押されたら
		PlaySoundMem(sound_decition, DX_PLAYTYPE_BACK, true);
		switch (TitleSelect) {//現在選択中の状態によって処理を分岐
		case To_Game://Stage1選択中なら
			TitleSelect = To_Game;
			Scene = eScene_Stage;//シーンをゲーム画面に変更
			break;
		case To_Exit://Stage1選択中なら
			TitleSelect = To_Exit;
			exit_flag = true;
			break;
		}
	}

	switch (TitleSelect) {//現在の選択状態に従って処理を分岐
	case To_Game://ゲーム選択中なら
		TitleSelect = To_Game;
		DrawExtendGraph(100, 300, 250, 350, graph_start_2, true);
		DrawExtendGraph(100, 360, 250, 410, graph_quit_1, true);
		break;
	case To_Exit://ゲーム選択中なら
		TitleSelect = To_Exit;
		DrawExtendGraph(100, 300, 250, 350, graph_start_1, true);
		DrawExtendGraph(100, 360, 250, 410, graph_quit_2, true);
		break;
	}
}

// ステージセレクト画面
void Stage_Select() {
	DrawExtendGraph(0, 0, 640, 480, graph_stage_select_background, true); //背景

	//メニュー選択処理
	if (key[KEY_INPUT_DOWN] == 1) {//下キーが押されていたら
		PlaySoundMem(sound_cursor, DX_PLAYTYPE_BACK, true);
		StageSelect = (StageSelect + 1) % eMenu_Num;//選択状態を一つ下げる
	}
	if (key[KEY_INPUT_UP] == 1) {//上キーが押されていたら
		PlaySoundMem(sound_cursor, DX_PLAYTYPE_BACK, true);
		StageSelect = (StageSelect + (eMenu_Num - 1)) % eMenu_Num;//選択状態を一つ上げる
	}
	if (key[KEY_INPUT_RETURN] == 1) {//エンターキーが押されたら
		PlaySoundMem(sound_decition, DX_PLAYTYPE_BACK, true);
		switch (StageSelect) {//現在選択中の状態によって処理を分岐
		case Stage_1://Stage1選択中なら
			StageSelect = Stage_1;
			Stage_Mng();
			Scene = eScene_Game;//シーンをゲーム画面に変更
			break;
		case Stage_2://Stage1選択中なら
			StageSelect = Stage_2;
			Stage_Mng();
			Scene = eScene_Game;//シーンをゲーム画面に変更
			break;
		case Stage_3://Stage1選択中なら
			StageSelect = Stage_3;
			Stage_Mng();
			Scene = eScene_Game;//シーンをゲーム画面に変更
			break;
		case Stage_4://Stage1選択中なら
			StageSelect = Stage_4;
			Stage_Mng();
			Scene = eScene_Game;//シーンをゲーム画面に変更
			break;
		}
	}

	//メニュー描画処理
	switch (StageSelect) {//現在の選択状態に従って処理を分岐
	case Stage_1://ゲーム選択中なら
		DrawExtendGraph(400, 180, 600, 230, graph_stage1_selected, true);
		DrawExtendGraph(400, 250, 600, 300, graph_stage2_no_selected, true);
		DrawExtendGraph(400, 320, 600, 370, graph_stage3_no_selected, true);
		DrawExtendGraph(400, 390, 600, 440, graph_stage4_no_selected, true);
		DrawExtendGraph(20, 230, 340, 350, graph_stage1_explanation, true);
		StageSelect = Stage_1;
		break;
	case Stage_2://ゲーム選択中なら
		DrawExtendGraph(400, 180, 600, 230, graph_stage1_no_selected, true);
		DrawExtendGraph(400, 250, 600, 300, graph_stage2_selected, true);
		DrawExtendGraph(400, 320, 600, 370, graph_stage3_no_selected, true);
		DrawExtendGraph(400, 390, 600, 440, graph_stage4_no_selected, true);
		DrawExtendGraph(20, 230, 340, 350, graph_stage2_explanation, true);
		StageSelect = Stage_2;
		break;
	case Stage_3://ゲーム選択中なら
		DrawExtendGraph(400, 180, 600, 230, graph_stage1_no_selected, true);
		DrawExtendGraph(400, 250, 600, 300, graph_stage2_no_selected, true);
		DrawExtendGraph(400, 320, 600, 370, graph_stage3_selected, true);
		DrawExtendGraph(400, 390, 600, 440, graph_stage4_no_selected, true);
		DrawExtendGraph(20, 230, 340, 350, graph_stage3_explanation, true);
		StageSelect = Stage_3;
		break;
	case Stage_4://ゲーム選択中なら
		DrawExtendGraph(400, 180, 600, 230, graph_stage1_no_selected, true);
		DrawExtendGraph(400, 250, 600, 300, graph_stage2_no_selected, true);
		DrawExtendGraph(400, 320, 600, 370, graph_stage3_no_selected, true);
		DrawExtendGraph(400, 390, 600, 440, graph_stage4_selected, true);
		DrawExtendGraph(20, 230, 340, 350, graph_stage4_explanation, true);
		StageSelect = Stage_4;
		break;

	}
}

//ゲームクリア画面
void Game_Clear() {
	DrawExtendGraph(0, 0, 640, 480, graph_clear_back, true);
	if (CheckSoundMem(sound_walk) == 1) {//ニワトリの歩行音を止める
		StopSoundMem(sound_walk);
	}
	if (CheckSoundMem(sound_stage == 1)) {// ゲーム画面のBGMを止める
		StopSoundMem(sound_stage);
	}
	if (CheckSoundMem(sound_clear) == 0) {//クリア画面のBGM
		PlaySoundMem(sound_cry, DX_PLAYTYPE_BACK, true);//鳴き声
		PlaySoundMem(sound_clear, DX_PLAYTYPE_BACK, true);//くりあBGM
	}
	if (key[KEY_INPUT_RETURN] == 1) {
		PlaySoundMem(sound_decition, DX_PLAYTYPE_BACK, true);
		Scene = eScene_Title;
	}
}

// ステージの判定とステージごとのパラメータ設定
void Stage_Mng(){
	InitRand();
	int type = 0;
	if (StageSelect == Stage_1) { //ステージ判別
		DrawString(300, 0, "Stage1", GetColor(255, 255, 255));//デバッグ用
		/*
			Stage1
			車線の数：２車線
			車の台数
				右行き：4台
				左行き：4台
			速さ：60px/s
			車のタイプ
				乗用車のみ

		*/
		ch.Init();
		car_num = 8;
		car = new Car[car_num];
		for (int i = 0; i < car_num; i++) {
			//type = rand() % 0;
			car[i].set_car_num(i);
			car[i].set_car_type(rand()%4);
			if (0 == i % 2) {
				car[i].set_Y(200);
				car[i].set_move_X(-40 + 40 * i * 2);
				car[i].set_speed(60);
				car[i].set_car_type(type);
			}
			else {
				car[i].set_Y(350);
				car[i].set_move_X(640 - 40 * i * 2);
				car[i].set_speed(-60);
				car[i].set_car_type(type);
			}
		}
	}
	else if (StageSelect == Stage_2) { //ステージ判別
		DrawString(300, 0, "Stage2", GetColor(255, 255, 255));//デバッグ用
		/*
			Stage2
			車線の数：4車線
			車の台数
				右行き：7台
				左行き：7台
			速さ：60px/s
			車のタイプ
				乗用車、トラック

		*/
		ch.Init();
		car_num = 14;
		car = new Car[car_num];
		for (int i = 0; i < car_num; i++) {
			type = rand() % 2;
			car[i].set_car_num(i);
			car[i].set_car_type(rand() % 4);
			if (0 == i % 4) {
				car[i].set_Y(140);
				car[i].set_move_X(40 + 50 * i);
				car[i].set_speed(180);
				car[i].set_car_type(type);
			}
			else if(1 == i % 4){
				car[i].set_Y(180);
				car[i].set_move_X(640 + 40 * i);
				car[i].set_speed(-60);
				car[i].set_car_type(type);
			}
			else if (2 == i % 4) {
				car[i].set_Y(260);
				car[i].set_move_X(40 *i);
				car[i].set_speed(60);
				car[i].set_car_type(type);
			}
			else if (3 == i % 4) {
				car[i].set_Y(300);
				car[i].set_move_X(640 - 40 * i);
				car[i].set_speed(-120);
				car[i].set_car_type(type);
			}
		}
	}
	else if (StageSelect == Stage_3) { //ステージ判別
		DrawString(300, 0, "Stage3", GetColor(255, 255, 255));//デバッグ用
		/*
			Stage3
			車線の数：8車線
			車の台数
				右行き：10台
				左行き：10台
			速さ：120px/s
			車のタイプ

		*/
		ch.Init();
		car_num = 20;
		car = new Car[car_num];
		for (int i = 0; i < car_num; i++) {
			type = rand() % 4;
			car[i].set_car_num(i);
			if (0 == i % 8) {
				car[i].set_Y(140);
				car[i].set_move_X(-40 - 30 * i);
				car[i].set_speed(120);
				car[i].set_car_type(type);
			}
			else if (1 == i % 8) {
				car[i].set_Y(180);
				car[i].set_move_X(640 + 30 * i);
				car[i].set_speed(-120);
				car[i].set_car_type(type);
			}
			else if (2 == i % 8) {
				car[i].set_Y(220);
				car[i].set_move_X(-40 - 30 * i);
				car[i].set_speed(120);
				car[i].set_car_type(type);
			}
			else if (3 == i % 8) {
				car[i].set_Y(260);
				car[i].set_move_X(640 + 30 * i);
				car[i].set_speed(-120);
				car[i].set_car_type(type);
			}
			else if (4 == i % 8) {
				car[i].set_Y(300);
				car[i].set_move_X(-40 - 30 * i);
				car[i].set_speed(60);
				car[i].set_car_type(type);
			}
			else if (5 == i % 8) {
				car[i].set_Y(340);
				car[i].set_move_X(640 + 30 * i);
				car[i].set_speed(-120);
				car[i].set_car_type(type);
			}
			else if (6 == i % 8) {
				car[i].set_Y(380);
				car[i].set_move_X(-40 - 30 * i);
				car[i].set_speed(120);
				car[i].set_car_type(type);
			}
			else if (7 == i % 8) {
				car[i].set_Y(420);
				car[i].set_move_X(640 + 30 * i);
				car[i].set_speed(-120);
				car[i].set_car_type(type);
			}
		}
	}
	else if (StageSelect == Stage_4) { //ステージ判別
		DrawString(300, 0, "Stage4", GetColor(255, 255, 255));//デバッグ用
		/*
			Stage4
			車線の数：10車線
			車の台数
				右行き：12台
				左行き：12台
			速さ：70px/s
			車のタイプ

		*/
		ch.Init();
		car_num = 36;
		car = new Car[car_num];
		for (int i = 0; i < car_num; i++) {
			type = rand() % 4;
			car[i].set_car_num(i);
			if (0 == i % 10) {
				car[i].set_Y(175);
				car[i].set_move_X(-40 - 25 * i);
				car[i].set_speed(60);
				car[i].set_car_type(type);
			}
			else if (1 == i % 10) {
				car[i].set_Y(200);
				car[i].set_move_X(640 + 25 * i);
				car[i].set_speed(-120);
				car[i].set_car_type(type);
			}
			else if (2 == i % 10) {
				car[i].set_Y(225);
				car[i].set_move_X(-40 - 25 * i);
				car[i].set_speed(120);
				car[i].set_car_type(type);
			}
			else if (3 == i % 10) {
				car[i].set_Y(250);
				car[i].set_move_X(640 + 25 * i);
				car[i].set_speed(-120);
				car[i].set_car_type(type);
			}
			else if (4 == i % 10) {
				car[i].set_Y(275);
				car[i].set_move_X(-40 - 25 * i);
				car[i].set_speed(240);
				car[i].set_car_type(type);
			}
			else if (5 == i % 10) {
				car[i].set_Y(300);
				car[i].set_move_X(640 + 25 * i);
				car[i].set_speed(-180);
				car[i].set_car_type(type);
			}
			else if (6 == i % 10) {
				car[i].set_Y(325);
				car[i].set_move_X(-40 - 25 * i);
				car[i].set_speed(120);
				car[i].set_car_type(type);
			}
			else if (7 == i % 10) {
				car[i].set_Y(350);
				car[i].set_move_X(640 + 25 * i);
				car[i].set_speed(-120);
				car[i].set_car_type(type);
			}
			else if (8 == i % 10) {
				car[i].set_Y(375);
				car[i].set_move_X(-40 - 25 * i);
				car[i].set_speed(60);
				car[i].set_car_type(type);
			}
			else if (9 == i % 10) {
				car[i].set_Y(400);
				car[i].set_move_X(640 + 25 * i);
				car[i].set_speed(-60);
				car[i].set_car_type(type);
			}
		}
	}
}

// 当たり判定 ゴール判定も含む
int judge() {
	for (int i = 0; i < car_num; i++) { // それぞれの車について
		if (car[i].get_move_X() < (ch.get_X() + 10) && (car[i].get_move_X() + 40) > (ch.get_X() - 10)) {// 横方向の干渉判定
			if (car[i].get_Y() < (ch.get_Y() + 10) && (car[i].get_Y() + 20) > (ch.get_Y() - 10)) {// 縦方向の干渉判定
				//これを満たして初めて干渉判定が出る
				return -1;
			}
		}
	}

	// ゴール判定
	if (ch.get_Y() < 90) {
		Scene = eScene_Gameclear;
	}
	return 1;
}

//ゲーム画面
void Game() {
	//ゲーム画面の処理
	int virtualY; //移動範囲判定用の仮想座標(計算用)
	DrawExtendGraph(0, 0, 640, 480, graph_road, true);//ゲーム背景の描画
	if (CheckSoundMem(sound_title == 1)) {// タイトル画面のBGMを止める
		StopSoundMem(sound_title);
	}
	if (CheckSoundMem(sound_stage) == 0) {//ゲーム画面のBGM
		PlaySoundMem(sound_stage, DX_PLAYTYPE_LOOP, false);
	}

	GetHitKeyStateAll_2(key);//キー情報の取得
	
	/* 各種デバッグ情報表示
	DrawString(100, 30, "道路を横断させろ！！！！！！", GetColor(255, 255, 255));
	DrawLine(100, 60, 400, 60, GetColor(255, 0, 0));
	DrawFormatString(200, 100, GetColor(255, 255, 255), "X: %d ,Y: %d", ch.get_X(), ch.get_Y());//チキンの現座標

	DrawBox(0, 440, 640, 480, GetColor(0, 0, 255), TRUE);
	DrawString(320, 460, "START AREA", GetColor(255, 255, 255));
	DrawBox(0, 60, 640, 100, GetColor(255,0,0), TRUE);
	DrawString(320, 80, "GOAL AREA", GetColor(255, 255, 255));

	
	FpsTimeFanction();//FPSの表示
	*/

	// 車の表示
	for (int i = 0; i < car_num; i++) {
		//DrawFormatString(500, 10 + (i * 20), GetColor(255, 255, 255), "CAR[%d]_X: %d", i, car[i].get_move_X());// CARの座標
		if (0 == i % 2) {// 右方向
			if (car[i].get_car_type() == 0) {//乗用車の場合
				//DrawBox(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, GetColor(0, 255, 0), TRUE);
				//DrawFormatString(car[i].get_move_X(), car[i].get_Y(), GetColor(255, 255, 255), "[%d]", i);
				DrawExtendGraph(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, graph_right_car, TRUE);
			}
			else if (car[i].get_car_type() == 1) {//バスの場合
				//DrawBox(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, GetColor(0, 255, 255), TRUE);
				//DrawFormatString(car[i].get_move_X(), car[i].get_Y(), GetColor(255, 255, 255), "[%d]", i);
				DrawExtendGraph(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, graph_right_bus, TRUE);
			}
			else if (car[i].get_car_type() == 2) {//トラックの場合
				//DrawBox(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, GetColor(0, 0, 255), TRUE);
				//DrawFormatString(car[i].get_move_X(), car[i].get_Y(), GetColor(255, 255, 255), "[%d]", i);
				DrawExtendGraph(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, graph_right_truck, TRUE);
			}
			else if (car[i].get_car_type() == 3) {//バイクの場合
				//DrawBox(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, GetColor(255, 0, 0), TRUE);
				//DrawFormatString(car[i].get_move_X(), car[i].get_Y(), GetColor(255, 255, 255), "[%d]", i);
				DrawExtendGraph(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 30, car[i].get_Y() + 20, graph_right_bike, TRUE);
			}
			
			if (car[i].get_move_X() > 640) {// 車がエリア外に出たら元の位置に初期化してあげる
				car[i].resetX(-40);
			}
			else {
				car[i].set_move_X(car[i].get_car_speed()/60);
			}
		}
		else { // 左方向
			if (car[i].get_car_type() == 0) {//乗用車の場合
				//DrawBox(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, GetColor(0, 255, 0), TRUE);
				//DrawFormatString(car[i].get_move_X(), car[i].get_Y(), GetColor(255, 255, 255), "[%d]", i);
				DrawExtendGraph(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, graph_left_car, TRUE);
			}
			else if (car[i].get_car_type() == 1) {//バスの場合
				//DrawBox(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, GetColor(0, 255, 255), TRUE);
				//DrawFormatString(car[i].get_move_X(), car[i].get_Y(), GetColor(255, 255, 255), "[%d]", i);
				DrawExtendGraph(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, graph_left_bus, TRUE);
			}
			else if (car[i].get_car_type() == 2) {//トラックの場合
				//DrawBox(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, GetColor(0, 0, 255), TRUE);
				//DrawFormatString(car[i].get_move_X(), car[i].get_Y(), GetColor(255, 255, 255), "[%d]", i);
				DrawExtendGraph(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, graph_left_truck, TRUE);
			}
			else if (car[i].get_car_type() == 3) {//バイクの場合
				//DrawBox(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, GetColor(255, 0, 0), TRUE);
				//DrawFormatString(car[i].get_move_X(), car[i].get_Y(), GetColor(255, 255, 255), "[%d]", i);
				DrawExtendGraph(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 30, car[i].get_Y() + 20, graph_left_bike, TRUE);
			}
			
			if (car[i].get_move_X() + 20 < 0) {// 車がエリア外に出たら元の位置に初期化してあげる
				car[i].resetX(640);
			}
			else {
				car[i].set_move_X(car[i].get_car_speed()/60);
			}
		}
	}


	
	//キー操作
	//チキンの可動域はY:100からY:460まで
	//DrawCircle(ch.get_X(), ch.get_Y(), 10, GetColor(255, 255, 255));//自機
	int graph_X, graph_Y;
	GetGraphSize(jiki, &graph_X, &graph_Y);
	DrawExtendGraph(ch.get_X() - 15, ch.get_Y() - 15, ch.get_X() + 15, ch.get_Y() + 15,jiki, TRUE);
	
	if (((key[KEY_INPUT_DOWN] > 0) || (key[KEY_INPUT_S]) > 0)) {
		virtualY = ch.get_Y() + 2; //現座標に2足してエリア外にならなければ実際に座標を動かす
		if (virtualY < 460) {
			if (CheckSoundMem(sound_walk) == 0) {
				PlaySoundMem(sound_walk, DX_PLAYTYPE_BACK, true);
			}
			ch.set_Y(2);
		}
	}
	if ((key[KEY_INPUT_UP] > 0) || (key[KEY_INPUT_W] > 0)) {
		virtualY = ch.get_Y() - 2; //現座標に2引いてエリア外にならなければ実際に座標を動かす
		if (virtualY > 80) {
			if (CheckSoundMem(sound_walk) == 0) {
				PlaySoundMem(sound_walk, DX_PLAYTYPE_BACK, true);
			}
			ch.set_Y(-2);
		}
	}

	// 当たり判定
	if (-1 == judge()) {// 車と当たったら20px戻す
		ch.set_Y(20);
	}
}

// ポーズ画面
void Pause() {
	if (CheckSoundMem(sound_stage == 1)) {// ゲーム画面のBGMを止める
		StopSoundMem(sound_stage);
	}
	DrawExtendGraph(0, 0, 640, 480, graph_pause_background, true); // ポーズ画面の背景を表示
	if (key[KEY_INPUT_DOWN] == 1) {//下キーが押されていたら
		PlaySoundMem(sound_cursor, DX_PLAYTYPE_BACK, true);
		PauseSelect = (PauseSelect + 1) % ePause_Num;//選択状態を一つ下げる
	}
	if (key[KEY_INPUT_UP] == 1) {//上キーが押されていたら
		PlaySoundMem(sound_cursor, DX_PLAYTYPE_BACK, true);
		PauseSelect = (PauseSelect + (ePause_Num - 1)) % ePause_Num;//選択状態を一つ上げる
	}
	if (key[KEY_INPUT_RETURN] == 1) {//エンターキーが押されたら
		PlaySoundMem(sound_decition, DX_PLAYTYPE_BACK, true);
		switch (PauseSelect) {//現在選択中の状態によって処理を分岐
		case Resume://Resume選択中なら
			PauseSelect = Resume;
			Scene = eScene_Game;//シーンをゲーム画面に変更
			break;
		case Stage_ReSelect://Stage_Select選択中なら
			PauseSelect = Stage_ReSelect;
			Scene = eScene_Stage;//シーンをステージセレクト画面に変更
			break;
		case Back_Title://Back_Title選択中なら
			PauseSelect = Back_Title;
			Scene = eScene_Title;//シーンをタイトル画面に変更
			break;
		}
	}

	switch (PauseSelect) {//現在の選択状態に従って処理を分岐
	case Resume://Resume選択中なら
		DrawExtendGraph(300, 180, 600, 240, graph_resume_selected, true);
		DrawExtendGraph(270, 260, 630, 380, graph_stage_reselect_no_selected, true);
		DrawExtendGraph(300, 400, 600, 440, graph_back_to_title_no_selected, true);
		PauseSelect = Resume;
		break;
	case Stage_ReSelect://Stage_ReSelect選択中なら
		DrawExtendGraph(300, 180, 600, 240, graph_resume_no_selected, true);
		DrawExtendGraph(270, 260, 630, 380, graph_stage_reselect_selected, true);
		DrawExtendGraph(300, 400, 600, 440, graph_back_to_title_no_selected, true);
		PauseSelect = Stage_ReSelect;
		break;
	case Back_Title://ゲーム選択中なら
		DrawExtendGraph(300, 180, 600, 240, graph_resume_no_selected, true);
		DrawExtendGraph(270, 260, 630, 380, graph_stage_reselect_no_selected, true);
		DrawExtendGraph(300, 400, 600, 440, graph_back_to_title_selected, true);
		PauseSelect = Back_Title;
		break;
	}
}

void Init_Sound_Volume() {
	ChangeVolumeSoundMem(250, sound_title);
	ChangeVolumeSoundMem(250, sound_stage);
	ChangeVolumeSoundMem(200, sound_clear);
	ChangeVolumeSoundMem(180, sound_pause);
	ChangeVolumeSoundMem(180, sound_cursor);
	ChangeVolumeSoundMem(200, sound_decition);
	ChangeVolumeSoundMem(180, sound_cry);
}

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE); //ログを出力しない
	SetMainWindowText("CHEKEN AND ACCROSSING!!");
	ChangeWindowMode(TRUE);//ウィンドウモードで起動する
	SetWindowSizeChangeEnableFlag(FALSE); //ウィンドウサイズの可変を許可しない
	SetAlwaysRunFlag(TRUE);
	SetMouseDispFlag(TRUE);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	SetDrawScreen(DX_SCREEN_BACK);
	int pauseFlag = -1;

	// 画像の読み込み
	graph_title = LoadGraph("img/title_image.png");
	graph_title_name = LoadGraph("img/title_font.png");
	jiki = LoadGraph("img/jiki.png");
	graph_right_bus = LoadGraph("img/bus_r.png");
	graph_right_car = LoadGraph("img/car_r.png");
	graph_right_truck = LoadGraph("img/truck_r.png");
	graph_right_bike = LoadGraph("img/bike_r.png");
	graph_left_bus = LoadGraph("img/bus_l.png");
	graph_left_car = LoadGraph("img/car_l.png");
	graph_left_truck = LoadGraph("img/truck_l.png");
	graph_left_bike = LoadGraph("img/bike_l.png");
	graph_start_1 = LoadGraph("img/start_1.png");
	graph_quit_1 = LoadGraph("img/quit_1.png");
	graph_start_2 = LoadGraph("img/start_2.png");
	graph_quit_2 = LoadGraph("img/quit_2.png");
	graph_stage_select_background = LoadGraph("img/stage_select_back.png");
	graph_stage1_selected = LoadGraph("img/stage1_selected.png");
	graph_stage1_no_selected = LoadGraph("img/stage1_no_selected.png");
	graph_stage2_selected = LoadGraph("img/stage2_selected.png");
	graph_stage2_no_selected = LoadGraph("img/stage2_no_selected.png");
	graph_stage3_selected = LoadGraph("img/stage3_selected.png");
	graph_stage3_no_selected = LoadGraph("img/stage3_no_selected.png");
	graph_stage4_selected = LoadGraph("img/stage4_selected.png");
	graph_stage4_no_selected = LoadGraph("img/stage4_no_selected.png");
	graph_stage1_explanation = LoadGraph("img/stage1_explanation.png");
	graph_stage2_explanation = LoadGraph("img/stage2_explanation.png");
	graph_stage3_explanation = LoadGraph("img/stage3_explanation.png");
	graph_stage4_explanation = LoadGraph("img/stage4_explanation.png");
	graph_road = LoadGraph("img/road.png");
	graph_pause_background = LoadGraph("img/pause_back.png");
	graph_resume_selected = LoadGraph("img/resume_selected.png");
	graph_stage_reselect_selected = LoadGraph("img/stage_reselect_selected.png");
	graph_back_to_title_selected = LoadGraph("img/back_to_title_selected.png");
	graph_resume_no_selected = LoadGraph("img/resume_no_selected.png");
	graph_stage_reselect_no_selected = LoadGraph("img/stage_reselect_no_selected.png");
	graph_back_to_title_no_selected = LoadGraph("img/back_to_title_no_selected.png");
	graph_clear_back = LoadGraph("img/clear.png");

	//サウンドデータの読み込み
	sound_title = LoadSoundMem("sound/title.ogg");
	sound_stage = LoadSoundMem("sound/stage.mp3");
	sound_clear = LoadSoundMem("sound/clear.mp3");
	sound_pause = LoadSoundMem("sound/pause.mp3");
	sound_cursor = LoadSoundMem("sound/cursor.mp3");
	sound_decition = LoadSoundMem("sound/decision.ogg");
	sound_walk = LoadSoundMem("sound/walk.mp3");
	sound_cry = LoadSoundMem("sound/cry.mp3");

	Init_Sound_Volume();
	//------------------------------------------------------
	while (0 == ProcessMessage()) {
		GetHitKeyStateAll_2(key);
		ClearDrawScreen(); // 画面のクリア


		switch (Scene) {//現在のシーンにあった処理をする
		case eScene_Title://現在のシーンがタイトルなら
			Title();//タイトル
			break;
		case eScene_Game://現在のシーンがゲームなら
			if (key[KEY_INPUT_ESCAPE] == 1) {
				PlaySoundMem(sound_pause, DX_PLAYTYPE_BACK, true);
				Scene = eScene_Pause;
			}
			Game();//ゲーム画面
			break;
		case eScene_Stage://現在のシーンがステージセレクトなら
			Stage_Select();//ステージセレクト
			break;
		case eScene_Pause://ポーズ画面
			if (key[KEY_INPUT_ESCAPE] == 1) {
				PlaySoundMem(sound_pause, DX_PLAYTYPE_BACK, true);
				Scene = eScene_Game;
			}
			Pause();
			break;
		case eScene_Gameclear: //ゲームクリア画面
			Game_Clear();
			break;
		}

		//プロセス終了
		if (exit_flag == true) {
			break;
		}

		ScreenFlip();
		counter++;
	}
	//------------------------------------------------------

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}