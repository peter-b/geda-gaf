/* Lepton EDA library
 * Copyright (C) 1998-2010 Ales Hvezda
 * Copyright (C) 1998-2015 gEDA Contributors
 * Copyright (C) 2017-2021 Lepton EDA Contributors
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
/*! \file undo.c
 */

#include <config.h>

#include <stdio.h>
#include <ctype.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "liblepton_priv.h"

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
LeptonUndo*
s_undo_return_tail (LeptonUndo *head)
{
  LeptonUndo *u_current=NULL;
  LeptonUndo *ret_struct=NULL;

  u_current = head;
  while ( u_current != NULL ) { /* goto end of list */
    ret_struct = u_current;
    u_current = u_current->next;
  }

  return(ret_struct);
}

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
LeptonUndo*
s_undo_return_head(LeptonUndo *tail)
{
  LeptonUndo *u_current=NULL;
  LeptonUndo *ret_struct=NULL;

  u_current = tail;
  while ( u_current != NULL ) { /* goto end of list */
    ret_struct = u_current;
    u_current = u_current->prev;
  }

  return(ret_struct);
}

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
LeptonUndo*
s_undo_new_head(void)
{
  LeptonUndo *u_new;

  u_new = (LeptonUndo *) g_malloc (sizeof (LeptonUndo));
  u_new->type = -1;
  u_new->filename = NULL;
  u_new->object_list = NULL;
  u_new->x = u_new->y = 0;
  u_new->scale = 0;

  u_new->page_control = 0;
  u_new->up = -2;

  u_new->prev = NULL;
  u_new->next = NULL;

  return(u_new);
}

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
void
s_undo_destroy_head (LeptonUndo *u_head)
{
  g_free(u_head);
}

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
LeptonUndo*
s_undo_add (LeptonUndo *head,
            int type,
            char *filename,
            GList *object_list,
            int x,
            int y,
            double scale,
            int page_control,
            int up)
{
  LeptonUndo *tail;
  LeptonUndo *u_new;

  u_new = (LeptonUndo *) g_malloc (sizeof (LeptonUndo));

  u_new->filename = g_strdup (filename);

  u_new->object_list = object_list;

  u_new->type = type;

  u_new->x = x;
  u_new->y = y;
  u_new->scale = scale;

  u_new->page_control = page_control;
  u_new->up = up;

  if (head == NULL) {
    u_new->prev = NULL; /* setup previous link */
    u_new->next = NULL;
    return(u_new);
  } else {
    tail = s_undo_return_tail(head);
    u_new->prev = tail; /* setup previous link */
    u_new->next = NULL;
    tail->next = u_new;
    return(tail->next);
  }
}

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
void
s_undo_print_all (LeptonUndo *head)
{
  LeptonUndo *u_current;

  u_current = head;

  printf("START printing undo ********************\n");
  printf("BOTTOM\n");
  while(u_current != NULL) {

    if (u_current->filename) printf("%s\n", u_current->filename);

    if (u_current->object_list) {
      lepton_object_list_print (u_current->object_list);
    }

    printf("\t%d %d %f\n", u_current->x, u_current->y, u_current->scale);
    u_current = u_current->next;
  }
  printf("TOS\n");
  printf("Number of levels: %d\n", s_undo_levels(head));
  printf("DONE printing undo ********************\n");
  printf("\n");

}

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
void
s_undo_destroy_all (LeptonUndo *head)
{
  LeptonUndo *u_current;
  LeptonUndo *u_prev;

  u_current = s_undo_return_tail(head);

  while (u_current != NULL) {
    u_prev = u_current->prev;
    g_free(u_current->filename);

    if (u_current->object_list) {
      lepton_object_list_delete (u_current->object_list);
      u_current->object_list = NULL;
    }

    g_free(u_current);
    u_current = u_prev;
  }
}


/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
void
s_undo_remove_rest (LeptonUndo *head)
{
  LeptonUndo *u_current;
  LeptonUndo *u_next;

  u_current = head;

  while (u_current != NULL) {
    u_next = u_current->next;

    if (u_current->filename) {
      unlink(u_current->filename);
      g_free(u_current->filename);
    }

    if (u_current->object_list) {
      lepton_object_list_delete (u_current->object_list);
      u_current->object_list = NULL;
    }

    g_free(u_current);
    u_current = u_next;
  }
}

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
int
s_undo_levels (LeptonUndo *head)
{
  LeptonUndo *u_current;
  int count = 0;

  u_current = head;
  while (u_current != NULL) {
    if (u_current->filename || u_current->object_list) {
      count++;
    }

    u_current = u_current->next;
  }

  return(count);
}

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
void s_undo_init (LeptonPage *p_current)
{
  p_current->undo_tos = p_current->undo_bottom = NULL;
  p_current->undo_current = NULL;
}

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
void
s_undo_free_all (LeptonPage *p_current)
{
  s_undo_destroy_all (p_current->undo_bottom);
  p_current->undo_bottom = NULL;
  p_current->undo_tos = NULL;
  p_current->undo_current = NULL;
}
