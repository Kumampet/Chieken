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

// ��ʃ��[�h
typedef enum {
	eScene_Title,    //�^�C�g�����
	eScene_Game,    //�Q�[�����
	eScene_Stage,  //�X�e�[�W�I�����
	eScene_Pause, //�|�[�Y���
	eScene_Gameover, // GameOver
	eScene_Gameclear, // Gameclear
} eScene;

// ��Փx�I��
typedef enum {
	Stage_1,//Stage1
	Stage_2,//Stage2
	Stage_3,//Stage3
	Stage_4,//Stage4

	eMenu_Num,        //�{���ڂ̐�
} eMenu;

// �^�C�g����ʃ��j���[
typedef enum {
	To_Game,//game
	To_Exit,//exit

	eTitle_Menu_Num,        //�{���ڂ̐�
} eTitle;

// �|�[�Y���j���[
typedef enum {
	Resume,//Resume
	Stage_ReSelect,//Stage_ReSelect
	Back_Title,//Back to Title

	ePause_Num,        //�{���ڂ̐�
} ePause;

static int Scene = eScene_Title;    //���݂̉��(�V�[��)
static int StageSelect = Stage_1;    //���݂̑I�����(������Stage1)
static int PauseSelect = Resume; //�|�[�Y���j���[�̏����I������
static int TitleSelect = To_Game; // �^�C�g����ʏ����I������
static 	int car_num;	// �X�e�[�W���̎Ԃ̐�

//�O���t�B�b�N�n���h���[
static int graph_title; //�^�C�g���w�i�C���[�W
static int graph_title_name; //�^�C�g������
static int jiki;// ���@�̃O���t�B�b�N�n���h��
static int graph_right_car;//�E������
static int graph_right_bus;//�E�����o�X
static int graph_right_truck;//�E�����g���b�N
static int graph_right_bike;//�E�����g���b�N
static int graph_left_car;//��������
static int graph_left_bus;//�������o�X
static int graph_left_truck;//�������g���b�N
static int graph_left_bike;//�������g���b�N
static int graph_start_1;
static int graph_start_2;
static int graph_quit_1;
static int graph_quit_2;
static int graph_stage_select_background;//�X�e�[�W�I����ʂ̔w�i
static int graph_stage1_selected;
static int graph_stage1_no_selected;
static int graph_stage2_selected;
static int graph_stage2_no_selected;
static int graph_stage3_selected;
static int graph_stage3_no_selected;
static int graph_stage4_selected;
static int graph_stage4_no_selected;
static int graph_stage1_explanation; //�X�e�[�W�̐���
static int graph_stage2_explanation; //�X�e�[�W�̐���
static int graph_stage3_explanation; //�X�e�[�W�̐���
static int graph_stage4_explanation; //�X�e�[�W�̐���
static int graph_road;//�X�e�[�W�̔w�i
static int graph_pause_background; //�|�[�Y��ʂ̔w�i
static int graph_resume_selected;
static int graph_stage_reselect_selected;
static int graph_back_to_title_selected;
static int graph_resume_no_selected;
static int graph_stage_reselect_no_selected;
static int graph_back_to_title_no_selected;
static int graph_clear_back;//�N���A���

//�T�E���h�n���h��
static int sound_title;//�^�C�g����ʂ�BGM
static int sound_stage;//�Q�[���v���C��ʂ�BGM
static int sound_clear;//�N���A�������̐_�O��
static int sound_pause;//�|�[�Y��ʎ��̃{�^����
static int sound_cursor;//�J�[�\����
static int sound_decition;//���艹
static int sound_walk;//���s��
static int sound_cry;//����

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
		FpsTime[0] = GetNowCount();               //1���ڂ̎��Ԏ擾
	if (FpsTime_i == 49) {
		FpsTime[1] = GetNowCount();               //50���ڂ̎��Ԏ擾
		Fps = 1000.0f / ((FpsTime[1] - FpsTime[0]) / 50.0f);//���肵���l����fps���v�Z
		FpsTime_i = 0;//�J�E���g��������
	}
	else
		FpsTime_i++;//���݉����ڂ��J�E���g
	if (Fps != 0)
		DrawFormatString(0, 0, GetColor(255, 255, 255), "FPS %.1f", Fps); //fps��\��
	return;
}

//�^�C�g�����
void Title() {
	if (CheckSoundMem(sound_clear == 1)) {// �N���A��ʂ�BGM���~�߂�
		StopSoundMem(sound_clear);
	}
	if (CheckSoundMem(sound_title) == 0) {//�^�C�g����ʂ�BGM
		PlaySoundMem(sound_title, DX_PLAYTYPE_LOOP, true);
	}
	DrawExtendGraph(0, 0, 640, 480, graph_title, TRUE);
	DrawExtendGraph(320, 240, 640, 480, graph_title_name, true);
	if (key[KEY_INPUT_DOWN] == 1) {//���L�[��������Ă�����
		PlaySoundMem(sound_cursor, DX_PLAYTYPE_BACK, true);
		TitleSelect = (TitleSelect + 1) % eTitle_Menu_Num;//�I����Ԃ��������
	}
	if (key[KEY_INPUT_UP] == 1) {//��L�[��������Ă�����
		PlaySoundMem(sound_cursor, DX_PLAYTYPE_BACK, true);
		TitleSelect = (TitleSelect + (eTitle_Menu_Num - 1)) % eTitle_Menu_Num;//�I����Ԃ���グ��
	}
	if (key[KEY_INPUT_RETURN] == 1) {//�G���^�[�L�[�������ꂽ��
		PlaySoundMem(sound_decition, DX_PLAYTYPE_BACK, true);
		switch (TitleSelect) {//���ݑI�𒆂̏�Ԃɂ���ď����𕪊�
		case To_Game://Stage1�I�𒆂Ȃ�
			TitleSelect = To_Game;
			Scene = eScene_Stage;//�V�[�����Q�[����ʂɕύX
			break;
		case To_Exit://Stage1�I�𒆂Ȃ�
			TitleSelect = To_Exit;
			exit_flag = true;
			break;
		}
	}

	switch (TitleSelect) {//���݂̑I����Ԃɏ]���ď����𕪊�
	case To_Game://�Q�[���I�𒆂Ȃ�
		TitleSelect = To_Game;
		DrawExtendGraph(100, 300, 250, 350, graph_start_2, true);
		DrawExtendGraph(100, 360, 250, 410, graph_quit_1, true);
		break;
	case To_Exit://�Q�[���I�𒆂Ȃ�
		TitleSelect = To_Exit;
		DrawExtendGraph(100, 300, 250, 350, graph_start_1, true);
		DrawExtendGraph(100, 360, 250, 410, graph_quit_2, true);
		break;
	}
}

// �X�e�[�W�Z���N�g���
void Stage_Select() {
	DrawExtendGraph(0, 0, 640, 480, graph_stage_select_background, true); //�w�i

	//���j���[�I������
	if (key[KEY_INPUT_DOWN] == 1) {//���L�[��������Ă�����
		PlaySoundMem(sound_cursor, DX_PLAYTYPE_BACK, true);
		StageSelect = (StageSelect + 1) % eMenu_Num;//�I����Ԃ��������
	}
	if (key[KEY_INPUT_UP] == 1) {//��L�[��������Ă�����
		PlaySoundMem(sound_cursor, DX_PLAYTYPE_BACK, true);
		StageSelect = (StageSelect + (eMenu_Num - 1)) % eMenu_Num;//�I����Ԃ���グ��
	}
	if (key[KEY_INPUT_RETURN] == 1) {//�G���^�[�L�[�������ꂽ��
		PlaySoundMem(sound_decition, DX_PLAYTYPE_BACK, true);
		switch (StageSelect) {//���ݑI�𒆂̏�Ԃɂ���ď����𕪊�
		case Stage_1://Stage1�I�𒆂Ȃ�
			StageSelect = Stage_1;
			Stage_Mng();
			Scene = eScene_Game;//�V�[�����Q�[����ʂɕύX
			break;
		case Stage_2://Stage1�I�𒆂Ȃ�
			StageSelect = Stage_2;
			Stage_Mng();
			Scene = eScene_Game;//�V�[�����Q�[����ʂɕύX
			break;
		case Stage_3://Stage1�I�𒆂Ȃ�
			StageSelect = Stage_3;
			Stage_Mng();
			Scene = eScene_Game;//�V�[�����Q�[����ʂɕύX
			break;
		case Stage_4://Stage1�I�𒆂Ȃ�
			StageSelect = Stage_4;
			Stage_Mng();
			Scene = eScene_Game;//�V�[�����Q�[����ʂɕύX
			break;
		}
	}

	//���j���[�`�揈��
	switch (StageSelect) {//���݂̑I����Ԃɏ]���ď����𕪊�
	case Stage_1://�Q�[���I�𒆂Ȃ�
		DrawExtendGraph(400, 180, 600, 230, graph_stage1_selected, true);
		DrawExtendGraph(400, 250, 600, 300, graph_stage2_no_selected, true);
		DrawExtendGraph(400, 320, 600, 370, graph_stage3_no_selected, true);
		DrawExtendGraph(400, 390, 600, 440, graph_stage4_no_selected, true);
		DrawExtendGraph(20, 230, 340, 350, graph_stage1_explanation, true);
		StageSelect = Stage_1;
		break;
	case Stage_2://�Q�[���I�𒆂Ȃ�
		DrawExtendGraph(400, 180, 600, 230, graph_stage1_no_selected, true);
		DrawExtendGraph(400, 250, 600, 300, graph_stage2_selected, true);
		DrawExtendGraph(400, 320, 600, 370, graph_stage3_no_selected, true);
		DrawExtendGraph(400, 390, 600, 440, graph_stage4_no_selected, true);
		DrawExtendGraph(20, 230, 340, 350, graph_stage2_explanation, true);
		StageSelect = Stage_2;
		break;
	case Stage_3://�Q�[���I�𒆂Ȃ�
		DrawExtendGraph(400, 180, 600, 230, graph_stage1_no_selected, true);
		DrawExtendGraph(400, 250, 600, 300, graph_stage2_no_selected, true);
		DrawExtendGraph(400, 320, 600, 370, graph_stage3_selected, true);
		DrawExtendGraph(400, 390, 600, 440, graph_stage4_no_selected, true);
		DrawExtendGraph(20, 230, 340, 350, graph_stage3_explanation, true);
		StageSelect = Stage_3;
		break;
	case Stage_4://�Q�[���I�𒆂Ȃ�
		DrawExtendGraph(400, 180, 600, 230, graph_stage1_no_selected, true);
		DrawExtendGraph(400, 250, 600, 300, graph_stage2_no_selected, true);
		DrawExtendGraph(400, 320, 600, 370, graph_stage3_no_selected, true);
		DrawExtendGraph(400, 390, 600, 440, graph_stage4_selected, true);
		DrawExtendGraph(20, 230, 340, 350, graph_stage4_explanation, true);
		StageSelect = Stage_4;
		break;

	}
}

//�Q�[���N���A���
void Game_Clear() {
	DrawExtendGraph(0, 0, 640, 480, graph_clear_back, true);
	if (CheckSoundMem(sound_walk) == 1) {//�j���g���̕��s�����~�߂�
		StopSoundMem(sound_walk);
	}
	if (CheckSoundMem(sound_stage == 1)) {// �Q�[����ʂ�BGM���~�߂�
		StopSoundMem(sound_stage);
	}
	if (CheckSoundMem(sound_clear) == 0) {//�N���A��ʂ�BGM
		PlaySoundMem(sound_cry, DX_PLAYTYPE_BACK, true);//����
		PlaySoundMem(sound_clear, DX_PLAYTYPE_BACK, true);//���肠BGM
	}
	if (key[KEY_INPUT_RETURN] == 1) {
		PlaySoundMem(sound_decition, DX_PLAYTYPE_BACK, true);
		Scene = eScene_Title;
	}
}

// �X�e�[�W�̔���ƃX�e�[�W���Ƃ̃p�����[�^�ݒ�
void Stage_Mng(){
	InitRand();
	int type = 0;
	if (StageSelect == Stage_1) { //�X�e�[�W����
		DrawString(300, 0, "Stage1", GetColor(255, 255, 255));//�f�o�b�O�p
		/*
			Stage1
			�Ԑ��̐��F�Q�Ԑ�
			�Ԃ̑䐔
				�E�s���F4��
				���s���F4��
			�����F60px/s
			�Ԃ̃^�C�v
				��p�Ԃ̂�

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
	else if (StageSelect == Stage_2) { //�X�e�[�W����
		DrawString(300, 0, "Stage2", GetColor(255, 255, 255));//�f�o�b�O�p
		/*
			Stage2
			�Ԑ��̐��F4�Ԑ�
			�Ԃ̑䐔
				�E�s���F7��
				���s���F7��
			�����F60px/s
			�Ԃ̃^�C�v
				��p�ԁA�g���b�N

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
	else if (StageSelect == Stage_3) { //�X�e�[�W����
		DrawString(300, 0, "Stage3", GetColor(255, 255, 255));//�f�o�b�O�p
		/*
			Stage3
			�Ԑ��̐��F8�Ԑ�
			�Ԃ̑䐔
				�E�s���F10��
				���s���F10��
			�����F120px/s
			�Ԃ̃^�C�v

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
	else if (StageSelect == Stage_4) { //�X�e�[�W����
		DrawString(300, 0, "Stage4", GetColor(255, 255, 255));//�f�o�b�O�p
		/*
			Stage4
			�Ԑ��̐��F10�Ԑ�
			�Ԃ̑䐔
				�E�s���F12��
				���s���F12��
			�����F70px/s
			�Ԃ̃^�C�v

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

// �����蔻�� �S�[��������܂�
int judge() {
	for (int i = 0; i < car_num; i++) { // ���ꂼ��̎Ԃɂ���
		if (car[i].get_move_X() < (ch.get_X() + 10) && (car[i].get_move_X() + 40) > (ch.get_X() - 10)) {// �������̊�����
			if (car[i].get_Y() < (ch.get_Y() + 10) && (car[i].get_Y() + 20) > (ch.get_Y() - 10)) {// �c�����̊�����
				//����𖞂����ď��߂Ċ����肪�o��
				return -1;
			}
		}
	}

	// �S�[������
	if (ch.get_Y() < 90) {
		Scene = eScene_Gameclear;
	}
	return 1;
}

//�Q�[�����
void Game() {
	//�Q�[����ʂ̏���
	int virtualY; //�ړ��͈͔���p�̉��z���W(�v�Z�p)
	DrawExtendGraph(0, 0, 640, 480, graph_road, true);//�Q�[���w�i�̕`��
	if (CheckSoundMem(sound_title == 1)) {// �^�C�g����ʂ�BGM���~�߂�
		StopSoundMem(sound_title);
	}
	if (CheckSoundMem(sound_stage) == 0) {//�Q�[����ʂ�BGM
		PlaySoundMem(sound_stage, DX_PLAYTYPE_LOOP, false);
	}

	GetHitKeyStateAll_2(key);//�L�[���̎擾
	
	/* �e��f�o�b�O���\��
	DrawString(100, 30, "���H�����f������I�I�I�I�I�I", GetColor(255, 255, 255));
	DrawLine(100, 60, 400, 60, GetColor(255, 0, 0));
	DrawFormatString(200, 100, GetColor(255, 255, 255), "X: %d ,Y: %d", ch.get_X(), ch.get_Y());//�`�L���̌����W

	DrawBox(0, 440, 640, 480, GetColor(0, 0, 255), TRUE);
	DrawString(320, 460, "START AREA", GetColor(255, 255, 255));
	DrawBox(0, 60, 640, 100, GetColor(255,0,0), TRUE);
	DrawString(320, 80, "GOAL AREA", GetColor(255, 255, 255));

	
	FpsTimeFanction();//FPS�̕\��
	*/

	// �Ԃ̕\��
	for (int i = 0; i < car_num; i++) {
		//DrawFormatString(500, 10 + (i * 20), GetColor(255, 255, 255), "CAR[%d]_X: %d", i, car[i].get_move_X());// CAR�̍��W
		if (0 == i % 2) {// �E����
			if (car[i].get_car_type() == 0) {//��p�Ԃ̏ꍇ
				//DrawBox(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, GetColor(0, 255, 0), TRUE);
				//DrawFormatString(car[i].get_move_X(), car[i].get_Y(), GetColor(255, 255, 255), "[%d]", i);
				DrawExtendGraph(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, graph_right_car, TRUE);
			}
			else if (car[i].get_car_type() == 1) {//�o�X�̏ꍇ
				//DrawBox(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, GetColor(0, 255, 255), TRUE);
				//DrawFormatString(car[i].get_move_X(), car[i].get_Y(), GetColor(255, 255, 255), "[%d]", i);
				DrawExtendGraph(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, graph_right_bus, TRUE);
			}
			else if (car[i].get_car_type() == 2) {//�g���b�N�̏ꍇ
				//DrawBox(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, GetColor(0, 0, 255), TRUE);
				//DrawFormatString(car[i].get_move_X(), car[i].get_Y(), GetColor(255, 255, 255), "[%d]", i);
				DrawExtendGraph(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, graph_right_truck, TRUE);
			}
			else if (car[i].get_car_type() == 3) {//�o�C�N�̏ꍇ
				//DrawBox(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, GetColor(255, 0, 0), TRUE);
				//DrawFormatString(car[i].get_move_X(), car[i].get_Y(), GetColor(255, 255, 255), "[%d]", i);
				DrawExtendGraph(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 30, car[i].get_Y() + 20, graph_right_bike, TRUE);
			}
			
			if (car[i].get_move_X() > 640) {// �Ԃ��G���A�O�ɏo���猳�̈ʒu�ɏ��������Ă�����
				car[i].resetX(-40);
			}
			else {
				car[i].set_move_X(car[i].get_car_speed()/60);
			}
		}
		else { // ������
			if (car[i].get_car_type() == 0) {//��p�Ԃ̏ꍇ
				//DrawBox(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, GetColor(0, 255, 0), TRUE);
				//DrawFormatString(car[i].get_move_X(), car[i].get_Y(), GetColor(255, 255, 255), "[%d]", i);
				DrawExtendGraph(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, graph_left_car, TRUE);
			}
			else if (car[i].get_car_type() == 1) {//�o�X�̏ꍇ
				//DrawBox(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, GetColor(0, 255, 255), TRUE);
				//DrawFormatString(car[i].get_move_X(), car[i].get_Y(), GetColor(255, 255, 255), "[%d]", i);
				DrawExtendGraph(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, graph_left_bus, TRUE);
			}
			else if (car[i].get_car_type() == 2) {//�g���b�N�̏ꍇ
				//DrawBox(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, GetColor(0, 0, 255), TRUE);
				//DrawFormatString(car[i].get_move_X(), car[i].get_Y(), GetColor(255, 255, 255), "[%d]", i);
				DrawExtendGraph(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, graph_left_truck, TRUE);
			}
			else if (car[i].get_car_type() == 3) {//�o�C�N�̏ꍇ
				//DrawBox(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 40, car[i].get_Y() + 20, GetColor(255, 0, 0), TRUE);
				//DrawFormatString(car[i].get_move_X(), car[i].get_Y(), GetColor(255, 255, 255), "[%d]", i);
				DrawExtendGraph(car[i].get_move_X(), car[i].get_Y(), car[i].get_move_X() + 30, car[i].get_Y() + 20, graph_left_bike, TRUE);
			}
			
			if (car[i].get_move_X() + 20 < 0) {// �Ԃ��G���A�O�ɏo���猳�̈ʒu�ɏ��������Ă�����
				car[i].resetX(640);
			}
			else {
				car[i].set_move_X(car[i].get_car_speed()/60);
			}
		}
	}


	
	//�L�[����
	//�`�L���̉����Y:100����Y:460�܂�
	//DrawCircle(ch.get_X(), ch.get_Y(), 10, GetColor(255, 255, 255));//���@
	int graph_X, graph_Y;
	GetGraphSize(jiki, &graph_X, &graph_Y);
	DrawExtendGraph(ch.get_X() - 15, ch.get_Y() - 15, ch.get_X() + 15, ch.get_Y() + 15,jiki, TRUE);
	
	if (((key[KEY_INPUT_DOWN] > 0) || (key[KEY_INPUT_S]) > 0)) {
		virtualY = ch.get_Y() + 2; //�����W��2�����ăG���A�O�ɂȂ�Ȃ���Ύ��ۂɍ��W�𓮂���
		if (virtualY < 460) {
			if (CheckSoundMem(sound_walk) == 0) {
				PlaySoundMem(sound_walk, DX_PLAYTYPE_BACK, true);
			}
			ch.set_Y(2);
		}
	}
	if ((key[KEY_INPUT_UP] > 0) || (key[KEY_INPUT_W] > 0)) {
		virtualY = ch.get_Y() - 2; //�����W��2�����ăG���A�O�ɂȂ�Ȃ���Ύ��ۂɍ��W�𓮂���
		if (virtualY > 80) {
			if (CheckSoundMem(sound_walk) == 0) {
				PlaySoundMem(sound_walk, DX_PLAYTYPE_BACK, true);
			}
			ch.set_Y(-2);
		}
	}

	// �����蔻��
	if (-1 == judge()) {// �ԂƓ���������20px�߂�
		ch.set_Y(20);
	}
}

// �|�[�Y���
void Pause() {
	if (CheckSoundMem(sound_stage == 1)) {// �Q�[����ʂ�BGM���~�߂�
		StopSoundMem(sound_stage);
	}
	DrawExtendGraph(0, 0, 640, 480, graph_pause_background, true); // �|�[�Y��ʂ̔w�i��\��
	if (key[KEY_INPUT_DOWN] == 1) {//���L�[��������Ă�����
		PlaySoundMem(sound_cursor, DX_PLAYTYPE_BACK, true);
		PauseSelect = (PauseSelect + 1) % ePause_Num;//�I����Ԃ��������
	}
	if (key[KEY_INPUT_UP] == 1) {//��L�[��������Ă�����
		PlaySoundMem(sound_cursor, DX_PLAYTYPE_BACK, true);
		PauseSelect = (PauseSelect + (ePause_Num - 1)) % ePause_Num;//�I����Ԃ���グ��
	}
	if (key[KEY_INPUT_RETURN] == 1) {//�G���^�[�L�[�������ꂽ��
		PlaySoundMem(sound_decition, DX_PLAYTYPE_BACK, true);
		switch (PauseSelect) {//���ݑI�𒆂̏�Ԃɂ���ď����𕪊�
		case Resume://Resume�I�𒆂Ȃ�
			PauseSelect = Resume;
			Scene = eScene_Game;//�V�[�����Q�[����ʂɕύX
			break;
		case Stage_ReSelect://Stage_Select�I�𒆂Ȃ�
			PauseSelect = Stage_ReSelect;
			Scene = eScene_Stage;//�V�[�����X�e�[�W�Z���N�g��ʂɕύX
			break;
		case Back_Title://Back_Title�I�𒆂Ȃ�
			PauseSelect = Back_Title;
			Scene = eScene_Title;//�V�[�����^�C�g����ʂɕύX
			break;
		}
	}

	switch (PauseSelect) {//���݂̑I����Ԃɏ]���ď����𕪊�
	case Resume://Resume�I�𒆂Ȃ�
		DrawExtendGraph(300, 180, 600, 240, graph_resume_selected, true);
		DrawExtendGraph(270, 260, 630, 380, graph_stage_reselect_no_selected, true);
		DrawExtendGraph(300, 400, 600, 440, graph_back_to_title_no_selected, true);
		PauseSelect = Resume;
		break;
	case Stage_ReSelect://Stage_ReSelect�I�𒆂Ȃ�
		DrawExtendGraph(300, 180, 600, 240, graph_resume_no_selected, true);
		DrawExtendGraph(270, 260, 630, 380, graph_stage_reselect_selected, true);
		DrawExtendGraph(300, 400, 600, 440, graph_back_to_title_no_selected, true);
		PauseSelect = Stage_ReSelect;
		break;
	case Back_Title://�Q�[���I�𒆂Ȃ�
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

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE); //���O���o�͂��Ȃ�
	SetMainWindowText("CHEKEN AND ACCROSSING!!");
	ChangeWindowMode(TRUE);//�E�B���h�E���[�h�ŋN������
	SetWindowSizeChangeEnableFlag(FALSE); //�E�B���h�E�T�C�Y�̉ς������Ȃ�
	SetAlwaysRunFlag(TRUE);
	SetMouseDispFlag(TRUE);
	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}
	SetDrawScreen(DX_SCREEN_BACK);
	int pauseFlag = -1;

	// �摜�̓ǂݍ���
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

	//�T�E���h�f�[�^�̓ǂݍ���
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
		ClearDrawScreen(); // ��ʂ̃N���A


		switch (Scene) {//���݂̃V�[���ɂ���������������
		case eScene_Title://���݂̃V�[�����^�C�g���Ȃ�
			Title();//�^�C�g��
			break;
		case eScene_Game://���݂̃V�[�����Q�[���Ȃ�
			if (key[KEY_INPUT_ESCAPE] == 1) {
				PlaySoundMem(sound_pause, DX_PLAYTYPE_BACK, true);
				Scene = eScene_Pause;
			}
			Game();//�Q�[�����
			break;
		case eScene_Stage://���݂̃V�[�����X�e�[�W�Z���N�g�Ȃ�
			Stage_Select();//�X�e�[�W�Z���N�g
			break;
		case eScene_Pause://�|�[�Y���
			if (key[KEY_INPUT_ESCAPE] == 1) {
				PlaySoundMem(sound_pause, DX_PLAYTYPE_BACK, true);
				Scene = eScene_Game;
			}
			Pause();
			break;
		case eScene_Gameclear: //�Q�[���N���A���
			Game_Clear();
			break;
		}

		//�v���Z�X�I��
		if (exit_flag == true) {
			break;
		}

		ScreenFlip();
		counter++;
	}
	//------------------------------------------------------

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}