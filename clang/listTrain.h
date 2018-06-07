#ifndef __LISTTRAIN_H__
#define __LISTTRAIN_H__

typedef struct listTrain
{
	struct listTrain* prev;
	struct listTrain* next;
}ListTrain, *PTrainNode, TrainHead, *PTrainHead;

#define CONTANOR_OF(addr, type, name) ((type*)((void*)addr - (unsigned char)(&((type*)0)->name)))
#define FOR_EACH(cursor, pHead) for(cursor = pHead->next; cursor != pHead; cursor = cursor->next)
#define DECLARE_HEAD(name) PTrainHead name = {&name, &name}

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

static inline void __deleteNode(PTrainNode front, PTrainNode pNode, PTrainNode back)
{
	front->next = back;
	back->prev = front;
	__initNode(pNode);
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

static inline pTrainNode deleteTrainNode(PTrainNode pNode)
{
	__deleteNode(pNode->prev, pNode, pNode->next);
	return pNode;
}

#endif //TrainNode.h
