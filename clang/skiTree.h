#ifndef __SKITREE_H__
#define __SKITREE_H__

typedef struct _rb_node{
	unsigned long rb_parent;
	struct _rb_node *rb_left;
	struct _rb_node *rb_right;
}__attribute__((aligned(sizeof(unsigned long)))) TTreeNode, *PTreeNode;

typedef struct _rb_root{
	PTreeNode root;
}TTreeRoot, *PTreeRoot;

typedef int (*TREECMP_FUNC)(PTreeNode, PTreeNode);

#define DECLARE_HEAD_TREE(name) TTreeNode name = {.root = NULL}

#ifndef CONTAINER_OF
#define CONTAINER_OF(addr, type, name) ((type*)((unsigned long)addr - (unsigned long)(&((type*)0)->name)))
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif


#define RB_RED (0)
#define RB_BLACK (1)
#define RB_LEFTCHILD (0)
#define RB_RIGHTCHILD (1)

#define GET_PARENT_TREE(node) ((PTreeNode)((node)->rb_parent & (~0x3)))
#define GET_COLOR_TREE(node) ((node)?(node)->rb_parent & 0x1:RB_BLACK)
#define GET_POSITION_TREE(node) (((node)->rb_parent >> 1) & 0x1)

#define SET_RED_COLOR_TREE(node)  ((node)?(node)->rb_parent &= (~0x1):0)
#define SET_BLACK_COLOR_TREE(node) ((node)?(node)->rb_parent |= 0x1:0)
#define SET_COLOR_TREE(node, color) ((color)?SET_BLACK_COLOR_TREE(node):SET_RED_COLOR_TREE(node))
#define SET_ROOT_TREE(node) ((node)->rb_parent &= 0x1)

#define IS_LEFT_CHILD_TREE(node) (!GET_POSITION_TREE(node))
#define IS_RIGHT_CHILD_TREE(node) (GET_POSITION_TREE(node))
#define IS_ROOT_TREE(node) (GET_PARENT_TREE(node) == NULL)

static inline void __init_rbnode(PTreeNode pNode)
{
	pNode->rb_parent = 0; //color is 0(red)
	pNode->rb_left = NULL;
	pNode->rb_right = NULL;
}

static inline void __add_rbnode(PTreeNode pParent, PTreeNode pNode, PTreeNode *ppLnk, uint8 pos)
{
	*ppLnk = pNode;
	if(!pNode)return;
	pNode->rb_parent = ((pNode->rb_parent & 0x1) | (pos << 1) | ((unsigned long)pParent & (~0x3)));
}

static inline void init_rbnode(PTreeNode pNode)
{
	__init_rbnode(pNode);
}

static inline void add_rbnode_left(PTreeNode pParent, PTreeNode pNode)
{
	if(pParent)
		__add_rbnode(pParent, pNode, &pParent->rb_left, RB_LEFTCHILD);
	else if(pNode)
		SET_ROOT_TREE(pNode);
}

static inline void add_rbnode_right(PTreeNode pParent, PTreeNode pNode)
{
	if(pParent)
		__add_rbnode(pParent, pNode, &pParent->rb_right, RB_RIGHTCHILD);
	else if(pNode)
		SET_ROOT_TREE(pNode);
}

static inline void __change_parent_rbnode(PTreeNode parent, PTreeNode pNode, PTreeNode child)
{
	if(IS_LEFT_CHILD_TREE(pNode))
		add_rbnode_left(parent, child);
	else
		add_rbnode_right(parent, child);
}

static inline PTreeNode __rotate_rbnode_left(PTreeNode pNode, PTreeNode child)
{
	__change_parent_rbnode(GET_PARENT_TREE(pNode), pNode, child);

	add_rbnode_right(pNode, child->rb_left);
	add_rbnode_left(child, pNode);
	return child;
}

static inline PTreeNode __rotate_rbnode_right(PTreeNode pNode, PTreeNode child)
{
	__change_parent_rbnode(GET_PARENT_TREE(pNode), pNode, child);

	add_rbnode_left(pNode, child->rb_right);
	add_rbnode_right(child, pNode);
	return child;
}

static inline PTreeNode rotate_rbnode_left(PTreeNode pNode)
{
	if(pNode->rb_right == NULL)return pNode;
	return __rotate_rbnode_left(pNode, pNode->rb_right);
}

static inline PTreeNode rotate_rbnode_right(PTreeNode pNode)
{
	if(pNode->rb_left == NULL)return pNode;
	return __rotate_rbnode_right(pNode, pNode->rb_left);
}

static inline void swap_parent_rbnode(PTreeNode parent1, PTreeNode pNode1, PTreeNode parent2, PTreeNode pNode2)
{
	if(IS_LEFT_CHILD_TREE(pNode1)){
		__change_parent_rbnode(parent2, pNode2, pNode1);
		add_rbnode_left(parent1, pNode2);
	}else{
		__change_parent_rbnode(parent2, pNode2, pNode1);
		add_rbnode_right(parent1, pNode2);
	}
}

static inline PTreeNode delete_rbnode(PTreeNode pNode)
{
	if(pNode->rb_left && pNode->rb_right)
		return NULL;

	if(pNode->rb_left)
		__change_parent_rbnode(GET_PARENT_TREE(pNode), pNode, pNode->rb_left);
	else if(pNode->rb_right)
		__change_parent_rbnode(GET_PARENT_TREE(pNode), pNode, pNode->rb_right);
	else
		__change_parent_rbnode(GET_PARENT_TREE(pNode), pNode, NULL);

	__init_rbnode(pNode);
	return pNode;
}

void insertRBLeaf(PTreeRoot, PTreeNode, TREECMP_FUNC);
void eraseRBLeaf(PTreeRoot, PTreeNode);
PTreeNode searchRBLeaf(PTreeRoot, PTreeNode, TREECMP_FUNC);

#endif
