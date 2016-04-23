/***************************************************************************
 *
 * Multitouch X driver
 * Copyright (C) 2008 Henrik Rydberg <rydberg@euromail.se>
 * Copyright (C) 2011 Ryan Bourgeois <bluedragonx@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 **************************************************************************/

#ifndef COMMON_H
#define COMMON_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xorg-server.h>
#include <xf86.h>
#include <xf86_OSproc.h>
#include <xf86Xinput.h>
#include <errno.h>
#include <mtdev-mapping.h>
#include <mtdev-plumbing.h>
#include <stdint.h>
#include <sys/time.h>

#define DIM_FINGER 32
#define DIM_TOUCHES 32

/* year-proof millisecond event time */
typedef __u64 mstime_t;

/* all bit masks have this type */
typedef unsigned int bitmask_t;

#define BITMASK(x) (1U << (x))
#define BITONES(x) (BITMASK(x) - 1U)
#define GETBIT(m, x) (((m) >> (x)) & 1U)
#define SETBIT(m, x) (m |= BITMASK(x))
#define CLEARBIT(m, x) (m &= ~BITMASK(x))
#define MODBIT(m, x, b) ((b) ? SETBIT(m, x) : CLEARBIT(m, x))
#define PRBITMASK "0x%x"

#define ABSVAL(x) ((x) < 0 ? -1*(x) : (x))
#define MINVAL(x, y) ((x) < (y) ? (x) : (y)) 
#define MAXVAL(x, y) ((x) > (y) ? (x) : (y))
#define MODVAL(x, y) ((x) - ((int)((x) / (y))) * (y))
#define SQRVAL(x) ((x) * (x))
#define CLAMPVAL(x, min, max) MAXVAL(MINVAL(x, max), min)

/* Retrieve the current time and place it in tv.
 */
static inline void microtime(struct timeval* tv)
{
	gettimeofday(tv, NULL);
}

/* Copy one time value to another.
 */
static inline void timercp(struct timeval* dest, const struct timeval* src)
{
	memcpy(dest, src, sizeof(struct timeval));
}

/* Convert a timeval to milliseconds since the epoch. Truncates additional
 * timer resolution effectively rounding down.
 */
static inline mstime_t timertoms(const struct timeval* tv)
{
	return (mstime_t)(tv->tv_sec*1000) + (mstime_t)(tv->tv_usec/1000);
}

/* Convert a value in milliseconds to a timeval and place the value in tv.
 */
static inline void timerfromms(struct timeval* tv, const mstime_t ms)
{
	tv->tv_sec = (time_t)(ms/1000);
	tv->tv_usec = (suseconds_t)((ms%1000)*1000);
}

/* Convert a timeval to microseconds.
 */
static inline suseconds_t timertomicro(const struct timeval* tv)
{
	return tv->tv_sec * 1000000 + tv->tv_usec;
}

/* Add milliseconds to a timeval and place the resulting value in dest.
 */
static inline void timeraddms(const struct timeval* a, const mstime_t b, struct timeval* dest)
{
	struct timeval tv;
	timerfromms(&tv, b);
	timeradd(a, &tv, dest);
}

/* Clamp value to 15 bits.
 */
static inline int clamp15(int x)
{
	return x < -32767 ? -32767 : x > 32767 ? 32767 : x;
}

/* Absolute scale is assumed to fit in 15 bits.
 */
static inline int dist2(int dx, int dy)
{
	dx = clamp15(dx);
	dy = clamp15(dy);
	return dx * dx + dy * dy;
}

/* Count number of bits (Sean Eron Andersson's Bit Hacks).
 */
static inline int bitcount(unsigned v)
{
	v -= ((v>>1) & 0x55555555);
	v = (v&0x33333333) + ((v>>2) & 0x33333333);
	return (((v + (v>>4)) & 0xF0F0F0F) * 0x1010101) >> 24;
}

/* Return index of first bit [0-31], -1 on zero\
 */
#define firstbit(v) (__builtin_ffs(v) - 1)

/* Boost-style foreach bit.
 */
#define foreach_bit(i, m)						\
	for (i = firstbit(m); i >= 0; i = firstbit((m) & (~0U << i + 1)))

/* Robust system ioctl calls.
 */
#define SYSCALL(call) while (((call) == -1) && (errno == EINTR))

#endif
