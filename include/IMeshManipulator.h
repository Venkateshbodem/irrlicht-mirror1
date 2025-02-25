// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef IRR_I_MESH_MANIPULATOR_H_INCLUDED
#define IRR_I_MESH_MANIPULATOR_H_INCLUDED

#include "IReferenceCounted.h"
#include "vector3d.h"
#include "aabbox3d.h"
#include "matrix4.h"
#include "IAnimatedMesh.h"
#include "IMeshBuffer.h"
#include "SVertexManipulator.h"

namespace irr
{
namespace scene
{

	struct SMesh;

	//! An interface for easy manipulation of meshes.
	/** Scale, set alpha value, flip surfaces, and so on. This exists for
	fixing problems with wrong imported or exported meshes quickly after
	loading. It is not intended for doing mesh modifications and/or
	animations during runtime.
	*/
	class IMeshManipulator : public virtual IReferenceCounted
	{
	public:

		//! Flips the direction of surfaces.
		/** Changes backfacing triangles to frontfacing
		triangles and vice versa.
		\param mesh Mesh on which the operation is performed. */
		virtual void flipSurfaces(IMesh* mesh) const = 0;

		//! Sets the alpha vertex color value of the whole mesh to a new value.
		/** \param mesh Mesh on which the operation is performed.
		\param alpha New alpha value. Must be a value between 0 and 255. */
		void setVertexColorAlpha(IMesh* mesh, s32 alpha) const
		{
			apply(scene::SVertexColorSetAlphaManipulator(alpha), mesh);
		}

		//! Sets the alpha vertex color value of the whole mesh to a new value.
		/** \param buffer Meshbuffer on which the operation is performed.
		\param alpha New alpha value. Must be a value between 0 and 255. */
		void setVertexColorAlpha(IMeshBuffer* buffer, s32 alpha) const
		{
			apply(scene::SVertexColorSetAlphaManipulator(alpha), buffer);
		}

		//! Sets the colors of all vertices to one color
		/** \param mesh Mesh on which the operation is performed.
		\param color New color. */
		void setVertexColors(IMesh* mesh, video::SColor color) const
		{
			apply(scene::SVertexColorSetManipulator(color), mesh);
		}

		//! Sets the colors of all vertices to one color
		/** \param buffer Meshbuffer on which the operation is performed.
		\param color New color. */
		void setVertexColors(IMeshBuffer* buffer, video::SColor color) const
		{
			apply(scene::SVertexColorSetManipulator(color), buffer);
		}

		//! Recalculates all normals of the mesh.
		/** \param mesh: Mesh on which the operation is performed.
		\param smooth: If the normals shall be smoothed.
		\param angleWeighted: If the normals shall be smoothed in relation to their angles. More expensive, but also higher precision. */
		virtual void recalculateNormals(IMesh* mesh, bool smooth = false,
				bool angleWeighted = false) const=0;

		//! Recalculates all normals of the mesh buffer.
		/** \param buffer: Mesh buffer on which the operation is performed.
		\param smooth: If the normals shall be smoothed.
		\param angleWeighted: If the normals shall be smoothed in relation to their angles. More expensive, but also higher precision. */
		virtual void recalculateNormals(IMeshBuffer* buffer,
				bool smooth = false, bool angleWeighted = false) const=0;

		//! Recalculates tangents, requires a tangent mesh
		/** \param mesh Mesh on which the operation is performed.
		\param recalculateNormals If the normals shall be recalculated, otherwise original normals of the mesh are used unchanged.
		\param smooth If the normals shall be smoothed.
		\param angleWeighted If the normals shall be smoothed in relation to their angles. More expensive, but also higher precision.
		*/
		virtual void recalculateTangents(IMesh* mesh,
				bool recalculateNormals=false, bool smooth=false,
				bool angleWeighted=false) const=0;

		//! Recalculates tangents, requires a tangent mesh buffer
		/** \param buffer Meshbuffer on which the operation is performed.
		\param recalculateNormals If the normals shall be recalculated, otherwise original normals of the buffer are used unchanged.
		\param smooth If the normals shall be smoothed.
		\param angleWeighted If the normals shall be smoothed in relation to their angles. More expensive, but also higher precision.
		*/
		virtual void recalculateTangents(IMeshBuffer* buffer,
				bool recalculateNormals=false, bool smooth=false,
				bool angleWeighted=false) const=0;

		//! Scales the actual mesh, not a scene node.
		/** Note: When your scale are not uniform then
		prefer the transform function to have correct normals.
		\param mesh Mesh on which the operation is performed.
		\param factor Scale factor for each axis. */
		void scale(IMesh* mesh, const core::vector3df& factor) const
		{
			apply(SVertexPositionScaleManipulator(factor), mesh, true);
		}

		//! Scales the actual meshbuffer, not a scene node.
		/** Note: When your scale are not uniform then
		prefer the transform function to have correct normals.
		\param buffer Meshbuffer on which the operation is performed.
		\param factor Scale factor for each axis. */
		void scale(IMeshBuffer* buffer, const core::vector3df& factor) const
		{
			apply(SVertexPositionScaleManipulator(factor), buffer, true);
		}

		//! Scales the actual mesh, not a scene node.
		/** \deprecated Use scale() instead. This method may be removed by Irrlicht 1.9
		\param mesh Mesh on which the operation is performed.
		\param factor Scale factor for each axis. */
		IRR_DEPRECATED void scaleMesh(IMesh* mesh, const core::vector3df& factor) const {return scale(mesh,factor);}

		//! Scale the texture coords of a mesh.
		/** \param mesh Mesh on which the operation is performed.
		\param factor Vector which defines the scale for each axis.
		\param level Number of texture coord, starting from 1. Support for level 2 exists for LightMap buffers. */
		void scaleTCoords(scene::IMesh* mesh, const core::vector2df& factor, u32 level=1) const
		{
			apply(SVertexTCoordsScaleManipulator(factor, level), mesh);
		}

		//! Scale the texture coords of a meshbuffer.
		/** \param buffer Meshbuffer on which the operation is performed.
		\param factor Vector which defines the scale for each axis.
		\param level Number of texture coord, starting from 1. Support for level 2 exists for LightMap buffers. */
		void scaleTCoords(scene::IMeshBuffer* buffer, const core::vector2df& factor, u32 level=1) const
		{
			apply(SVertexTCoordsScaleManipulator(factor, level), buffer);
		}

		//! Applies a transformation to a mesh
		/** \param mesh Mesh on which the operation is performed.
		\param m transformation matrix. 
		\param normalsUpdate When 0 - don't update normals. 
		                     When 1 - update normals with inner 3x3 matrix of the inverse transposed of the transformation matrix
							          should be set when the matrix has rotation or non-uniform scaling
		\param normalizeNormals When true it normalizes all normals again. 
		                        Recommended to set this when normalsUpdate is 1 and there is any scaling
		*/
		void transform(IMesh* mesh, const core::matrix4& m, u32 normalsUpdate = 0, bool normalizeNormals=false) const
		{
			apply(SVertexPositionTransformManipulator(m), mesh, true);

			if ( normalsUpdate == 1 )
			{
				core::matrix4 invT;
				if ( m.getInverse(invT) )
				{
					invT = invT.getTransposed();
					apply(SVertexNormalRotateScaleManipulator(invT), mesh, false);
				}
			}
			if ( normalizeNormals )
			{
				apply(SVertexNormalizeNormalManipulator(), mesh, false);
			}
		}

		//! Applies a transformation to a meshbuffer
		/** \param buffer Meshbuffer on which the operation is performed.
		\param m transformation matrix. 
		\param normalsUpdate When 0 - don't update normals. 
		                     When 1 - update normals with inner 3x3 matrix of the inverse transposed of the transformation matrix
							          should be set when the matrix has rotation or non-uniform scaling
		\param normalizeNormals When true it normalizes all normals again. 
		                        Recommended to set this when normalsUpdate is 1 and there is any scaling
		*/
		void transform(IMeshBuffer* buffer, const core::matrix4& m, u32 normalsUpdate = 0, bool normalizeNormals=false) const
		{
			apply(SVertexPositionTransformManipulator(m), buffer, true);

			if ( normalsUpdate == 1 )
			{
				core::matrix4 invT;
				if ( m.getInverse(invT) )
				{
					invT = invT.getTransposed();
					apply(SVertexNormalRotateScaleManipulator(invT), buffer, false);
				}
			}
			if ( normalizeNormals )
			{
				apply(SVertexNormalizeNormalManipulator(), buffer, false);
			}
		}

		//! Applies a transformation to a mesh
		/** \deprecated Use transform() instead. This method may be removed by Irrlicht 1.9
		\param mesh Mesh on which the operation is performed.
		\param m transformation matrix. */
		IRR_DEPRECATED virtual void transformMesh(IMesh* mesh, const core::matrix4& m) const {return transform(mesh,m);}

		//! Creates a planar texture mapping on the mesh
		/** \param mesh: Mesh on which the operation is performed.
		\param resolution: resolution of the planar mapping. This is
		the value specifying which is the relation between world space
		and texture coordinate space. */
		virtual void makePlanarTextureMapping(IMesh* mesh, f32 resolution=0.001f) const=0;

		//! Creates a planar texture mapping on the meshbuffer
		/** \param meshbuffer: Buffer on which the operation is performed.
		\param resolution: resolution of the planar mapping. This is
		the value specifying which is the relation between world space
		and texture coordinate space. */
		virtual void makePlanarTextureMapping(scene::IMeshBuffer* meshbuffer, f32 resolution=0.001f) const=0;

		//! Creates a planar texture mapping on the buffer
		/** This method is currently implemented towards the LWO planar mapping. A more general biasing might be required.
		\param mesh Mesh on which the operation is performed.
		\param resolutionS Resolution of the planar mapping in horizontal direction. This is the ratio between object space and texture space.
		\param resolutionT Resolution of the planar mapping in vertical direction. This is the ratio between object space and texture space.
		\param axis The axis along which the texture is projected. The allowed values are 0 (X), 1(Y), and 2(Z).
		\param offset Vector added to the vertex positions (in object coordinates).
		*/
		virtual void makePlanarTextureMapping(scene::IMesh* mesh,
				f32 resolutionS, f32 resolutionT,
				u8 axis, const core::vector3df& offset) const=0;

		//! Creates a planar texture mapping on the meshbuffer
		/** This method is currently implemented towards the LWO planar mapping. A more general biasing might be required.
		\param buffer Buffer on which the operation is performed.
		\param resolutionS Resolution of the planar mapping in horizontal direction. This is the ratio between object space and texture space.
		\param resolutionT Resolution of the planar mapping in vertical direction. This is the ratio between object space and texture space.
		\param axis The axis along which the texture is projected. The allowed values are 0 (X), 1(Y), and 2(Z).
		\param offset Vector added to the vertex positions (in object coordinates).
		*/
		virtual void makePlanarTextureMapping(scene::IMeshBuffer* buffer,
				f32 resolutionS, f32 resolutionT,
				u8 axis, const core::vector3df& offset) const=0;

		//! Clones a static IMesh into a modifiable SMesh.
		/** \param mesh Mesh to copy.
		\return Cloned mesh. If you no longer need the
		cloned mesh, you should call SMesh::drop(). See
		IReferenceCounted::drop() for more information. */
		virtual SMesh* createMeshCopy(const IMesh* mesh) const = 0;

		//! Creates a copy of the mesh, which will only consist of S3DVertexTangents vertices.
		/** This is useful if you want to draw tangent space normal
		mapped geometry because it calculates the tangent and binormal
		data which is needed there.
		Note: Only 16-bit meshbuffers supported so far
		\param mesh Input mesh
		\param recalculateNormals The normals are recalculated if set,
		otherwise the original ones are kept. Note that keeping the
		normals may introduce inaccurate tangents if the normals are
		very different to those calculated from the faces.
		\param smooth The normals/tangents are smoothed across the
		meshbuffer's faces if this flag is set.
		\param angleWeighted Improved smoothing calculation used
		\param recalculateTangents Whether are actually calculated, or just the mesh with proper type is created.
		\return Mesh consisting only of S3DVertexTangents vertices. If
		you no longer need the cloned mesh, you should call
		IMesh::drop(). See IReferenceCounted::drop() for more
		information. */
		virtual IMesh* createMeshWithTangents(const IMesh* mesh,
				bool recalculateNormals=false, bool smooth=false,
				bool angleWeighted=false, bool recalculateTangents=true) const=0;

		//! Creates a copy of the mesh, which will only consist of S3DVertex2TCoord vertices.
		/** Note: Only 16-bit meshbuffers supported so far
		\param mesh Input mesh
		\return Mesh consisting only of S3DVertex2TCoord vertices. If
		you no longer need the cloned mesh, you should call
		IMesh::drop(). See IReferenceCounted::drop() for more
		information. */
		virtual IMesh* createMeshWith2TCoords(const IMesh* mesh) const = 0;

		//! Creates a copy of the mesh, which will only consist of S3DVertex vertices.
		/** Note: Only 16-bit meshbuffers supported so far
		\param mesh Input mesh
		\return Mesh consisting only of S3DVertex vertices. If
		you no longer need the cloned mesh, you should call
		IMesh::drop(). See IReferenceCounted::drop() for more
		information. */
		virtual IMesh* createMeshWith1TCoords(const IMesh* mesh) const = 0;

		//! Creates a copy of a mesh with all vertices unwelded
		/** Note: Only 16-bit meshbuffers supported so far
		\param mesh Input mesh
		\return Mesh consisting only of unique faces. All vertices
		which were previously shared are now duplicated. If you no
		longer need the cloned mesh, you should call IMesh::drop(). See
		IReferenceCounted::drop() for more information. */
		virtual IMesh* createMeshUniquePrimitives(const IMesh* mesh) const = 0;

		//! Creates a copy of a mesh with vertices welded 
		/** Note: Only 16-bit meshbuffers supported so far, 32-bit buffer are cloned
		\param mesh Input mesh
		\param tolerance The threshold for vertex comparisons.
		\return Mesh without redundant vertices. If you no longer need
		the cloned mesh, you should call IMesh::drop(). See
		IReferenceCounted::drop() for more information. */
		virtual IMesh* createMeshWelded(const IMesh* mesh, f32 tolerance=core::ROUNDING_ERROR_f32) const = 0;

		//! Get amount of polygons in mesh.
		/** \param mesh Input mesh
		\return Number of polygons in mesh. */
		virtual s32 getPolyCount(const IMesh* mesh) const = 0;

		//! Get amount of polygons in mesh.
		/** \param mesh Input mesh
		\return Number of polygons in mesh. */
		virtual s32 getPolyCount(IAnimatedMesh* mesh) const = 0;

		//! Create a new AnimatedMesh and adds the mesh to it
		/** \param mesh Input mesh
		\param type The type of the animated mesh to create.
		\return Newly created animated mesh with mesh as its only
		content. When you don't need the animated mesh anymore, you
		should call IAnimatedMesh::drop(). See
		IReferenceCounted::drop() for more information. */
		virtual IAnimatedMesh * createAnimatedMesh(IMesh* mesh,
			scene::E_ANIMATED_MESH_TYPE type = scene::EAMT_UNKNOWN) const = 0;

		//! Vertex cache optimization according to the Forsyth paper
		/** More information can be found at
		http://home.comcast.net/~tom_forsyth/papers/fast_vert_cache_opt.html

		The function is thread-safe (read: you can optimize several
		meshes in different threads).

		\param mesh Source mesh for the operation.
		\return A new mesh optimized for the vertex cache. */
		virtual IMesh* createForsythOptimizedMesh(const IMesh *mesh) const = 0;

		//! Optimize the mesh with an algorithm tuned for heightmaps.
		/**
		This differs from usual simplification methods in two ways:
		- it's intended to be lossless
		- it has special care for the borders, which are useful with heightmap tiles

		This function is thread-safe. Remember to weld afterwards - this
		function only moves vertices, it does not weld.

		\param mesh Mesh to operate on.
		*/
		virtual void heightmapOptimizeMesh(IMesh * const mesh, const f32 tolerance = core::ROUNDING_ERROR_f32) const = 0;

		//! Optimize the meshbuffer with an algorithm tuned for heightmaps.
		/**
		This differs from usual simplification methods in two ways:
		- it's intended to be lossless
		- it has special care for the borders, which are useful with heightmap tiles

		This function is thread-safe. Remember to weld afterward - this
		function only moves vertices, it does not weld.

		\param mb Meshbuffer to operate on.
		*/
		virtual void heightmapOptimizeMesh(IMeshBuffer * const mb, const f32 tolerance = core::ROUNDING_ERROR_f32) const = 0;

		//! Apply a manipulator on the Meshbuffer
		/** \param func A functor defining the mesh manipulation.
		\param buffer The Meshbuffer to apply the manipulator to.
		\param boundingBoxUpdate Specifies if the bounding box should be updated during manipulation.
		\return True if the functor was successfully applied, else false. */
		template <typename Functor>
		bool apply(const Functor& func, IMeshBuffer* buffer, bool boundingBoxUpdate=false) const
		{
			return apply_(func, buffer, boundingBoxUpdate, func);
		}


		//! Apply a manipulator on the Mesh
		/** \param func A functor defining the mesh manipulation.
		\param mesh The Mesh to apply the manipulator to.
		\param boundingBoxUpdate Specifies if the bounding box should be updated during manipulation.
		\return True if the functor was successfully applied, else false. */
		template <typename Functor>
		bool apply(const Functor& func, IMesh* mesh, bool boundingBoxUpdate=false) const
		{
			if (!mesh)
				return true;
			bool result = true;
			bool hasBoundingBox = false;
			core::aabbox3df bufferbox(1,-1);
			for (u32 i=0; i<mesh->getMeshBufferCount(); ++i)
			{
				result &= apply(func, mesh->getMeshBuffer(i), boundingBoxUpdate);
				if (boundingBoxUpdate && mesh->getMeshBuffer(i)->getBoundingBox().isValid())
				{
					if ( hasBoundingBox )
						bufferbox.addInternalBox(mesh->getMeshBuffer(i)->getBoundingBox());
					else
					{
						bufferbox.reset(mesh->getMeshBuffer(i)->getBoundingBox());
						hasBoundingBox = true;
					}
				}
			}
			if (boundingBoxUpdate)
				mesh->setBoundingBox(bufferbox);
			return result;
		}

protected:
		//! Apply a manipulator based on the type of the functor
		/** \param func A functor defining the mesh manipulation.
		\param buffer The Meshbuffer to apply the manipulator to.
		\param boundingBoxUpdate Specifies if the bounding box should be updated during manipulation.
		\param typeTest Unused parameter, which handles the proper call selection based on the type of the Functor which is passed in two times.
		\return True if the functor was successfully applied, else false. */
		template <typename Functor>
		bool apply_(const Functor& func, IMeshBuffer* buffer, bool boundingBoxUpdate, const IVertexManipulator& typeTest) const
		{
			if (!buffer)
				return true;

			core::aabbox3df bufferbox(1,-1);
			for (u32 i=0; i<buffer->getVertexCount(); ++i)
			{
				switch (buffer->getVertexType())
				{
				case video::EVT_STANDARD:
					{
						video::S3DVertex* verts = (video::S3DVertex*)buffer->getVertices();
						func(verts[i]);
					}
					break;
				case video::EVT_2TCOORDS:
					{
						video::S3DVertex2TCoords* verts = (video::S3DVertex2TCoords*)buffer->getVertices();
						func(verts[i]);
					}
					break;
				case video::EVT_TANGENTS:
					{
						video::S3DVertexTangents* verts = (video::S3DVertexTangents*)buffer->getVertices();
						func(verts[i]);
					}
					break;
				}
				if (boundingBoxUpdate)
				{
					if (0==i)
						bufferbox.reset(buffer->getPosition(0));
					else
						bufferbox.addInternalPoint(buffer->getPosition(i));
				}
			}
			if (boundingBoxUpdate)
				buffer->setBoundingBox(bufferbox);
			return true;
		}
};

} // end namespace scene
} // end namespace irr

#endif
