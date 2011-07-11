/* critbit-impl.h -- associative array implemented as critbit tree */

#ifndef CRITBIT_IMPL_H
#define CRITBIT_IMPL_H

#include "types.h"

#include "item.h"

#include "critbit.h"

/* ----------------------------------------------------------------------- */

/* an external node */
typedef struct critbit__extnode
{
  item_t                   item;
  size_t                   keylen; // may not need this - check later
}
critbit__extnode_t;

/* an internal node */
typedef struct critbit__node
{
  /* using an array here rather than separate left,right elements makes some
   * operations more convenient */
  struct critbit__node    *child[2];  /* left, right children */
  int                      byte;      /* byte offset of critbit */
  unsigned char            otherbits; /* inverted mask of critbit */
}
critbit__node_t;

struct critbit
{
  critbit__node_t         *root;

  int                      intcount; /* count of internal nodes */
  int                      extcount; /* count of external nodes */

  const void              *default_value;

  critbit_compare         *compare;
  critbit_destroy_key     *destroy_key;
  critbit_destroy_value   *destroy_value;
};

/* ----------------------------------------------------------------------- */

/* These have a eversed sense than original paper/code. */
#define IS_INTERNAL(p) (((intptr_t) (p) & 1) == 0)
#define IS_EXTERNAL(p) (!IS_INTERNAL(p))
#define IS_LEAF(p) IS_EXTERNAL(p)

/* Take a node pointer and make it into an extnode. */
#define FROM_STORE(p) (critbit__extnode_t *) ((intptr_t) (p) - 1)
/* Take an extnode and make it into a node pointer. */
#define TO_STORE(p) (critbit__node_t *) ((intptr_t) (p) + 1)

/* ----------------------------------------------------------------------- */

/* internal tree walk functions. callback returns a pointer to a
 * critbit__node_t, so internal for that reason. */

typedef error (critbit__walk_internal_callback)(critbit__node_t *n,
                                                int              level,
                                                void            *opaque);

error critbit__walk_internal(critbit_t                       *t,
                             critbit_walk_flags               flags,
                             critbit__walk_internal_callback *cb,
                             void                            *opaque);

error critbit__breadthwalk_internal(critbit_t                       *t,
                                    critbit_walk_flags               flags,
                                    critbit__walk_internal_callback *cb,
                                    void                            *opaque);

/* ----------------------------------------------------------------------- */

#endif /* CRITBIT_IMPL_H */
