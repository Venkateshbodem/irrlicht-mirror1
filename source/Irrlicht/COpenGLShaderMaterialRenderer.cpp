// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "COpenGLShaderMaterialRenderer.h"

#ifdef _IRR_COMPILE_WITH_OPENGL_

#include "IShaderConstantSetCallBack.h"
#include "IVideoDriver.h"
#include "os.h"

#include "COpenGLDriver.h"
#include "COpenGLCacheHandler.h"

namespace irr
{
namespace video
{


//! Constructor
COpenGLShaderMaterialRenderer::COpenGLShaderMaterialRenderer(video::COpenGLDriver* driver,
	s32& outMaterialTypeNr, const c8* vertexShaderProgram, const c8* pixelShaderProgram,
	IShaderConstantSetCallBack* callback, E_MATERIAL_TYPE baseMaterial, s32 userData)
	: Driver(driver), CallBack(callback), Alpha(false), Blending(false), FixedBlending(false),
		AlphaTest(false), VertexShader(0), UserData(userData)
{
	#ifdef _DEBUG
	setDebugName("COpenGLShaderMaterialRenderer");
	#endif

	PixelShader.set_used(4);
	for (u32 i=0; i<4; ++i)
	{
		PixelShader[i]=0;
	}

	switch (baseMaterial)
	{
	case EMT_TRANSPARENT_VERTEX_ALPHA:
	case EMT_TRANSPARENT_ALPHA_CHANNEL:
	case EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA:
	case EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA:
		Alpha = true;
		break;
	case EMT_TRANSPARENT_ADD_COLOR:
	case EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR:
	case EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR:
		FixedBlending = true;
		break;
	case EMT_ONETEXTURE_BLEND:
		Blending = true;
		break;
	case EMT_TRANSPARENT_ALPHA_CHANNEL_REF:
		AlphaTest = true;
		break;
	default:
		break;
	}

	if (CallBack)
		CallBack->grab();

	init(outMaterialTypeNr, vertexShaderProgram, pixelShaderProgram, EVT_STANDARD);
}


//! constructor only for use by derived classes who want to
//! create a fall back material for example.
COpenGLShaderMaterialRenderer::COpenGLShaderMaterialRenderer(COpenGLDriver* driver,
				IShaderConstantSetCallBack* callback,
				E_MATERIAL_TYPE baseMaterial, s32 userData)
: Driver(driver), CallBack(callback), Alpha(false), Blending(false), FixedBlending(false),
	AlphaTest(false), VertexShader(0), UserData(userData)
{
	PixelShader.set_used(4);
	for (u32 i=0; i<4; ++i)
	{
		PixelShader[i]=0;
	}

	switch (baseMaterial)
	{
	case EMT_TRANSPARENT_VERTEX_ALPHA:
	case EMT_TRANSPARENT_ALPHA_CHANNEL:
	case EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA:
	case EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA:
		Alpha = true;
		break;
	case EMT_TRANSPARENT_ADD_COLOR:
	case EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR:
	case EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR:
		FixedBlending = true;
		break;
	case EMT_ONETEXTURE_BLEND:
		Blending = true;
		break;
	case EMT_TRANSPARENT_ALPHA_CHANNEL_REF:
		AlphaTest = true;
		break;
	default:
		break;
	}

	if (CallBack)
		CallBack->grab();
}


//! Destructor
COpenGLShaderMaterialRenderer::~COpenGLShaderMaterialRenderer()
{
	if (CallBack)
		CallBack->drop();

	if (VertexShader)
		Driver->extGlDeletePrograms(1, &VertexShader);

	for (u32 i=0; i<PixelShader.size(); ++i)
		if (PixelShader[i])
			Driver->extGlDeletePrograms(1, &PixelShader[i]);
}


void COpenGLShaderMaterialRenderer::init(s32& outMaterialTypeNr,
		const c8* vertexShaderProgram, const c8* pixelShaderProgram,
		E_VERTEX_TYPE type)
{
	outMaterialTypeNr = -1;

	bool success;

	// create vertex shader
	success=createVertexShader(vertexShaderProgram);

	// create pixel shader
	if (!createPixelShader(pixelShaderProgram) || !success)
		return;

	// register as a new material
	outMaterialTypeNr = Driver->addMaterialRenderer(this);
}


bool COpenGLShaderMaterialRenderer::OnRender(IMaterialRendererServices* service, E_VERTEX_TYPE vtxtype)
{
	// call callback to set shader constants
	if (CallBack && (VertexShader || PixelShader[0]))
		CallBack->OnSetConstants(service, UserData);

	return true;
}


void COpenGLShaderMaterialRenderer::OnSetMaterial(const video::SMaterial& material, const video::SMaterial& lastMaterial,
	bool resetAllRenderstates, video::IMaterialRendererServices* services)
{
	if (Driver->getActivePipelineState() == COpenGLDriver::EOAP_FIXED)
		Driver->setActivePipelineState(COpenGLDriver::EOAP_FIXED_TO_SHADER);
	else
		Driver->setActivePipelineState(COpenGLDriver::EOAP_SHADER);

	COpenGLCacheHandler* cacheHandler = Driver->getCacheHandler();

	if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
	{
		if (VertexShader)
		{
			// set new vertex shader
#ifdef GL_ARB_vertex_program
			Driver->extGlBindProgram(GL_VERTEX_PROGRAM_ARB, VertexShader);
			glEnable(GL_VERTEX_PROGRAM_ARB);
#elif defined(GL_NV_vertex_program)
			Driver->extGlBindProgram(GL_VERTEX_PROGRAM_NV, VertexShader);
			glEnable(GL_VERTEX_PROGRAM_NV);
#endif
		}

		// set new pixel shader
		if (PixelShader[0])
		{
			GLuint nextShader=PixelShader[0];
			if (material.FogEnable)
			{
				GLint curFogMode;
				glGetIntegerv(GL_FOG_MODE, &curFogMode);
//				if (Driver->LinearFog && PixelShader[1])
				if (curFogMode==GL_LINEAR && PixelShader[1])
					nextShader=PixelShader[1];
//				else if (!Driver->LinearFog && PixelShader[2])
				else if (curFogMode==GL_EXP && PixelShader[2])
					nextShader=PixelShader[2];
				else if (curFogMode==GL_EXP2 && PixelShader[3])
					nextShader=PixelShader[3];
			}
#ifdef GL_ARB_fragment_program
			Driver->extGlBindProgram(GL_FRAGMENT_PROGRAM_ARB, nextShader);
			glEnable(GL_FRAGMENT_PROGRAM_ARB);
#elif defined(GL_NV_fragment_program)
			Driver->extGlBindProgram(GL_FRAGMENT_PROGRAM_NV, nextShader);
			glEnable(GL_FRAGMENT_PROGRAM_NV);
#endif
		}
	}

	Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

	if (Alpha)
	{
		cacheHandler->setBlend(true);
		cacheHandler->setBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else if (FixedBlending)
	{
		cacheHandler->setBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
		cacheHandler->setBlend(true);
	}
	else if (Blending)
	{
		E_BLEND_FACTOR srcRGBFact,dstRGBFact,srcAlphaFact,dstAlphaFact;
		E_MODULATE_FUNC modulate;
		u32 alphaSource;
		unpack_textureBlendFuncSeparate(srcRGBFact, dstRGBFact, srcAlphaFact, dstAlphaFact, modulate, alphaSource, material.MaterialTypeParam);

		if (Driver->queryFeature(EVDF_BLEND_SEPARATE))
        {
			cacheHandler->setBlendFuncSeparate(Driver->getGLBlend(srcRGBFact), Driver->getGLBlend(dstRGBFact),
                Driver->getGLBlend(srcAlphaFact), Driver->getGLBlend(dstAlphaFact));
        }
        else
        {
			cacheHandler->setBlendFunc(Driver->getGLBlend(srcRGBFact), Driver->getGLBlend(dstRGBFact));
        }

		cacheHandler->setBlend(true);
	}
	else if (AlphaTest)
	{
		cacheHandler->setAlphaTest(true);
		cacheHandler->setAlphaFunc(GL_GREATER, 0.5f);
	}

	if (CallBack)
		CallBack->OnSetMaterial(material);
}


void COpenGLShaderMaterialRenderer::OnUnsetMaterial()
{
	// disable vertex shader
#ifdef GL_ARB_vertex_program
	if (VertexShader)
		glDisable(GL_VERTEX_PROGRAM_ARB);
#elif defined(GL_NV_vertex_program)
	if (VertexShader)
		glDisable(GL_VERTEX_PROGRAM_NV);
#endif

#ifdef GL_ARB_fragment_program
	if (PixelShader[0])
		glDisable(GL_FRAGMENT_PROGRAM_ARB);
#elif defined(GL_NV_fragment_program)
	if (PixelShader[0])
		glDisable(GL_FRAGMENT_PROGRAM_NV);
#endif

	COpenGLCacheHandler* cacheHandler = Driver->getCacheHandler();
	if (Alpha || FixedBlending || Blending)
	{
		cacheHandler->setBlend(false);
	}
	else if (AlphaTest)
	{
		cacheHandler->setAlphaTest(false);
	}
}


//! Returns if the material is transparent.
bool COpenGLShaderMaterialRenderer::isTransparent() const
{
	return (Alpha || Blending || FixedBlending);
}


// This method needs a properly cleaned error state before the checked instruction is called
bool COpenGLShaderMaterialRenderer::checkError(const irr::c8* type)
{
#if defined(GL_ARB_vertex_program) || defined(GL_NV_vertex_program) || defined(GL_ARB_fragment_program) || defined(GL_NV_fragment_program)
	GLenum g = glGetError();
	if (g == GL_NO_ERROR)
		return false;

	core::stringc errString = type;
	errString += " compilation failed";

	errString += " at position ";
	GLint errPos=-1;
#if defined(GL_ARB_vertex_program) || defined(GL_ARB_fragment_program)
	glGetIntegerv( GL_PROGRAM_ERROR_POSITION_ARB, &errPos );
#else
	glGetIntegerv( GL_PROGRAM_ERROR_POSITION_NV, &errPos );
#endif
	errString += core::stringc(s32(errPos));
	errString += ":\n";
#if defined(GL_ARB_vertex_program) || defined(GL_ARB_fragment_program)
	errString += reinterpret_cast<const char*>(glGetString(GL_PROGRAM_ERROR_STRING_ARB));
#else
	errString += reinterpret_cast<const char*>(glGetString(GL_PROGRAM_ERROR_STRING_NV));
#endif
#else
	core::stringc errString("Shaders not supported.");
#endif
	os::Printer::log(errString.c_str(), ELL_ERROR);
	return true;
}


bool COpenGLShaderMaterialRenderer::createPixelShader(const c8* pxsh)
{
	if (!pxsh)
		return true;

	const core::stringc inshdr(pxsh);
	core::stringc shdr;
	const s32 pos = inshdr.find("#_IRR_FOG_MODE_");
	const u32 numShaders = (-1 != pos)?4:1;

	for (u32 i=0; i<numShaders; ++i)
	{
		if (i==0)
		{
			shdr=inshdr;
		}
		else
		{
			shdr = inshdr.subString(0, pos);
			switch (i) {
				case 1: shdr += "OPTION ARB_fog_linear;"; break;
				case 2: shdr += "OPTION ARB_fog_exp;"; break;
				case 3: shdr += "OPTION ARB_fog_exp2;"; break;
			}
			shdr += inshdr.subString(pos+16, inshdr.size()-pos-16);
		}
		Driver->extGlGenPrograms(1, &PixelShader[i]);
#ifdef GL_ARB_fragment_program
		Driver->extGlBindProgram(GL_FRAGMENT_PROGRAM_ARB, PixelShader[i]);
#elif defined GL_NV_fragment_program
		Driver->extGlBindProgram(GL_FRAGMENT_PROGRAM_NV, PixelShader[i]);
#endif

		// clear error buffer
		while(glGetError() != GL_NO_ERROR)
			{}

#ifdef GL_ARB_fragment_program
		// compile
		Driver->extGlProgramString(GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
				shdr.size(), shdr.c_str());
#elif defined GL_NV_fragment_program
		Driver->extGlLoadProgram(GL_FRAGMENT_PROGRAM_NV, PixelShader[i],
				shdr.size(), shdr.c_str());
#endif

		if (checkError("Pixel shader"))
		{
			Driver->extGlDeletePrograms(1, &PixelShader[i]);
			PixelShader[i]=0;

			return false;
		}
	}

	return true;
}


bool COpenGLShaderMaterialRenderer::createVertexShader(const c8* vtxsh)
{
	if (!vtxsh)
		return true;

	Driver->extGlGenPrograms(1, &VertexShader);
#ifdef GL_ARB_vertex_program
	Driver->extGlBindProgram(GL_VERTEX_PROGRAM_ARB, VertexShader);
#elif defined GL_NV_vertex_program
	Driver->extGlBindProgram(GL_VERTEX_PROGRAM_NV, VertexShader);
#endif

	// clear error buffer
	while(glGetError() != GL_NO_ERROR)
	{}

	// compile
#ifdef GL_ARB_vertex_program
	Driver->extGlProgramString(GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
			(GLsizei)strlen(vtxsh), vtxsh);
#elif defined GL_NV_vertex_program
	Driver->extGlLoadProgram(GL_VERTEX_PROGRAM_NV, VertexShader,
			(GLsizei)strlen(vtxsh), vtxsh);
#endif

	if (checkError("Vertex shader"))
	{
		Driver->extGlDeletePrograms(1, &VertexShader);
		VertexShader=0;

		return false;
	}

	return true;
}


} // end namespace video
} // end namespace irr

#endif

