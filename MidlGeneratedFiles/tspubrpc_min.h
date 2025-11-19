

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Mon Jan 18 19:14:07 2038
 */
/* Compiler settings for ..\SamplePlugin\tspubrpc_min.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0628 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __tspubrpc_min_h__
#define __tspubrpc_min_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if defined(_CONTROL_FLOW_GUARD_XFG)
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */ 

/* header files for imported files */
#include "wtypes.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_tspubrpc_min_0000_0000 */
/* [local] */ 

typedef unsigned long TNotificationId;

#define	WTS_NOTIFY_NONE	( 0 )

#define	WTS_NOTIFY_CONNECT	( 0x2 )

#define	WTS_NOTIFY_DISCONNECT	( 0x4 )

#define	WTS_NOTIFY_LOCK	( 0x400 )

#define	WTS_NOTIFY_UNLOCK	( 0x800 )

typedef /* [context_handle] */ void *NOTIFY_HANDLE;

typedef struct _SESSION_CHANGE
    {
    long SessionId;
    TNotificationId NotificationId;
    } 	SESSION_CHANGE;

typedef struct _SESSION_CHANGE *PSESSION_CHANGE;



extern RPC_IF_HANDLE __MIDL_itf_tspubrpc_min_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_tspubrpc_min_0000_0000_v0_0_s_ifspec;

#ifndef __TermSrvNotification_INTERFACE_DEFINED__
#define __TermSrvNotification_INTERFACE_DEFINED__

/* interface TermSrvNotification */
/* [unique][version][uuid] */ 

HRESULT RpcRegisterAsyncNotification( 
    /* [in] */ handle_t hBinding,
    /* [in] */ long SessionId,
    /* [in] */ TNotificationId Mask,
    /* [out] */ NOTIFY_HANDLE *phNotify);

HRESULT RpcWaitAsyncNotification( 
    /* [in] */ NOTIFY_HANDLE hNotify,
    /* [size_is][size_is][out] */ PSESSION_CHANGE *SessionChange,
    /* [out] */ unsigned long *pEntries);

HRESULT RpcUnRegisterAsyncNotification( 
    /* [out][in] */ NOTIFY_HANDLE *phNotify);



extern RPC_IF_HANDLE TermSrvNotification_v1_0_c_ifspec;
extern RPC_IF_HANDLE TermSrvNotification_v1_0_s_ifspec;
#endif /* __TermSrvNotification_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

void __RPC_USER NOTIFY_HANDLE_rundown( NOTIFY_HANDLE );

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


