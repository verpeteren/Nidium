/*
   Copyright 2016 Nidium Inc. All rights reserved.
   Use of this source code is governed by a MIT license
   that can be found in the LICENSE file.
*/
#include "Graphics/Spiro.h"

namespace Nidium {
namespace Graphics {

// {{{ Spiro Context

void Spiro::nancheck(bezctx_skia *bc) {
	if ( !bc->gotnans ) {       /* Called when we get passed a NaN. */
		bc->gotnans = true;
	}
}

/* This routine starts a new contour */
void Spiro::bezctx_skia_moveto(bezctx *z, double x, double y, int is_open) {
	bezctx_skia *bc = (bezctx_skia *)z;

	if ( !finite(x) || !finite(y)) {
		/* Protection against NaNs */
		Spiro::nancheck(bc);
		x = y = 0;
	}

	if (!bc->is_open) {
		//fprintf(bc->f, "z\n");
	}
	bc->skiactx->moveTo(x, y);
	//fprintf(bc->f, "%g %g m\n", x, y);
	bc->is_open = (is_open == 1) ? true: false ;
	bc->x = x;
	bc->y = y;
}

/* This routine creates a linear spline from the previous point specified to this one */
void Spiro::bezctx_skia_lineto(bezctx *z, double x, double y) {
	bezctx_skia *bc = (bezctx_skia *)z;

	if ( !finite(x) || !finite(y)) {
		Spiro::nancheck(bc);
		x = y = 0;
    }
	//fprintf(bc->f, "%g %g l\n", x, y);
	bc->skiactx->lineTo(x, y);
	bc->x = x;
	bc->y = y;
}

void Spiro::bezctx_skia_quadto(bezctx *z, double xm, double ym, double x3, double y3) {
	bezctx_skia *bc = (bezctx_skia *)z;
	if ( !finite(xm) || !finite(ym) || !finite(x3) || !finite(y3)) {
		Spiro::nancheck(bc);
		xm = ym = x3 = y3 = 0;
	}

	//fprintf(bc->f, "%g %g %g %g %g %g c\n", x1, y1, x2, y2, x3, y3);
	bc->skiactx->quadraticCurveTo(xm, ym, x3, y3); //TODO: check, is it the other way around?
	bc->x = x3;
	bc->y = y3;
}

void Spiro::bezctx_skia_curveto(bezctx *z, double x1, double y1, double x2, double y2, double x3, double y3) {
	bezctx_skia *bc = (bezctx_skia *)z;

	if ( !finite(x1) || !finite(y1) || !finite(x2) || !finite(y2) || !finite(x3) || !finite(y3)) {
		Spiro::nancheck(bc);
		x1 = y1 = x2 = y2 = x3 = y3 = 0;
	}
	//fprintf(bc->f, "%g %g %g %g %g %g c\n", x1, y1, x2, y2, x3, y3);
	bc->skiactx->bezierCurveTo(x1, y1, x2, y2, x3, y3); //todo check
	bc->x = x3;
	bc->y = y3;
}

/* Allocates and initializes a new PostScript bezier context */
bezctx_skia * Spiro::new_bezctx_skia(Nidium::Graphics::SkiaContext *skiactx) {
	 bezctx_skia *result = (bezctx_skia*) malloc(sizeof(*result) * 1);
    //todo handle error

	result->base.moveto = bezctx_skia_moveto;
	result->base.lineto = bezctx_skia_lineto;
	result->base.quadto = bezctx_skia_quadto;
	result->base.curveto = bezctx_skia_curveto;
	result->base.mark_knot = NULL;
	result->is_open = true;
	result->skiactx = skiactx;
	result->gotnans = false;

    result->skiactx->beginPath();

    return result;
}

/* Finishes an old PostScript bezier context */
void Spiro::bezctx_skia_close(bezctx_skia *z) {
	bezctx_skia *bc = (bezctx_skia *)z;

	if (!bc->is_open) {
		//fprintf(bc->skiactx, "z\n");
	}
	bc->skiactx->closePath();
	free(bc);
}
// }}}


} // namespace Graphics
} // namespace Nidium

