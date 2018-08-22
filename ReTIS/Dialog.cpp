
#include "Main.h"

void DebugMsgBox(TCHAR * tszParam, ...)
{
	TCHAR tszStr[1024];
	va_list list;

	va_start(list, tszParam);
	_vstprintf_s(tszStr, 1024, tszParam, list);
	va_end(list);

	MessageBox(GetForegroundWindow(), tszStr, _TEXT("DEBUG"), MB_OK);
}

void	cDialog::Update() {

	if (str_count >= (int)message.size()) {
		if (end - start > draw_time) {
			// �S�����`�悵���������
			IsHide = true;
			start = 0, end = 0;	// �`��I�t�Z�b�g������
			str_count = 0;		// ������J�E���g������
			message = "";		// �`�敶���񏉊���
			out = "";
		}
		end = clock();
	}
	else if (!IsHide) {
		// �ꕶ�����ǉ����Ă���
		if (end - start > wait_time) {
			out += message[str_count];
			str_count++;
			start = end;
		}
		end = clock();
	}
}

void	cDialog::Render() {

	if (!IsHide) {
		// �e�L�X�g�{�b�N�X
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawBox(20, 500, 1260, 700, 0xFFFFFF, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// ���b�Z�[�W
		DrawFormatStringToHandle(40, 520, 0xFFFFFF, font_handle[FONT_MESSAGE], out.c_str());
	}
}