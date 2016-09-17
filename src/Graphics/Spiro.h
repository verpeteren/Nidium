/*
   Copyright 2016 Nidium Inc. All rights reserved.
   Use of this source code is governed by a MIT license
   that can be found in the LICENSE file.
*/
#ifndef graphics_bezier_h__
#define graphics_bezier_h__

#include <spiroentrypoints.h>
#include "Graphics/SkiaContext.h"

namespace Nidium {
namespace Graphics {

/* This is a superclass of bezctx */
typedef struct bezctx_skia {
    /*this is the entry for the base */
    struct {
    void (*moveto)(bezctx *bc, double x, double y, int is_open);
    void (*lineto)(bezctx *bc, double x, double y);
    void (*quadto)(bezctx *bc, double x1, double y1, double x2, double y2);
    void (*curveto)(bezctx *bc, double x1, double y1, double x2, double y2,
            double x3, double y3);
    void (*mark_knot)(bezctx *bc, int knot_idx);
    } base;
	/* Our modifications */
	Nidium::Graphics::SkiaContext * skiactx;
	bool gotnans;
	int is_open;
	double x, y; //TODO: reuse this later to create plate file
} bezctx_skia;


bezctx_skia * new_bezctx_skia(Nidium::Graphics::SkiaContext *skiactx);
void bezctx_skia_close(bezctx_skia *bc);

class Spiro {
public:
	static void nancheck(bezctx_skia *bc);
	static void bezctx_skia_moveto(bezctx *z, double x, double y, int is_open);
	static void bezctx_skia_lineto(bezctx *z, double x, double y);
	static void bezctx_skia_quadto(bezctx *z, double xm, double ym, double x3, double y3);
	static void bezctx_skia_curveto(bezctx *z, double x1, double y1, double x2, double y2, double x3, double y3);
	static bezctx_skia * new_bezctx_skia(Nidium::Graphics::SkiaContext *skiactx);
	static void bezctx_skia_close(bezctx_skia *z);
};

} // namespace Graphics
} // namespace Nidium

#endif

