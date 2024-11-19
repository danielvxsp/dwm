#include <X11/XF86keysym.h>

static const unsigned int borderpx  = 2;  /* border width */
static const unsigned int snap      = 2;  /* snap pixel */
static const unsigned int gappx     = 6;    
static const int showbar            = 1;   
static const int topbar             = 1; 
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       =   "monospace:size=10";

#include "/home/ninog/.cache/wal/colors-wal-dwm.h"

/* "workspaces" */
static const char *tags[] = { "1", "2", "3", "4" };

static const Rule rules[] = {
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
    { "mpv",      NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    
	{ "><>",      NULL },   
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = { "kitty", NULL };

static const Key keys[] = {
	/* modifier                     key        function            argument */
    { 0,                XF86XK_AudioLowerVolume, spawn,            SHCMD("pamixer --decrease 5") },
    { 0,                XF86XK_AudioRaiseVolume, spawn,            SHCMD("pamixer --increase 5") },
    { 0,                XF86XK_AudioMute,        spawn,            SHCMD("pamixer --toggle-mute") },
    { MODKEY,                       XK_d,      spawn,              {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,              {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,          {0} },
	{ MODKEY,                       XK_j,      focusstack,         {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,         {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_d,      incnmaster,         {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,           {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,           {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,               {0} },
	{ MODKEY,                       XK_Tab,    view,               {0} },
	{ MODKEY,                       XK_q,      killclient,         {0} },
	{ MODKEY,                       XK_t,      setlayout,          {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,          {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,          {.v = &layouts[2]} },
    { MODKEY,                       XK_space,  setlayout,          {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating,     {0} },
	{ MODKEY,                       XK_0,      view,               {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,                {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,           {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,           {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,             {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,             {.i = +1 } },
	TAGKEYS(                        XK_1,                          0)
	TAGKEYS(                        XK_2,                          1)
	TAGKEYS(                        XK_3,                          2)
	TAGKEYS(                        XK_4,                          3)
	{ MODKEY|ShiftMask,             XK_q,      quit,               {0} },
};

/* button definitions */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

