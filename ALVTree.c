#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#define ERROR -1
#define ElemType int
#define Max(A, B) ((A) > (B) ? (A) : (B))

#define INSERT 'I'
#define REMOVE 'R'
#define SEARCH 'S'
#define PRINT 'P'
#define QUIT 'Q'
#define DRAW 'D'

typedef enum { False, True } bool;

typedef struct ALVTreeNode {
	struct ALVTreeNode* left, * right, *parent;
	ElemType data;
	int blcfactor;
	int layer;
}ALVTreeNode, * ALVTree;

void ClearLine() {
	char c;
	do {
		c = getchar();
	} while (c != '\n');
}

void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

void GetLayer(ALVTree T);
void GetBFactor(ALVTree T);
ALVTree SLRotate(ALVTree T);
ALVTree SRRotate(ALVTree T);
ALVTree LRRotate(ALVTree T);
ALVTree RLRotate(ALVTree T);
ALVTree Insert_ALVTree(ALVTree T, ElemType data);
ALVTree Remove_ALVTree(ALVTree T, ElemType data);
bool Search_ALVTree(ALVTree T, ElemType data);
void Print_ALVTree(ALVTree T, int layer);
void Draw_ALVTree(ALVTree T);
void Edge2Dot(ALVTree T, FILE *fp);

void GetLayer(ALVTree T) {
	if (T) {
		if (T->left && T->right)
			T->layer = Max(T->left->layer, T->right->layer) + 1;
		else if (T->left)
			T->layer = T->left->layer + 1;
		else if (T->right)
			T->layer = T->right->layer + 1;
		else
			T->layer = 0;
	}
}

void GetBFactor(ALVTree T) {
	if (T) {
		if (T->left && T->right)
			T->blcfactor = T->right->layer - T->left->layer;
		else if (T->left)
			T->blcfactor = -1 - T->left->layer;
		else if (T->right)
			T->blcfactor = T->right->layer + 1;
		else
			T->blcfactor = 0;
	}
}

ALVTree SLRotate(ALVTree T) {
	if (T->right) {
		ALVTreeNode* A = T, * B = T->right;
		A->right = B->left;
		if (B->left)
			B->left->parent = A;
		B->left = A;
		B->parent = A->parent;
		A->parent = B;
		GetLayer(A);
		GetBFactor(A);
		GetLayer(B);
		GetBFactor(B);
		return B;
	}
	else
		return T;
}

ALVTree SRRotate(ALVTree T) {
	if (T->left) {
		ALVTreeNode* A = T, * B = T->left;
		A->left = B->right;
		if (B->right)
			B->right->parent = A;
		B->right = A;
		B->parent = A->parent;
		A->parent = B;
		GetLayer(A);
		GetBFactor(A);
		GetLayer(B);
		GetBFactor(B);
		return B;
	}
	else
		return T;
}

ALVTree LRRotate(ALVTree T) {
	if (T->left && T->left->right) {
		T->left = SLRotate(T->left);
		T = SRRotate(T);
		return T;
	}
	else
		return T;
}

ALVTree RLRotate(ALVTree T) {
	if (T->right && T->right->left) {
		T->right = SRRotate(T->right);
		T = SLRotate(T);
		return T;
	}
	else
		return T;
}

ALVTree Insert_ALVTree(ALVTree T, ElemType data) {
	if (T) {
		if (data < T->data) {
			if (T->left) {
				T->left = Insert_ALVTree(T->left, data);
				if (T->right) {
					T->layer = Max(T->left->layer, T->right->layer) + 1;
					T->blcfactor = T->right->layer - T->left->layer;
				}
				else {
					T->layer = T->left->layer + 1;
					T->blcfactor = -1 - T->left->layer;
				}

				if (T->blcfactor == -2) {
					if (T->left->blcfactor == -1)
						T = SRRotate(T);
					else if (T->left->blcfactor == 1)
						T = LRRotate(T);
				}
			}
			else {
				ALVTreeNode* node;
				if (!(node = (ALVTreeNode*)malloc(sizeof(ALVTreeNode))))
					exit(ERROR);
				node->data = data;
				node->left = NULL;
				node->right = NULL;
				node->layer = 0;
				node->blcfactor = 0;
				node->parent = T;
				T->left = node;
				if (T->right) {
					T->layer = Max(T->left->layer, T->right->layer) + 1;
					T->blcfactor = T->right->layer - T->left->layer;
				}
				else {
					T->layer = T->left->layer + 1;
					T->blcfactor = -1 - T->left->layer;
				}
			}
		}
		else if (data > T->data) {
			if (T->right) {
				T->right = Insert_ALVTree(T->right, data);
				if (T->left) {
					T->layer = Max(T->left->layer, T->right->layer) + 1;
					T->blcfactor = T->right->layer - T->left->layer;
				}
				else {
					T->layer = T->right->layer + 1;
					T->blcfactor = T->right->layer + 1;
				}

				if (T->blcfactor == 2) {
					if (T->right->blcfactor == 1)
						T = SLRotate(T);
					else if (T->right->blcfactor == -1)
						T = RLRotate(T);
				}
			}
			else {
				ALVTreeNode* node;
				if (!(node = (ALVTreeNode*)malloc(sizeof(ALVTreeNode))))
					exit(ERROR);
				node->data = data;
				node->left = NULL;
				node->right = NULL;
				node->layer = 0;
				node->blcfactor = 0;
				node->parent = T;
				T->right = node;
				if (T->left) {
					T->layer = Max(T->left->layer, T->right->layer) + 1;
					T->blcfactor = T->right->layer - T->left->layer;
				}
				else {
					T->layer = T->right->layer + 1;
					T->blcfactor = T->right->layer + 1;
				}
			}
		}


	}
	else {
		if (!(T = (ALVTree)malloc(sizeof(ALVTreeNode))))
			exit(ERROR);
		T->data = data;
		T->left = NULL;
		T->right = NULL;
		T->layer = 0;
		T->blcfactor = 0;
		T->parent = NULL;
	}
	return T;
}

ALVTree Remove_ALVTree(ALVTree T, ElemType data) {
	if (T) {
		if (T->data == data) {
			if (T->left && T->right) {
				ALVTreeNode* pre;
				for (pre = T->left; pre->right; pre = pre->right);
				swap(&pre->data, &T->data);
				T->left = Remove_ALVTree(T->left, data);

				GetLayer(T);
				GetBFactor(T);

				if (T->blcfactor == -2) {
					if (T->left->blcfactor == -1)
						T = SRRotate(T);
					else if (T->left->blcfactor == 1)
						T = LRRotate(T);
				}
				if (T->blcfactor == 2) {
					if (T->right->blcfactor == 1)
						T = SLRotate(T);
					else if (T->right->blcfactor == -1)
						T = RLRotate(T);
				}
			}
			else if (T->left) {
				T->left->parent = T->parent;
				ALVTree child = T->left;
				free(T);
				return child;
			}
			else if (T->right) {
				T->right->parent = T->parent;
				ALVTree child = T->right;
				free(T);
				return child;
			}
			else {
				free(T);
				return NULL;
			}
		}
		else if (T->data > data) {
			T->left = Remove_ALVTree(T->left, data);
			GetLayer(T);
			GetBFactor(T);

			if (T->blcfactor == -2) {
				if (T->left->blcfactor == -1)
					T = SRRotate(T);
				else if (T->left->blcfactor == 1)
					T = LRRotate(T);
			}
			if (T->blcfactor == 2) {
				if (T->right->blcfactor == 1)
					T = SLRotate(T);
				else if (T->right->blcfactor == -1)
					T = RLRotate(T);
			}
		}
		else {
			T->right = Remove_ALVTree(T->right, data);
			GetLayer(T);
			GetBFactor(T);

			if (T->blcfactor == -2) {
				if (T->left->blcfactor == -1)
					T = SRRotate(T);
				else if (T->left->blcfactor == 1)
					T = LRRotate(T);
			}
			if (T->blcfactor == 2) {
				if (T->right->blcfactor == 1)
					T = SLRotate(T);
				else if (T->right->blcfactor == -1)
					T = RLRotate(T);
			}
		}
	}
	return T;
}

bool Search_ALVTree(ALVTree T, ElemType data) {
	if (T) {
		if (data == T->data)
			return True;
		else if (data < T->data)
			return Search_ALVTree(T->left, data);
		else
			return Search_ALVTree(T->right, data);
	}
	else
		return False;
	
}

void Print_ALVTree(ALVTree T, int layer) {
	int i;
	if (layer <= 0)
		exit(ERROR);

	for (i = 0; i < layer; i++) {
		printf("-");
	}

	if (!T)
		printf(" NULL");
	else {
		printf(" %d", T->data);
		if (T->left || T->right) {
			printf("\n");
			Print_ALVTree(T->left, layer + 1);
			printf("\n");
			Print_ALVTree(T->right, layer + 1);
		}
	}
}

void Draw_ALVTree(ALVTree T) {
	FILE* fp;
	char filename[10];
	sprintf(filename, "Tree.dot");
	fp = fopen(filename, "w+");
	if (fp) {
		fprintf(fp, "digraph {\n");
		Edge2Dot(T, fp);
		fprintf(fp, "}");
		fclose(fp);
		system("dot -Tpng Tree.dot -o Tree.png");
		system("del Tree.dot");
		system("Tree.png");
	}
}

void Edge2Dot(ALVTree T, FILE* fp) {
	if (T->parent)
		fprintf(fp, "\t%d -> %d;\n", T->parent->data, T->data);
	if (T->left || T->right) {
		if (T->left)
			Edge2Dot(T->left, fp);
		else
			fprintf(fp, "\tNULL%d [label = \"NULL\"];\n\t%d -> NULL%d;\n", T->data, T->data, T->data);
		if (T->right)
			Edge2Dot(T->right, fp);
		else
			fprintf(fp, "\tNULL%d [label = \"NULL\"];\n\t%d -> NULL%d;\n", T->data, T->data, T->data);
	}

	if (!T->parent && !T->left && !T->right)
		fprintf(fp, "%d;", T->data);
}

main() {
	char c;
	ALVTree T = NULL;
	printf("The draw function REQUIRES Graphviz package!!!\n");
	printf("You can download it from: http://www.graphviz.org/download/ \n");
	while (1) {
		printf("I: Insert\nR: Remove\nS: Search\nP: Print\nD: Draw\nQ: Quit\n");
		printf("Input Instruction:");
		c = getchar();
		switch (c) {
		case INSERT:
			c = getchar();
			if (c != '\n') {
				printf("Invalid Instruction\n");
				ClearLine();
				getch();
			}
			else {
				printf("Input Data:");
				int data;
				while (!(scanf("%d", &data))) {
					ClearLine();
					printf("Invalid Data\n");
					printf("Input Data:");
				}
				getchar();
				T = Insert_ALVTree(T, data);
			}
			break;

		case REMOVE:
			c = getchar();
			if (c != '\n') {
				printf("Invalid Instruction\n");
				ClearLine();
				getch();
			}
			else {
				printf("Input Data:");
				int data;
				while (!(scanf("%d", &data))) {
					printf("Invalid Data\n");
					ClearLine();
					printf("Input Data:");
				}
				getchar();
				T = Remove_ALVTree(T, data);
			}
			break;

		case SEARCH:
			c = getchar();
			if (c != '\n') {
				printf("Invalid Instruction\n");
				ClearLine();
				getch();
			}
			else {
				printf("Input Data:");
				int data;
				while (!(scanf("%d", &data))) {
					printf("Invalid Data\n");
					ClearLine();
					printf("Input Data:");
				}
				getchar();
				if (Search_ALVTree(T, data) == True)
					printf("True\n");
				else
					printf("False\n");
				getch();
			}
			break;

		case DRAW:
			c = getchar();
			if (c != '\n') {
				printf("Invalid Instruction\n");
				ClearLine();
			}
			else {
				Draw_ALVTree(T);
				printf("\n");
			}
			getch();
			break;

		case PRINT:
			c = getchar();
			if (c != '\n') {
				printf("Invalid Instruction\n");
				ClearLine();
			}
			else {
				Print_ALVTree(T, 1);
				printf("\n");
			}
			getch();
			break;

		case QUIT:
			c = getchar();
			if (c != '\n') {
				printf("Invalid Instruction\n");
				ClearLine();
				ClearLine();
			}
			else {
				return 0;
			}

		case '\n':
			break;

		default:
			printf("Invalid Instruction\n");
			ClearLine();
			getch();
			break;
		}
		system("cls");
	}
}