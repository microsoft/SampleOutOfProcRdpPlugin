

/* this ALWAYS GENERATED file contains the RPC server stubs */


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

#if defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/

#include <string.h>
#include "tspubrpc_min.h"

#define TYPE_FORMAT_STRING_SIZE   65                                
#define PROC_FORMAT_STRING_SIZE   155                               
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _tspubrpc_min_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } tspubrpc_min_MIDL_TYPE_FORMAT_STRING;

typedef struct _tspubrpc_min_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } tspubrpc_min_MIDL_PROC_FORMAT_STRING;

typedef struct _tspubrpc_min_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } tspubrpc_min_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax_2_0 = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};

#if defined(_CONTROL_FLOW_GUARD_XFG)
#define XFG_TRAMPOLINES(ObjectType)\
NDR_SHAREABLE unsigned long ObjectType ## _UserSize_XFG(unsigned long * pFlags, unsigned long Offset, void * pObject)\
{\
return  ObjectType ## _UserSize(pFlags, Offset, (ObjectType *)pObject);\
}\
NDR_SHAREABLE unsigned char * ObjectType ## _UserMarshal_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserMarshal(pFlags, pBuffer, (ObjectType *)pObject);\
}\
NDR_SHAREABLE unsigned char * ObjectType ## _UserUnmarshal_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserUnmarshal(pFlags, pBuffer, (ObjectType *)pObject);\
}\
NDR_SHAREABLE void ObjectType ## _UserFree_XFG(unsigned long * pFlags, void * pObject)\
{\
ObjectType ## _UserFree(pFlags, (ObjectType *)pObject);\
}
#define XFG_TRAMPOLINES64(ObjectType)\
NDR_SHAREABLE unsigned long ObjectType ## _UserSize64_XFG(unsigned long * pFlags, unsigned long Offset, void * pObject)\
{\
return  ObjectType ## _UserSize64(pFlags, Offset, (ObjectType *)pObject);\
}\
NDR_SHAREABLE unsigned char * ObjectType ## _UserMarshal64_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserMarshal64(pFlags, pBuffer, (ObjectType *)pObject);\
}\
NDR_SHAREABLE unsigned char * ObjectType ## _UserUnmarshal64_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserUnmarshal64(pFlags, pBuffer, (ObjectType *)pObject);\
}\
NDR_SHAREABLE void ObjectType ## _UserFree64_XFG(unsigned long * pFlags, void * pObject)\
{\
ObjectType ## _UserFree64(pFlags, (ObjectType *)pObject);\
}
#define XFG_BIND_TRAMPOLINES(HandleType, ObjectType)\
static void* ObjectType ## _bind_XFG(HandleType pObject)\
{\
return ObjectType ## _bind((ObjectType) pObject);\
}\
static void ObjectType ## _unbind_XFG(HandleType pObject, handle_t ServerHandle)\
{\
ObjectType ## _unbind((ObjectType) pObject, ServerHandle);\
}
#define XFG_TRAMPOLINE_FPTR(Function) Function ## _XFG
#define XFG_TRAMPOLINE_FPTR_DEPENDENT_SYMBOL(Symbol) Symbol ## _XFG
#else
#define XFG_TRAMPOLINES(ObjectType)
#define XFG_TRAMPOLINES64(ObjectType)
#define XFG_BIND_TRAMPOLINES(HandleType, ObjectType)
#define XFG_TRAMPOLINE_FPTR(Function) Function
#define XFG_TRAMPOLINE_FPTR_DEPENDENT_SYMBOL(Symbol) Symbol
#endif

extern const tspubrpc_min_MIDL_TYPE_FORMAT_STRING tspubrpc_min__MIDL_TypeFormatString;
extern const tspubrpc_min_MIDL_PROC_FORMAT_STRING tspubrpc_min__MIDL_ProcFormatString;
extern const tspubrpc_min_MIDL_EXPR_FORMAT_STRING tspubrpc_min__MIDL_ExprFormatString;

/* Standard interface: __MIDL_itf_tspubrpc_min_0000_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: TermSrvNotification, ver. 1.0,
   GUID={0x11899a43,0x2b68,0x4a76,{0x92,0xe3,0xa3,0xd6,0xad,0x8c,0x26,0xce}} */


extern const MIDL_SERVER_INFO TermSrvNotification_ServerInfo;

extern const RPC_DISPATCH_TABLE TermSrvNotification_v1_0_DispatchTable;

static const RPC_SERVER_INTERFACE TermSrvNotification___RpcServerInterface =
    {
    sizeof(RPC_SERVER_INTERFACE),
    {{0x11899a43,0x2b68,0x4a76,{0x92,0xe3,0xa3,0xd6,0xad,0x8c,0x26,0xce}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    (RPC_DISPATCH_TABLE*)&TermSrvNotification_v1_0_DispatchTable,
    0,
    0,
    0,
    &TermSrvNotification_ServerInfo,
    0x04000000
    };
RPC_IF_HANDLE TermSrvNotification_v1_0_s_ifspec = (RPC_IF_HANDLE)& TermSrvNotification___RpcServerInterface;
#ifdef __cplusplus
namespace {
#endif

extern const MIDL_STUB_DESC TermSrvNotification_StubDesc;
#ifdef __cplusplus
}
#endif

extern const NDR_RUNDOWN RundownRoutines[];

#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif

static const tspubrpc_min_MIDL_PROC_FORMAT_STRING tspubrpc_min__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure RpcRegisterAsyncNotification */

			0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x0 ),	/* 0 */
/*  8 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 10 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 12 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 14 */	NdrFcShort( 0x10 ),	/* 16 */
/* 16 */	NdrFcShort( 0x40 ),	/* 64 */
/* 18 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 20 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x0 ),	/* 0 */
/* 26 */	NdrFcShort( 0x0 ),	/* 0 */
/* 28 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter SessionId */

/* 30 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 32 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 34 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter Mask */

/* 36 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 38 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 40 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter phNotify */

/* 42 */	NdrFcShort( 0x110 ),	/* Flags:  out, simple ref, */
/* 44 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 46 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Return value */

/* 48 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 50 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 52 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RpcWaitAsyncNotification */

/* 54 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 56 */	NdrFcLong( 0x0 ),	/* 0 */
/* 60 */	NdrFcShort( 0x1 ),	/* 1 */
/* 62 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 64 */	0x30,		/* FC_BIND_CONTEXT */
			0x40,		/* Ctxt flags:  in, */
/* 66 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 68 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 70 */	NdrFcShort( 0x24 ),	/* 36 */
/* 72 */	NdrFcShort( 0x24 ),	/* 36 */
/* 74 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x4,		/* 4 */
/* 76 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 78 */	NdrFcShort( 0x1 ),	/* 1 */
/* 80 */	NdrFcShort( 0x0 ),	/* 0 */
/* 82 */	NdrFcShort( 0x0 ),	/* 0 */
/* 84 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hNotify */

/* 86 */	NdrFcShort( 0x8 ),	/* Flags:  in, */
/* 88 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 90 */	NdrFcShort( 0xa ),	/* Type Offset=10 */

	/* Parameter SessionChange */

/* 92 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 94 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 96 */	NdrFcShort( 0xe ),	/* Type Offset=14 */

	/* Parameter pEntries */

/* 98 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 100 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 102 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 104 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 106 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 108 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RpcUnRegisterAsyncNotification */

/* 110 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 112 */	NdrFcLong( 0x0 ),	/* 0 */
/* 116 */	NdrFcShort( 0x2 ),	/* 2 */
/* 118 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 120 */	0x30,		/* FC_BIND_CONTEXT */
			0xe0,		/* Ctxt flags:  via ptr, in, out, */
/* 122 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 124 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 126 */	NdrFcShort( 0x38 ),	/* 56 */
/* 128 */	NdrFcShort( 0x40 ),	/* 64 */
/* 130 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 132 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 134 */	NdrFcShort( 0x0 ),	/* 0 */
/* 136 */	NdrFcShort( 0x0 ),	/* 0 */
/* 138 */	NdrFcShort( 0x0 ),	/* 0 */
/* 140 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter phNotify */

/* 142 */	NdrFcShort( 0x118 ),	/* Flags:  in, out, simple ref, */
/* 144 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 146 */	NdrFcShort( 0x3c ),	/* Type Offset=60 */

	/* Return value */

/* 148 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 150 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 152 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const tspubrpc_min_MIDL_TYPE_FORMAT_STRING tspubrpc_min__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/*  4 */	NdrFcShort( 0x2 ),	/* Offset= 2 (6) */
/*  6 */	0x30,		/* FC_BIND_CONTEXT */
			0xa0,		/* Ctxt flags:  via ptr, out, */
/*  8 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 10 */	0x30,		/* FC_BIND_CONTEXT */
			0x41,		/* Ctxt flags:  in, can't be null */
/* 12 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 14 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 16 */	NdrFcShort( 0x2 ),	/* Offset= 2 (18) */
/* 18 */	
			0x12, 0x0,	/* FC_UP */
/* 20 */	NdrFcShort( 0xa ),	/* Offset= 10 (30) */
/* 22 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 24 */	NdrFcShort( 0x8 ),	/* 8 */
/* 26 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 28 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 30 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 32 */	NdrFcShort( 0x0 ),	/* 0 */
/* 34 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x54,		/* FC_DEREFERENCE */
/* 36 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 38 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 40 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 44 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 46 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 48 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (22) */
/* 50 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 52 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 54 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 56 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 58 */	NdrFcShort( 0x2 ),	/* Offset= 2 (60) */
/* 60 */	0x30,		/* FC_BIND_CONTEXT */
			0xe1,		/* Ctxt flags:  via ptr, in, out, can't be null */
/* 62 */	0x0,		/* 0 */
			0x0,		/* 0 */

			0x0
        }
    };

static const NDR_RUNDOWN RundownRoutines[] = 
    {
    NOTIFY_HANDLE_rundown
    };


static const unsigned short TermSrvNotification_FormatStringOffsetTable[] =
    {
    0,
    54,
    110
    };


#ifdef __cplusplus
namespace {
#endif
static const MIDL_STUB_DESC TermSrvNotification_StubDesc = 
    {
    (void *)& TermSrvNotification___RpcServerInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    0,
    RundownRoutines,
    0,
    0,
    0,
    tspubrpc_min__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x8010274, /* MIDL Version 8.1.628 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };
#ifdef __cplusplus
}
#endif

static const RPC_DISPATCH_FUNCTION TermSrvNotification_table[] =
    {
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    0
    };
static const RPC_DISPATCH_TABLE TermSrvNotification_v1_0_DispatchTable = 
    {
    3,
    (RPC_DISPATCH_FUNCTION*)TermSrvNotification_table
    };

static const SERVER_ROUTINE TermSrvNotification_ServerRoutineTable[] = 
    {
    (SERVER_ROUTINE)RpcRegisterAsyncNotification,
    (SERVER_ROUTINE)RpcWaitAsyncNotification,
    (SERVER_ROUTINE)RpcUnRegisterAsyncNotification
    };

static const MIDL_SERVER_INFO TermSrvNotification_ServerInfo = 
    {
    &TermSrvNotification_StubDesc,
    TermSrvNotification_ServerRoutineTable,
    tspubrpc_min__MIDL_ProcFormatString.Format,
    TermSrvNotification_FormatStringOffsetTable,
    0,
    0,
    0,
    0};
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* defined(_M_AMD64)*/

