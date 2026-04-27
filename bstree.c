#include <stdio.h>
#include <stdlib.h>

#include "bstree.h"

/* START [_BSTNode] */
typedef struct _BSTNode {
  void *info;
  struct _BSTNode *left;
  struct _BSTNode *right;
} BSTNode;
/* END [_BSTNode] */

/* START [_BSTree] */
struct _BSTree {
  BSTNode *root;
  P_ele_print print_ele;
  P_ele_cmp cmp_ele;
};
/* END [_BSTree] */

/*** BSTNode TAD private functions ***/
void _tree_rangeSearch_rec(BSTNode *node, void *min, void *max, List *list, P_ele_cmp cmp);
int _tree_countLongSongs_rec(BSTNode *node, int min_duration);
BSTNode *_bst_node_new() {
  BSTNode *pn = NULL;

  pn = malloc(sizeof(BSTNode));
  if (!pn) {
    return NULL;
  }

  pn->left = NULL;
  pn->right = NULL;
  pn->info = NULL;

  return pn;
}

void _bst_node_free(BSTNode *pn) {
  if (!pn) {
    return;
  }

  free(pn);
}

void _bst_node_free_rec(BSTNode *pn) {
  if (!pn) {
    return;
  }

  _bst_node_free_rec(pn->left);
  _bst_node_free_rec(pn->right);
  _bst_node_free(pn);

  return;
}

int _bst_depth_rec(BSTNode *pn) {
  int depth_l, depth_r;

  if (!pn) {
    return 0;
  }

  depth_l = _bst_depth_rec(pn->left);
  depth_r = _bst_depth_rec(pn->right);

  if (depth_r > depth_l) {
    return depth_r + 1;
  } else {
    return depth_l + 1;
  }
}

int _bst_size_rec(BSTNode *pn) {
  int count = 0;

  if (!pn) {
    return count;
  }

  count += _bst_size_rec(pn->left);
  count += _bst_size_rec(pn->right);

  return count + 1;
}

int _bst_preOrder_rec(BSTNode *pn, FILE *pf, P_ele_print print_ele) {
  int count = 0;

  if (!pn) {
    return count;
  }

  count += print_ele(pf, pn->info);
  count += _bst_preOrder_rec(pn->left, pf, print_ele);
  count += _bst_preOrder_rec(pn->right, pf, print_ele);

  return count;
}

int _bst_inOrder_rec(BSTNode *pn, FILE *pf, P_ele_print print_ele) {
  int count = 0;

  if (!pn) {
    return count;
  }

  count += _bst_inOrder_rec(pn->left, pf, print_ele);
  count += print_ele(pf, pn->info);
  count += _bst_inOrder_rec(pn->right, pf, print_ele);

  return count;
}

int _bst_postOrder_rec(BSTNode *pn, FILE *pf, P_ele_print print_ele) {
  int count = 0;

  if (!pn) {
    return count;
  }

  count += _bst_postOrder_rec(pn->left, pf, print_ele);
  count += _bst_postOrder_rec(pn->right, pf, print_ele);
  count += print_ele(pf, pn->info);

  return count;
}

/*** BSTree TAD functions ***/
BSTree *tree_init(P_ele_print print_ele, P_ele_cmp cmp_ele) {
  BSTree *tree;

  if (!print_ele || !cmp_ele) {
    return NULL;
  }

  tree = malloc(sizeof(BSTree));
  if (!tree) {
    return NULL;
  }

  tree->root = NULL;
  tree->print_ele = print_ele;
  tree->cmp_ele = cmp_ele;

  return tree;
}

void tree_destroy(BSTree *tree) {
  if (!tree) {
    return;
  }

  _bst_node_free_rec(tree->root);
  free(tree);

  return;
}

Bool tree_isEmpty(const BSTree *tree) {
  if (!tree || !tree->root) {
    return TRUE;
  }
  return FALSE;
}

int tree_depth(const BSTree *tree) {
  if (!tree) {
    return -1;
  }

  return _bst_depth_rec(tree->root);
}

size_t tree_size(const BSTree *tree) {
  if (!tree) {
    return -1;
  }

  return _bst_size_rec(tree->root);
}

int tree_preOrder(FILE *f, const BSTree *tree) {
  if (!f || !tree) {
    return -1;
  }

  return _bst_preOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

int tree_inOrder(FILE *f, const BSTree *tree) {
  if (!f || !tree) {
    return -1;
  }

  return _bst_inOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

int tree_postOrder(FILE *f, const BSTree *tree) {
  if (!f || !tree) {
    return -1;
  }

  return _bst_postOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

/**** TO D O: find_min, find_max, insert, contains, remove ****/
void *tree_find_min(BSTree *tree) {
  BSTree aux_tree;

  if (!tree || !tree->root) {
    return NULL;
  }

  if (!tree->root->left) {
    return tree->root->info;
  }

  aux_tree.root = tree->root->left;
  aux_tree.print_ele = tree->print_ele;
  aux_tree.cmp_ele = tree->cmp_ele;

  return tree_find_min(&aux_tree);
}

void *tree_find_max(BSTree *tree) {
  BSTree aux_tree;

  if (!tree || !tree->root) {
    return NULL;
  }

  if (!tree->root->right) {
    return tree->root->info;
  }

  aux_tree.root = tree->root->right;
  aux_tree.print_ele = tree->print_ele;
  aux_tree.cmp_ele = tree->cmp_ele;

  return tree_find_max(&aux_tree);
}

Bool tree_contains(BSTree *tree, const void *elem) {
  BSTree aux_tree;
  int cmp;

  if (!tree || !tree->root || !elem) {
    return FALSE;
  }

  cmp = tree->cmp_ele(elem, tree->root->info);
  if (cmp == 0) {
    return TRUE;
  }

  aux_tree.print_ele = tree->print_ele;
  aux_tree.cmp_ele = tree->cmp_ele;
  aux_tree.root = (cmp < 0) ? tree->root->left : tree->root->right;

  return tree_contains(&aux_tree, elem);
}

Status tree_insert(BSTree *tree, const void *elem) {
  BSTree aux_tree;
  BSTNode *new_node;
  int cmp;
  Status st;

  if (!tree || !elem) {
    return ERROR;
  }

  if (!tree->root) {
    new_node = _bst_node_new();
    if (!new_node) {
      return ERROR;
    }

    new_node->info = (void *)elem;
    tree->root = new_node;
    return OK;
  }

  cmp = tree->cmp_ele(elem, tree->root->info);
  if (cmp == 0) {
    return OK;
  }

  aux_tree.print_ele = tree->print_ele;
  aux_tree.cmp_ele = tree->cmp_ele;
  aux_tree.root = (cmp < 0) ? tree->root->left : tree->root->right;

  st = tree_insert(&aux_tree, elem);
  if (cmp < 0) {
    tree->root->left = aux_tree.root;
  } else {
    tree->root->right = aux_tree.root;
  }

  return st;
}

Status tree_remove(BSTree *tree, const void *elem) {
  BSTree aux_tree;
  BSTNode *aux_node;
  void *replacement;
  int cmp;
  Status st;

  if (!tree || !elem) {
    return ERROR;
  }

  if (!tree->root) {
    return OK;
  }

  cmp = tree->cmp_ele(elem, tree->root->info);

  if (cmp < 0 || cmp > 0) {
    aux_tree.print_ele = tree->print_ele;
    aux_tree.cmp_ele = tree->cmp_ele;
    aux_tree.root = (cmp < 0) ? tree->root->left : tree->root->right;

    st = tree_remove(&aux_tree, elem);
    if (cmp < 0) {
      tree->root->left = aux_tree.root;
    } else {
      tree->root->right = aux_tree.root;
    }

    return st;
  }

  if (!tree->root->left && !tree->root->right) {
    _bst_node_free(tree->root);
    tree->root = NULL;
    return OK;
  }

  if (!tree->root->left) {
    aux_node = tree->root->right;
    _bst_node_free(tree->root);
    tree->root = aux_node;
    return OK;
  }

  if (!tree->root->right) {
    aux_node = tree->root->left;
    _bst_node_free(tree->root);
    tree->root = aux_node;
    return OK;
  }

  aux_tree.root = tree->root->right;
  aux_tree.print_ele = tree->print_ele;
  aux_tree.cmp_ele = tree->cmp_ele;

  replacement = tree_find_min(&aux_tree);
  tree->root->info = replacement;
  st = tree_remove(&aux_tree, replacement);
  tree->root->right = aux_tree.root;

  return st;
}
