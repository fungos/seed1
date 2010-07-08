#include "Image.h"
#include "RendererDevice.h"
#include "Texture.h"
#include "Screen.h"
#include "Matrix4x4.h"

namespace Seed {

Image::Image()
	: pTexture(NULL)
	, pRes(NULL)
	, pPool(NULL)
	, pFilename(NULL)
	, fAspectHalfWidth(0.0f)
	, fAspectHalfHeight(0.0f)
	, fAspectWidth(0.0f)
	, fAspectHeight(0.0f)
	, iHalfWidth(0)
	, iHalfHeight(0)
	, iWidth(0)
	, iHeight(0)
	, fTexS0(0.0f)
	, fTexS1(0.0f)
	, fTexT0(0.0f)
	, fTexT1(0.0f)
	, bDynamic(FALSE)
{
}

Image::~Image()
{
}

INLINE BOOL Image::Unload()
{
	if (!bDynamic)
	{
		sRelease(pTexture);
	}

	pTexture = NULL;

	return TRUE;
}

INLINE BOOL Image::Load(const char *filename, IMemoryPool *pool)
{
	return this->Load(filename, pResourceManager, pool);
}

BOOL Image::Load(const char *filename, ResourceManager *res, IMemoryPool *pool)
{
	ASSERT_NULL(pool);
	ASSERT_NULL(filename);
	ASSERT_NULL(res);

	if (this->Unload())
	{
		pFilename = filename;
		pPool = pool;
		pRes = res;

		pTexture = static_cast<ITexture *>(res->Get(filename, Seed::ObjectTexture, pool));

		iWidth = pTexture->GetWidthInPixel();
		iHeight = pTexture->GetHeightInPixel();
		f32 aspectH = pScreen->GetAspectRatio();

		f32 w = pTexture->GetWidth();
		f32 h = pTexture->GetHeight();

		ITransformable2D::SetWidth(w); // set normalized width
		ITransformable2D::SetHeight(h); // set normalized height

		fAspectWidth = w;
		fAspectHeight = h * aspectH;
		fAspectHalfWidth = fAspectWidth / 2.0f;
		fAspectHalfHeight = fAspectHeight / 2.0f;

		f32 rInvWidth = 1.0F / pTexture->GetAtlasWidthInPixel(); // full width from image, not only frame area
		f32 rInvHeight = 1.0F / pTexture->GetAtlasHeightInPixel(); // full height from image, not only frame area

		// Normalized Pixel Half Width/Height for pixel based vertex rendering
		iHalfWidth = static_cast<s32>(pScreen->GetWidth() * (w / 2.0f));
		iHalfHeight = static_cast<s32>(pScreen->GetHeight() *  (h / 2.0f));

		fTexS0 = 0.0f;
		fTexS1 = static_cast<f32>(iWidth * rInvWidth);
		fTexT0 = 0.0f;
		fTexT1 = static_cast<f32>(iHeight * rInvHeight);

		bDynamic = FALSE;
	}

	return TRUE;
}

INLINE void Image:: Update(f32 delta)
{
	UNUSED(delta);
	if (this->IsChanged())
	{
		vert[0].cVertex = Vector3f(-fAspectHalfWidth, -fAspectHalfHeight, (f32)iPriority);
		vert[0].iColor = iColor;
		vert[0].cCoords = Point2f(fTexS0, fTexT0);

		vert[1].cVertex = Vector3f(+fAspectHalfWidth, -fAspectHalfHeight, (f32)iPriority);
		vert[1].iColor = iColor;
		vert[1].cCoords = Point2f(fTexS1, fTexT0);

		vert[2].cVertex = Vector3f(-fAspectHalfWidth, +fAspectHalfHeight, (f32)iPriority);
		vert[2].iColor = iColor;
		vert[2].cCoords = Point2f(fTexS0, fTexT1);

		vert[3].cVertex = Vector3f(+fAspectHalfWidth, +fAspectHalfHeight, (f32)iPriority);
		vert[3].iColor = iColor;
		vert[3].cCoords = Point2f(fTexS1, fTexT1);

		f32 x = fAspectHalfWidth + this->GetX();
		f32 y = fAspectHalfHeight + this->GetY() * pScreen->GetAspectRatio();
		f32 lx = this->GetLocalX();
		f32 ly = this->GetLocalY();

		Matrix4x4f t1, t2, r, s;
		t1 = TranslationMatrix(lx + x, ly + y, 0.0f);
		r = RotationMatrix(AxisZ, DegToRad(this->GetRotation()));
		s = ScaleMatrix(this->GetScaleX(), this->GetScaleY(), 1.0f);
		t2 = TranslationMatrix(-lx, -ly, 0.0f);
		Matrix4x4f transform = ((t1 * r) * s) * t2;

		transform.Transform(vert[0].cVertex);
		transform.Transform(vert[1].cVertex);
		transform.Transform(vert[2].cVertex);
		transform.Transform(vert[3].cVertex);

		bTransformationChanged = FALSE;
	}
}

INLINE void Image::Render()
{
	if (pTexture && pTexture->GetData())
	{
		RendererPacket packet;
		packet.iSize = 4;
		packet.nMeshType = Seed::TriangleStrip;
		packet.pVertexData = &vert;
		packet.pTexture = pTexture;
		packet.nBlendMode = eBlendOperation;
		packet.iColor = iColor;
		pRendererDevice->UploadData(&packet);
	}
}

INLINE BOOL Image::Load(ITexture *texture)
{
	ASSERT_NULL(texture);

	if (!bDynamic)
	{
		sRelease(pTexture);
	}
	pTexture = NULL;

	pFilename = "[dynamic texture]";
	pTexture = texture;

	iWidth = pTexture->GetWidthInPixel();
	iHeight = pTexture->GetHeightInPixel();
	f32 aspectH = pScreen->GetAspectRatio();

	f32 w = pTexture->GetWidth();
	f32 h = pTexture->GetHeight();

	ITransformable2D::SetWidth(w); // set normalized width
	ITransformable2D::SetHeight(h); // set normalized height

	fAspectWidth = w;
	fAspectHeight = h * aspectH;
	fAspectHalfWidth = fAspectWidth / 2.0f;
	fAspectHalfHeight = fAspectHeight / 2.0f;

	f32 rInvWidth = 1.0F / pTexture->GetAtlasWidthInPixel(); // full width from image, not only frame area
	f32 rInvHeight = 1.0F / pTexture->GetAtlasHeightInPixel(); // full height from image, not only frame area

	// Normalized Pixel Half Width/Height for pixel based vertex rendering
	iHalfWidth = static_cast<s32>(pScreen->GetWidth() * (w / 2.0f));
	iHalfHeight = static_cast<s32>(pScreen->GetHeight() *  (h / 2.0f));

	fTexS0 = 0.0f;
	fTexS1 = static_cast<f32>(iWidth * rInvWidth);
	fTexT0 = 0.0f;
	fTexT1 = static_cast<f32>(iHeight * rInvHeight);

	bDynamic = TRUE;

	return TRUE;
}

INLINE int Image::GetObjectType() const
{
	return Seed::ObjectImage;
}

INLINE const char *Image::GetObjectName() const
{
	return "Image";
}

} // namespace
