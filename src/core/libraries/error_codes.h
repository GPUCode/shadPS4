// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

constexpr int ORBIS_OK = 0;

// posix error codes
constexpr int POSIX_EPERM = 1;
constexpr int POSIX_ENOENT = 2;
constexpr int POSIX_ESRCH = 3;
constexpr int POSIX_EINTR = 4;
constexpr int POSIX_EIO = 5;
constexpr int POSIX_ENXIO = 6;
constexpr int POSIX_E2BIG = 7;
constexpr int POSIX_ENOEXEC = 8;
constexpr int POSIX_EBADF = 9;
constexpr int POSIX_ECHILD = 10;
constexpr int POSIX_EDEADLK = 11;
constexpr int POSIX_ENOMEM = 12;
constexpr int POSIX_EACCES = 13;
constexpr int POSIX_EFAULT = 14;
constexpr int POSIX_ENOTBLK = 15;
constexpr int POSIX_EBUSY = 16;
constexpr int POSIX_EEXIST = 17;
constexpr int POSIX_EXDEV = 18;
constexpr int POSIX_ENODEV = 19;
constexpr int POSIX_ENOTDIR = 20;
constexpr int POSIX_EISDIR = 21;
constexpr int POSIX_EINVAL = 22;
constexpr int POSIX_ENFILE = 23;
constexpr int POSIX_EMFILE = 24;
constexpr int POSIX_ENOTTY = 25;
constexpr int POSIX_ETXTBSY = 26;
constexpr int POSIX_EFBIG = 27;
constexpr int POSIX_ENOSPC = 28;
constexpr int POSIX_ESPIPE = 29;
constexpr int POSIX_EROFS = 30;
constexpr int POSIX_EMLINK = 31;
constexpr int POSIX_EPIPE = 32;
constexpr int POSIX_EDOM = 33;
constexpr int POSIX_ERANGE = 34;
constexpr int POSIX_EAGAIN = 35;
constexpr int POSIX_EWOULDBLOCK = 35;
constexpr int POSIX_EINPROGRESS = 36;
constexpr int POSIX_EALREADY = 37;
constexpr int POSIX_ENOTSOCK = 38;
constexpr int POSIX_EDESTADDRREQ = 39;
constexpr int POSIX_EMSGSIZE = 40;
constexpr int POSIX_EPROTOTYPE = 41;
constexpr int POSIX_ENOPROTOOPT = 42;
constexpr int POSIX_EPROTONOSUPPORT = 43;
constexpr int POSIX_ESOCKTNOSUPPORT = 44;
constexpr int POSIX_EOPNOTSUPP = 45;
constexpr int POSIX_ENOTSUP = 45;
constexpr int POSIX_EPFNOSUPPORT = 46;
constexpr int POSIX_EAFNOSUPPORT = 47;
constexpr int POSIX_EADDRINUSE = 48;
constexpr int POSIX_EADDRNOTAVAIL = 49;
constexpr int POSIX_ENETDOWN = 50;
constexpr int POSIX_ENETUNREACH = 51;
constexpr int POSIX_ENETRESET = 52;
constexpr int POSIX_ECONNABORTED = 53;
constexpr int POSIX_ECONNRESET = 54;
constexpr int POSIX_ENOBUFS = 55;
constexpr int POSIX_EISCONN = 56;
constexpr int POSIX_ENOTCONN = 57;
constexpr int POSIX_ESHUTDOWN = 58;
constexpr int POSIX_ETOOMANYREFS = 59;
constexpr int POSIX_ETIMEDOUT = 60;
constexpr int POSIX_ECONNREFUSED = 61;
constexpr int POSIX_ELOOP = 62;
constexpr int POSIX_ENAMETOOLONG = 63;
constexpr int POSIX_EHOSTDOWN = 64;
constexpr int POSIX_EHOSTUNREACH = 65;
constexpr int POSIX_ENOTEMPTY = 66;
constexpr int POSIX_EPROCLIM = 67;
constexpr int POSIX_EUSERS = 68;
constexpr int POSIX_EDQUOT = 69;
constexpr int POSIX_ESTALE = 70;
constexpr int POSIX_EREMOTE = 71;
constexpr int POSIX_EBADRPC = 72;
constexpr int POSIX_ERPCMISMATCH = 73;
constexpr int POSIX_EPROGUNAVAIL = 74;
constexpr int POSIX_EPROGMISMATCH = 75;
constexpr int POSIX_EPROCUNAVAIL = 76;
constexpr int POSIX_ENOLCK = 77;
constexpr int POSIX_ENOSYS = 78;
constexpr int POSIX_EFTYPE = 79;
constexpr int POSIX_EAUTH = 80;
constexpr int POSIX_ENEEDAUTH = 81;
constexpr int POSIX_EIDRM = 82;
constexpr int POSIX_ENOMSG = 83;
constexpr int POSIX_EOVERFLOW = 84;
constexpr int POSIX_ECANCELED = 85;
constexpr int POSIX_EILSEQ = 86;
constexpr int POSIX_ENOATTR = 87;
constexpr int POSIX_EDOOFUS = 88;
constexpr int POSIX_EBADMSG = 89;
constexpr int POSIX_EMULTIHOP = 90;
constexpr int POSIX_ENOLINK = 91;
constexpr int POSIX_EPROTO = 92;
constexpr int POSIX_ENOTCAPABLE = 93;
constexpr int POSIX_ECAPMODE = 94;
constexpr int POSIX_ENOBLK = 95;
constexpr int POSIX_EICV = 96;
constexpr int POSIX_ENOPLAYGOENT = 97;
constexpr int POSIX_EREVOKE = 98;
constexpr int POSIX_ESDKVERSION = 99;
constexpr int POSIX_ESTART = 100;
constexpr int POSIX_ESTOP = 101;
constexpr int POSIX_EINVALID2MB = 102;
constexpr int POSIX_ELAST = 102;
constexpr int POSIX_EADHOC = 160;
constexpr int POSIX_EINACTIVEDISABLED = 163;
constexpr int POSIX_ENETNODATA = 164;
constexpr int POSIX_ENETDESC = 165;
constexpr int POSIX_ENETDESCTIMEDOUT = 166;
constexpr int POSIX_ENETINTR = 167;
constexpr int POSIX_ERETURN = 205;
constexpr int POSIX_EFPOS = 152;
constexpr int POSIX_ENODATA = 1040;
constexpr int POSIX_ENOSR = 1050;
constexpr int POSIX_ENOSTR = 1051;
constexpr int POSIX_ENOTRECOVERABLE = 1056;
constexpr int POSIX_EOTHER = 1062;
constexpr int POSIX_EOWNERDEAD = 1064;
constexpr int POSIX_ETIME = 1074;

constexpr int SCE_OK = 0;

// kernel error codes
constexpr int SCE_KERNEL_ERROR_UNKNOWN = 0x80020000;
constexpr int SCE_KERNEL_ERROR_EPERM = 0x80020001;
constexpr int SCE_KERNEL_ERROR_ENOENT = 0x80020002;
constexpr int SCE_KERNEL_ERROR_ESRCH = 0x80020003;
constexpr int SCE_KERNEL_ERROR_EINTR = 0x80020004;
constexpr int SCE_KERNEL_ERROR_EIO = 0x80020005;
constexpr int SCE_KERNEL_ERROR_ENXIO = 0x80020006;
constexpr int SCE_KERNEL_ERROR_E2BIG = 0x80020007;
constexpr int SCE_KERNEL_ERROR_ENOEXEC = 0x80020008;
constexpr int SCE_KERNEL_ERROR_EBADF = 0x80020009;
constexpr int SCE_KERNEL_ERROR_ECHILD = 0x8002000A;
constexpr int SCE_KERNEL_ERROR_EDEADLK = 0x8002000B;
constexpr int SCE_KERNEL_ERROR_ENOMEM = 0x8002000C;
constexpr int SCE_KERNEL_ERROR_EACCES = 0x8002000D;
constexpr int SCE_KERNEL_ERROR_EFAULT = 0x8002000E;
constexpr int SCE_KERNEL_ERROR_ENOTBLK = 0x8002000F;
constexpr int SCE_KERNEL_ERROR_EBUSY = 0x80020010;
constexpr int SCE_KERNEL_ERROR_EEXIST = 0x80020011;
constexpr int SCE_KERNEL_ERROR_EXDEV = 0x80020012;
constexpr int SCE_KERNEL_ERROR_ENODEV = 0x80020013;
constexpr int SCE_KERNEL_ERROR_ENOTDIR = 0x80020014;
constexpr int SCE_KERNEL_ERROR_EISDIR = 0x80020015;
constexpr int SCE_KERNEL_ERROR_EINVAL = 0x80020016;
constexpr int SCE_KERNEL_ERROR_ENFILE = 0x80020017;
constexpr int SCE_KERNEL_ERROR_EMFILE = 0x80020018;
constexpr int SCE_KERNEL_ERROR_ENOTTY = 0x80020019;
constexpr int SCE_KERNEL_ERROR_ETXTBSY = 0x8002001A;
constexpr int SCE_KERNEL_ERROR_EFBIG = 0x8002001B;
constexpr int SCE_KERNEL_ERROR_ENOSPC = 0x8002001C;
constexpr int SCE_KERNEL_ERROR_ESPIPE = 0x8002001D;
constexpr int SCE_KERNEL_ERROR_EROFS = 0x8002001E;
constexpr int SCE_KERNEL_ERROR_EMLINK = 0x8002001F;
constexpr int SCE_KERNEL_ERROR_EPIPE = 0x80020020;
constexpr int SCE_KERNEL_ERROR_EDOM = 0x80020021;
constexpr int SCE_KERNEL_ERROR_ERANGE = 0x80020022;
constexpr int SCE_KERNEL_ERROR_EAGAIN = 0x80020023;
constexpr int SCE_KERNEL_ERROR_EWOULDBLOCK = 0x80020023;
constexpr int SCE_KERNEL_ERROR_EINPROGRESS = 0x80020024;
constexpr int SCE_KERNEL_ERROR_EALREADY = 0x80020025;
constexpr int SCE_KERNEL_ERROR_ENOTSOCK = 0x80020026;
constexpr int SCE_KERNEL_ERROR_EDESTADDRREQ = 0x80020027;
constexpr int SCE_KERNEL_ERROR_EMSGSIZE = 0x80020028;
constexpr int SCE_KERNEL_ERROR_EPROTOTYPE = 0x80020029;
constexpr int SCE_KERNEL_ERROR_ENOPROTOOPT = 0x8002002A;
constexpr int SCE_KERNEL_ERROR_EPROTONOSUPPORT = 0x8002002B;
constexpr int SCE_KERNEL_ERROR_ESOCKTNOSUPPORT = 0x8002002C;
constexpr int SCE_KERNEL_ERROR_EOPNOTSUPP = 0x8002002D;
constexpr int SCE_KERNEL_ERROR_ENOTSUP = 0x8002002D;
constexpr int SCE_KERNEL_ERROR_EPFNOSUPPORT = 0x8002002E;
constexpr int SCE_KERNEL_ERROR_EAFNOSUPPORT = 0x8002002F;
constexpr int SCE_KERNEL_ERROR_EADDRINUSE = 0x80020030;
constexpr int SCE_KERNEL_ERROR_EADDRNOTAVAIL = 0x80020031;
constexpr int SCE_KERNEL_ERROR_ENETDOWN = 0x80020032;
constexpr int SCE_KERNEL_ERROR_ENETUNREACH = 0x80020033;
constexpr int SCE_KERNEL_ERROR_ENETRESET = 0x80020034;
constexpr int SCE_KERNEL_ERROR_ECONNABORTED = 0x80020035;
constexpr int SCE_KERNEL_ERROR_ECONNRESET = 0x80020036;
constexpr int SCE_KERNEL_ERROR_ENOBUFS = 0x80020037;
constexpr int SCE_KERNEL_ERROR_EISCONN = 0x80020038;
constexpr int SCE_KERNEL_ERROR_ENOTCONN = 0x80020039;
constexpr int SCE_KERNEL_ERROR_ESHUTDOWN = 0x8002003A;
constexpr int SCE_KERNEL_ERROR_ETOOMANYREFS = 0x8002003B;
constexpr int SCE_KERNEL_ERROR_ETIMEDOUT = 0x8002003C;
constexpr int SCE_KERNEL_ERROR_ECONNREFUSED = 0x8002003D;
constexpr int SCE_KERNEL_ERROR_ELOOP = 0x8002003E;
constexpr int SCE_KERNEL_ERROR_ENAMETOOLONG = 0x8002003F;
constexpr int SCE_KERNEL_ERROR_EHOSTDOWN = 0x80020040;
constexpr int SCE_KERNEL_ERROR_EHOSTUNREACH = 0x80020041;
constexpr int SCE_KERNEL_ERROR_ENOTEMPTY = 0x80020042;
constexpr int SCE_KERNEL_ERROR_EPROCLIM = 0x80020043;
constexpr int SCE_KERNEL_ERROR_EUSERS = 0x80020044;
constexpr int SCE_KERNEL_ERROR_EDQUOT = 0x80020045;
constexpr int SCE_KERNEL_ERROR_ESTALE = 0x80020046;
constexpr int SCE_KERNEL_ERROR_EREMOTE = 0x80020047;
constexpr int SCE_KERNEL_ERROR_EBADRPC = 0x80020048;
constexpr int SCE_KERNEL_ERROR_ERPCMISMATCH = 0x80020049;
constexpr int SCE_KERNEL_ERROR_EPROGUNAVAIL = 0x8002004A;
constexpr int SCE_KERNEL_ERROR_EPROGMISMATCH = 0x8002004B;
constexpr int SCE_KERNEL_ERROR_EPROCUNAVAIL = 0x8002004C;
constexpr int SCE_KERNEL_ERROR_ENOLCK = 0x8002004D;
constexpr int SCE_KERNEL_ERROR_ENOSYS = 0x8002004E;
constexpr int SCE_KERNEL_ERROR_EFTYPE = 0x8002004F;
constexpr int SCE_KERNEL_ERROR_EAUTH = 0x80020050;
constexpr int SCE_KERNEL_ERROR_ENEEDAUTH = 0x80020051;
constexpr int SCE_KERNEL_ERROR_EIDRM = 0x80020052;
constexpr int SCE_KERNEL_ERROR_ENOMSG = 0x80020053;
constexpr int SCE_KERNEL_ERROR_EOVERFLOW = 0x80020054;
constexpr int SCE_KERNEL_ERROR_ECANCELED = 0x80020055;
constexpr int SCE_KERNEL_ERROR_EILSEQ = 0x80020056;
constexpr int SCE_KERNEL_ERROR_ENOATTR = 0x80020057;
constexpr int SCE_KERNEL_ERROR_EDOOFUS = 0x80020058;
constexpr int SCE_KERNEL_ERROR_EBADMSG = 0x80020059;
constexpr int SCE_KERNEL_ERROR_EMULTIHOP = 0x8002005A;
constexpr int SCE_KERNEL_ERROR_ENOLINK = 0x8002005B;
constexpr int SCE_KERNEL_ERROR_EPROTO = 0x8002005C;
constexpr int SCE_KERNEL_ERROR_ENOTCAPABLE = 0x8002005D;
constexpr int SCE_KERNEL_ERROR_ECAPMODE = 0x8002005E;
constexpr int SCE_KERNEL_ERROR_ENOBLK = 0x8002005F;
constexpr int SCE_KERNEL_ERROR_EICV = 0x80020060;
constexpr int SCE_KERNEL_ERROR_ENOPLAYGOENT = 0x80020061;
constexpr int SCE_KERNEL_ERROR_EREVOKE = 0x80020062;
constexpr int SCE_KERNEL_ERROR_ESDKVERSION = 0x80020063;
constexpr int SCE_KERNEL_ERROR_ESTART = 0x80020064;
constexpr int SCE_KERNEL_ERROR_ESTOP = 0x80020065;

// videoOut
constexpr int SCE_VIDEO_OUT_ERROR_INVALID_VALUE = 0x80290001;   // invalid argument
constexpr int SCE_VIDEO_OUT_ERROR_INVALID_ADDRESS = 0x80290002; // invalid addresses
constexpr int SCE_VIDEO_OUT_ERROR_INVALID_PITCH = 0x80290004;
constexpr int SCE_VIDEO_OUT_ERROR_INVALID_TILING_MODE = 0x80290007;  // invalid tiling mode
constexpr int SCE_VIDEO_OUT_ERROR_INVALID_ASPECT_RATIO = 0x80290008; // invalid aspect ration
constexpr int SCE_VIDEO_OUT_ERROR_RESOURCE_BUSY = 0x80290009;        // already opened
constexpr int SCE_VIDEO_OUT_ERROR_INVALID_INDEX = 0x8029000A;        // invalid buffer index
constexpr int SCE_VIDEO_OUT_ERROR_INVALID_HANDLE = 0x8029000B;       // invalid handle
constexpr int SCE_VIDEO_OUT_ERROR_INVALID_EVENT_QUEUE = 0x8029000C;  // Invalid event queue
constexpr int SCE_VIDEO_OUT_ERROR_NO_EMPTY_SLOT = 0x8029000F;
constexpr int SCE_VIDEO_OUT_ERROR_SLOT_OCCUPIED = 0x80290010;   // slot already used
constexpr int SCE_VIDEO_OUT_ERROR_FLIP_QUEUE_FULL = 0x80290012; // flip queue is full
constexpr int SCE_VIDEO_OUT_ERROR_INVALID_OPTION = 0x8029001A;  // Invalid buffer attribute option
