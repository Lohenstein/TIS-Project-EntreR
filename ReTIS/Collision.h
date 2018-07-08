#pragma once

class cObject {
protected:
	VECTOR	pos;						// ���W
	VECTOR	old;						// �����p�ߋ����W
	VECTOR	size;						// �I�u�W�F�N�g�̃T�C�Y
	int		type;						// �I�u�W�F�N�g�̃^�C�v
public:
	VECTOR	GetPos()  { return pos; }	// �ʒu�̃A�h���X��Ԃ�
	VECTOR	GetSize() { return size; }	// �I�u�W�F�N�g�̃T�C�Y��Ԃ�
	VECTOR  GetOld()  { return old; }
	int		GetType() { return type; }
	void	SetPos(float x, float y) {pos = { x, y, 0.f };}
	virtual void HitAction(cObject *hit) = 0;			// �Փˌ�̏���
};

enum eObjectType {
	MapTile,		// �}�b�v
	MoveFloor,		// ������
	DropFloor,		// ������痎���鏰
	Character,		// �G���v���C���[ ���g��Ȃ�
	Enemy,			// �G
	Player,			// �v���C���[
	PlayerBullet,	// �v���C���[�L�������甭�˂����e�i���ڂ�e�j
	EnemyBullet,	// �G���甭�˂����e
	Bullet			// �e ���g��Ȃ�
};

extern void		CheckHitRectAndRect(cObject *obj1, cObject *obj2);
extern float	col_CheckRadian(VECTOR p1, VECTOR p2);
extern int		col_HitRadian(VECTOR p1, VECTOR p2, float rad);
extern int		col_HitRadianR(VECTOR p1, VECTOR p2);