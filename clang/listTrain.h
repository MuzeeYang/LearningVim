#ifndef __LISTTRAIN_H__
#define __LISTTRAIN_H__

typedef struct listTrain
{
	struct listTrain* prev;
	struct listTrain* next;
}ListTrain, *pTrainNode, TrainHead, *pTrainHead;

#define CONTANOR_OF(addr, type, name) ((type*)((void*)addr - (unsigned char)(&((type*)0)->name)))
#define FOR_EACH(cursor, pHead) for(cursor = pHead->next; cursor != pHead; cursor = cursor->next)

static inline void __initNode(pTrainNode pNode)
{
	pNode->prev = NULL;
	pNode->next = NULL;
}

static inline void __insertNode(pTrainNode front, pTrainNode pNode, pTrainNode back)
{
	front->next = pNode;
	back->prev = pNode;
	pNode->prev = front;
	pNode->next = back;
}

static inline void __deleteNode(pTrainNode front, pTrainNode pNode, pTrainNode back)
{
	front->next = back;
	back->prev = front;
	__initNode(pNode);
}

static inline void initTrainNode(pTrainNode pNode)
{
	__initNode(pNode);
}

static inline void insertTrainNodeFront(pTrainNode entry, pTrainNode pNode)
{
	__insertNode(entry->prev, pNode, entry);
}

static inline void insertTrainNodeBack(pTrainNode entry, pTrainNode pNode)
{
	__insertNode(entry, pNode, entry->next);
}

static inline pTrainNode deleteTrainNode(pTrainNode pNode)
{
	__deleteNode(pNode->prev, pNode, pNode->next);
	return pNode;
}

#endif //TrainNode.h
