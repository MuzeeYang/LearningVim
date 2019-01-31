#ifndef __LISTTRAIN_H__
#define __LISTTRAIN_H__

typedef struct _listTrain
{
	struct _listTrain* prev;
	struct _listTrain* next;
}TTrainNode, *PTrainNode;

typedef int (*LISTCMP_FUNC)(PTrainNode, PTrainNode);

#define DECLARE_HEAD_LIST(name) TTrainNode name = {&name, &name}

#define IS_EMPTY_LIST(pHead) ((pHead)->next == (pHead))
#define FOR_EACH(pHead, cursor) for(cursor = (pHead)->next; cursor != (pHead); cursor = cursor->next)
#define GET_MIDPTR_LIST(pHead, cursor) do{\
	PTrainNode _ptr;\
	for(cursor = (pHead)->next, _ptr = (pHead)->prev; _ptr != cursor && _ptr->prev != cursor; cursor = cursor->next, _ptr = _ptr->prev);\
}while(0)
#define GET_NUM_LIST(pHead, num) do{\
	PTrainNode _ptr;\
	num = 0;\
	FOR_EACH(pHead, _ptr)num++;\
}while(0) 

static inline void __initNode(PTrainNode pNode)
{
	pNode->prev = pNode;
	pNode->next = pNode;
}

static inline void __insertNode(PTrainNode front, PTrainNode pNode, PTrainNode back)
{
	front->next = pNode;
	back->prev = pNode;
	pNode->prev = front;
	pNode->next = back;
}

static inline void __deleteNode(PTrainNode front, PTrainNode back)
{
	front->next = back;
	back->prev = front;
}

static inline void __rotateNode(PTrainNode front, PTrainNode back)
{
	__deleteNode(front->prev, front->next);
	__insertNode(back, front, back->next);
}

static inline void __swapNode(PTrainNode front, PTrainNode pNode1, PTrainNode pNode2, PTrainNode back)
{
	__deleteNode(pNode1->prev, pNode1->next);
	__deleteNode(pNode2->prev, pNode2->next);
	__insertNode(front, pNode2, front->next);
	__insertNode(back->prev, pNode1, back);
}

static inline void initTrainNode(PTrainNode pNode)
{
	__initNode(pNode);
}

static inline void insertTrainNodeFront(PTrainNode entry, PTrainNode pNode)
{
	__insertNode(entry->prev, pNode, entry);
}

static inline void insertTrainNodeBack(PTrainNode entry, PTrainNode pNode)
{
	__insertNode(entry, pNode, entry->next);
}

static inline PTrainNode deleteTrainNode(PTrainNode pNode)
{
	__deleteNode(pNode->prev, pNode->next);
	__initNode(pNode);
	return pNode;
}

static inline PTrainNode replaceTrainNode(PTrainNode entry, PTrainNode pNode)
{
	__insertNode(entry->prev, pNode, entry->next);
	__initNode(entry);
	return entry;
}

static inline void rotateTrainNodeFront(PTrainNode pNode)
{
	__rotateNode(pNode->prev, pNode);
}

static inline void rotateTrainNodeBack(PTrainNode pNode)
{
	__rotateNode(pNode, pNode->next);
}

static inline void swapTrainNode(PTrainNode pNode1, PTrainNode pNode2)
{
	if(pNode1 == pNode2)return;
	__swapNode(pNode1->prev, pNode1, pNode2, pNode2->next);
}

//=======multiple nodes operation=======
static inline void __insertNodes(PTrainNode front, PTrainNode start, PTrainNode end, PTrainNode back)
{
	front->next = start;
	start->prev = front;
	back->prev = end;
	end->next = back;
}

static inline void cutTrainNodes(PTrainNode newhead, PTrainNode start, PTrainNode end)
{
	__deleteNode(start->prev, end->next);
	__insertNodes(newhead, start, end, newhead);
}

static inline void catTrainNodes(PTrainNode desthead, PTrainNode sourhead)
{
	if(IS_EMPTY_LIST(sourhead))return;
	__insertNodes(desthead->prev, sourhead->next, sourhead->prev, desthead);
	__initNode(sourhead);
}

void sortInsert(PTrainNode pHead, LISTCMP_FUNC cmpFunc);
void sortMerge(PTrainNode pHead, LISTCMP_FUNC cmpFunc);
void pushList(PTrainNode pHead, PTrainNode pNode);
PTrainNode popList(PTrainNode pHead);
PTrainNode shiftList(PTrainNode pHead);

#endif //TrainNode.h
