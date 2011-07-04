/* linkedlist-show.c -- associative array implemented as ordered linked list */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memento.h"

#include "types.h"
#include "string.h"
#include "string-kv.h"

#include "linkedlist.h"

#include "linkedlist-impl.h"

/* ----------------------------------------------------------------------- */

typedef struct linkedlist__show_args
{
  linkedlist_show_key     *key;
  linkedlist_show_destroy *key_destroy;
  linkedlist_show_value   *value;
  linkedlist_show_destroy *value_destroy;
  FILE                    *f;
}
linkedlist__show_args_t;

static error linkedlist__node_show(linkedlist__node_t *n, void *opaque)
{
  linkedlist__show_args_t *args = opaque;
  const char              *key;
  const char              *value;

  key   = args->key   && n->item.key   ? args->key(n->item.key)     : NULL;
  value = args->value && n->item.value ? args->value(n->item.value) : NULL;

  (void) fprintf(args->f, "linkedlist: %p: %s -> %s\n", n,
                 key   ? key   : "(null)",
                 value ? value : "(null)");

  if (args->key_destroy   && key)   args->key_destroy((char *) key);
  if (args->value_destroy && value) args->value_destroy((char *) value);

  return error_OK;
}

error linkedlist_show(const linkedlist_t      *t,
                      linkedlist_show_key     *key,
                      linkedlist_show_destroy *key_destroy,
                      linkedlist_show_value   *value,
                      linkedlist_show_destroy *value_destroy,
                      FILE                    *f)
{
  linkedlist__show_args_t args;

  args.key           = key;
  args.key_destroy   = key_destroy;
  args.value         = value;
  args.value_destroy = value_destroy;
  args.f             = f;

  // wah wah wah - this will fail as stringkv_fmt has a single internal buffer

  if (args.key == NULL)
    args.key = stringkv_fmt;
  if (args.value == NULL)
    args.value = stringkv_fmt;
  if (args.key_destroy == NULL)
    args.key_destroy = stringkv_fmt_destroy;
  if (args.value_destroy == NULL)
    args.value_destroy = stringkv_fmt_destroy;

  return linkedlist__walk_internal((linkedlist_t *) t,
                                   linkedlist__node_show,
                                   &args);
}

/* ----------------------------------------------------------------------- */