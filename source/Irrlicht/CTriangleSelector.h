// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef IRR_C_TRIANGLE_SELECTOR_H_INCLUDED
#define IRR_C_TRIANGLE_SELECTOR_H_INCLUDED

#include "ITriangleSelector.h"
#include "IMesh.h"
#include "irrArray.h"
#include "aabbox3d.h"

namespace irr
{
namespace scene
{

class ISceneNode;
class IAnimatedMeshSceneNode;

//! Stupid triangle selector without optimization
class CTriangleSelector : public ITriangleSelector
{
public:

	//! Constructs a selector based on a mesh
	CTriangleSelector(ISceneNode* node);

	//! Constructs a selector based on a mesh
	CTriangleSelector(const IMesh* mesh, ISceneNode* node, bool separateMeshbuffers);

	//! Constructs a selector based on a meshbuffer
	CTriangleSelector(const IMeshBuffer* meshBuffer, irr::u32 materialIndex, ISceneNode* node);

	//! Constructs a selector based on an animated mesh scene node
	//!\param node An animated mesh scene node, which must have a valid mesh
	CTriangleSelector(IAnimatedMeshSceneNode* node, bool separateMeshbuffers);

	//! Constructs a selector based on a bounding box
	CTriangleSelector(const core::aabbox3d<f32>& box, ISceneNode* node);

	//! Gets all triangles.
	virtual void getTriangles(core::triangle3df* triangles, s32 arraySize, s32& outTriangleCount,
		const core::matrix4* transform, bool useNodeTransform, 
		irr::core::array<SCollisionTriangleRange>* outTriangleInfo) const IRR_OVERRIDE;

	//! Gets all triangles which lie within a specific bounding box.
	virtual void getTriangles(core::triangle3df* triangles, s32 arraySize, s32& outTriangleCount,
		const core::aabbox3d<f32>& box, const core::matrix4* transform, bool useNodeTransform, 
		irr::core::array<SCollisionTriangleRange>* outTriangleInfo) const IRR_OVERRIDE;

	//! Gets all triangles which have or may have contact with a 3d line.
	virtual void getTriangles(core::triangle3df* triangles, s32 arraySize,
		s32& outTriangleCount, const core::line3d<f32>& line,
		const core::matrix4* transform, bool useNodeTransform, 
		irr::core::array<SCollisionTriangleRange>* outTriangleInfo) const IRR_OVERRIDE;

	//! Returns amount of all available triangles in this selector
	virtual s32 getTriangleCount() const IRR_OVERRIDE;

	//! Return the scene node associated with a given triangle.
	virtual ISceneNode* getSceneNodeForTriangle(u32 triangleIndex) const IRR_OVERRIDE { return SceneNode; }

	// Get the number of TriangleSelectors that are part of this one
	virtual u32 getSelectorCount() const IRR_OVERRIDE;

	// Get the TriangleSelector based on index based on getSelectorCount
	virtual ITriangleSelector* getSelector(u32 index) IRR_OVERRIDE;

	// Get the TriangleSelector based on index based on getSelectorCount
	virtual const ITriangleSelector* getSelector(u32 index) const IRR_OVERRIDE;

protected:
	//! Create from a mesh
	virtual void createFromMesh(const IMesh* mesh, bool createBufferRanges);

	//! Create from a meshbuffer
	virtual void createFromMeshBuffer(const IMeshBuffer* meshBuffer);

	//! Update when the mesh has changed
	virtual void updateFromMesh(const IMesh* mesh) const;

	//! Update when the meshbuffer has changed
	virtual void updateFromMeshBuffer(const IMeshBuffer* meshBuffer) const;

	//! Update bounding box from triangles
	void updateBoundingBox() const;

	//! Update the triangle selector, which will only have an effect if it
	//! was built from an animated mesh and that mesh's frame has changed
	//! since the last time it was updated.
	virtual void update(void) const;

	struct SBufferTriangleRange
	{
		SBufferTriangleRange() 
			: RangeStart(0), RangeSize(0), MeshBuffer(0), MaterialIndex(0)
		{}

		//! First index in Triangles array
		irr::u32 RangeStart;

		//! Number of elements in Triangles array
		irr::u32 RangeSize;

		//! Meshbuffer from which the triangles are from
		//! Is 0 when we use a single range for all buffers
		const IMeshBuffer* MeshBuffer;

		//! Index of selected material in the SceneNode. 
		//! Only valid when MeshBuffer is also set, otherwise 0
		irr::u32 MaterialIndex;
	};

	SBufferTriangleRange SingleBufferRange;	// When avoiding extra heap allocations
	irr::core::array<SBufferTriangleRange> BufferRanges;

	ISceneNode* SceneNode;
	mutable core::array<core::triangle3df> Triangles; // (mutable for CTriangleBBSelector)
	mutable core::aabbox3df BoundingBox; // Allows for trivial rejection

	IAnimatedMeshSceneNode* AnimatedNode;
	mutable u32 LastMeshFrame;
};

} // end namespace scene
} // end namespace irr

#endif
