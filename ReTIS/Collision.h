#pragma once

class cObject {
protected:
	VECTOR	pos;						// ���W
	VECTOR	size;						// �I�u�W�F�N�g�̃T�C�Y
	int		type;						// �I�u�W�F�N�g�̃^�C�v
public:
	VECTOR	GetPos()  { return pos; }	// �ʒu�̃A�h���X��Ԃ�
	VECTOR	GetSize() { return size; }	// �I�u�W�F�N�g�̃T�C�Y��Ԃ�
	int		GetType() { return type; }
	virtual void HitAction(cObject *hit) = 0;			// �Փˌ�̏���
};

enum eObjectType {
	MapTile,
	MoveFloor,
	DropFloor,
	Character,
	Enemy,
	Player
};

extern void		CheckHitRectAndRect(cObject *obj1, cObject *obj2);
extern float	col_CheckRadian(VECTOR p1, VECTOR p2);
extern int		col_HitRadian(VECTOR p1, VECTOR p2, float rad);
extern int		col_HitRadianR(VECTOR p1, VECTOR p2);