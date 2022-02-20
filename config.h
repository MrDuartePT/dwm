/*  __     __ ____   */
/* |  \   /  |  _ \  */ 
/* | |\\_//| | | | | Gon√alo Duarte*/
/* | | \_/ | | |_| | https://github.com/MrDuartePT/mrduarte_dwm.git*/
/* |_|     |_|____/  */ 

/* See LICENSE file for copyright and license details. */

/*Includes*/

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;     /* 0 means no bar */
static const int topbar             = 1;     /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

static const char *const autostart[] = {
	"/home/mrduarte/autostart.sh", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[F]",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTMOD Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },


/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *shutcmd[] = { "poweroff", NULL };

/* Open APPS */
static const char *filebrowse[] = { "thunar", NULL };
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *browser[] = { "firefox", NULL };
static const char *discord[] = { "discord", NULL };
static const char *xppen[] = { "/usr/lib/pentablet/pentablet", NULL };
static const char *screenshoot[] = { "bash", "/home/mrduarte/.dmenu/dmenu-scrot.sh", NULL };
static const char *pdf[] = { "okular", NULL };
static const char *journal[] = { "xournal" , NULL };

/* Volume Control */
static const char *upvol[]   = { "/usr/bin/pactl", "set-sink-volume", "0", "+5%",     NULL };
static const char *downvol[] = { "/usr/bin/pactl", "set-sink-volume", "0", "-5%",     NULL };
static const char *mutevol[] = { "/usr/bin/pactl", "set-sink-mute",   "0", "toggle",  NULL };
static const char *upvol1per[]   = { "/usr/bin/pactl", "set-sink-volume", "0", "+1%",   NULL };
static const char *downvol1per[] = { "/usr/bin/pactl", "set-sink-volume", "0", "-1%",   NULL };

/* KB & Screen Backlight Control */
static const char *upDisBrit[]   = { "xbacklight", "-ctrl", "intel_backlight", "-inc", "5", NULL };
static const char *downDisBrit[] = { "xbacklight", "-ctrl", "intel_backlight", "-dec", "5", NULL };
static const char *upDisBrit1per[]   = { "xbacklight", "-ctrl", "intel_backlight", "-inc", "1", NULL };
static const char *downDisBrit1per[] = { "xbacklight", "-ctrl", "intel_backlight", "-dec", "1",  NULL };
static const char *upKBBrit[]   = { "xbacklight", "-ctrl", "smc::kbd_backlight", "-inc", "5", NULL };
static const char *downKBBrit[] = { "xbacklight", "-ctrl", "smc::kbd_backlight", "-dec", "5", NULL };
static const char *upKBBrit1per[]   = { "xbacklight", "-ctrl", "smc::kbd_backlight", "-inc", "1", NULL };
static const char *downKBBrit1per[] = { "xbacklight", "-ctrl", "smc::kbd_backlight", "-dec", "1",  NULL };


/* KB Layouts */
static const char *ptlayout[] = { "setxkbmap", "pt",	NULL};
static const char *enlayout[] = { "setxkbmap", "us",    NULL};


#include "selfrestart.c"

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,			XK_f,      spawn,          {.v = filebrowse } },
        { MODKEY,                       XK_d,      spawn,          {.v = discord } },
        { MODKEY,                       XK_b,      spawn,          {.v = browser } },
        { MODKEY|ShiftMask,             XK_x,      spawn,          {.v = xppen } },
        { MODKEY,                       XK_p,      spawn,          {.v = pdf } },
        { MODKEY,                       XK_o,      spawn,          {.v = journal } },
        { MODKEY|ShiftMask|ControlMask, XF86XK_PowerOff,  spawn,   {.v = shutcmd } },
        { MODKEY|ShiftMask,             XK_backslash,     spawn,   {.v = screenshoot } },
	{ MODKEY,			XK_space,  spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
        { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
        { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_Right,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_Left,   focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Right,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Left,   tagmon,         {.i = +1 } },
        { MODKEY|ControlMask,           XK_comma,  cyclelayout,    {.i = -1 } },
        { MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
    	{ MODKEY|ShiftMask,             XK_q,      self_restart,   {0} }, 
	{ 0,                       	XF86XK_AudioLowerVolume, spawn,   {.v = downvol } },
	{ 0,                      	XF86XK_AudioMute,        spawn,   {.v = mutevol } },
	{ 0,                       	XF86XK_AudioRaiseVolume, spawn,   {.v = upvol   } },
	{ MODKEY,                       XF86XK_AudioLowerVolume, spawn,   {.v = downvol1per } },
	{ MODKEY,                       XF86XK_AudioRaiseVolume, spawn,   {.v = upvol1per } },
	{ 0,                            XF86XK_MonBrightnessDown,spawn,   {.v = downDisBrit } },
        { 0,                            XF86XK_MonBrightnessUp,  spawn,   {.v = upDisBrit   } },
        { MODKEY,                       XF86XK_MonBrightnessDown,spawn,   {.v = downDisBrit1per } },
	{ MODKEY,                       XF86XK_MonBrightnessUp,  spawn,   {.v = upDisBrit1per } },
	{ 0,                            XF86XK_KbdBrightnessDown, spawn,   {.v = downKBBrit } },
        { 0,                            XF86XK_KbdBrightnessUp,   spawn,   {.v = upKBBrit   } },
        { MODKEY,                       XF86XK_KbdBrightnessDown, spawn,   {.v = downKBBrit1per } },
	{ MODKEY,                       XF86XK_KbdBrightnessUp,   spawn,   {.v = upKBBrit1per } },
	{ MODKEY|ShiftMask,             XF86XK_LaunchA, 	 spawn,   {.v = ptlayout} },
	{ MODKEY,                       XF86XK_LaunchA, 	 spawn,   {.v = enlayout} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },

};
