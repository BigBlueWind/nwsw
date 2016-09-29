/** @file uglib.h
*/

#pragma once

typedef unsigned int guint32;
typedef signed char gint8;
typedef unsigned char guint8;
typedef signed short gint16;
typedef unsigned short guint16;
typedef signed int gint32;

typedef void* gpointer;
typedef const void *gconstpointer;

typedef char   gchar;
typedef short  gshort;
typedef long   glong;
typedef int    gint;
typedef gint   gboolean;

typedef unsigned char   guchar;
typedef unsigned short  gushort;
typedef unsigned long   gulong;
typedef unsigned int    guint;

typedef float   gfloat;
typedef double  gdouble;

typedef unsigned int uint32_t;
typedef int gint32_t;

typedef unsigned int gsize;

#define GPOINTER_TO_INT(p)	((gint)   (p))
#define GINT_TO_POINTER(i)	((gpointer)  (i))

typedef gint            (*GCompareFunc)         (gconstpointer  a,
                                                 gconstpointer  b);
typedef gint            (*GCompareDataFunc)     (gconstpointer  a,
                                                 gconstpointer  b,
						 gpointer       user_data);
typedef void            (*GFunc)                (gpointer       data,
                                                 gpointer       user_data);

typedef struct _GList GList;

struct _GList
{
  gpointer data;
  GList *next;
  GList *prev;
};

GList*   g_list_find_custom             (GList            *list,
					 gconstpointer     data,
					 GCompareFunc      func);
GList*   g_list_insert_sorted           (GList            *list,
					 gpointer          data,
					 GCompareFunc      func);
GList*   g_list_first                   (GList            *list);
GList*   g_list_last                    (GList            *list);
#define g_list_next(list)	        ((list) ? (((GList *)(list))->next) : NULL)
#define g_list_previous(list)	        ((list) ? (((GList *)(list))->prev) : NULL)
GList*   g_list_remove                  (GList            *list,
					 gconstpointer     data);
void     g_list_foreach                 (GList            *list,
					 GFunc             func,
					 gpointer          user_data);
void     g_list_free                    (GList            *list);
gpointer g_list_nth_data                (GList            *list,
					 guint             n);
guint    g_list_length                  (GList            *list);
GList*   g_list_nth                     (GList            *list,
					 guint             n);
GList*   g_list_append                  (GList            *list,
					 gpointer          data);
gint     g_list_position                (GList            *list,
					 GList            *llink);
guint    g_list_length                  (GList            *list);
GList*   g_list_remove_link             (GList            *list,
					 GList            *llink);
void     g_list_free_1                  (GList            *list);
GList*   g_list_find                    (GList            *list,
					 gconstpointer     data);

typedef struct _GArray		GArray;
typedef struct _GByteArray	GByteArray;
typedef struct _GPtrArray	GPtrArray;

struct _GArray
{
  gchar *data;
  guint len;
};

struct _GByteArray
{
  guint8 *data;
  guint	  len;
};

struct _GPtrArray
{
  gpointer *pdata;
  guint	    len;
};

GArray* g_array_new               (gboolean          zero_terminated,
				   gboolean          clear_,
				   guint             element_size);
GArray* g_array_sized_new         (gboolean          zero_terminated,
				   gboolean          clear_,
				   guint             element_size,
				   guint             reserved_size);
gchar*  g_array_free              (GArray           *array,
				   gboolean          free_segment);
GArray* g_array_append_vals       (GArray           *array,
				   gconstpointer     data,
				   guint             len);
GArray* g_array_insert_vals       (GArray           *array,
				   guint             index_,
				   gconstpointer     data,
				   guint             len);
#define g_array_append_val(a,v)	  g_array_append_vals (a, &(v), 1)
#define g_array_index(a,t,i)      (((t*) (void *) (a)->data) [(i)])
#define g_array_insert_val(a,i,v) g_array_insert_vals (a, i, &(v), 1)

#undef	MAX
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

#undef	MIN
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

#undef	ABS
#define ABS(a)	   (((a) < 0) ? -(a) : (a))

#  define G_STRUCT_OFFSET(struct_type, member)	\
      ((glong) ((guint8*) &((struct_type*) 0)->member))

gpointer g_slice_alloc          	(gsize	       block_size);
gpointer g_slice_alloc0         	(gsize         block_size);
void     g_slice_free_chain_with_offset (gsize         block_size,
					 gpointer      mem_chain,
					 gsize         next_offset);
void     g_slice_free1          	(gsize         block_size,
					 gpointer      mem_block);

#define  g_slice_new(type)      ((type*) g_slice_alloc (sizeof (type)))
#define  g_slice_new0(type)     ((type*) g_slice_alloc0 (sizeof (type)))
#define g_slice_free_chain(type, mem_chain, next)	do {	\
  if (1) g_slice_free_chain_with_offset (sizeof (type),		\
                 (mem_chain), G_STRUCT_OFFSET (type, next)); 	\
  else   (void) ((type*) 0 == (mem_chain));			\
} while (0)
#define g_slice_free(type, mem)				do {	\
  if (1) g_slice_free1 (sizeof (type), (mem));			\
  else   (void) ((type*) 0 == (mem)); 				\
} while (0)

#if !(defined (G_STMT_START) && defined (G_STMT_END))
#  define G_STMT_START  do
#  define G_STMT_END    while (0)
#endif
#define G_UNLIKELY(expr) (expr)
#define G_LIKELY(expr) (expr)

#define g_return_val_if_fail(expr,val)		G_STMT_START{ (void)0; }G_STMT_END

void	 g_free	          (gpointer	 mem);
gpointer g_realloc        (gpointer	 mem,
			   gsize	 n_bytes);

#define g_memmove(dest,src,len) G_STMT_START { memmove ((dest), (src), (len)); } G_STMT_END
