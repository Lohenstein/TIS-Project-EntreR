#pragma once

#define		TASK_MAX	(1024)

typedef struct {
	int		type;
	int		task[TASK_MAX];
	int		task[TASK_MAX];
}LOAD_HANDLER;

class cMultiLoader {
private:
	int		task[TASK_MAX];

public:
	int		MultiLoadGraph() {
		SetUseASyncLoadFlag(TRUE);
		SetUseASyncLoadFlag(FALSE);
	}
		;
	int		MultiLoadDivGraphconst(const TCHAR *FileName, int AllNum, int XNum, int YNum, int   XSize, int   YSize, int *HandleArray, int NotUse3DFlag = FALSE) {
		SetUseASyncLoadFlag(TRUE);
		LoadDivGraph(FileName, AllNum, XNum, YNum, XSize, YSize, HandleArray, NotUse3DFlag);
		SetUseASyncLoadFlag(FALSE);
	}
};
