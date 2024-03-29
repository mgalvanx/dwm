/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "st"

/* appearance */
static unsigned int borderpx  = .5;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static unsigned int gappih    = 20;       /* horiz inner gap between windows */
static unsigned int gappiv    = 10;       /* vert inner gap between windows */
static unsigned int gappoh    = 0;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 0;       /* vert outer gap between windows and screen edge */
static int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static int showbar            = 0;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char font[]            = "monospace:size=10";
static char dmenufont[]       = "monospace:size=10";
static const char *fonts[]          = { font };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
       [SchemeTitle]  = { normfgcolor, normbgcolor,  normbgcolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class         instance  title           tags mask  iscentered  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",        NULL,     NULL,           0,         0,          1,          0,           0,        -1 },
	{ "firefox",     NULL,     NULL,           1<<1,      0,          0,          0,          -1,         0 },
	{ "firefox-esr", NULL,     NULL,           1<<1,      0,          0,          0,          -1,         2 },
	{ "thunderbird", NULL,     NULL,           1<<4,      0,          0,          0,          -1,        -1 },
	{ "mpv",         NULL,     "newsboat-mpv", 0,         0,          1,          0,           0,        -1 },
	{ "mpv",         NULL,     NULL,           0,         0,          1,          0,           0,        -1 },
	{ "libreoffice", NULL,     NULL,           0,         0,          0,          0,           0,        -1 },
	{ "St",          NULL,     NULL,           0,         0,          0,          1,           0,        -1 },
	{ "St",          NULL,     "nmtui",        0,         1,          1,          1,           0,        -1 },
	{ "St",          NULL,     "unlock",       0,         1,          1,          1,           0,         0 },
	{ "St",          NULL,     "phone",        0,         1,          1,          1,           0,        -1 },
	{ "St",          NULL,     "music",        0,         1,          1,          1,           0,        -1 },
	{ "St",          NULL,     "pulsemixer",   0,         1,          1,          1,           0,        -1 },
	{ "St",          NULL,     "home",         1,         1,          1,          1,           0,         0 },
	{ "St",          NULL,     "laptop",       1,         0,          0,          1,           0,         0 },
	{ "St",          NULL,     "term",         1,         0,          0,          1,           0,         2 },
	{ "St",          NULL,     "scratch",      2,         0,          0,          1,           0,         2 },
	{ "St",          NULL,     "system",       1,         0,          0,          1,           0,         1 },
	{ "scrcpy",      NULL,     NULL,           0,         0,          0,          1,           0,         1 },
	{ NULL,          NULL,     "Event Tester", 0,         0,          0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "font",               STRING,  &font },
		{ "dmenufont",          STRING,  &dmenufont },
		{ "color0",		STRING,	&normbordercolor },
		{ "color8",		STRING,	&selbordercolor },
		{ "color0",		STRING,	&normbgcolor },
		{ "color4",		STRING,	&normfgcolor },
		{ "color0",		STRING,	&selfgcolor },
		{ "color4",		STRING,	&selbgcolor },
		{ "borderpx",		INTEGER, &borderpx },
		{ "snap",		INTEGER, &snap },
		{ "showbar",		INTEGER, &showbar },
		{ "topbar",		INTEGER, &topbar },
		{ "nmaster",		INTEGER, &nmaster },
		{ "resizehints",	INTEGER, &resizehints },
		{ "mfact",		FLOAT,	&mfact },
		{ "gappih",		INTEGER, &gappih },
		{ "gappiv",		INTEGER, &gappiv },
		{ "gappoh",		INTEGER, &gappoh },
		{ "gappov",		INTEGER, &gappov },
		{ "swallowfloating",	INTEGER, &swallowfloating },
		{ "smartgaps",		INTEGER, &smartgaps },
};

#include "movestack.c"
#include "shiftview.c"
#include <X11/XF86keysym.h>

static Key keys[] = {
	/* modifier                     key        function        argument */

	{ MODKEY,                       XK_a,      view,     {0} },
	{ MODKEY,                      XK_s,      spawn,          SHCMD("pulsemixer-term") },
	{ MODKEY|ShiftMask,                      XK_s,      spawn,          SHCMD("tmux-system") },
	/*{ MODKEY|ShiftMask,             XK_a,      defaultgaps,    {0} },*/
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_d,      spawn,          SHCMD("passcheck") },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,		                    XK_f,      spawn,	  SHCMD("fmenu") }, 
/*	{ MODKEY|ShiftMask,		XK_f,      setlayout,	   {.v = &layouts[8]} },	*/
/*	{ MODKEY,		        XK_g,	   shiftview,  	   { .i = -1 } }, */

/* Vim Hot Key */
/*{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,		XK_semicolon,      shiftview,      { .i = 1 } },*/

/*Colemak Vim*/
	{ MODKEY,                       XK_m,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_n,      focusstack,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_n,      movestack,      {.i = +1 } },
	{ MODKEY,                       XK_e,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_e,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_i,      setmfact,       {.f = +0.05} },

	/*{ MODKEY,			XK_apostrophe,	togglescratch,	{.ui = 1} },*/

/*	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } }, */
/*	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } }, */
	{ MODKEY,                       XK_Return, spawn,          SHCMD("tmux-home") },
/*	{ MODKEY|ShiftMask,             XK_Return, spawn,          SHCMD("xterm") },*/
	{ MODKEY,                       XK_h, spawn,               SHCMD("tmux-term") }, 
	{ MODKEY|ShiftMask,             XK_h, spawn,               SHCMD("tmux-scratch") },
/*	{ MODKEY,                       XK_Tab, spawn,          SHCMD("tmux-term") }, */
 	/* { MODKEY,                       XK_Return, zoom,           {0} }, */

	/* modifier                     key        function        argument */
/*{ MODKEY,                       XK_q,      killclient,     {0} },*/

	{ MODKEY,                       XK_w,      spawn,          SHCMD("$BROWSER") },
  { MODKEY|ShiftMask,             XK_w,      spawn,          SHCMD("nmtui-term") },
	{ MODKEY,                       XK_p,      spawn,          SHCMD("maimshot") },
  { MODKEY|ShiftMask,             XK_p,      spawn,          SHCMD("maimpick") },



	{ MODKEY,			XK_t,	   setlayout,	   {.v = &layouts[0]} }, /* tile */
	{ MODKEY|ShiftMask,		XK_t,	   setlayout,	   {.v = &layouts[1]} }, /* bstack */
	{ MODKEY,			XK_y,      setlayout,	   {.v = &layouts[2]} }, /* spiral */
	{ MODKEY|ShiftMask,		XK_y,	   setlayout,	   {.v = &layouts[3]} }, /* dwindle */
	{ MODKEY,			XK_u,      setlayout,	   {.v = &layouts[4]} }, /* deck */
	{ MODKEY|ShiftMask,		XK_u,	   setlayout,	   {.v = &layouts[5]} }, /* monocle */
	{ MODKEY,			XK_l,      setlayout,	   {.v = &layouts[6]} }, /* centeredmaster */
	{ MODKEY|ShiftMask,		XK_l,      setlayout,	   {.v = &layouts[7]} }, /* centeredfloatingmaster */ 


/* Change back to i if using vim keys  
/*	{ MODKEY,			XK_i,      setlayout,	   {.v = &layouts[6]} }, /* centeredmaster */
/*	{ MODKEY|ShiftMask,		XK_i,      setlayout,	   {.v = &layouts[7]} }, /* centeredfloatingmaster */ 

	{ MODKEY,                       XK_b,      togglebar,      {0} },

	{ MODKEY,                       XK_o,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,      incnmaster,     {.i = -1 } },

	{ MODKEY,		                    XK_v,      spawn,	  SHCMD("mplay") }, 
	{ MODKEY|ShiftMask,		          XK_v,      spawn,	  SHCMD("mload") }, 

	{ MODKEY,                       XK_k, spawn,          SHCMD("pixel") },
	{ MODKEY|ShiftMask,             XK_k, spawn,          SHCMD("tmux-phone") },
	{ MODKEY,                       XK_j, spawn,          SHCMD("tmux-music") },
	/*
        { MODKEY,			XK_p,			spawn,		SHCMD("mpc toggle") },
	{ MODKEY|ShiftMask,		XK_p,			spawn,		SHCMD("mpc pause ; pauseallmpv") },
	{ MODKEY,			XK_bracketleft,		spawn,		SHCMD("mpc seek -10") },
	{ MODKEY|ShiftMask,		XK_bracketleft,		spawn,		SHCMD("mpc seek -60") },
	{ MODKEY,			XK_bracketright,	spawn,		SHCMD("mpc seek +10") },
	{ MODKEY|ShiftMask,		XK_bracketright,	spawn,		SHCMD("mpc seek +60") },
	{ MODKEY,			XK_backslash,		view,		{0} },
	{ MODKEY|ShiftMask,		XK_backslash,		spawn,		SHCMD("") }, */
  { MODKEY,                  XK_BackSpace,   spawn,          SHCMD("sysact") },

	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_F4,    defaultgaps,    {0} },
	{ MODKEY,                       XK_F5,    togglegaps,           {0} },
  { MODKEY,			XK_z,             incrgaps,	   {.i = +3 } },
	{ MODKEY,			XK_x,             incrgaps,	   {.i = -3 } },
	{ MODKEY,                       XK_c,      spawn,          SHCMD("comments") }, 
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} }, 

	/* { MODKEY,			XK_v,		spawn,		SHCMD("") }, */
  /*{ MODKEY,       XK_n,      spawn,          SHCMD(TERMINAL " -e worknote") },*/
	/*{ MODKEY,			XK_n,		spawn,		SHCMD(TERMINAL " -e nvim -c VimwikiIndex") },
	{ MODKEY|ShiftMask,		XK_n,		spawn,		SHCMD(TERMINAL " -e newsboat; pkill -RTMIN+6 dwmblocks") },
	{ MODKEY,			XK_m,		spawn,		SHCMD(TERMINAL " -e ncmpcpp") },
	{ MODKEY|ShiftMask,		XK_m,		spawn,		SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") },*/

	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
  { MODKEY,		        XK_slash,  spawn,	   SHCMD("dlynx") }, 
	{ MODKEY|ShiftMask,		XK_slash,  spawn,	   SHCMD("dmyt") }, 
/*	{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },*/
	{ MODKEY,                       XK_space,  zoom,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

  { MODKEY,			XK_minus,	spawn,		SHCMD("pamixer --allow-boost -d 5; notify-send 'Decreasing Volume by 5'") },
	{ MODKEY|ShiftMask,		XK_minus,	spawn,		SHCMD("pamixer --allow-boost -d 15; notify-send 'Decreasing Volume by 15") },
	{ MODKEY,			XK_equal,	spawn,		SHCMD("pamixer --allow-boost -i 5; notify-send 'Increasing Volume by 5'") },
	{ MODKEY|ShiftMask,		XK_equal,	spawn,		SHCMD("pamixer --allow-boost -i 15; notify-send 'Increasing Volume by 15'") },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|Mod4Mask,              XK_u,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_i,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_o,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_6,      incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_7,      incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_8,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_9,      incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
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

