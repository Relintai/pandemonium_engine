#ifndef RENDERING_SERVER_CONSTANTS_H
#define RENDERING_SERVER_CONSTANTS_H

/*************************************************************************/
/*  rendering_server_constants.h                                         */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

// Use for constants etc that need not be included as often as RenderingServer.h
// to reduce dependencies and prevent slow compilation.

// This is a "cheap" include, and can be used from scene side code as well as servers.

// Uncomment to provide comparison of node culling versus item culling
// #define RENDERING_SERVER_CANVAS_TIME_NODE_CULLING

// N.B. ONLY allow these defined in DEV_ENABLED builds, they will slow
// performance, and are only necessary to use for debugging.
#ifdef DEV_ENABLED

// Uncomment this define to store canvas item names in RenderingServerCanvas.
// This is relatively expensive, but is invaluable for debugging the canvas scene tree
// especially using _print_tree() in RenderingServerCanvas.
// #define RENDERING_SERVER_CANVAS_DEBUG_ITEM_NAMES

// Uncomment this define to verify local bounds of canvas items,
// to check that the hierarchical culling is working correctly.
// This is expensive.
// #define RENDERING_SERVER_CANVAS_CHECK_BOUNDS

// Uncomment this define to produce debugging output for physics interpolation.
// #define VISUAL_SERVER_DEBUG_PHYSICS_INTERPOLATION

#endif // DEV_ENABLED

#endif // RENDERING_SERVER_CONSTANTS_H
