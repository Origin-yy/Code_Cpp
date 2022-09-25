#include <stdio.h>
#include <stdlib.h>
#define MAX 100
typedef struct NodeType {
  int id;
  int password;
  struct NodeType *next;
} NodeType;

void CreaList(NodeType **, int);

NodeType *GetNode(int, int);

void PrntList(NodeType *);

int IsEmptyList(NodeType *);

void JosephusOperate(NodeType **, int);

int main(void) {
  int n = 0;
  int m = 0;
  NodeType *pHead = NULL;
  do {

    if (n > MAX) {
      printf("人数太多，请输入！\n");
    }
    printf("请输入人数n(最多%d个)", MAX);
    scanf("%d", &n);
  } while (n > MAX);
  printf("请输入初始密码m:");
  scanf("%d", &m);
  CreaList(&pHead, n);
  printf("\n--------打印循环链表--------\n");
  PrntList(pHead);
  printf("\n--------打印出队情况--------\n");
  JosephusOperate(&pHead, m);
  return 1;
}
void CreaList(NodeType **ppHead, int n) {
  int i = 0;
  int iPassword = 0;
  NodeType *pNew = NULL;
  NodeType *pCur = NULL;
  for (i = 1; i <= n; i++) {
    printf("输入第%d个人的密码：", i);
    scanf("%d", &iPassword);
    pNew = GetNode(i, iPassword);
    if (*ppHead == NULL) {
      *ppHead = pCur = pNew;
      pCur->next = *ppHead;
    } else {
      pNew->next = pCur->next;
      pCur->next = pNew;
      pCur = pNew;
    }
  }
  printf("完成单向循环链表的创建\n");
}
NodeType *GetNode(int ild, int iPassword) {
  NodeType *pNew = NULL;
  pNew = (NodeType *)malloc(sizeof(NodeType));
  if (!pNew) {
    printf("Error,the memory is not enough！\n");
    exit(-1);
  }
  pNew->id = ild;
  pNew->password = iPassword;
  pNew->next = NULL;
  return pNew;
}
void PrntList(NodeType *pHead) {
  NodeType *pCur = pHead;
  if (!IsEmptyList(pHead)) {
    printf("--ID-- --PASSWORD--\n");
    do {
      printf("%3d%7d\n", pCur->id, pCur->password);
      pCur = pCur->next;
    } while (pCur != pHead);
  }
}
int IsEmptyList(NodeType *pHead) {
  if (!pHead) {

    printf("The list is empty!\n");
    return 1;
  }
  return 0;
}
void JosephusOperate(NodeType **ppHead, int iPassword) {
  int iCounter = 0;
  int iFlag = 1;
  NodeType *pPrv = NULL;
  NodeType *pCur = NULL;
  NodeType *pDel = NULL;
  pPrv = pCur = *ppHead;

  while (pPrv->next != *ppHead)
    pPrv = pPrv->next;
  while (iFlag) {
    for (iCounter = 1; iCounter < iPassword; iCounter++) {
      pPrv = pCur;
      pCur = pCur->next;
    }
    if (pPrv == pCur)
      iFlag = 0;
    pDel = pCur;
    pPrv->next = pCur->next;
    pCur = pCur->next;
    iPassword = pDel->password;
    printf("第%d个人出列（密码：%d）\n", pDel->id, pDel->password);
    free(pDel);
  }
  *ppHead = NULL;
  getchar();
}
