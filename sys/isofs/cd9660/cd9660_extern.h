/*	$OpenBSD: cd9660_extern.h,v 1.1 1997/11/08 20:54:29 niklas Exp $	*/
/*	$NetBSD: cd9660_extern.h,v 1.1 1997/01/24 00:24:53 cgd Exp $	*/

/*-
 * Copyright (c) 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley
 * by Pace Willisson (pace@blitz.com).  The Rock Ridge Extension
 * Support code is derived from software contributed to Berkeley
 * by Atsushi Murai (amurai@spec.co.jp).
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)iso.h	8.4 (Berkeley) 12/5/94
 */

/*
 * Definitions used in the kernel for cd9660 file system support.
 */

/* CD-ROM Format type */
enum ISO_FTYPE  { ISO_FTYPE_DEFAULT, ISO_FTYPE_9660, ISO_FTYPE_RRIP, ISO_FTYPE_ECMA };

#ifndef	ISOFSMNT_ROOT
#define	ISOFSMNT_ROOT	0
#endif

struct iso_mnt {
	int im_flags;

	struct mount *im_mountp;
	dev_t im_dev;
	struct vnode *im_devvp;

	int logical_block_size;
	int im_bshift;
	int im_bmask;
	
	int volume_space_size;
	struct netexport im_export;
	
	char root[ISODCL (157, 190)];
	int root_extent;
	int root_size;
	enum ISO_FTYPE  iso_ftype;
	
	int rr_skip;
	int rr_skip0;
};

#define VFSTOISOFS(mp)	((struct iso_mnt *)((mp)->mnt_data))

#define blkoff(imp, loc)	((loc) & (imp)->im_bmask)
#define lblktosize(imp, blk)	((blk) << (imp)->im_bshift)
#define lblkno(imp, loc)	((loc) >> (imp)->im_bshift)
#define blksize(imp, ip, lbn)	((imp)->logical_block_size)

int cd9660_mount __P((struct mount *,
	    char *, caddr_t, struct nameidata *, struct proc *));
int cd9660_start __P((struct mount *, int, struct proc *));
int cd9660_unmount __P((struct mount *, int, struct proc *));
int cd9660_root __P((struct mount *, struct vnode **));
int cd9660_quotactl __P((struct mount *, int, uid_t, caddr_t, struct proc *));
int cd9660_statfs __P((struct mount *, struct statfs *, struct proc *));
int cd9660_sync __P((struct mount *, int, struct ucred *, struct proc *));
int cd9660_vget __P((struct mount *, ino_t, struct vnode **));
int cd9660_fhtovp __P((struct mount *, struct fid *, struct mbuf *,
	    struct vnode **, int *, struct ucred **));
int cd9660_vptofh __P((struct vnode *, struct fid *));
int cd9660_init __P((struct vfsconf *));
#define cd9660_sysctl ((int (*) __P((int *, u_int, void *, size_t *, void *, \
                                    size_t, struct proc *)))eopnotsupp)

int cd9660_mountroot __P((void)); 

extern int (**cd9660_vnodeop_p) __P((void *));
extern int (**cd9660_specop_p) __P((void *));
#ifdef FIFO
extern int (**cd9660_fifoop_p) __P((void *));
#endif

int isofncmp __P((const u_char *, int, const u_char *, int));
void isofntrans __P((u_char *, int, u_char *, u_short *, int, int));
ino_t isodirino __P((struct iso_directory_record *, struct iso_mnt *));
