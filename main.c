#include <stdio.h>
#include <malloc.h>
#define NUMBER 24 // Input값 24개

//트리 노드 구조체 정의
typedef struct _S_segment{
	int min;
	int max;
	struct _S_segment* left;
	struct _S_segment* right;
	long int segment_flag;
} S_segment;

S_segment* initialize(int, int);			// Initialize 함수
S_segment* createSegmentTree(int, int);		// Segment 생성 함수
void perorder_verify(S_segment* node);		// 트리가 잘 생성되었는지 확인
S_segment* root;							// 루트 노드

void insertSegment(int segment, S_segment* node);	// Segment 입력 함수
void query(int querypoint, S_segment* node);		// 쿼리 함수

int seg_min[NUMBER] = {1,2,3,4,5,6,7,8,10,11,12,13,14,15,16,17,18,19,20,1,3,4,8,16};	// Input Data
int seg_max[NUMBER] = {7,6,9,15,10,8,13,19,17,6,20,1,13,17,6,8,10,15,17,3,5,7,9,20};	// Input Data

int main(){
	 
	int i;
	int input;	
	root = initialize(1, 20);

	for (i = 0; i < NUMBER; i++) {
		if (seg_max[i] < seg_min[i]) {
			int t = seg_max[i];
			seg_max[i] = seg_min[i];
			seg_min[i] = t;
		}
		insertSegment(i, root);
	}
	printf("please input query point... : ");
	scanf_s("%d", &input);
	query(input, root);
	getch();
}

S_segment* initialize(int RANGE_MIN, int RANGE_MAX){

	S_segment* address = createSegmentTree(RANGE_MIN, RANGE_MAX);
	perorder_verify(address);
	return address;
}

S_segment* createSegmentTree(int RANGE_MIN, int RANGE_MAX){

	if (RANGE_MAX < RANGE_MIN) return 0;								// MAX < MIN 이면 리턴
	if (RANGE_MAX == RANGE_MIN) {										// MAX == MIN이면 LEAF 생성, 노드의 MIN = MAX 
		S_segment* address = (S_segment*)malloc(sizeof(S_segment));
		address->min = address->max = RANGE_MIN;
		address->left = address->right = NULL;
		address->segment_flag = 0;
		return address;
	}
	else {
		S_segment* address = (S_segment*)malloc(sizeof(S_segment));		// MAX > MIN 이면 하위 노드 생성, MIX, MAX값 넣기
		address->min = RANGE_MIN;
		address->max = RANGE_MAX;
		address->left = createSegmentTree(RANGE_MIN, (RANGE_MIN + RANGE_MAX - 1) / 2);
		address->right = createSegmentTree((RANGE_MIN + RANGE_MAX - 1) / 2 + 1, RANGE_MAX);
		address->segment_flag = 0;
		return address;
	}
	return 0;
}

void perorder_verify(S_segment * node){	// 제대로 트리가 생성되었는지 확인

	if (node) {
		printf("MIN : %3d    MAX : %3d  \n",node->min,node->max);
		perorder_verify(node->left);
		perorder_verify(node->right);
	}
}

void insertSegment(int segment, S_segment * node){	// Segment 생성

	if (node) {
		if (seg_min[segment] > node->min || seg_max[segment] < node->max) {
			insertSegment(segment, node->left);
			insertSegment(segment, node->right);
		}
		else if (seg_min[segment] > node->max || seg_max[segment] < node->min) return;
		else {
			node->segment_flag = node->segment_flag | (1 << segment);
			// Segment가 올바른 노드에 삽입되었는지 확인
			printf("Segment[%d] (%2d-%2d) Inserted in node [%2d - %2d]\n", segment, seg_min[segment], seg_max[segment], node->min, node->max);
		}
	}
}

void query(int querypoint, S_segment * node){	// 쿼리문

	if (node) {
		int i;
		for (i = 0; i < NUMBER; i++) 
			if (node->segment_flag & (1 << i)) printf("Segment %2d found!\n", i);
		if (node->left) { if (node->left->max < querypoint) query(querypoint, node->right); else query(querypoint, node->left); }
	}
}
