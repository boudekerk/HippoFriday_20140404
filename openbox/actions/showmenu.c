#include "openbox/actions.h"
#include "openbox/menu.h"
#include <glib.h>

typedef struct {
    gchar   *name;
} Options;

static gpointer setup_func(xmlNodePtr node);
static void     free_func(gpointer options);
static gboolean run_func(ObActionsData *data, gpointer options);

void action_showmenu_startup(void)
{
    actions_register("ShowMenu", setup_func, free_func, run_func);
}

static gpointer setup_func(xmlNodePtr node)
{
    xmlNodePtr n;
    Options *o;

    o = g_slice_new0(Options);

    if ((n = obt_xml_find_node(node, "menu")))
        o->name = obt_xml_node_string(n);
    return o;
}

static void free_func(gpointer options)
{
    Options *o = options;
    g_free(o->name);
    g_slice_free(Options, o);
}

/* Always return FALSE because its not interactive */
static gboolean run_func(ObActionsData *data, gpointer options)
{
    Options *o = options;

    /* you cannot call ShowMenu from inside a menu */
    if (data->uact != OB_USER_ACTION_MENU_SELECTION && o->name)
        menu_show(o->name, data->x, data->y, data->button != 0, data->client);

    return FALSE;
}
