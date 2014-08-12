/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

/**
 * This file is included by src/gfile/gfile.c
 */

/********************************************************
 * The ChibiOS BaseFileStream file-system VMT
 ********************************************************/

static void ChibiOSBFSClose(GFILE *f);
static int ChibiOSBFSRead(GFILE *f, void *buf, int size);
static int ChibiOSBFSWrite(GFILE *f, const void *buf, int size);
static bool_t ChibiOSBFSSetpos(GFILE *f, long int pos);
static long int ChibiOSBFSGetsize(GFILE *f);
static bool_t ChibiOSBFSEof(GFILE *f);

static const GFILEVMT FsCHIBIOSVMT = {
	0,													// next
	GFSFLG_SEEKABLE|GFSFLG_WRITEABLE,					// flags
	0,													// prefix
	0, 0, 0, 0,
	0, ChibiOSBFSClose, ChibiOSBFSRead, ChibiOSBFSWrite,
	ChibiOSBFSSetpos, ChibiOSBFSGetsize, ChibiOSBFSEof,
	0, 0, 0,
	#if GFILE_NEED_FILELISTS
		0, 0, 0,
	#endif
};

static void ChibiOSBFSClose(GFILE *f) {
	chFileStreamClose(((BaseFileStream *)f->obj));
}
static int ChibiOSBFSRead(GFILE *f, void *buf, int size) {
	return chSequentialStreamRead(((BaseFileStream *)f->obj), (uint8_t *)buf, size);
}
static int ChibiOSBFSWrite(GFILE *f, const void *buf, int size) {
	return chSequentialStreamWrite(((BaseFileStream *)f->obj), (uint8_t *)buf, size);
}
static bool_t ChibiOSBFSSetpos(GFILE *f, long int pos) {
	chFileStreamSeek(((BaseFileStream *)f->obj), pos);
	return TRUE;
}
static long int ChibiOSBFSGetsize(GFILE *f) { return chFileStreamGetSize(((BaseFileStream *)f->obj)); }
static bool_t ChibiOSBFSEof(GFILE *f) { return f->pos >= chFileStreamGetSize(((BaseFileStream *)f->obj)); }

GFILE *		gfileOpenBaseFileStream(void *BaseFileStreamPtr, const char *mode) {
	GFILE *			f;

	// Get an empty file and set the flags
	if (!(f = findemptyfile(mode)))
		return 0;

	// File is open - fill in all the details
	f->vmt = &FsCHIBIOSVMT;
	f->obj = BaseFileStreamPtr;
	f->pos = 0;
	f->flags |= GFILEFLG_OPEN|GFILEFLG_CANSEEK;
	return f;
}
