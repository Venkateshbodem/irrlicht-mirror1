// Copyright (C) 2002-2006 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CCubeSceneNode.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "S3DVertex.h"
#include "os.h"

namespace irr
{
namespace scene
{

//! constructor
CCubeSceneNode::CCubeSceneNode(f32 size, ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position, const core::vector3df& rotation,	const core::vector3df& scale)
: ISceneNode(parent, mgr, id, position, rotation, scale), Size(size)
{
	#ifdef _DEBUG
	setDebugName("CCubeSceneNode");
	#endif

	u16 u[36] = {   0,2,1,   0,3,2,   1,5,4,   1,2,5,   4,6,7,   4,5,6, 
            7,3,0,   7,6,3,   9,5,2,   9,8,5,   0,11,10,   0,10,7};

	s32 i; // new ISO for scoping problem with different compilers
	for (i=0; i<36; ++i)
		Indices[i] = u[i];

	Material.Wireframe = false;
	Material.Lighting = false;

	setSize();
}



//! destructor
CCubeSceneNode::~CCubeSceneNode()
{
}


void CCubeSceneNode::setSize()
{
	// we are creating the cube mesh here. 

	// nicer texture mapping sent in by Dr Andros C Bragianos 
	// .. and then improved by jox.

	video::SColor clr(255,255,255,255);

	Vertices[0]  = video::S3DVertex(0,0,0, -1,-1,-1, clr, 0, 1); 
	Vertices[1]  = video::S3DVertex(1,0,0,  1,-1,-1, clr, 1, 1); 
	Vertices[2]  = video::S3DVertex(1,1,0,  1, 1,-1, clr, 1, 0); 
	Vertices[3]  = video::S3DVertex(0,1,0, -1, 1,-1, clr, 0, 0); 
	Vertices[4]  = video::S3DVertex(1,0,1,  1,-1, 1, clr, 0, 1); 
	Vertices[5]  = video::S3DVertex(1,1,1,  1, 1, 1, clr, 0, 0); 
	Vertices[6]  = video::S3DVertex(0,1,1, -1, 1, 1, clr, 1, 0); 
	Vertices[7]  = video::S3DVertex(0,0,1, -1,-1, 1, clr, 1, 1); 
	Vertices[8]  = video::S3DVertex(0,1,1, -1, 1, 1, clr, 0, 1); 
	Vertices[9]  = video::S3DVertex(0,1,0, -1, 1,-1, clr, 1, 1); 
	Vertices[10] = video::S3DVertex(1,0,1,  1,-1, 1, clr, 1, 0); 
	Vertices[11] = video::S3DVertex(1,0,0,  1,-1,-1, clr, 0, 0); 

	Box.reset(0,0,0); 

	for (int i=0; i<12; ++i)
	{
		Vertices[i].Pos -= core::vector3df(0.5f, 0.5f, 0.5f);
		Vertices[i].Pos *= Size;
		Box.addInternalPoint(Vertices[i].Pos);
	}
}



//! renders the node.
void CCubeSceneNode::render()
{
	/*
        011         111
          /6--------/5        y
         /  |      / |        ^  z
        /   |     /  |        | /
    010 3---------2  |        |/
        |   7- - -| -4 101     *---->x
        |  /      |  /
        |/        | /
        0---------1/
       000       100       
	*/

	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setMaterial(Material);

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	
	driver->drawIndexedTriangleList(&Vertices[0], 12, &Indices[0], 12);
}



//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CCubeSceneNode::getBoundingBox() const
{
	return Box;
}


void CCubeSceneNode::OnPreRender()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnPreRender();
}


//! returns the material based on the zero based index i. To get the amount
//! of materials used by this scene node, use getMaterialCount().
//! This function is needed for inserting the node into the scene hirachy on a
//! optimal position for minimizing renderstate changes, but can also be used
//! to directly modify the material of a scene node.
video::SMaterial& CCubeSceneNode::getMaterial(s32 i)
{
	return Material;
}


//! returns amount of materials used by this scene node.
s32 CCubeSceneNode::getMaterialCount()
{
	return 1;
}


//! Writes attributes of the scene node.
void CCubeSceneNode::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options)
{
	ISceneNode::serializeAttributes(out, options);

	out->addFloat("Size", Size);
}


//! Reads attributes of the scene node.
void CCubeSceneNode::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	Size =	in->getAttributeAsFloat("Size");
	Size = irr::core::max_(Size, 0.0001f);
	setSize();

	ISceneNode::deserializeAttributes(in, options);
}



} // end namespace scene
} // end namespace irr

