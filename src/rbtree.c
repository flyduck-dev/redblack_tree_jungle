#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
//all 통과!
void left_rotate(rbtree *t, node_t *x) { 
  node_t *y = x->right;
  x->right = y->left;  
                                              
  if (y->left != t->nil) {                            
    y->left->parent = x;                           
  }
  
  y->parent = x->parent;                
  
  if (x->parent == t->nil) {                        
    t->root = y;
  } else if (x == x->parent->left) {     
    x->parent->left = y;
  } else {                     
    x->parent->right = y;
  }
  y->left = x;                         
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x) { 
  node_t *y = x->left; 
  x->left = y->right; 
  
  if (y->right != t->nil) { 
    y->right->parent = x;
  }
  
  y->parent = x->parent; 
  
  if (x->parent == t->nil) {
    t->root = y;
  } else if (x->parent->right == x) { 
    x->parent->right = y;  
  } else { 
    x->parent->left = y;
  }
  
  y->right = x; //x의 오른쪽 자식으로 y를 설정
  x->parent = y; //y의 부모를 x로 설정
}


void rbtree_insert_fixup(rbtree *t, node_t *z) {
  while (z -> parent -> color == RBTREE_RED) {
    if (z->parent == z-> parent -> parent -> left) {       
      node_t *uncle = z -> parent -> parent -> right;       // z 조부모의 오른쪽 자식이 uncle
      
      // 경우 1에 해당(부모도 red, 삼촌도 red), uncle이 red라면 색깔 조정을 하는 것만으로 특성 4 위반을 보완할 수도 있다.
      if (uncle -> color == RBTREE_RED) {               
        z->parent->color = RBTREE_BLACK;             // 부모를 black으로
        uncle->color = RBTREE_BLACK;                 // 삼촌도 black으로
        z -> parent -> parent -> color = RBTREE_RED; // 조부모는 red로
        z = z -> parent -> parent;                   // 조부모를 기준으로 재탐색
        
      } else {
        if (z == z->parent->right) {      // 만약 z가 부모의 오른쪽 자식이었다면, 부모가 grand parent의 왼쪽 자식이었으므로 triangle에 해당한다.
          z = z->parent;                  // z의 부모를 축으로 삼아
          left_rotate(t, z);              // 부모를 기준으로 왼쪽으로 회전
        }

        z -> parent -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
      
    // b. z의 부모노드가 조부모의 오른쪽 자식이라면
    } else {                  
      // 삼촌 노드는 조부모의 왼쪽 자식                        
      node_t *uncle = z->parent->parent->left;
      // 경우 1에 해당(부모도 red, 삼촌도 red)
        if (uncle->color == RBTREE_RED) {
          z->parent->color = RBTREE_BLACK;
          uncle->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          z = z->parent->parent;
        } else { // 삼촌이 black 이라면
          // 경우 2. 신규 red가 부모노드의 왼쪽 자식이라면
          if (z == z->parent->left) {
            z = z->parent;
            right_rotate(t, z); // 부모를 기준으로 오른쪽 회전 진행
          }
          // 경우 3. 신규 red가 부모노드의 오른쪽 자식이라면
          z->parent->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          left_rotate(t, z->parent->parent);
        }
    }
  }
  
  t->root->color = RBTREE_BLACK;
}

void postorder_delete_rbtree(rbtree *t, node_t *x) {
  if (x != t->nil) {
    postorder_delete_rbtree(t, x->left);
    postorder_delete_rbtree(t, x->right);
    free(x);
  }
}


rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->root = p->nil;
  p->nil->color = RBTREE_BLACK;
  return p;
}

void delete_rbtree(rbtree *t) {
  postorder_delete_rbtree(t, t->root);
  free(t->nil);
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *y = t->nil;  
  node_t *x = t->root;
  
  while (x != t->nil) {  // nil이 아니라면 계속 찾고
    y = x;               // y에는 미리 x의 값(주소값)을 저장해놓는다. parent를 저장해놓기 위해서이다.
    if (key < x->key) {  // 삽입하려는 값이 현재 x가 가르키는 노드의 키 값보다 작다면 왼쪽으로 탐색한다.
      x = x->left;
    } else {             // 삽입하려는 값이 현재 x가 가르키는 노드의 키 값보다 크다면 오른쪽으로 탐색한다.
      x = x->right;      // 추가로, 만약 (x->key == key)를 만족하여 중복되는 값이 있다면 else문을 만나 오른쪽에 넣도록 동작하게 된다.
    }
  }
  // 새로운 노드 z 생성(할당)
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
                 
  z->key = key;
  z->parent = y; ////////
  
  if (y == t->nil) {
    t->root = z;  
  } else if (z->key < y->key) { // 만약 새로 삽입할 z의 키 값이 parent의 키 값보다 작다면 parent(y)의 왼쪽에 넣어야 한다.
    y->left = z;
  } else {             // 만약 새로 삽입할 z의 키 값이 parent의 키 값보다 크다면 parent(y)의 오른쪽에 넣어야 한다.
    y->right = z;
  }
  
  // 새로 삽입할 노드 z는 단말 노드(리프 노드, leaf node)이기 때문에 left, right는 nil
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;// 새로 삽입할 노드의 색은 처음에 red
  rbtree_insert_fixup(t, z);// 특성이 위반되는 케이스들 보완
  return t->root;  
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  if (t->root == t->nil) {
    return NULL;                                      // nil 개념을 사용하여 전체 코드를 짰으나, 탐색 테스트 케이스에서 NULL을 요구한다. 따라서 t->nil이 아니라 NULL로 작성하였다.
  }
  node_t *x = t->root;
  
  while (x != t->nil) {
    if (x->key > key) {
      x = x->left;
    } else if (x->key < key) {
      x = x->right;
    } else {
      return x;
    }
  }
  
  // TODO: implement find
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *x = t->root;
  
  while (x->left != t->nil) {
    x = x->left;
  }
  
  // TODO: implement find
  return x;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *x = t->root;
  
  while (x->right != t->nil) {
    x = x->right;
  }
  // TODO: implement find
  return x;
}

node_t *find_min_successor(rbtree *t, node_t *y) {
  while (y->left != t->nil) {                         // y의 왼쪽 자식이 nil이 아닐 때까지 계속 파고들어간다.
    y = y->left;                                      
  }
  
  // y의 왼쪽 자식이 nil이라면 멈추기 때문에(y가 nil이면 멈추는게 아니라) y는 유의미한 값을 가진 노드를 가르키는 주소값이다. 여기서는 successor라고 보면 된다. 
  return y;
}

//노드 u가 노드 v로 대체되어 트리의 구조를 유지
//삭제된 노드를 대체할 노드를 선택하여 트리의 균형을 유지
void rbtree_transplant(rbtree *t, node_t *u, node_t *v) { // 노드 u를 노드 v로 대체
  if (u->parent == t->nil) { //노드 u의 부모 노드가 nil(즉, u가 루트 노드인 경우)인지 확인
    t->root = v;             //루트 노드를 v로 대체
  } else if (u == u->parent->left) { //u가 부모 노드의 왼쪽 자식인지 확인
    u->parent->left = v;             //부모 노드의 왼쪽 자식을 v로 대체
  } else {                   //그렇지 않다면
    u->parent->right = v;    //부모 노드의 오른쪽 자식을 v로 대체
  }
  v->parent = u->parent;   //v의 부모를 u의 부모로 설정
}

void rbtree_erase_fixup(rbtree *t, node_t *x) {
  node_t *w;                                          // x의 형제 노드를 가리키는 w 포인터를 미리 선언한다.
  
  while (x != t->root && x->color == RBTREE_BLACK) {  // x가 root가 되면 단순히 그냥 검은색으로 바꾸면된다. 그리고 while문 아래는 x가 doubly black일 떄 이뤄진다.
    // doubly black인 x가 왼쪽 자식일 때
    if (x == x->parent->left) {
      w = x->parent->right;
      
      // <경우 1> : 경우 2, 3, 4로 위임 가능
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;                         // 회전을 끝내고 난 후에는 x->parent->right가 새로운 노드가 되고 밑의 if, else if, else 중 한 가지, 즉 경우 2, 3, 4의 한 가지로 위임된다.
      }
      
      // 위의 if문을 만나지 않았으므로, w->color == RBTREE_BLACK인 경우이다.
      // <경우 2> : 경우 1, 2, 3, 4로 위임 가능
      // x->parent로 짬 때리는 경우이다.
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;                        // x의 extra black을 x->parent로 넘긴다. 그러면서 w는 red가 된다.
        x = x->parent;                                // 새롭게 doubly black 혹은 red and black이 x->parent이 짬 맞아서 재조정을 진행하도록 위임한다.
      } else {
        
        // <경우 3> : 경우 4로 위임 가능
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        
        // <경우 4> : 특성이 위반되는 것을 해결한다. 경우 4는 다른 경우로 위임되지 않고 위반을 해결(특성을 만족)한다.
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;                                  // 경우 4를 거치면 특성 위반이 해결되는 것이므로 x를 root로 설정하여 while문을 빠져나가도록 한다.
      }
    
    // doubly black인 x가 오른쪽 자식일 때
    } else {
      w = x->parent->left;
      
      // <경우 1>
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      
      // <경우 2>
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        
        // <경우 3>
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        
        // <경우 4>
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  
  x->color = RBTREE_BLACK;                            // x가 root 노드이거나, red and black이면 해당 코드를 만나서 black이 되고 특성들을 만족시키게 된다.
}


int rbtree_erase(rbtree *t, node_t *z) { // z: 삭제하고자 하는 노드
  node_t *y = z;  
  color_t y_original_color = y->color; 
  
  node_t *x;  // x는 y의 자식으로, 오른쪽 서브트리에서 가장 작은 값을 가진 노드를 탐색

  if (z->left == t->nil) { // z의 자식이 오른쪽 자식 1개뿐이라면
    x = z->right;          // 그 z의 자식 x를
    rbtree_transplant(t, z, z->right);  // 그냥 z 자리에 심으면 된다.
  } else if (z->right == t->nil) {
    x = z->left;
    rbtree_transplant(t, z, z->left);
  } else {
    y = find_min_successor(t, z->right);  // z의 오른쪽 자식에서, 즉 오른쪽 서브 트리에서 successor를 찾는다. 이는 z보다 크지만 오른쪽 서브 트리에서 가장 작은, z 바로 다음으로 큰 successor를 찾는 것이다.
    y_original_color = y->color;          // 여기서는 이제 실제로 삭제될 노드 successor의 색을 저장해둬야 한다.
    x = y->right;                         // y는 실제로 삭제될 거니까, y의 자리에 x가 대체자로 올라와야 한다.
    
    if (y->parent == z) {                // 이건 왜 하는지 잘 모르겠다. transplant하면서 자연스럽게 부모가 바뀌지 않나?
      x->parent = y;                                  
    } else {
      rbtree_transplant(t, y, y->right);              // z에 y를 심기 전에, 먼저 y의 자리에 y의 자식을 심는다. 그렇지 않으면 y는 자식이 딸린 채로 z에 심어지기 때문이다. 해당 코드를 실행하면 y 노드 1개만 딱 떼어져 있다고 생각하면 편할 듯 하다.
      y->right = z->right;                            // z에 y를 심기 전에 행하는 사전 작업이다.
      y->right->parent = y;                           // 우리가 방금 y 하나만 딱 둥그러니 떼놨기 때문에 y의 자식과 그 자식의 부모를 y로 향하게 설정해줘야 한다.
    }                                                 // 근데 왜 오른쪽 자식만 먼저 하지?
    
    rbtree_transplant(t, z, y);                       // 이제 z에 y를 심는다. 정확히는 z의 부모는 이제 z가 아니라 y를 가르키게 된다는 것이다.
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;                              // y는 z로 올라오고, y의 데이터는 유지한 체 색깔만 바뀐다. 즉 z의 색깔이 아니라 y의 색깔이 삭제되는 것이다. z에 y가 이식된 후, z의 색깔을 y에 심었으니까.
  }
  
  // 만약 위에서 삭제한 색이, 즉 실제로 삭제되는 노드인 y의 색이 red였다면 레드 블랙 트리 특성 5가지 중에서 위반되는 것은 없다.
  // 허나 black이었다면 위반하는 항목들이 있기 때문에 재조정이 필요하다. 단순히 생각하면 black을 삭제하여 red, red끼리 만나거나 특정 노드에서 리프 노드까지 만나는 black의 갯수가 달라질 수 있다.
  if (y_original_color == RBTREE_BLACK) {
    rbtree_erase_fixup(t, x);                         // 대체하는 노드 y의 자식인 x가 y의 자리로 올라오면서 이 x에 extra black을 부여한다. 이 extra black을 처리하면서 재조정하는 것이 fixup의 원리이므로 x를 인자로 넣는다.
  }
  
  free(z);                                            // z가 삭제되고 그 자리에 y가 대체하는 것이므로 z를 free시켜준다.
  // TODO: implement erase
  return 0;
}

void inorder_rbtree_to_array(const rbtree *t, node_t *x, key_t *arr, int *idx, const size_t n) {
  if (x == t->nil) {
    return;
  }
  
  inorder_rbtree_to_array(t, x->left, arr, idx, n);
  if (*idx < n) {
    arr[(*idx)++] = x->key;                           // *idx는 0, 1, 2, 3...이다. 그리고 후위 연산자 ++이므로 0부터 인덱스가 시작된다.
  } else {
    return;
  }
  inorder_rbtree_to_array(t, x->right, arr, idx, n);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  node_t *x = t->root;
  if (x == t->nil) {
    return 0;
  }
  int cnt = 0;
  int *idx = &cnt;
  inorder_rbtree_to_array(t, x, arr, idx, n);
  
  // TODO: implement to_array
  return 0;
}

