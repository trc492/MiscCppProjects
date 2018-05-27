#if 0
/// Copyright (c) Microsoft Corporation. All rights reserved.
///
/// <module name="wtrace.h" />
///
/// <summary>
///     This module contains the free version of WPP tracing support.
/// </summary>
///
/// <remarks>
///     Environment: User/Kernel mode
/// </remarks>
#endif

#pragma once

#ifndef WPP_KERNEL_MODE
  #define WPP_ENUM_WMMSG
#endif

//
// Macros.
//

#ifdef WPP_KERNEL_MODE
  #define DEBUGBREAK            DbgBreakPoint
#else
  #define DEBUGBREAK            DebugBreak
#endif
#define GETLASTHRESULT()        HRESULT_FROM_WIN32(GetLastError())

//
// Type name enumerations.
//

#ifdef WPP_ENUM_WMMSG

#include <winuser.h>
typedef enum _WMMsg
{
    Wm_Null                     = WM_NULL,
    Wm_Create                   = WM_CREATE,
    Wm_Destroy                  = WM_DESTROY,
    Wm_Move                     = WM_MOVE,
    Wm_Size                     = WM_SIZE,
    Wm_Activate                 = WM_ACTIVATE,
    Wm_SetFocus                 = WM_SETFOCUS,
    Wm_KillFocus                = WM_KILLFOCUS,
    Wm_Enable                   = WM_ENABLE,
    Wm_SetRedraw                = WM_SETREDRAW,
    Wm_SetText                  = WM_SETTEXT,
    Wm_GetText                  = WM_GETTEXT,
    Wm_GetTextLen               = WM_GETTEXTLENGTH,
    Wm_Paint                    = WM_PAINT,
    Wm_Close                    = WM_CLOSE,
    Wm_QueryEndSession          = WM_QUERYENDSESSION,
    Wm_QueryOpen                = WM_QUERYOPEN,
    Wm_EndSession               = WM_ENDSESSION,
    Wm_Quit                     = WM_QUIT,
    Wm_EraseBackground          = WM_ERASEBKGND,
    Wm_SysColorChange           = WM_SYSCOLORCHANGE,
    Wm_ShowWindow               = WM_SHOWWINDOW,
    Wm_WinIniChange             = WM_WININICHANGE,
    Wm_SettingChange            = WM_SETTINGCHANGE,
    Wm_DevModeChange            = WM_DEVMODECHANGE,
    Wm_ActivateApp              = WM_ACTIVATEAPP,
    Wm_FontChange               = WM_FONTCHANGE,
    Wm_TimeChange               = WM_TIMECHANGE,
    Wm_CancelMode               = WM_CANCELMODE,
    Wm_SetCursor                = WM_SETCURSOR,
    Wm_MouseActivate            = WM_MOUSEACTIVATE,
    Wm_ChildActivate            = WM_CHILDACTIVATE,
    Wm_QueueSync                = WM_QUEUESYNC,
    Wm_GetMinMaxInfo            = WM_GETMINMAXINFO,
    Wm_PaintIcon                = WM_PAINTICON,
    Wm_IconEraseBackground      = WM_ICONERASEBKGND,
    Wm_NextDialogControl        = WM_NEXTDLGCTL,
    Wm_SpoolerStatus            = WM_SPOOLERSTATUS,
    Wm_DrawItem                 = WM_DRAWITEM,
    Wm_MeasureItem              = WM_MEASUREITEM,
    Wm_DeleteItem               = WM_DELETEITEM,
    Wm_VKeyToItem               = WM_VKEYTOITEM,
    Wm_CharToItem               = WM_CHARTOITEM,
    Wm_SetFont                  = WM_SETFONT,
    Wm_GetFont                  = WM_GETFONT,
    Wm_SetHotKey                = WM_SETHOTKEY,
    Wm_GetHotKey                = WM_GETHOTKEY,
    Wm_QueryDragIcon            = WM_QUERYDRAGICON,
    Wm_CompareItem              = WM_COMPAREITEM,
    Wm_GetObject                = WM_GETOBJECT,
    Wm_Compacting               = WM_COMPACTING,
    Wm_CommNotify               = WM_COMMNOTIFY,
    Wm_WindowPosChanging        = WM_WINDOWPOSCHANGING,
    Wm_WindowPosChanged         = WM_WINDOWPOSCHANGED,
    Wm_Power                    = WM_POWER,
    Wm_CopyData                 = WM_COPYDATA,
    Wm_CancelJournal            = WM_CANCELJOURNAL,
    Wm_Notify                   = WM_NOTIFY,
    Wm_InputLangChangeRequest   = WM_INPUTLANGCHANGEREQUEST,
    Wm_InputLangChange          = WM_INPUTLANGCHANGE,
    Wm_TCard                    = WM_TCARD,
    Wm_Help                     = WM_HELP,
    Wm_UserChanged              = WM_USERCHANGED,
    Wm_NotifyFormat             = WM_NOTIFYFORMAT,
    Wm_ContextMenu              = WM_CONTEXTMENU,
    Wm_StyleChanging            = WM_STYLECHANGING,
    Wm_StyleChanged             = WM_STYLECHANGED,
    Wm_DisplayChange            = WM_DISPLAYCHANGE,
    Wm_GetIcon                  = WM_GETICON,
    Wm_SetIcon                  = WM_SETICON,
    Wm_NCCreate                 = WM_NCCREATE,
    Wm_NCDestroy                = WM_NCDESTROY,
    Wm_NCCalcSize               = WM_NCCALCSIZE,
    Wm_NCHitTest                = WM_NCHITTEST,
    Wm_NCPaint                  = WM_NCPAINT,
    Wm_NCActivate               = WM_NCACTIVATE,
    Wm_GetDialogCode            = WM_GETDLGCODE,
    Wm_SyncPaint                = WM_SYNCPAINT,
    Wm_NCMouseMove              = WM_NCMOUSEMOVE,
    Wm_NCLeftButtonDown         = WM_NCLBUTTONDOWN,
    Wm_NCLeftButtonUp           = WM_NCLBUTTONUP,
    Wm_NCLeftButtonDoubleClick  = WM_NCLBUTTONDBLCLK,
    Wm_NCRightButtonDown        = WM_NCRBUTTONDOWN,
    Wm_NCRightButtonUp          = WM_NCRBUTTONUP,
    Wm_NCRightButtonDoubleClick = WM_NCRBUTTONDBLCLK,
    Wm_NCMiddleButtonDown       = WM_NCMBUTTONDOWN,
    Wm_NCMiddleButtonUp         = WM_NCMBUTTONUP,
    Wm_NCMiddleButtonDoubleClick= WM_NCMBUTTONDBLCLK,
    Wm_NCXButtonDown            = WM_NCXBUTTONDOWN,
    Wm_NCXButtonUp              = WM_NCXBUTTONUP,
    Wm_NCXButtonDoubleClick     = WM_NCXBUTTONDBLCLK,
    Wm_KeyFirst                 = WM_KEYFIRST,
    Wm_KeyDown                  = WM_KEYDOWN,
    Wm_KeyUp                    = WM_KEYUP,
    Wm_Char                     = WM_CHAR,
    Wm_DeadChar                 = WM_DEADCHAR,
    Wm_SysKeyDown               = WM_SYSKEYDOWN,
    Wm_SysKeyUp                 = WM_SYSKEYUP,
    Wm_SysChar                  = WM_SYSCHAR,
    Wm_SysDeadChar              = WM_SYSDEADCHAR,
    Wm_KeyLast                  = WM_KEYLAST,
    Wm_IMEStartComposition      = WM_IME_STARTCOMPOSITION,
    Wm_IMEEndComposition        = WM_IME_ENDCOMPOSITION,
    Wm_IMEComposition           = WM_IME_COMPOSITION,
    Wm_IMEKeyLast               = WM_IME_KEYLAST,
    Wm_InitDialog               = WM_INITDIALOG,
    Wm_Command                  = WM_COMMAND,
    Wm_SysCommand               = WM_SYSCOMMAND,
    Wm_Timer                    = WM_TIMER,
    Wm_HScroll                  = WM_HSCROLL,
    Wm_VScroll                  = WM_VSCROLL,
    Wm_InitMenu                 = WM_INITMENU,
    Wm_InitMenuPopup            = WM_INITMENUPOPUP,
    Wm_MenuSelect               = WM_MENUSELECT,
    Wm_MenuChar                 = WM_MENUCHAR,
    Wm_EnterIdle                = WM_ENTERIDLE,
    Wm_MenuRightButtonUp        = WM_MENURBUTTONUP,
    Wm_MenuDrag                 = WM_MENUDRAG,
    Wm_MenuGetObject            = WM_MENUGETOBJECT,
    Wm_UninitMenuPopup          = WM_UNINITMENUPOPUP,
    Wm_MenuCommand              = WM_MENUCOMMAND,
    Wm_ChangeUIState            = WM_CHANGEUISTATE,
    Wm_UpdateUIState            = WM_UPDATEUISTATE,
    Wm_QueryUIState             = WM_QUERYUISTATE,
    Wm_CtlColorMsgBox           = WM_CTLCOLORMSGBOX,
    Wm_CtlColorEdit             = WM_CTLCOLOREDIT,
    Wm_CtlColorListBox          = WM_CTLCOLORLISTBOX,
    Wm_CtlColorButton           = WM_CTLCOLORBTN,
    Wm_CtlColorDialog           = WM_CTLCOLORDLG,
    Wm_CtlColorScrollBar        = WM_CTLCOLORSCROLLBAR,
    Wm_CtlColorStatic           = WM_CTLCOLORSTATIC,
    Wm_MouseFirst               = WM_MOUSEFIRST,
    Wm_MouseMove                = WM_MOUSEMOVE,
    Wm_LeftButtonDown           = WM_LBUTTONDOWN,
    Wm_LeftButtonUp             = WM_LBUTTONUP,
    Wm_LeftButtonDoubleClick    = WM_LBUTTONDBLCLK,
    Wm_RightButtonDown          = WM_RBUTTONDOWN,
    Wm_RightButtonUp            = WM_RBUTTONUP,
    Wm_RightButtonDoubleClick   = WM_RBUTTONDBLCLK,
    Wm_MiddleButtonDown         = WM_MBUTTONDOWN,
    Wm_MiddleButtonUp           = WM_MBUTTONUP,
    Wm_MiddleButtonDoubleClick  = WM_MBUTTONDBLCLK,
    Wm_MouseWheel               = WM_MOUSEWHEEL,
    Wm_XButtonDown              = WM_XBUTTONDOWN,
    Wm_XButtonUp                = WM_XBUTTONUP,
    Wm_XButtonDoubleClick       = WM_XBUTTONDBLCLK,
    Wm_MouseLast                = WM_MOUSELAST,
    Wm_ParentNotify             = WM_PARENTNOTIFY,
    Wm_EnterMenuLoop            = WM_ENTERMENULOOP,
    Wm_ExitMenuLoop             = WM_EXITMENULOOP,
    Wm_NextMenu                 = WM_NEXTMENU,
    Wm_Sizing                   = WM_SIZING,
    Wm_CaptureChanged           = WM_CAPTURECHANGED,
    Wm_Moving                   = WM_MOVING,
    Wm_PowerBroadcast           = WM_POWERBROADCAST,
    Wm_DeviceChange             = WM_DEVICECHANGE,
    Wm_MDICreate                = WM_MDICREATE,
    Wm_MDIDestroy               = WM_MDIDESTROY,
    Wm_MDIActivate              = WM_MDIACTIVATE,
    Wm_MDIRestore               = WM_MDIRESTORE,
    Wm_MDINext                  = WM_MDINEXT,
    Wm_MDIMaximize              = WM_MDIMAXIMIZE,
    Wm_MDITitle                 = WM_MDITILE,
    Wm_MDICascade               = WM_MDICASCADE,
    Wm_MDIIconArrange           = WM_MDIICONARRANGE,
    Wm_MDIGetActive             = WM_MDIGETACTIVE,
    Wm_MDISetMenu               = WM_MDISETMENU,
    Wm_EnterSizeMove            = WM_ENTERSIZEMOVE,
    Wm_ExitSizeMove             = WM_EXITSIZEMOVE,
    Wm_DropFiles                = WM_DROPFILES,
    Wm_MDIRefreshMenu           = WM_MDIREFRESHMENU,
    Wm_IMESetContext            = WM_IME_SETCONTEXT,
    Wm_IMENotify                = WM_IME_NOTIFY,
    Wm_IMEControl               = WM_IME_CONTROL,
    Wm_IMECompositionFull       = WM_IME_COMPOSITIONFULL,
    Wm_IMESelect                = WM_IME_SELECT,
    Wm_IMEChar                  = WM_IME_CHAR,
    Wm_IMERequest               = WM_IME_REQUEST,
    Wm_IMEKeyDown               = WM_IME_KEYDOWN,
    Wm_IMEKeyUp                 = WM_IME_KEYUP,
    Wm_MouseHover               = WM_MOUSEHOVER,
    Wm_MouseLeave               = WM_MOUSELEAVE,
    Wm_NCMouseHover             = WM_NCMOUSEHOVER,
    Wm_NCMouseLeave             = WM_NCMOUSELEAVE,
    Wm_Cut                      = WM_CUT,
    Wm_Copy                     = WM_COPY,
    Wm_Paste                    = WM_PASTE,
    Wm_Clear                    = WM_CLEAR,
    Wm_Undo                     = WM_UNDO,
    Wm_RenderFormat             = WM_RENDERFORMAT,
    Wm_RenderAllFormats         = WM_RENDERALLFORMATS,
    Wm_DestroyClipboard         = WM_DESTROYCLIPBOARD,
    Wm_DrawClipboard            = WM_DRAWCLIPBOARD,
    Wm_PaintClipboard           = WM_PAINTCLIPBOARD,
    Wm_VScrollClipboard         = WM_VSCROLLCLIPBOARD,
    Wm_SizeClipboard            = WM_SIZECLIPBOARD,
    Wm_AskCBFormatName          = WM_ASKCBFORMATNAME,
    Wm_ChangeCBChain            = WM_CHANGECBCHAIN,
    Wm_HScrollClipboard         = WM_HSCROLLCLIPBOARD,
    Wm_QueryNewPalette          = WM_QUERYNEWPALETTE,
    Wm_PaletteIsChanging        = WM_PALETTEISCHANGING,
    Wm_PaletteChanged           = WM_PALETTECHANGED,
    Wm_HotKey                   = WM_HOTKEY,
    Wm_Print                    = WM_PRINT,
    Wm_PrintClient              = WM_PRINTCLIENT,
    Wm_AppCommand               = WM_APPCOMMAND,
    Wm_HandHeldFirst            = WM_HANDHELDFIRST,
    Wm_HandHeldLast             = WM_HANDHELDLAST,
    Wm_AFXFirst                 = WM_AFXFIRST,
    Wm_AFXLast                  = WM_AFXLAST,
    Wm_PenWinFirst              = WM_PENWINFIRST,
    Wm_PenWinLast               = WM_PENWINLAST,
    Wm_User                     = WM_USER,
    Wm_App                      = WM_APP
} WmMsg;

#ifdef WPP_HELPER_FUNC
  WmMsg _WinMsg = Wm_Null;
#endif

//begin_wpp config
//  CUSTOM_TYPE(WMMSG, ItemEnum(_WMMsg));
//end_wpp

#endif  //ifdef WPP_ENUM_WMMSG

//begin_wpp config
//  CUSTOM_TYPE(IRPMJ, ItemListByte(Create,CreateNamedPipe,Close,Read,Write,QueryInfo,SetInfo,QueryEA,SetEA,FlushBuffers,QueryVolInfo,SetVolInfo,DirectoryControl,FileSystemControl,DeviceControl,InternalDevControl,Shutdown,LockControl,CleanUp,CreateMailSlot,QuerySecurity,SetSecurity,Power,SystemControl,DeviceChange,QueryQuota,SetQuota,PnP));
//  CUSTOM_TYPE(PNPMN, ItemListByte(StartDevice,QueryRemoveDevice,RemoveDevice,CancelRemoveDevice,StopDevice,QueryStopDevice,CancelStopDevice,QueryDeviceRelations,QueryInterface,QueryCapabilities,QueryResources,QueryResRequirements,QueryDeviceText,FilterResRequirements,PNPMN_H0E,ReadConfig,WriteConfig,Eject,SetLock,QueryID,QueryPNPDeviceState,QueryBusInfo,DeviceUsageNotify,SurpriseRemoval,PNPMN_H18,QueryProximityDomain));
//  CUSTOM_TYPE(PWRMN, ItemListByte(WaitWake,PowerSequence,SetPower,QueryPower));
//  CUSTOM_TYPE(SYSCTRLMN, ItemListByte(QueryAllData,QuerySingleInstance,ChangeSingleInstance,ChangeSingleItem,EnableEvents,DisableEvents,EnableCollection,DisableCollection,RegInfo,ExecuteMethod,SYSCTRLMN_H0A,RegInfoEx));
//  CUSTOM_TYPE(PWRTYPE, ItemListByte(SystemPowerState,DevicePowerState));
//  CUSTOM_TYPE(SYSPWRSTATE, ItemListByte(Unspecified,Working,Sleeping1,Sleeping2,Sleeping3,Hibernate,Shutdown,Maximum));
//  CUSTOM_TYPE(DEVPWRSTATE, ItemListByte(Unspecified,D0,D1,D2,D3,Maximum));
//  CUSTOM_TYPE(QUERYID, ItemListByte(DeviceID,HardwareIDs,CompatIDs,InstanceID,DeviceSerialNumber));
//  CUSTOM_TYPE(CPLMSG, ItemListLong(Init=1,GetCount,Inquire,Select,DoubleClick,Stop,Exit,NewInquire,StartWParmsA,StartWParmsW,Setup=200));
//end_wpp

//
// Tracing Macros.
//

#define Fatal                   TRACE_LEVEL_FATAL
#define Err                     TRACE_LEVEL_ERROR
#define Warn                    TRACE_LEVEL_WARNING
#define Info                    TRACE_LEVEL_INFORMATION
#define Verbose                 TRACE_LEVEL_VERBOSE

#define WPP_DEF_FLAGS           WPP_DEFINE_BIT(Msg)

#ifdef WPP_CUSTOM_FLAGS
  #define WPP_CONTROL_GUIDS     WPP_DEFINE_CONTROL_GUID(        \
                                    WppCtrlGuid,                \
                                    WPP_CTRL_GUID,              \
                                    WPP_DEF_FLAGS               \
                                    WPP_CUSTOM_FLAGS            \
                                )
#else
    #define WPP_CONTROL_GUIDS   WPP_DEFINE_CONTROL_GUID(        \
                                    WppCtrlGuid,                \
                                    WPP_CTRL_GUID,              \
                                    WPP_DEF_FLAGS               \
                                )
#endif

#define WPP_LEVEL_LOGGER(lvl)   (WPP_CONTROL(WPP_BIT_ ## Msg).Logger),
#define WPP_LEVEL_ENABLED(lvl)  (WPP_CONTROL(WPP_BIT_ ## Msg).Level >= lvl)

#define WPP_LEVEL_rc_LOGGER(lvl,rc)     WPP_LEVEL_LOGGER(lvl)
#define WPP_LEVEL_rc_ENABLED(lvl,rc)    WPP_LEVEL_ENABLED(lvl)

//Macro: TMsg
//begin_wpp config
//  USEPREFIX(TMsg, "%!STDPREFIX!%!FUNC!_%!LEVEL!: ");
//  FUNC TMsg(LEVEL,MSG,...);
//end_wpp

//Macro: TMsgHr
//begin_wpp config
//  USEPREFIX(TMsgHr, "%!STDPREFIX!%!FUNC!_%!LEVEL!: ");
//  FUNC TMsgHr(LEVEL,rc,MSG,...);
//  USESUFFIX(TMsgHr, " (hr=%!HRESULT!)", rc);
//end_wpp

//Macro: TMsgErr
//begin_wpp config
//  USEPREFIX(TMsgErr, "%!STDPREFIX!%!FUNC!_%!LEVEL!: ");
//  FUNC TMsgErr(LEVEL,rc,MSG,...);
//  USESUFFIX(TMsgErr, " (err=%!WINERROR!)", rc);
//end_wpp

//Macro: TMsgNt
//begin_wpp config
//  USEPREFIX(TMsgNt, "%!STDPREFIX!%!FUNC!_%!LEVEL!: ");
//  FUNC TMsgNt(LEVEL,rc,MSG,...);
//  USESUFFIX(TMsgNt, " (status=%!STATUS!)", rc);
//end_wpp

#define WPP_LEVEL_exp_PRE(lvl,exp)              {if (exp) {
#define WPP_LEVEL_exp_POST(lvl,exp)             ;}}
#define WPP_LEVEL_exp_LOGGER(lvl,exp)           WPP_LEVEL_LOGGER(lvl)
#define WPP_LEVEL_exp_ENABLED(lvl,exp)          WPP_LEVEL_ENABLED(lvl)

#define WPP_LEVEL_exp_rc_PRE(lvl,exp,rc)        {if (exp) {
#define WPP_LEVEL_exp_rc_POST(lvl,exp,rc)       ;}}
#define WPP_LEVEL_exp_rc_LOGGER(lvl,exp,rc)     WPP_LEVEL_LOGGER(lvl)
#define WPP_LEVEL_exp_rc_ENABLED(lvl,exp,rc)    WPP_LEVEL_ENABLED(lvl)

//Macro: TExp
//begin_wpp config
//  USEPREFIX(TExp, "%!STDPREFIX!%!FUNC!_Exp: ");
//  FUNC TExp{LEVEL=Err}(exp,MSG,...);
//end_wpp

//Macro: TExpHr
//begin_wpp config
//  USEPREFIX(TExpHr, "%!STDPREFIX!%!FUNC!_Exp: ");
//  FUNC TExpHr{LEVEL=Err}(exp,rc,MSG,...);
//  USESUFFIX(TExpHr, " (hr=%!HRESULT!)", rc);
//end_wpp

//Macro: TExpErr
//begin_wpp config
//  USEPREFIX(TExpErr, "%!STDPREFIX!%!FUNC!_Exp: ");
//  FUNC TExpErr{LEVEL=Err}(exp,rc,MSG,...);
//  USESUFFIX(TExpErr, " (err=%!WINERROR!)", rc);
//end_wpp

//Macro: TExpNt
//begin_wpp config
//  USEPREFIX(TExpNt, "%!STDPREFIX!%!FUNC!_Exp: ");
//  FUNC TExpNt{LEVEL=Err}(exp,rc,MSG,...);
//  USESUFFIX(TExpNt, " (status=%!STATUS!)", rc);
//end_wpp

#define WPP_LEVEL_expe_PRE(lvl,exp)             {if (exp) {
#define WPP_LEVEL_expe_POST(lvl,exp)            ; goto Exit;}}
#define WPP_LEVEL_expe_LOGGER(lvl,exp)          WPP_LEVEL_LOGGER(lvl)
#define WPP_LEVEL_expe_ENABLED(lvl,exp)         WPP_LEVEL_ENABLED(lvl)

#define WPP_LEVEL_expe_rc_PRE(lvl,exp,rc)       {if (exp) {
#define WPP_LEVEL_expe_rc_POST(lvl,exp,rc)      ; goto Exit;}}
#define WPP_LEVEL_expe_rc_LOGGER(lvl,exp,rc)    WPP_LEVEL_LOGGER(lvl)
#define WPP_LEVEL_expe_rc_ENABLED(lvl,exp,rc)   WPP_LEVEL_ENABLED(lvl)

//Macro: TExpExit
//begin_wpp config
//  USEPREFIX(TExpExit, "%!STDPREFIX!%!FUNC!_Exp: ");
//  FUNC TExpExit{LEVEL=Err}(expe,MSG,...);
//end_wpp

//Macro: TExpHrExit
//begin_wpp config
//  USEPREFIX(TExpHrExit, "%!STDPREFIX!%!FUNC!_Exp: ");
//  FUNC TExpHrExit{LEVEL=Err}(expe,rc,MSG,...);
//  USESUFFIX(TExpHrExit, " (hr=%!HRESULT!)", rc);
//end_wpp

//Macro: TExpErrExit
//begin_wpp config
//  USEPREFIX(TExpErrExit, "%!STDPREFIX!%!FUNC!_Exp: ");
//  FUNC TExpErrExit{LEVEL=Err}(expe,rc,MSG,...);
//  USESUFFIX(TExpErrExit, " (err=%!WINERROR!)", rc);
//end_wpp

//Macro: TExpNtExit
//begin_wpp config
//  USEPREFIX(TExpNtExit, "%!STDPREFIX!%!FUNC!_Exp: ");
//  FUNC TExpNtExit{LEVEL=Err}(expe,rc,MSG,...);
//  USESUFFIX(TExpNtExit, " (status=%!STATUS!)", rc);
//end_wpp

#define WPP_INIT_TRACING(s)
#define WPP_CLEANUP()
#define TMsg(l,m,...)
#define TMsgHr(l,c,m,...)
#define TMsgErr(l,c,m,...)
#define TMsgNt(l,c,m,...)
#define TExp(e,m,...)
#define TExpHr(e,c,m,...)
#define TExpErr(e,c,m,...)
#define TExpNt(e,c,m,...)
#define TExpExit(e,m,...)
#define TExpHrExit(e,c,m,...)
#define TExpErrExit(e,c,m,...)
#define TExpNtExit(e,c,m,...)
#define TDebug(m,...)
#define TAssert(x)
#define TEnter(f,p,m,...)
#define TEnterVoid(f,p)
#define TExit(f,p,m,...)
#define TExitVoid(f,p)
#define TExitVoidMsg(f,p,m,...)
#define TExitHr(f,p,r)
#define TExitHrMsg(f,p,r,m,...)
#define TExitErr(f,p,r)
#define TExitErrMsg(f,p,r,m,...)
#define TExitNt(f,p,r)
#define TExitNtMsg(f,p,r,m,...)
#define TExitDec(f,p,r)
#define TExitDecMsg(f,p,r,m,...)
#define TExitHex(f,p,r)
#define TExitHexMsg(f,p,r,m,...)
#define TExitPtr(f,p,r)
#define TExitPtrMsg(f,p,r,m,...)
#define TExitChr(f,p,r)
#define TExitChrMsg(f,p,r,m,...)
#define TExitStr(f,p,r)
#define TExitStrMsg(f,p,r,m,...)
#define TExitWStr(f,p,r)
#define TExitWStrMsg(f,p,r,m,...)
#define TFunc(f,i,m,...)
#define TFuncHr(f,i,p,m,...)
#define TFuncErr(f,i,p,m,...)
#define TFuncNt(f,i,p,m,...)
#define TFuncDec(f,i,p,m,...)
#define TFuncHex(f,i,p,m,...)
#define TFuncPtr(f,i,p,m,...)
#define TFuncChr(f,i,p,m,...)
#define TFuncStr(f,i,p,m,...)
#define TFuncWStr(f,i,p,m,...)

#ifdef __cplusplus
extern "C" {
#endif

//
// Helper functions.
//

typedef struct _TimeStamp
{
    BOOLEAN   fHighPrecision;
    ULONGLONG ullInitialTime;
    ULONGLONG ullInitialCounter;
    ULONGLONG ullCounterFreq;
} TIMESTAMP, *PTIMESTAMP;

#ifdef WPP_HELPER_FUNC

#include <strsafe.h>

BOOLEAN
InitTimeStamp(
    __in BOOLEAN     fHighPrecision,
    __out PTIMESTAMP TimeStamp
    )
{
    BOOLEAN rc = TRUE;
#ifdef WPP_KERNEL_MODE
    LARGE_INTEGER liTime, liCount, liFreq;
    KSPIN_LOCK SpinLock;
    KIRQL OldIrql;

    TimeStamp->fHighPrecision = fHighPrecision;
    KeInitializeSpinLock(&SpinLock);
    KeAcquireSpinLock(&SpinLock, &OldIrql);
    KeQuerySystemTime(&liTime);
    if (fHighPrecision)
    {
        liCount = KeQueryPerformanceCounter(&liFreq);
        TimeStamp->ullInitialCounter = (ULONGLONG)liCount.QuadPart;
        TimeStamp->ullCounterFreq = (ULONGLONG)liFreq.QuadPart;
    }
    else
    {
        TimeStamp->ullInitialCounter = KeQueryInterruptTime();
        TimeStamp->ullCounterFreq = 0;
    }
    KeReleaseSpinLock(&SpinLock, OldIrql);
    TimeStamp->ullInitialTime = (ULONGLONG)liTime.QuadPart;
#else
    LARGE_INTEGER liCount, liFreq;

    TimeStamp->fHighPrecision = fHighPrecision;
    GetSystemTimeAsFileTime((PFILETIME)&TimeStamp->ullInitialTime);
    if (fHighPrecision)
    {
        rc &= QueryPerformanceCounter(&liCount);
        rc &= QueryPerformanceFrequency(&liFreq);
        TimeStamp->ullInitialCounter = (ULONGLONG)liCount.QuadPart;
        TimeStamp->ullCounterFreq = (ULONGLONG)liFreq.QuadPart;
    }
    else
    {
        TimeStamp->ullInitialCounter = TimeStamp->ullInitialTime;
        TimeStamp->ullCounterFreq = 0;
    }
#endif

    return rc;
}       //InitTimeStamp

__inline
ULONGLONG
QueryLowPrecisionTimeCounter(
    VOID
    )
{
    //
    // Time counter is in the unit of 100 nano-seconds.
    //
#ifdef WPP_KERNEL_MODE
    return KeQueryInterruptTime();
#else
    ULONGLONG ullCounter;
    GetSystemTimeAsFileTime((PFILETIME)&ullCounter);
    return ullCounter;
#endif
}       //QueryLowPrecisionTimeCounter

__inline
ULONGLONG
QueryHighPrecisionTimeCounter(
    VOID
    )
{
    //
    // Time counter is in the unit of performance counter ticks.
    //
#ifdef WPP_KERNEL_MODE
    return (ULONGLONG)KeQueryPerformanceCounter(NULL).QuadPart;
#else
    ULONGLONG ullCounter = 0;
    QueryPerformanceCounter((PLARGE_INTEGER)&ullCounter);
    return ullCounter;
#endif
}       //QueryHighPrecisionTimeCounter

__inline
ULONGLONG
QueryTimeCounter(
    __in PTIMESTAMP TimeStamp
    )
{
    if (TimeStamp->fHighPrecision)
    {
        return QueryHighPrecisionTimeCounter();
    }
    else
    {
        return QueryLowPrecisionTimeCounter();
    }
}       //QueryTimeCounter

__inline
ULONGLONG
LowPrecisionElapseTime(
    __in PTIMESTAMP TimeStamp,  //not used
    __in ULONGLONG  StartCount,
    __in ULONGLONG  EndCount
    )
{
    //
    // Elapse time is in the unit of 100 nano-seconds.
    //
    UNREFERENCED_PARAMETER(TimeStamp);
    return EndCount - StartCount;
}       //LowPrecisionElapseTime

__inline
ULONGLONG
HighPrecisionElapseTime(
    __in PTIMESTAMP TimeStamp,
    __in ULONGLONG  StartCount,
    __in ULONGLONG  EndCount
    )
{
    //
    // The algorithm calculates the Elapse time in the units of
    // 100 nano-seconds and to minimize the chance of overflow.
    // ElapseTime = (EndCount - StartCount)*10000000/CounterFreq
    //
    ULONGLONG ElapseTime;
    ULONGLONG ElapseCount = EndCount - StartCount;

    // Elapse time in seconds.
    ElapseTime = ElapseCount/TimeStamp->ullCounterFreq;

    // Elapse time in milli-seconds.
    ElapseCount = (ElapseCount%TimeStamp->ullCounterFreq)*1000;
    ElapseTime = ElapseTime*1000 + ElapseCount/TimeStamp->ullCounterFreq;

    // Elapse time in 100 nano-seconds.
    ElapseCount = (ElapseCount%TimeStamp->ullCounterFreq)*10000;
    ElapseTime = ElapseTime*10000 + ElapseCount/TimeStamp->ullCounterFreq;

    return ElapseTime;
}       //HighPrecisionElapseTime

__inline
ULONGLONG
ElapseTime(
    __in PTIMESTAMP TimeStamp,
    __in ULONGLONG  StartCount,
    __in ULONGLONG  EndCount
    )
{
    if (TimeStamp->fHighPrecision)
    {
        return HighPrecisionElapseTime(TimeStamp, StartCount, EndCount);
    }
    else
    {
        return LowPrecisionElapseTime(TimeStamp, StartCount, EndCount);
    }
}       //ElapseTime

__inline
ULONGLONG
QueryLowPrecisionTimeStamp(
    __in PTIMESTAMP TimeStamp
    )
{
    return LowPrecisionElapseTime(TimeStamp,
                                  TimeStamp->ullInitialCounter,
                                  QueryLowPrecisionTimeCounter()) +
           TimeStamp->ullInitialTime;
}       //QueryLowPrecisionTimeStamp

__inline
ULONGLONG
QueryHighPrecisionTimeStamp(
    __in PTIMESTAMP TimeStamp
    )
{
    return HighPrecisionElapseTime(TimeStamp,
                                   TimeStamp->ullInitialCounter,
                                   QueryHighPrecisionTimeCounter()) +
           TimeStamp->ullInitialTime;
}       //QueryHighPrecisionTimeStamp

__inline
ULONGLONG
QueryTimeStamp(
    __in PTIMESTAMP TimeStamp
    )
{
    return ElapseTime(TimeStamp,
                      TimeStamp->ullInitialCounter,
                      QueryTimeCounter(TimeStamp)) +
           TimeStamp->ullInitialTime;
}       //QueryTimeStamp

#else   //ifdef WPP_HELPER_FUNC

BOOLEAN
InitTimeStamp(
    __in BOOLEAN     fHighPrecision,
    __out PTIMESTAMP TimeStamp
    );

__inline
ULONGLONG
QueryLowPrecisionTimeCounter(
    VOID
    );

__inline
ULONGLONG
QueryHighPrecisionTimeCounter(
    VOID
    );

__inline
ULONGLONG
QueryTimeCounter(
    __in PTIMESTAMP TimeStamp
    );

__inline
ULONGLONG
LowPrecisionElapseTime(
    __in PTIMESTAMP TimeStamp,  //not used
    __in ULONGLONG  StartCount,
    __in ULONGLONG  EndTount
    );

__inline
ULONGLONG
HighPrecisionElapseTime(
    __in PTIMESTAMP TimeStamp,
    __in ULONG      StartCount,
    __in ULONG      EndCount
    );

__inline
ULONGLONG
ElapseTime(
    __in PTIMESTAMP TimeStamp,
    __in ULONGLONG  StartCount,
    __in ULONGLONG  EndCount
    );

__inline
ULONGLONG
QueryLowPrecisionTimeStamp(
    __in PTIMESTAMP TimeStamp
    );

__inline
ULONGLONG
QueryHighPrecisionTimeStamp(
    __in PTIMESTAMP TimeStamp
    );

__inline
ULONGLONG
QueryTimeStamp(
    __in PTIMESTAMP TimeStamp
    );

#endif  //ifdef WPP_HELPER_FUNC

#ifdef __cplusplus
}
#endif

