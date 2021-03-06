/* --------------------------------------------------------------------------
 *    Name: destroy.c
 * Purpose: Associative array implemented as a binary search tree
 * ----------------------------------------------------------------------- */

#include <stdlib.h>

#include "base/memento/memento.h"
#include "base/errors.h"
#include "base/types.h"

#include "datastruct/bstree.h"

#include "impl.h"

static error bstree__destroy_node(bstree__node_t *n, int level, void *opaque)
{
  NOT_USED(level);

  bstree__node_destroy(opaque, n);

  return error_OK;
}

void bstree_destroy(bstree_t *t)
{
  (void) bstree__walk_internal_post(t, bstree__destroy_node, t);

  free(t);
}

