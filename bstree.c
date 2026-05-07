#include <stdio.h>
#include <stdlib.h>

#include "bstree.h"
#include "music.h"

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
static BSTNode *_bst_node_new(void);
static void _bst_node_free(BSTNode *pn);
static void _bst_node_free_rec(BSTNode *pn);
static int _bst_depth_rec(BSTNode *pn);
static int _bst_size_rec(BSTNode *pn);
static int _bst_preOrder_rec(BSTNode *pn, FILE *pf, P_ele_print print_ele);
static int _bst_inOrder_rec(BSTNode *pn, FILE *pf, P_ele_print print_ele);
static int _bst_postOrder_rec(BSTNode *pn, FILE *pf, P_ele_print print_ele);
static void *_bst_find_min_rec(BSTNode *pn);
static void *_bst_find_max_rec(BSTNode *pn);
static Bool _bst_contains_rec(BSTNode *pn, const void *elem, P_ele_cmp cmp);
static BSTNode *_bst_insert_rec(BSTNode *pn, const void *elem, P_ele_cmp cmp,
                                Status *status);
static BSTNode *_bst_remove_rec(BSTNode *pn, const void *elem, P_ele_cmp cmp);
static Status _tree_rangeSearch_rec(BSTNode *node, void *min, void *max,
                                    List *list, P_ele_cmp cmp);
static int _tree_countLongSongs_rec(BSTNode *node, int min_duration);

static BSTNode *_bst_node_new(void) {
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

static void _bst_node_free(BSTNode *pn) {
  if (!pn) {
    return;
  }

  free(pn);
}

static void _bst_node_free_rec(BSTNode *pn) {
  if (!pn) {
    return;
  }

  _bst_node_free_rec(pn->left);
  _bst_node_free_rec(pn->right);
  _bst_node_free(pn);

  return;
}

static int _bst_depth_rec(BSTNode *pn) {
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

static int _bst_size_rec(BSTNode *pn) {
  int count = 0;

  if (!pn) {
    return count;
  }

  count += _bst_size_rec(pn->left);
  count += _bst_size_rec(pn->right);

  return count + 1;
}

static int _bst_preOrder_rec(BSTNode *pn, FILE *pf, P_ele_print print_ele) {
  int count = 0;

  if (!pn) {
    return count;
  }

  count += print_ele(pf, pn->info);
  count += _bst_preOrder_rec(pn->left, pf, print_ele);
  count += _bst_preOrder_rec(pn->right, pf, print_ele);

  return count;
}

static int _bst_inOrder_rec(BSTNode *pn, FILE *pf, P_ele_print print_ele) {
  int count = 0;

  if (!pn) {
    return count;
  }

  count += _bst_inOrder_rec(pn->left, pf, print_ele);
  count += print_ele(pf, pn->info);
  count += _bst_inOrder_rec(pn->right, pf, print_ele);

  return count;
}

static int _bst_postOrder_rec(BSTNode *pn, FILE *pf, P_ele_print print_ele) {
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

static void *_bst_find_min_rec(BSTNode *pn) {
  if (!pn) {
    return NULL;
  }

  if (!pn->left) {
    return pn->info;
  }

  return _bst_find_min_rec(pn->left);
}

static void *_bst_find_max_rec(BSTNode *pn) {
  if (!pn) {
    return NULL;
  }

  if (!pn->right) {
    return pn->info;
  }

  return _bst_find_max_rec(pn->right);
}

static Bool _bst_contains_rec(BSTNode *pn, const void *elem, P_ele_cmp cmp) {
  int cmp_value;

  if (!pn || !elem || !cmp) {
    return FALSE;
  }

  cmp_value = cmp(elem, pn->info);
  if (cmp_value == 0) {
    return TRUE;
  }

  if (cmp_value < 0) {
    return _bst_contains_rec(pn->left, elem, cmp);
  }

  return _bst_contains_rec(pn->right, elem, cmp);
}

static BSTNode *_bst_insert_rec(BSTNode *pn, const void *elem, P_ele_cmp cmp,
                                Status *status) {
  BSTNode *new_node;
  int cmp_value;

  if (!elem || !cmp || !status) {
    return pn;
  }

  if (!pn) {
    new_node = _bst_node_new();
    if (!new_node) {
      *status = ERROR;
      return NULL;
    }

    new_node->info = (void *)elem;
    *status = OK;
    return new_node;
  }

  cmp_value = cmp(elem, pn->info);
  if (cmp_value < 0) {
    pn->left = _bst_insert_rec(pn->left, elem, cmp, status);
  } else if (cmp_value > 0) {
    pn->right = _bst_insert_rec(pn->right, elem, cmp, status);
  } else {
    *status = OK;
  }

  return pn;
}

static BSTNode *_bst_remove_rec(BSTNode *pn, const void *elem, P_ele_cmp cmp) {
  BSTNode *aux_node;
  void *replacement;
  int cmp_value;

  if (!pn) {
    return NULL;
  }

  cmp_value = cmp(elem, pn->info);
  if (cmp_value < 0) {
    pn->left = _bst_remove_rec(pn->left, elem, cmp);
    return pn;
  }

  if (cmp_value > 0) {
    pn->right = _bst_remove_rec(pn->right, elem, cmp);
    return pn;
  }

  if (!pn->left && !pn->right) {
    _bst_node_free(pn);
    return NULL;
  }

  if (!pn->left) {
    aux_node = pn->right;
    _bst_node_free(pn);
    return aux_node;
  }

  if (!pn->right) {
    aux_node = pn->left;
    _bst_node_free(pn);
    return aux_node;
  }

  replacement = _bst_find_min_rec(pn->right);
  pn->info = replacement;
  pn->right = _bst_remove_rec(pn->right, replacement, cmp);

  return pn;
}

void *tree_find_min(BSTree *tree) {
  if (!tree) {
    return NULL;
  }

  return _bst_find_min_rec(tree->root);
}

void *tree_find_max(BSTree *tree) {
  if (!tree) {
    return NULL;
  }

  return _bst_find_max_rec(tree->root);
}

Bool tree_contains(BSTree *tree, const void *elem) {
  if (!tree || !elem) {
    return FALSE;
  }

  return _bst_contains_rec(tree->root, elem, tree->cmp_ele);
}

Status tree_insert(BSTree *tree, const void *elem) {
  Status status = OK;

  if (!tree || !elem) {
    return ERROR;
  }

  tree->root = _bst_insert_rec(tree->root, elem, tree->cmp_ele, &status);

  return status;
}

Status tree_remove(BSTree *tree, const void *elem) {
  if (!tree || !elem) {
    return ERROR;
  }

  tree->root = _bst_remove_rec(tree->root, elem, tree->cmp_ele);

  return OK;
}

List *tree_rangeSearch(const BSTree *tree, void *min, void *max) {
  List *list = NULL;

  if (!tree || !min || !max) {
    return NULL;
  }

  list = list_new();
  if (!list) {
    return NULL;
  }

  if (tree->cmp_ele(min, max) > 0) {
    return list;
  }

  if (_tree_rangeSearch_rec(tree->root, min, max, list, tree->cmp_ele) == ERROR) {
    list_free(list);
    return NULL;
  }

  return list;
}

static Status _tree_rangeSearch_rec(BSTNode *node, void *min, void *max,
                                    List *list, P_ele_cmp cmp) {
  int cmp_min, cmp_max;

  if (!node) {
    return OK;
  }

  if (!min || !max || !list || !cmp) {
    return ERROR;
  }

  cmp_min = cmp(node->info, min);
  cmp_max = cmp(node->info, max);

  if (cmp_min > 0) {
    if (_tree_rangeSearch_rec(node->left, min, max, list, cmp) == ERROR) {
      return ERROR;
    }
  }

  if (cmp_min >= 0 && cmp_max <= 0) {
    if (list_pushBack(list, node->info) == ERROR) {
      return ERROR;
    }
  }

  if (cmp_max < 0) {
    if (_tree_rangeSearch_rec(node->right, min, max, list, cmp) == ERROR) {
      return ERROR;
    }
  }

  return OK;
}

int tree_countLongSongs(const BSTree *tree, int min_duration) {
  if (!tree || min_duration < 0) {
    return -1;
  }

  return _tree_countLongSongs_rec(tree->root, min_duration);
}

static int _tree_countLongSongs_rec(BSTNode *node, int min_duration) {
  int count;

  if (!node) {
    return 0;
  }

  count = _tree_countLongSongs_rec(node->left, min_duration);
  count += _tree_countLongSongs_rec(node->right, min_duration);

  if (music_getDuration((Music *)node->info) > min_duration) {
    count++;
  }

  return count;
}
