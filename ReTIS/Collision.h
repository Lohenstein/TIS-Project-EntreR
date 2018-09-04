#pragma once

class cObject {
protected:
	VECTOR	pos;						// ���W
	VECTOR	old;						// �����p�ߋ����W
	VECTOR	size;						// �I�u�W�F�N�g�̃T�C�Y
	int		type;						// �I�u�W�F�N�g�̃^�C�v
	float	rad;
	short	angle;
	float	speed;
	short	count;
public:
	VECTOR	GetPos()  { return pos; }	// �ʒu�̃A�h���X��Ԃ�
	VECTOR	GetSize() { return size; }	// �I�u�W�F�N�g�̃T�C�Y��Ԃ�
	VECTOR  GetOld()  { return old; }
	float 	GetSpeed() { return speed; }
	int		GetType() { return type; }
	void	SetPos(float x, float y) {pos = { x, y, 0.f };}
	virtual void HitAction(cObject *hit) = 0;			// �Փˌ�̏���
	float GetRadrightbottom() { return cos(rad); }
	float GetRadleftbottom() {
		rad = d2r(angle * 2);
		return sin(rad);
	}
	float GetRadlefttop() {
		rad = d2r(angle * 3);
		return sin(rad);
	}
	float GetRadrighttop() {
		rad = d2r(angle * 4);
		return cos(rad);
	}
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
	PlayerAttack,	// �v���C���[�̍U���@bullet.Shot�Ŏg�p
	Bullet,			// �e ���g��Ȃ�
	Clear,			// �N���A����
	NormalCoin,		// �R�C��
	EneCoin,		// �G�l���M�[�R�C��
	RareCoin,		// ���A�R�C��
	TimeCoin,		// ���ԃR�C��
	WireAnchor,
	Spring,
	JugemBullet,	// �G�W���Q��
	Crumblewall,	// �����
	Gear,			// ����
	MoveWall,
	WireAnchorWire
};

extern void		CheckHitRectAndRect(cObject *obj1, cObject *obj2);
extern void		CheckHitRectAndCircle(cObject *obj1, cObject *obj2);
extern void		CheckHitRectAndRollingRect(cObject *obj1, cObject *obj2);
//extern void		CheckHitRollingRectAndCircle(cObject *obj1, cObject *obj2);
extern float	col_CheckRadian(VECTOR p1, VECTOR p2);
extern int		col_HitRadian(VECTOR p1, VECTOR p2, float rad);
extern int		col_HitRadianR(VECTOR p1, VECTOR p2);