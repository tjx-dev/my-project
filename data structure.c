//只要题目要求“反转链表”或“翻转链表指向”，就用下面这个模板
#include <stdio.h>
#include <stdlib.h>

/* 链表节点 */
typedef struct ListNode {
    int val;
    struct ListNode *next;
} ListNode;

ListNode* reverseList( ListNode* head) {
    ListNode *prev = NULL;   // 已经反转好的那半截
    ListNode *cur  = head;   // 当前要处理的节点
    ListNode *next = NULL;   // 临时保存 cur 的下一个节点

    while (cur != NULL) {
        next = cur->next;   // ① 先记住下一个节点
        cur->next = prev;   // ② 把当前节点接到“已反转”的头上
        prev = cur;         // ③ prev 向前移
        cur = next;         // ④ cur 向前移
    }

    return prev;   // 最后 prev 就是新的头节点
}






















