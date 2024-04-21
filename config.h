/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 20;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 20;       /* vert inner gap between windows */
static const unsigned int gappoh    = 20;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static const int swallowfloating    = 1;        /* 1 means swallow floating windows by default */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "JetBrains Mono Medium:pixelsize=14:antialias=true:autohint=true", "JoyPixels:pixelsize=14:antialias=true:autohint=true", "Nerd mono:pixelsize=14:antialias=true:autohint=true"  };
static const char dmenufont[]       = { "JetBrains Mono Medium:pixelsize=14:antialias=true:autohint=true" };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#f94144";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask	iscentered  isfloating  isterminal  noswallow  monitor  scratch key */
	{ "Gimp",    NULL,     NULL,           0,			0,			1,          0,           0,        -1,		0 },
	{ "Firefox", NULL,     NULL,           1 << 8,		0,			0,          0,          -1,        -1,		0 },
	{ "St",      NULL,     NULL,           0,			0,			0,          1,           0,        -1,		0 },
	{ NULL,      "float",  NULL,		   0,			1,			1,          1,           0,        -1,		0 },
	{ NULL,      NULL,     "Event Tester", 0,			0,			0,          0,           1,        -1,		0 }, 
	{ NULL,		 NULL,	   "qalc",		   0,			1,			1,          1,           0,        -1,	  'q' },
	{ NULL,		 NULL,	   "scratchpad",   0,			1,			1,          1,           0,        -1,	  's' },
	{ "Mullvad VPN",		 NULL,	   NULL,    	   0,			0,			1,          0,           0,        -1,	  'v' },
	{ NULL,		 NULL,	   "media",		   0,			0,			1,          1,           0,        -1,	  'm' },
	{ NULL,		 NULL,	   "newsboat",	   0,			0,			0,          1,           0,        -1,	  'n' },
	{ "discord", NULL,	   NULL,		   0,			0,			0,          0,           0,        -1,	  'd' },
	/* { "open.spotify.com", NULL,	   NULL,	   0,			0,			0,          0,           0,        -1,	  'g' }, */
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "D[]",      deck },
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
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_v,     ACTION##stack, {.i = 0 } }, \
	/* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
	/* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
	/* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };

/*First arg only serves to match against key in rules*/
static const char *pad[] = {"s", "st", "-g", "100x25", "-t", "scratchpad", NULL}; 
static const char *qalc[] = {"q", "st", "-t", "qalc", "-g", "50x20", "-e", "qalc", "-i", NULL}; 
static const char *media[] = {"m", "st", "-t", "media", NULL}; 
static const char *newsboat[] = {"n", "st", "-e", "newsboat", NULL}; 
static const char *discordcmd[] = {"d", "discord", NULL}; 
/* static const char *spotify[] = {"g", "brave", "--app=https://open.spotify.com", NULL}; */ 

static Key keys[] = {
	/* modifier                     key        function        argument */
	/* { MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } }, */
	{ MODKEY,						XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,						XK_grave, togglescratch,  {.v = pad } },
	/* { MODKEY|ShiftMask,				XK_Return, togglescratch,  {.v = pad } }, */
	{ MODKEY|ShiftMask,				XK_semicolon, togglescratch,  {.v = pad } },
	{ MODKEY|ShiftMask,				XK_d,      togglescratch,  {.v = discordcmd } },
	/* { MODKEY|ShiftMask,				XK_s,      togglescratch,  {.v = spotify } }, */
	{ MODKEY,						XK_n,      togglescratch,  {.v = newsboat } },
	{ MODKEY,						XK_apostrophe, togglescratch,  {.v = qalc } },
	{ MODKEY,						XK_backslash, togglescratch,  {.v = media } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY|ShiftMask,             XK_j,      focusmon,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      focusmon,    {.i = -1 } },
	/* { MODKEY,                       XK_j,      focusstack,     {.i = +1 } }, */
	/* { MODKEY,                       XK_k,      focusstack,     {.i = -1 } }, */
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,			            XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.02} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.02} },
	/* { MODKEY,                       XK_Return, zoom,           {0} }, */
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY,			            XK_i,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY|ShiftMask,             XK_u,      setlayout,      {.v = &layouts[11]} },
	{ MODKEY,                       XK_space,  zoom,           {0} },
	{ MODKEY|ShiftMask,             XK_f,	   togglefloating, {0} },
	{ MODKEY,			            XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_F5,     xrdb,           {.v = NULL } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY,						XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_s,      togglesticky,   {0} },

	{ MODKEY,			            XK_z,      incrgaps,       {.i = +5 } },
	{ MODKEY,					    XK_x,      incrgaps,       {.i = -5 } },
	/* { MODKEY|Mod4Mask,              XK_i,      incrigaps,      {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,              XK_o,      incrogaps,      {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,              XK_6,      incrihgaps,     {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,              XK_7,      incrivgaps,     {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,              XK_8,      incrohgaps,     {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,              XK_9,      incrovgaps,     {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } }, */
	{ MODKEY,						XK_a,      togglegaps,     {0} },
	{ MODKEY|ShiftMask,				XK_a,	   defaultgaps,    {0} },

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

