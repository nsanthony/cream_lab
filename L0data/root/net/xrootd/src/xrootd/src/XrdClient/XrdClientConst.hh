//////////////////////////////////////////////////////////////////////////
//                                                                      //
// XrdClientConst                                                       //
//                                                                      //
// Author: Fabrizio Furano (INFN Padova, 2004)                          //
// Adapted from TXNetFile (root.cern.ch) originally done by             //
//  Alvise Dorigo, Fabrizio Furano                                      //
//          INFN Padova, 2003                                           //
//                                                                      //
// Constants for Xrd                                                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//       $Id: XrdClientConst.hh 31508 2009-12-02 19:11:01Z brun $


#ifndef _XRC_CONST_H
#define _XRC_CONST_H

#define DFLT_CONNECTTIMEOUT     120
#define NAME_CONNECTTIMEOUT     (char *)"ConnectTimeout"

#define DFLT_REQUESTTIMEOUT     300
#define NAME_REQUESTTIMEOUT     (char *)"RequestTimeout"

#define DFLT_MAXREDIRECTCOUNT   16
#define NAME_MAXREDIRECTCOUNT   (char *)"MaxRedirectcount"

#define DFLT_DEBUG              0
#define NAME_DEBUG              (char *)"DebugLevel"

#define DFLT_RECONNECTWAIT      5
#define NAME_RECONNECTWAIT      (char *)"ReconnectWait"
#define DFLT_RECONNECTTIMEOUT   DFLT_RECONNECTWAIT
#define NAME_RECONNECTTIMEOUT   NAME_RECONNECTWAIT

#define DFLT_REDIRCNTTIMEOUT	36000
#define NAME_REDIRCNTTIMEOUT    (char *)"RedirCntTimeout"

#define DFLT_FIRSTCONNECTMAXCNT 8
#define NAME_FIRSTCONNECTMAXCNT (char *)"FirstConnectMaxCnt"

#define DFLT_TRANSACTIONTIMEOUT 300
#define NAME_TRANSACTIONTIMEOUT (char *)"TransactionTimeout"


#define TXSOCK_ERR_TIMEOUT	-1
#define TXSOCK_ERR		-2
#define TXSOCK_ERR_INTERRUPT	-3

// the default number of parallel streams PER physical connection
// 0 means that the multistream support is disabled
#define DFLT_MULTISTREAMCNT     0
#define NAME_MULTISTREAMCNT     (char *)"ParStreamsPerPhyConn"

// The minimum size to use to split big single requests
//  through multiple streams
#define DFLT_MULTISTREAMSPLITSIZE (128*1024)

// keep/dont-keep the socket open (required by optimized rootd fallback)
#define DFLT_KEEPSOCKOPENIFNOTXRD 0
#define NAME_KEEPSOCKOPENIFNOTXRD (char *)"KeepSockOpenIfNotXrd"

// Printable version
#define XRD_CLIENT_VERSION      (char *)"kXR_ver002+kXR_asyncap"

// Version and capabilities sent to the server
#define XRD_CLIENT_CURRENTVER   (kXR_ver002)
#define XRD_CLIENT_CAPVER       ((kXR_char)kXR_asyncap | XRD_CLIENT_CURRENTVER)

// Defaults for ReadAhead and Cache
#define DFLT_READCACHESIZE      0
#define NAME_READCACHESIZE      (char *)"ReadCacheSize"

// 0 = LRU
// 1 = Remove least offest
#define DFLT_READCACHEBLKREMPOLICY       0
#define NAME_READCACHEBLKREMPOLICY       (char *)"ReadCacheBlkRemPolicy"

#define DFLT_READAHEADSIZE      (0)
#define NAME_READAHEADSIZE      (char *)"ReadAheadSize"

// Align all the read requests to multiples of a number
#define DFLT_READTRIMBLKSZ      (0)
#define NAME_READTRIMBLKSZ      (char *)"ReadTrimBlockSize"

// The default read ahead strategy to use
#define DFLT_READAHEADSTRATEGY  (1) // This is the sequential readahead
#define NAME_READAHEADSTRATEGY  (char *)"ReadAheadStrategy"


// To be used in copy-like apps when the data is to be accessed only once
// ... to reduce additional cache overhead
#define DFLT_REMUSEDCACHEBLKS   0
#define NAME_REMUSEDCACHEBLKS   (char *)"RemoveUsedCacheBlocks"

// When writing async, purge immediately the written blocks from the cache
#define DFLT_PURGEWRITTENBLOCKS   0
#define NAME_PURGEWRITTENBLOCKS    (char *)"PurgeWrittenBlocks"

#define NAME_REDIRDOMAINALLOW_RE   (char *)"RedirDomainAllowRE"
#define NAME_REDIRDOMAINDENY_RE    (char *)"RedirDomainDenyRE"
#define NAME_CONNECTDOMAINALLOW_RE (char *)"ConnectDomainAllowRE"
#define NAME_CONNECTDOMAINDENY_RE  (char *)"ConnectDomainDenyRE"

#define PROTO (char *)"root"

// The max number of threads spawned to do parallel opens
// Note for dummies: this is not the max number of parallel opens
#define DFLT_MAXCONCURRENTOPENS    100

#define READV_MAXCHUNKS            512
#define READV_MAXCHUNKSIZE         (1024*192)

// SOCKS4 support
#define NAME_SOCKS4HOST            (char *)"Socks4Server"
#define NAME_SOCKS4PORT            (char *)"Socks4Port"

// Default TCP windows size
// A value of '0' implies "use the default OS settings"
// which enables window scaling on some platforms (linux, MacOsX)
// but may be to small on others (solaris); the preprocessor macro
// is set based on the platform information found in configure
#if defined(__linux__) || defined(__macos__)
#define DFLT_DFLTTCPWINDOWSIZE     (0)
#else
#define DFLT_DFLTTCPWINDOWSIZE     (262144)
#endif
#define NAME_DFLTTCPWINDOWSIZE     (char *)"DfltTcpWindowSize"

// A connection towards a data server timeouts quickly
#define DFLT_DATASERVERCONN_TTL    300
#define NAME_DATASERVERCONN_TTL    (char *)"DataServerConn_ttl"

// A connection towards a Load Balancer timeouts after many seconds of no use
#define DFLT_LBSERVERCONN_TTL      1200
#define NAME_LBSERVERCONN_TTL      (char *)"LBServerConn_ttl"

#define TRUE  1
#define FALSE 0

#define xrdmin(a, b) (a < b ? a : b)
#define xrdmax(a, b) (a > b ? a : b)

#endif
