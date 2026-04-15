/* See LICENSE file for copyright and license details. */

// enable multi media key
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx = 0; /* border pixel of windows */
static const unsigned int snap     = 0; /* snap pixel */
static const int          showbar  = 1; /* 0 means no bar */
static const int          topbar   = 1; /* 0 means bottom bar */
// use font original name instead of fontconfig alias
/* emoji font must be at the second place to take effect!!! */
static const char *fonts[] = {
    "Noto Sans:size=10",
    "Noto Color Emoji:size=10",
    "Arimo Nerd Font:size=10",
    "Noto Sans SC:size=10",
};
static const char         col_gray1[] = "#222222";
static const char         col_gray2[] = "#444444";
static const char         col_gray3[] = "#868e96";
static const char         col_gray4[] = "#adb5bd";
static const char         col_cyan[]  = "#1c1b22";
static const unsigned int baralpha    = 0xee;
static const unsigned int borderalpha = OPAQUE;
static const char        *colors[][3] = {
    /*               fg         bg         border   */
    [SchemeNorm] = {col_gray3, col_cyan, col_gray2},
    [SchemeSel]  = {col_gray4, col_cyan, col_cyan },
};

static const unsigned int alphas[][3] = {
    /*               fg      bg        border*/
    [SchemeNorm] = {OPAQUE, baralpha, borderalpha},
    [SchemeSel]  = {OPAQUE, baralpha, borderalpha},
};

/* tagging */
static const char *tags[] = {"1 ", "2 󰇧", "3 󰘝", "4 ", "5 󰭹", "6 󰊖", "7 ", "8 󰒃", "9 󱘖"};

static const char ptagf[]  = "%s %s"; /* format of a tag label */
static const char etagf[]  = "[%s]";  /* format of an empty tag */
static const char ntagf[]  = "%s";    /* format of a non-empty non-selected tag */
static const int  lcaselbl = 0;       /* 1 means make tag label lowercase */

static const unsigned int ulinepad     = 5; /* horizontal padding between the underline and tag */
static const unsigned int ulinestroke  = 2; /* thickness / height of the underline */
static const unsigned int ulinevoffset = 0; /* how far above the bottom of the bar the line should appear */
static const int          ulineall     = 0; /* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     *	get WM info: xprop | grep -E "(WM_CLASS|WM_NAME)"
     *	  click app to finish
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    {"Alacritty",               NULL, NULL, 1 << 0, 0, -1},
    {"firefox",                 NULL, NULL, 1 << 1, 0, -1},
    {"eudic",                   NULL, NULL, 1 << 2, 0, -1},
    {"Localsend",               NULL, NULL, 1 << 2, 0, -1},
    {"Zathura",                 NULL, NULL, 1 << 3, 0, -1},
    {"Foliate",                 NULL, NULL, 1 << 3, 0, -1},
    {"wechat",                  NULL, NULL, 1 << 4, 0, -1},
    {"org.mozilla.Thunderbird", NULL, NULL, 1 << 4, 0, -1},
    {"Mail",                    NULL, NULL, 1 << 4, 0, -1},
    {"jetbrains-toolbox",       NULL, NULL, 1 << 5, 1, -1},
    {"jetbrains-studio",        NULL, NULL, 1 << 5, 0, -1},
    {"jetbrains-idea",          NULL, NULL, 1 << 5, 0, -1},
    {"Code",                    NULL, NULL, 1 << 5, 0, -1},
    {"Lutris",                  NULL, NULL, 1 << 5, 0, -1},
    {"wemeetapp",               NULL, NULL, 1 << 6, 0, -1},
    {"Virt-manager",            NULL, NULL, 1 << 6, 0, -1},
    {"KeePassXC",               NULL, NULL, 1 << 7, 0, -1},
    {"KeeWeb",                  NULL, NULL, 1 << 7, 0, -1},
    {"Clash-verge",             NULL, NULL, 1 << 8, 0, -1},
};

/* layout(s) */
static const float mfact          = 0.5; /* factor of master area size [0.05..0.95] */
static const int   nmaster        = 1;   /* number of clients in master area */
static const int   resizehints    = 0;   /* 1 means respect size hints in tiled resizals */
static const int   lockfullscreen = 1;   /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"󱣴 ", monocle}, /* monocle as default layout */
    {" ",  tile   }, /* stack layout */
    {"󰈺 ", NULL   }, /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                                                              \
    {MODKEY, KEY, view, {.ui = 1 << TAG}}, {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},                  \
        {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                                                              \
        {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                                                                     \
    {                                                                                                                  \
        .v = (const char *[]) {                                                                                        \
            "/bin/sh", "-c", cmd, NULL                                                                                 \
        }                                                                                                              \
    }

/* commands */
static char        dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]  = {"dmenu_run", "-m", dmenumon, NULL};
static const char *termcmd[]   = {"alacritty", NULL};
static const char *browser[]   = {"firefox", NULL};
static const char *locker[]    = {"slock", NULL};

static const Key keys[] = {
    /* modifier                     key        function        argument */
    {MODKEY,             XK_s,                     spawn,          {.v = dmenucmd}           },
    {MODKEY,             XK_Return,                spawn,          {.v = termcmd}            },
    {MODKEY,             XK_b,                     spawn,          {.v = browser}            },
    {MODKEY | ShiftMask, XK_b,                     togglebar,      {0}                       },
    {Mod1Mask,           XK_Tab,                   focusstack,     {.i = +1}                 },
    {MODKEY,             XK_j,                     focusstack,     {.i = +1}                 },
    {MODKEY,             XK_k,                     focusstack,     {.i = -1}                 },
    {MODKEY,             XK_i,                     incnmaster,     {.i = +1}                 },
    {MODKEY,             XK_d,                     incnmaster,     {.i = -1}                 },
    {MODKEY,             XK_bracketleft,           setmfact,       {.f = -0.05}              },
    {MODKEY,             XK_bracketright,          setmfact,       {.f = +0.05}              },
    {MODKEY,             XK_Return,                zoom,           {0}                       },
    {MODKEY,             XK_Tab,                   view,           {0}                       },
    {MODKEY | ShiftMask, XK_k,                     killclient,     {0}                       },
    {MODKEY,             XK_m,                     setlayout,      {.v = &layouts[0]}        },
    {MODKEY,             XK_t,                     setlayout,      {.v = &layouts[1]}        },
    {MODKEY,             XK_f,                     setlayout,      {.v = &layouts[2]}        },
    {MODKEY,             XK_o,                     setlayout,      {0}                       },
    {MODKEY,             XK_space,                 spawn,          SHCMD("j4-dmenu-desktop") },
    {MODKEY | ShiftMask, XK_space,                 togglefloating, {0}                       },
    {MODKEY,             XK_0,                     view,           {.ui = ~0}                },
    {MODKEY | ShiftMask, XK_0,                     tag,            {.ui = ~0}                },
    {MODKEY,             XK_comma,                 focusmon,       {.i = -1}                 },
    {MODKEY,             XK_period,                focusmon,       {.i = +1}                 },
    {MODKEY | ShiftMask, XK_comma,                 tagmon,         {.i = -1}                 },
    {MODKEY | ShiftMask, XK_period,                tagmon,         {.i = +1}                 },
    {MODKEY | ShiftMask, XK_f,                     togglefullscr,  {0}                       },
    {0,                  XF86XK_MonBrightnessDown, spawn,          SHCMD("bright_down.sh")   },
    {MODKEY,             XK_F1,                    spawn,          SHCMD("bright_down.sh")   },
    {0,                  XF86XK_MonBrightnessUp,   spawn,          SHCMD("bright_up.sh")     },
    {MODKEY,             XK_F2,                    spawn,          SHCMD("bright_up.sh")     },
    {0,                  XF86XK_AudioLowerVolume,  spawn,          SHCMD("volume_down.sh")   },
    {MODKEY,             XK_F3,                    spawn,          SHCMD("volume_down.sh")   },
    {0,                  XF86XK_AudioRaiseVolume,  spawn,          SHCMD("volume_up.sh")     },
    {MODKEY,             XK_F4,                    spawn,          SHCMD("volume_up.sh")     },
    {0,                  XF86XK_AudioMute,         spawn,          SHCMD("toggle_mute.sh")   },
    {MODKEY,             XK_F9,                    spawn,          SHCMD("notify-sysinfo.sh")},
    {MODKEY,             XK_F12,                   spawn,          SHCMD("sysops.sh")        },
    {MODKEY,             XK_a,                     spawn,          SHCMD("jumpwindow.sh")    },
    {MODKEY,             XK_e,                     spawn,          SHCMD("thunar")           },
    {MODKEY,             XK_c,                     spawn,          SHCMD("flameshot gui")    },
    {MODKEY,             XK_p,                     spawn,          SHCMD("screenshot.sh")    },
    {MODKEY | ShiftMask, XK_l,                     spawn,          {.v = locker}             },
    {MODKEY | ShiftMask, XK_q,                     quit,           {0}                       },
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3) TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5)
        TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7) TAGKEYS(XK_9, 8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function        argument */
    {ClkLtSymbol,   0,      Button1, setlayout,      {0}               },
    {ClkLtSymbol,   0,      Button3, setlayout,      {.v = &layouts[2]}},
    {ClkWinTitle,   0,      Button2, zoom,           {0}               },
    {ClkStatusText, 0,      Button2, spawn,          {.v = termcmd}    },
    {ClkClientWin,  MODKEY, Button1, movemouse,      {0}               },
    {ClkClientWin,  MODKEY, Button2, togglefloating, {0}               },
    {ClkClientWin,  MODKEY, Button3, resizemouse,    {0}               },
    {ClkTagBar,     0,      Button1, view,           {0}               },
    {ClkTagBar,     0,      Button3, toggleview,     {0}               },
    {ClkTagBar,     MODKEY, Button1, tag,            {0}               },
    {ClkTagBar,     MODKEY, Button3, toggletag,      {0}               },
};

static const char *ipcsockpath   = "/tmp/dwm.sock";
static IPCCommand  ipccommands[] = {IPCCOMMAND(view, 1, {ARG_TYPE_UINT}),
                                    IPCCOMMAND(toggleview, 1, {ARG_TYPE_UINT}),
                                    IPCCOMMAND(tag, 1, {ARG_TYPE_UINT}),
                                    IPCCOMMAND(toggletag, 1, {ARG_TYPE_UINT}),
                                    IPCCOMMAND(tagmon, 1, {ARG_TYPE_UINT}),
                                    IPCCOMMAND(focusmon, 1, {ARG_TYPE_SINT}),
                                    IPCCOMMAND(focusstack, 1, {ARG_TYPE_SINT}),
                                    IPCCOMMAND(zoom, 1, {ARG_TYPE_NONE}),
                                    IPCCOMMAND(incnmaster, 1, {ARG_TYPE_SINT}),
                                    IPCCOMMAND(killclient, 1, {ARG_TYPE_SINT}),
                                    IPCCOMMAND(togglefloating, 1, {ARG_TYPE_NONE}),
                                    IPCCOMMAND(setmfact, 1, {ARG_TYPE_FLOAT}),
                                    IPCCOMMAND(setlayoutsafe, 1, {ARG_TYPE_PTR}),
                                    IPCCOMMAND(quit, 1, {ARG_TYPE_NONE})};
