/*
 * Copyright (C) 2017  Elvis Teixeira
 *
 * This source code is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any
 * later version.
 *
 * This source code is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SLOPE_CONFIG_H
#define SLOPE_CONFIG_H

#define SLOPE_MAJOR_VERSION  2

#define SLOPE_MINOR_VERSION  6

#define SLOPE_PATCH_VERSION  0

#define SLOPE_VERSION_STRING  "2.6.0"

#define SLOPE_BUILD_DEBUG  0

#if SLOPE_BUILD_DEBUG
#  define SLOPE_ASSERT assert
#else
#  define SLOPE_ASSERT /* nothing */
#endif

#endif /* SLOPE_CONFIG_H */
