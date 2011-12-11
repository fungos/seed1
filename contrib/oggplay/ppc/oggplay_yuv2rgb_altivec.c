#include "oggplay_private.h"
#include "cpu.h"

#include <altivec.h>

/* coeffiecients for AltiVec YUV->RGB conversion */
static vector signed short   CY;
static vector signed short   CRV;
static vector signed short   CBU;
static vector signed short   CGU;
static vector signed short   CGV;
static vector unsigned short CSHIFT;


/**
 * Initialize the static coef vectors
 */
static void
init_altivec_coeffs()
{
	CY = vec_splat ((vector signed short){0x253f}, 0);
	CRV = vec_splat ((vector signed short){0x3312}, 0);
	CBU = vec_splat ((vector signed short){0x4093}, 0);
	CGU = vec_splat ((vector signed short){0xf37d}, 0);
	CGV = vec_splat ((vector signed short){0xe5fc}, 0);
	CSHIFT = vec_splat ((vector unsigned short){0x2}, 0);
}


#define vec_unh(x) \
    (vector signed short) \
        vec_perm(x,(__typeof__(x)){0}, \
                 ((vector unsigned char){0x10,0x00,0x10,0x01,0x10,0x02,0x10,0x03,\
                                         0x10,0x04,0x10,0x05,0x10,0x06,0x10,0x07}))
#define vec_unl(x) \
    (vector signed short) \
        vec_perm(x,(__typeof__(x)){0}, \
                 ((vector unsigned char){0x10,0x08,0x10,0x09,0x10,0x0A,0x10,0x0B,\
                                         0x10,0x0C,0x10,0x0D,0x10,0x0E,0x10,0x0F}))

#define vec_packclp(x,y) \
    (vector unsigned char)vec_packs \
        ((vector unsigned short)vec_max (x,((vector signed short) {0})), \
         (vector unsigned short)vec_max (y,((vector signed short) {0})))

#define vec_mstrgb32(T,x0,x1,x2,x3,ptr)                                       \
do {                                                                          \
    T _0,_1,_2,_3;                                                            \
    _0 = vec_mergeh (x0,x1);                                                  \
    _1 = vec_mergeh (x2,x3);                                                  \
    _2 = (T)vec_mergeh ((vector unsigned short)_0,(vector unsigned short)_1); \
    _3 = (T)vec_mergel ((vector unsigned short)_0,(vector unsigned short)_1); \
    vec_st (_2, 0*16, (T *)ptr);                                              \
    vec_st (_3, 1*16, (T *)ptr);                                              \
    _0 = vec_mergel (x0,x1);                                                  \
    _1 = vec_mergel (x2,x3);                                                  \
    _2 = (T)vec_mergeh ((vector unsigned short)_0,(vector unsigned short)_1); \
    _3 = (T)vec_mergel ((vector unsigned short)_0,(vector unsigned short)_1); \
    vec_st (_2, 2*16, (T *)ptr);                                              \
    vec_st (_3, 3*16, (T *)ptr);                                              \
    ptr += 4;                                                                 \
}  while (0);

/**
 * macros for various output
 */
#define OUTPUT_BGRA(a,b,c,ptr)  vec_mstrgb32(__typeof__(a),c,b,a,((__typeof__ (a)){0xFF}),ptr)
#define OUTPUT_RGBA(a,b,c,ptr)  vec_mstrgb32(__typeof__(a),a,b,c,((__typeof__ (a)){0xFF}),ptr)
#define OUTPUT_ARGB(a,b,c,ptr)  vec_mstrgb32(__typeof__(a),((__typeof__ (a)){0xFF}),a,b,c,ptr)

/**
 * Function template for YUV420->RGB palet conversion with AltiVec
 */
#define YUV420_CONVERT_ALTIVEC(FUNC, OUTPUT_FUNC) \
static void \
(FUNC)(const OggPlayYUVChannels* yuv, OggPlayRGBChannels* rgb)	\
{										\
	int		i,j, w, h; 						\
	unsigned char 	*py1, *py2, *pu, *pv, *po0, *po1, *dst0, *dst1; 	\
	unsigned char 	*ptry0, *ptry1, *ptru, *ptrv; 				\
  	vector unsigned char y0,y1; 						\
										\
  	vector signed char  u,v;						\
										\
  	vector signed short Y0,Y1,Y2,Y3;					\
	vector signed short U,V;						\
	vector signed short vx,ux,uvx;						\
	vector signed short vx0,ux0,uvx0;					\
	vector signed short vx1,ux1,uvx1;					\
	vector signed short R0,G0,B0;						\
	vector signed short R1,G1,B1;						\
	vector unsigned char R,G,B;						\
										\
	vector unsigned char *y1ivP, *y2ivP, *uivP, *vivP, *out0, *out1;	\
	vector unsigned char align_perm;					\
										\
	ptry0 = yuv->ptry;							\
	ptry1 = yuv->ptry+yuv->y_width;						\
	ptru = yuv->ptru;							\
	ptrv = yuv->ptrv;							\
										\
	/* po0 even, po1 even lines */						\
	po0 = rgb->ptro;							\
	po1 = rgb->ptro+(rgb->rgb_width*4);					\
										\
	w = yuv->y_width/16;							\
	h = yuv->y_height/2;							\
	for (i = 0; i < h; ++i)							\
	{									\
		dst0 = po0;							\
		dst1 = po1;							\
		pu   = ptru;							\
		pv   = ptrv;							\
		py1  = ptry0;							\
		py2  = ptry1;							\
		for (j = 0; j < w; ++j, 					\
				dst0 += 64, dst1 +=64, 				\
				py1 += 16, py2 += 16,				\
				pu += 8, pv += 8)				\
		{								\
			out0  = (vector unsigned char *) dst0;			\
			out1  = (vector unsigned char *) dst1;			\
			y1ivP = (vector unsigned char *) py1;			\
			y2ivP = (vector unsigned char *) py2;			\
			uivP  = (vector unsigned char *) pu;			\
			vivP  = (vector unsigned char *) pv;			\
										\
			align_perm = vec_lvsl (0, py1);				\
			y0 = (vector unsigned char)				\
				vec_perm (y1ivP[0], y1ivP[1], align_perm);	\
										\
			align_perm = vec_lvsl (0, py2);				\
			y1 = (vector unsigned char)				\
				vec_perm (y2ivP[0], y2ivP[1], align_perm);	\
										\
			align_perm = vec_lvsl (0, pu);				\
			u = (vector signed char)				\
				vec_perm (uivP[0], uivP[1], align_perm);	\
										\
			align_perm = vec_lvsl (0, pv);				\
			v = (vector signed char)				\
				vec_perm (vivP[0], vivP[1], align_perm);	\
										\
			/* U -= 128, V -=128 */					\
			u  = (vector signed char)				\
				vec_sub (u,(vector signed char)			\
			     		vec_splat((vector signed char){128},0));\
			v  = (vector signed char)				\
				vec_sub (v,(vector signed char)			\
			     		vec_splat((vector signed char){128},0));\
										\
			U  = vec_unpackh (u);					\
			V  = vec_unpackh (v);					\
			U = vec_sl (U, CSHIFT);					\
			V = vec_sl (V, CSHIFT);					\
										\
			/* Y -= 16 */						\
			y0 = (vector unsigned char)				\
				vec_sub (y0, (vector unsigned char) 		\
					vec_splat((vector unsigned char){16},0));\
			y1 = (vector unsigned char)				\
				vec_sub (y1, (vector unsigned char) 		\
					vec_splat((vector unsigned char){16},0));\
										\
			Y0 = vec_unh (y0);					\
			Y1 = vec_unl (y0);					\
			Y2 = vec_unh (y1);					\
			Y3 = vec_unl (y1);					\
										\
			Y0 = vec_sl (Y0, CSHIFT);				\
			Y1 = vec_sl (Y1, CSHIFT);				\
			Y2 = vec_sl (Y2, CSHIFT);				\
			Y3 = vec_sl (Y3, CSHIFT);				\
										\
			/* Y *= CY */						\
			Y0 = vec_mradds (Y0, CY, (vector signed short){0});	\
			Y1 = vec_mradds (Y1, CY, (vector signed short){0});	\
			Y2 = vec_mradds (Y2, CY, (vector signed short){0});     \
			Y3 = vec_mradds (Y3, CY, (vector signed short){0});     \
										\
			/*   ux  = CBU*U >> 8 */				\
			ux = vec_mradds (U, CBU, (vector signed short){0});	\
			ux0  = vec_mergeh (ux,ux);				\
			ux1  = vec_mergel (ux,ux);				\
										\
			/* vx  = CRV*V >> 8*/ 					\
			vx = vec_mradds (V, CRV, (vector signed short){0});	\
			vx0  = vec_mergeh (vx,vx);				\
			vx1  = vec_mergel (vx,vx);				\
										\
			/* uvx = ((CGU*u) + (CGV*V)) >> 8 */			\
			uvx = vec_mradds (U, CGU, (vector signed short){0});	\
			uvx = vec_mradds (V, CGV, uvx);				\
			uvx0 = vec_mergeh (uvx,uvx);				\
			uvx1 = vec_mergel (uvx,uvx);				\
										\
			R0 = vec_add (Y0,vx0);					\
			G0 = vec_add (Y0,uvx0);					\
			B0 = vec_add (Y0,ux0);					\
			R1 = vec_add (Y1,vx1);					\
			G1 = vec_add (Y1,uvx1);					\
			B1 = vec_add (Y1,ux1);					\
										\
			R  = vec_packclp (R0,R1);				\
			G  = vec_packclp (G0,G1);				\
			B  = vec_packclp (B0,B1);				\
										\
			OUTPUT_FUNC(R,G,B,out0);				\
										\
			R0 = vec_add (Y2,vx0);                                  \
			G0 = vec_add (Y2,uvx0);                                 \
			B0 = vec_add (Y2,ux0);                                  \
			R1 = vec_add (Y3,vx1);                                  \
			G1 = vec_add (Y3,uvx1);                                 \
			B1 = vec_add (Y3,ux1);                                  \
			R  = vec_packclp (R0,R1);                               \
			G  = vec_packclp (G0,G1);                               \
			B  = vec_packclp (B0,B1);                        	\
										\
			OUTPUT_FUNC(R,G,B,out1);				\
		}								\
		po0 += (rgb->rgb_width*8);					\
		po1 += (rgb->rgb_width*8);					\
										\
		ptry0 += yuv->y_width * 2;					\
		ptry1 += yuv->y_width * 2;					\
		ptru  += yuv->uv_width;						\
		ptrv  += yuv->uv_width;						\
	}									\
}

/**
 * yuv420 ->
 */
YUV420_CONVERT_ALTIVEC(yuv420_to_rgba_altivec, OUTPUT_RGBA)
YUV420_CONVERT_ALTIVEC(yuv420_to_bgra_altivec, OUTPUT_BGRA)
YUV420_CONVERT_ALTIVEC(yuv420_to_argb_altivec, OUTPUT_ARGB)

