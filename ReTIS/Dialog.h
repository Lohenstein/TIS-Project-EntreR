#pragma once

using namespace std;

extern void DebugMsgBox(TCHAR * tszParam, ...);

// �_�C�A���O�\���N���X
class cDialog {
public:
	// �ϐ�
	string	message;			// ���b�Z�[�W������S��
	string	out;				// �o�͂���Ă��镶����
	clock_t wait_time = 50;		// �I�t�Z�b�g�^�C��
	clock_t draw_time = 3000;	// �S������\�����Ă��������܂�
	clock_t start = 0, end = 0; // �I�t�Z�b�g�v���p�ϐ�
	int		str_count = 0;		// �������J�E���g
	bool	IsHide = false;		// �_�C�A���O�̕\��/��\��

	// �֐�
	void	Render();
	void	Update();
	void	SetMessageStr(string str) { message = str; }
	void	Hide(bool flag) { IsHide = flag; }

	// �R���X�g���N�^
	cDialog(string str) {
		SetMessageStr(str);
		start = clock();
	}
	~cDialog() {
		//delete this;
	}
};
