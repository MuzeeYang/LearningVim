#include "skiTree.h"

static void __insert_adjust(PTreeRoot pRoot, PTreeNode pNode)
{
	PTreeNode parent, gparent, uncle;
	while(!IS_ROOT_TREE(pNode)){
		//parent is black or node is root
		if(GET_COLOR_TREE(GET_PARENT_TREE(pNode)))
			break;

		parent = GET_PARENT_TREE(pNode);
		gparent = GET_PARENT_TREE(parent);
		if(IS_LEFT_CHILD_TREE(parent)){
			uncle = gparent->rb_right;
			//case 1: uncle is red
			if(GET_COLOR_TREE(uncle) == RB_RED){
				SET_BLACK_COLOR_TREE(parent);
				SET_BLACK_COLOR_TREE(uncle);
				SET_RED_COLOR_TREE(gparent);
				pNode = gparent;
				continue;
			}

			//case 2: node is right child, uncle is black
			if(IS_RIGHT_CHILD_TREE(pNode)){
				rotate_rbnode_left(parent);
				pNode = parent;
				parent = GET_PARENT_TREE(pNode);
			}

			//case 3: node is left child, uncle is black
			if(IS_LEFT_CHILD_TREE(pNode)){
				SET_BLACK_COLOR_TREE(parent);
				SET_RED_COLOR_TREE(gparent);
				pNode = rotate_rbnode_right(gparent);
				break;
			}
		}else{
			uncle = gparent->rb_left;
			//case 1: uncle is red
			if(GET_COLOR_TREE(uncle) == RB_RED){
				SET_BLACK_COLOR_TREE(parent);
				SET_BLACK_COLOR_TREE(uncle);
				SET_RED_COLOR_TREE(gparent);
				pNode = gparent;
				continue;
			}

			//case 2: node is left child, uncle is black
			if(IS_LEFT_CHILD_TREE(pNode)){
				rotate_rbnode_right(parent);
				pNode = parent;
				parent = GET_PARENT_TREE(pNode);
			}

			//case 3: node is left child, uncle is black
			if(IS_RIGHT_CHILD_TREE(pNode)){
				SET_BLACK_COLOR_TREE(parent);
				SET_RED_COLOR_TREE(gparent);
				pNode = rotate_rbnode_left(gparent);
				break;
			}
		}
	}

	if(IS_ROOT_TREE(pNode)){
		SET_BLACK_COLOR_TREE(pNode);
		pRoot->root = pNode;
	}
}

static void __erase_adjust(PTreeRoot pRoot, PTreeNode pNode)
{
	if(GET_COLOR_TREE(pNode) == RB_RED)
		return;

	PTreeNode parent, sibling, lnephew, rnephew, root;

	root = pRoot->root;
	while(!IS_ROOT_TREE(pNode)){
		parent = GET_PARENT_TREE(pNode);
		printf_debug("%d\n", IS_LEFT_CHILD_TREE(pNode));
		if(IS_LEFT_CHILD_TREE(pNode)){
			//case 1: pNode has red left child
			printf_debug("%d\n", GET_COLOR_TREE(pNode->rb_left));
			if(GET_COLOR_TREE(pNode->rb_left) == RB_RED){
				SET_BLACK_COLOR_TREE(pNode->rb_left);
				break;
			}

			//case 2: sibling is red
			sibling = parent->rb_right;
			printf_debug("%d\n", GET_COLOR_TREE(sibling));
			if(GET_COLOR_TREE(sibling) == RB_RED){
				SET_RED_COLOR_TREE(parent);
				SET_BLACK_COLOR_TREE(sibling);
				root = rotate_rbnode_left(parent);
				sibling = parent->rb_right;
			}

			//case 3: sibling is black, nephews are black;
			lnephew = sibling->rb_left;
			rnephew = sibling->rb_right;
			printf_debug("%d, %d\n", GET_COLOR_TREE(lnephew), GET_COLOR_TREE(rnephew));
			if(GET_COLOR_TREE(lnephew) && GET_COLOR_TREE(rnephew)){
				SET_RED_COLOR_TREE(sibling);
				if(GET_COLOR_TREE(parent)){
					pNode = parent;
					continue;
				}else{
					SET_BLACK_COLOR_TREE(parent);
					break;
				}
			}

			//case 4: sibling is black, right nephews is red
			printf_debug("%d\n", GET_COLOR_TREE(rnephew));
			if(GET_COLOR_TREE(rnephew) == RB_RED){
				SET_COLOR_TREE(sibling, GET_COLOR_TREE(parent));
				SET_BLACK_COLOR_TREE(parent);
				SET_BLACK_COLOR_TREE(rnephew);
				root = rotate_rbnode_left(parent);
				break;
			}

			//case 5: sibling is black, left nephews is red
			printf_debug("%d\n", GET_COLOR_TREE(lnephew));
			if(GET_COLOR_TREE(lnephew) == RB_RED){
				SET_RED_COLOR_TREE(sibling);
				SET_BLACK_COLOR_TREE(lnephew);
				root = rotate_rbnode_right(sibling);
				continue;
			}

		}else{
			//case 1: pNode has red right child
			if(GET_COLOR_TREE(pNode->rb_right) == RB_RED){
				SET_BLACK_COLOR_TREE(pNode->rb_right);
				break;
			}

			//case 2: sibling is red
			sibling = parent->rb_left;
			if(GET_COLOR_TREE(sibling) == RB_RED){
				SET_RED_COLOR_TREE(parent);
				SET_BLACK_COLOR_TREE(sibling);
				root = rotate_rbnode_right(parent);
				sibling = parent->rb_left;
			}

			//case 3: sibling is black, nephews are black;
			lnephew = sibling->rb_left;
			rnephew = sibling->rb_right;
			if(GET_COLOR_TREE(lnephew) && GET_COLOR_TREE(rnephew)){
				SET_RED_COLOR_TREE(sibling);
				if(GET_COLOR_TREE(parent)){
					pNode = parent;
					continue;
				}else{
					SET_BLACK_COLOR_TREE(parent);
					break;
				}
			}

			//case 4: sibling is black, left nephews is red
			if(GET_COLOR_TREE(sibling) && GET_COLOR_TREE(lnephew) == RB_RED){
				SET_COLOR_TREE(sibling, GET_COLOR_TREE(parent));
				SET_BLACK_COLOR_TREE(parent);
				SET_BLACK_COLOR_TREE(lnephew);
				root = rotate_rbnode_right(parent);
				break;
			}

			//case 5: sibling is black, right nephews is red
			if(GET_COLOR_TREE(sibling) && GET_COLOR_TREE(rnephew) == RB_RED){
				SET_RED_COLOR_TREE(sibling);
				SET_BLACK_COLOR_TREE(rnephew);
				root = rotate_rbnode_left(sibling);
				continue;
			}
		}
	}

	if(IS_ROOT_TREE(root))
		pRoot->root = root;
}

static void __insert_deal(PTreeRoot pRoot, PTreeNode pNode, TREECMP_FUNC cmpFunc)
{
	if(pRoot->root == NULL){
		SET_ROOT_TREE(pNode);
		pRoot->root = pNode;
		return;
	}

	PTreeNode root = pRoot->root;
	while(1){
		if(cmpFunc(pNode, root) <= 0){
			if(root->rb_left){
				root = root->rb_left;
				continue;
			}
			add_rbnode_left(root, pNode);
			break;
		}else{
			if(root->rb_right){
				root = root->rb_right;
				continue;
			}
			add_rbnode_right(root, pNode);
			break;
		}
	}
}

static void __erase_aim(PTreeRoot pRoot, PTreeNode pNode)
{
	if(pNode->rb_left && pNode->rb_right){
		PTreeNode pCursor = pNode->rb_left;
		while(pCursor->rb_right)pCursor = pCursor->rb_right;

		//swap_rbnode(pCursor, pNode)
		if(pCursor == pNode->rb_left){
			add_rbnode_left(pNode, pCursor->rb_left);
			add_rbnode_right(pCursor, pNode->rb_right);
			pNode->rb_right = NULL;
			swap_parent_rbnode(GET_PARENT_TREE(pNode), pNode, pCursor, pCursor);
		}else{
			add_rbnode_right(pCursor, pNode->rb_right);
			pNode->rb_right = pCursor->rb_left;
			add_rbnode_left(pCursor, pNode->rb_left);
			add_rbnode_left(pNode, pNode->rb_right);
			pNode->rb_right = NULL;
			swap_parent_rbnode(GET_PARENT_TREE(pNode), pNode, GET_PARENT_TREE(pCursor), pCursor);
		}

		//swap color
		if(GET_COLOR_TREE(pNode)){
			SET_COLOR_TREE(pNode, GET_COLOR_TREE(pCursor));
			SET_BLACK_COLOR_TREE(pCursor);
		}else{
			SET_COLOR_TREE(pNode, GET_COLOR_TREE(pCursor));
			SET_RED_COLOR_TREE(pCursor);
		}


		if(IS_ROOT_TREE(pCursor))
			pRoot->root = pCursor;

	}
}

static void __erase_deal(PTreeRoot pRoot, PTreeNode pNode)
{
	if(pRoot->root == pNode){
		if(pNode->rb_left)
			pRoot->root = pNode->rb_left;
		else if(pNode->rb_right)
			pRoot->root = pNode->rb_right;
		else
			pRoot->root = NULL;
	}

	delete_rbnode(pNode);
}

void insertRBLeaf(PTreeRoot pRoot, PTreeNode pNode, TREECMP_FUNC cmpFunc)
{
	__insert_deal(pRoot, pNode, cmpFunc);
	__insert_adjust(pRoot, pNode);
}

void eraseRBLeaf(PTreeRoot pRoot, PTreeNode pNode)
{
	__erase_aim(pRoot, pNode);
	__erase_adjust(pRoot, pNode);
	__erase_deal(pRoot, pNode);
}

PTreeNode searchRBLeaf(PTreeRoot pRoot, PTreeNode pNode, TREECMP_FUNC cmpFunc)
{
	PTreeNode root = pRoot->root;
	while(1){
		if(cmpFunc(pNode, root) == 0)
			break;

		if(cmpFunc(pNode, root) < 0 && root->rb_left)
			root = root->rb_left;
		else if(cmpFunc(pNode, root) > 0 && root->rb_right)
			root = root->rb_right;
		else
			break;
	}

	return root;
}
