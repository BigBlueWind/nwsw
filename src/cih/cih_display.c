/**
 * @file      cih_display.c
 * @details   Copyright Powerwave Technologies, Inc., 2010
 * @brief     This file contains processing of messages which comes from USer
 *
 * @author    Madhusudana Rao Kata
 * @date      10-05-2010
 *
 */

#include "cih.h"

/* Global Commands */
static const char *global_scope_cmds[] =
{
  "exit",
  "export",
  "help",
  "info",
  "revert",
  "set",
  "get",
  NULL
};
static const char *global_scope_helpCmds[] =
{
  "Exit from base station",
  "Export",
  "help",
  "info",
  "revert",
  "set",
  "get",
  NULL_STR
};

/* Set Commands */
static const char *set_cmds[] =
{
  "set bsTable.",
  "set eNbTable.",
  NULL
};

static const char *set_helpCmds[] =
{
  "{bsNum(0-10),bsName(stringFormat)} Ex {0,pwav}",
  "{eNbNum(0-10),eNbName(stringFormat)} Ex {0,pwav}",
  "{abcNum(0-10),eNbName(stringFormat)} Ex {0,pwav}",
  "{defNum(0-10),eNbName(stringFormat)} Ex {0,pwav}",
  NULL_STR
};
/* SET -- BsTable Commands */
static const char *set_bstable_cmds[] =
{
  "set bsTable.Id",
  "set bsTable.Action",
  NULL
};

static const char *set_bstable_helpCmds[] =
{
  "{bsNum(0-10)}",
  "{bsName(stringFormat)} Ex {0,pwav}}",
  NULL_STR
};



/* Get Commands */
static const char *get_cmds[] =
{
  "get bsTable.",
  "get eNbTable.",
  NULL
};

static const char *get_helpCmds[] =
{
  "{bsNum(0-10),bsName(stringFormat)} Ex {0,pwav}",
  "{eNbNum(0-10),eNbName(stringFormat)} Ex {0,pwav}",
  "{abcNum(0-10),eNbName(stringFormat)} Ex {0,pwav}",
  "{defNum(0-10),eNbName(stringFormat)} Ex {0,pwav}",
  NULL_STR
};



TBOOL  initial_match(const char *line1, const char *line2, int word_end)
{
  if (word_end <= 0)
          return (TRUE);
  return (strncmp(line1, line2, word_end) EQ 0);
}

int add_stuff(WordCompletion *cpl, const char *line1, const char **list,const char **helplist,
    int word_end)
{
  int i, err;

  for (i = 0; list[i] NOTEQ NULL; i++)
  {
    if (initial_match(line1, list[i], word_end))
    {
      printf("\n%s\n", helplist[i]);
      err = cpl_add_completion(cpl, line1, 0, word_end,list[i] + word_end, "", "");
      if (err NOTEQ 0)
      {
        return (err);
      }
    }
  }
  return (0);
}

CPL_MATCH_FN(cmd_cpl_fn)
{
  /*
   * The MAX/MIN tests below are to make sure we have at least
   * enough characters to distinguish from other prefixes (MAX)
   * but only check MIN(what we have, what we're checking).
   */
  data = NULL;
  if (strncmp(line, "set ", sizeof("set")) EQ 0)
  {
    return (add_stuff(cpl, line, set_cmds, set_helpCmds ,word_end));
  }
  if (strncmp(line, "set bstable.", sizeof("set bstable.")) EQ 0)
  {
    return (add_stuff(cpl, line, set_bstable_cmds, set_bstable_helpCmds ,word_end));
  }

  if (strncmp(line, "get ", sizeof("get")) EQ 0)
  {
    return (add_stuff(cpl, line, get_cmds,get_helpCmds, word_end));
  }
  return (add_stuff(cpl, line, global_scope_cmds, global_scope_helpCmds ,word_end));
}

