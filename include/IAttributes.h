// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef IRR_I_ATTRIBUTES_H_INCLUDED
#define IRR_I_ATTRIBUTES_H_INCLUDED

#include "IReferenceCounted.h"
#include "SColor.h"
#include "vector3d.h"
#include "vector2d.h"
#include "line2d.h"
#include "line3d.h"
#include "triangle3d.h"
#include "position2d.h"
#include "rect.h"
#include "dimension2d.h"
#include "matrix4.h"
#include "quaternion.h"
#include "plane3d.h"
#include "triangle3d.h"
#include "line2d.h"
#include "line3d.h"
#include "irrString.h"
#include "irrArray.h"
#include "IXMLReader.h"
#include "IXMLWriter.h"
#include "EAttributes.h"
#include "path.h"

namespace irr
{
namespace video
{
	class ITexture;
} // end namespace video
namespace io
{

//! Provides a generic interface for attributes and their values and the possibility to serialize them
class IAttributes : public virtual IReferenceCounted
{
public:

	//! Returns amount of attributes in this collection of attributes.
	virtual u32 getAttributeCount() const = 0;

	//! Returns attribute name by index.
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual const c8* getAttributeName(s32 index) const = 0;

	//! Returns the type of an attribute
	//! \param attributeName: Name for the attribute
	virtual E_ATTRIBUTE_TYPE getAttributeType(const c8* attributeName) const = 0;

	//! Returns attribute type by index.
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual E_ATTRIBUTE_TYPE getAttributeType(s32 index) const = 0;

	//! Returns the type string of the attribute
	//! \param attributeName: String for the attribute type
	//! \param defaultNotFound Value returned when attributeName was not found
	virtual const wchar_t* getAttributeTypeString(const c8* attributeName, const wchar_t* defaultNotFound = L"unknown") const = 0;

	//! Returns the type string of the attribute by index.
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	//! \param defaultNotFound Value returned for an invalid index
	virtual const wchar_t* getAttributeTypeString(s32 index, const wchar_t* defaultNotFound = L"unknown") const = 0;

	//! Returns if an attribute with a name exists
	virtual bool existsAttribute(const c8* attributeName) const = 0;

	//! Returns attribute index from name, -1 if not found
	virtual s32 findAttribute(const c8* attributeName) const = 0;

	//! Removes all attributes
	virtual void clear() = 0;

	//! Reads attributes from a xml file.
	//! \param reader The XML reader to read from
	//! \param readCurrentElementOnly If set to true, reading only works if current element has the name 'attributes' or
	//! the name specified using elementName.
	//! \param elementName The surrounding element name. If it is null, the default one, "attributes" will be taken.
	//! If set to false, the first appearing list of attributes are read.
	virtual bool read(io::IXMLReader* reader, bool readCurrentElementOnly=false, const wchar_t* elementName=0) = 0;

	//! Write these attributes into a xml file
	//! \param writer: The XML writer to write to
	//! \param writeXMLHeader: Writes a header to the XML file, required if at the beginning of the file
	//! \param elementName: The surrounding element name. If it is null, the default one, "attributes" will be taken.
	virtual bool write(io::IXMLWriter* writer, bool writeXMLHeader=false, const wchar_t* elementName=0) = 0;


	/*
		Integer Attribute
	*/

	//! Adds an attribute as integer
	virtual void addInt(const c8* attributeName, s32 value) = 0;

	//! Sets an attribute as integer value
	virtual void setAttribute(const c8* attributeName, s32 value) = 0;

	//! Gets an attribute as integer value
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual s32 getAttributeAsInt(const c8* attributeName, irr::s32 defaultNotFound=0) const = 0;

	//! Gets an attribute as integer value
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual s32 getAttributeAsInt(s32 index, irr::s32 defaultNotFound=0) const = 0;

	//! Sets an attribute as integer value
	virtual void setAttribute(s32 index, s32 value) = 0;

	/*
		Float Attribute
	*/

	//! Adds an attribute as float
	virtual void addFloat(const c8* attributeName, f32 value) = 0;

	//! Sets a attribute as float value
	virtual void setAttribute(const c8* attributeName, f32 value) = 0;

	//! Gets an attribute as float value
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual f32 getAttributeAsFloat(const c8* attributeName, irr::f32 defaultNotFound=0.f) const = 0;

	//! Gets an attribute as float value
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual f32 getAttributeAsFloat(s32 index, irr::f32 defaultNotFound=0.f) const = 0;

	//! Sets an attribute as float value
	virtual void setAttribute(s32 index, f32 value) = 0;

	/*
		String Attribute
	*/

	//! Adds an attribute as string
	virtual void addString(const c8* attributeName, const c8* value) = 0;

	//! Sets an attribute value as string.
	//! \param attributeName: Name for the attribute
	//! \param value: Value for the attribute. Set this to 0 to delete the attribute
	virtual void setAttribute(const c8* attributeName, const c8* value) = 0;

	//! Gets an attribute as string.
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	//! \return Returns value of the attribute previously set by setAttribute()
	//! or defaultNotFound if attribute is not set.
	virtual core::stringc getAttributeAsString(const c8* attributeName, const core::stringc& defaultNotFound=core::stringc()) const = 0;

	//! Gets an attribute as string.
	//! \param attributeName Name of the attribute to get.
	//! \param target Buffer where the string is copied to.
	virtual void getAttributeAsString(const c8* attributeName, c8* target) const = 0;

	//! Returns attribute value as string by index.
	//! \param index Index value, must be between 0 and getAttributeCount()-1.
	virtual core::stringc getAttributeAsString(s32 index, const core::stringc& defaultNotFound=core::stringc()) const = 0;

	//! Sets an attribute value as string.
	//! \param index Index value, must be between 0 and getAttributeCount()-1.
	//! \param value String to which the attribute is set.
	virtual void setAttribute(s32 index, const c8* value) = 0;

	// wide strings

	//! Adds an attribute as string
	virtual void addString(const c8* attributeName, const wchar_t* value) = 0;

	//! Sets an attribute value as string.
	//! \param attributeName: Name for the attribute
	//! \param value: Value for the attribute. Set this to 0 to delete the attribute
	virtual void setAttribute(const c8* attributeName, const wchar_t* value) = 0;

	//! Gets an attribute as string.
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	//! \return Returns value of the attribute previously set by setAttribute()
	//! or defaultNotFound if attribute is not set.
	virtual core::stringw getAttributeAsStringW(const c8* attributeName, const core::stringw& defaultNotFound = core::stringw()) const = 0;

	//! Gets an attribute as string.
	//! \param attributeName: Name of the attribute to get.
	//! \param target: Buffer where the string is copied to.
	virtual void getAttributeAsStringW(const c8* attributeName, wchar_t* target) const = 0;

	//! Returns attribute value as string by index.
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual core::stringw getAttributeAsStringW(s32 index, const core::stringw& defaultNotFound = core::stringw()) const = 0;

	//! Sets an attribute value as string.
	//! \param index Index value, must be between 0 and getAttributeCount()-1.
	//! \param value String to which the attribute is set.
	virtual void setAttribute(s32 index, const wchar_t* value) = 0;

	/*
		Binary Data Attribute
	*/

	//! Adds an attribute as binary data
	virtual void addBinary(const c8* attributeName, void* data, s32 dataSizeInBytes) = 0;

	//! Sets an attribute as binary data
	virtual void setAttribute(const c8* attributeName, void* data, s32 dataSizeInBytes ) = 0;

	//! Gets an attribute as binary data
	/** \param attributeName: Name of the attribute to get.
	\param outData Pointer to buffer where data shall be stored.
	\param maxSizeInBytes Maximum number of bytes to write into outData.
	*/
	virtual void getAttributeAsBinaryData(const c8* attributeName, void* outData, s32 maxSizeInBytes) const = 0;

	//! Gets an attribute as binary data
	/** \param index: Index value, must be between 0 and getAttributeCount()-1.
	\param outData Pointer to buffer where data shall be stored.
	\param maxSizeInBytes Maximum number of bytes to write into outData.
	*/
	virtual void getAttributeAsBinaryData(s32 index, void* outData, s32 maxSizeInBytes) const = 0;

	//! Sets an attribute as binary data
	virtual void setAttribute(s32 index, void* data, s32 dataSizeInBytes ) = 0;


	/*
		Array Attribute
	*/

	//! Adds an attribute as wide string array
	virtual void addArray(const c8* attributeName, const core::array<core::stringw>& value) = 0;

	//! Sets an attribute value as a wide string array.
	//! \param attributeName: Name for the attribute
	//! \param value: Value for the attribute. Set this to 0 to delete the attribute
	virtual void setAttribute(const c8* attributeName, const core::array<core::stringw>& value) = 0;

	//! Gets an attribute as an array of wide strings.
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	//! \return Returns value of the attribute previously set by setAttribute()
	//! or defaultNotFound if attribute is not set.
	virtual core::array<core::stringw> getAttributeAsArray(const c8* attributeName, const core::array<core::stringw>& defaultNotFound = core::array<core::stringw>()) const = 0;

	//! Returns attribute value as an array of wide strings by index.
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual core::array<core::stringw> getAttributeAsArray(s32 index, const core::array<core::stringw>& defaultNotFound = core::array<core::stringw>()) const = 0;

	//! Sets an attribute as an array of wide strings
	virtual void setAttribute(s32 index, const core::array<core::stringw>& value) = 0;


	/*
		Bool Attribute
	*/

	//! Adds an attribute as bool
	virtual void addBool(const c8* attributeName, bool value) = 0;

	//! Sets an attribute as boolean value
	virtual void setAttribute(const c8* attributeName, bool value) = 0;

	//! Gets an attribute as boolean value
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual bool getAttributeAsBool(const c8* attributeName, bool defaultNotFound=false) const = 0;

	//! Gets an attribute as boolean value
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual bool getAttributeAsBool(s32 index, bool defaultNotFound=false) const = 0;

	//! Sets an attribute as boolean value
	virtual void setAttribute(s32 index, bool value) = 0;

	/*
		Enumeration Attribute
	*/

	//! Adds an attribute as enum
	virtual void addEnum(const c8* attributeName, const c8* enumValue, const c8* const* enumerationLiterals) = 0;

	//! Adds an attribute as enum
	virtual void addEnum(const c8* attributeName, s32 enumValue, const c8* const* enumerationLiterals) = 0;

	//! Sets an attribute as enumeration
	virtual void setAttribute(const c8* attributeName, const c8* enumValue, const c8* const* enumerationLiterals) = 0;

	//! Gets an attribute as enumeration
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual const c8* getAttributeAsEnumeration(const c8* attributeName, const c8* defaultNotFound = 0) const = 0;

	//! Gets an attribute as enumeration
	/** \param attributeName: Name of the attribute to get.
	\param enumerationLiteralsToUse: Use these enumeration literals to get
	the index value instead of the set ones. This is useful when the
	attribute list maybe was read from an xml file, and only contains the
	enumeration string, but no information about its index.
	\return Returns value of the attribute previously set by setAttribute()
	*/
	virtual s32 getAttributeAsEnumeration(const c8* attributeName, const c8* const* enumerationLiteralsToUse, s32 defaultNotFound = -1) const = 0;

	//! Gets an attribute as enumeration
	/** \param index: Index value, must be between 0 and getAttributeCount()-1.
	\param enumerationLiteralsToUse: Use these enumeration literals to get
	the index value instead of the set ones. This is useful when the
	attribute list maybe was read from an xml file, and only contains the
	enumeration string, but no information about its index.
	\param defaultNotFound Value returned when the attribute referenced by the index was not found.
	\return Returns value of the attribute previously set by setAttribute()
	*/
	virtual s32 getAttributeAsEnumeration(s32 index, const c8* const* enumerationLiteralsToUse, s32 defaultNotFound = -1) const = 0;

	//! Gets an attribute as enumeration
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual const c8* getAttributeAsEnumeration(s32 index) const = 0;

	//! Gets the list of enumeration literals of an enumeration attribute
	//! \param attributeName Name of the attribute to get.
	//! \param outLiterals Set of strings to choose the enum name from.
	virtual void getAttributeEnumerationLiteralsOfEnumeration(const c8* attributeName, core::array<core::stringc>& outLiterals) const = 0;

	//! Gets the list of enumeration literals of an enumeration attribute
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	//! \param outLiterals Set of strings to choose the enum name from.
	virtual void getAttributeEnumerationLiteralsOfEnumeration(s32 index, core::array<core::stringc>& outLiterals) const = 0;

	//! Sets an attribute as enumeration
	virtual void setAttribute(s32 index, const c8* enumValue, const c8* const* enumerationLiterals) = 0;


	/*
		SColor Attribute
	*/

	//! Adds an attribute as color
	virtual void addColor(const c8* attributeName, video::SColor value) = 0;


	//! Sets a attribute as color
	virtual void setAttribute(const c8* attributeName, video::SColor color) = 0;

	//! Gets an attribute as color
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual video::SColor getAttributeAsColor(const c8* attributeName, const video::SColor& defaultNotFound = video::SColor(0)) const = 0;

	//! Gets an attribute as color
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual video::SColor getAttributeAsColor(s32 index, const video::SColor& defaultNotFound = video::SColor(0)) const = 0;

	//! Sets an attribute as color
	virtual void setAttribute(s32 index, video::SColor color) = 0;

	/*
		SColorf Attribute
	*/

	//! Adds an attribute as floating point color
	virtual void addColorf(const c8* attributeName, video::SColorf value) = 0;

	//! Sets a attribute as floating point color
	virtual void setAttribute(const c8* attributeName, video::SColorf color) = 0;

	//! Gets an attribute as floating point color
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual video::SColorf getAttributeAsColorf(const c8* attributeName, const video::SColorf& defaultNotFound = video::SColorf(0)) const = 0;

	//! Gets an attribute as floating point color
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual video::SColorf getAttributeAsColorf(s32 index, const video::SColorf& defaultNotFound = video::SColorf(0)) const = 0;

	//! Sets an attribute as floating point color
	virtual void setAttribute(s32 index, video::SColorf color) = 0;


	/*
		Vector3d Attribute
	*/

	//! Adds an attribute as 3d vector
	virtual void addVector3d(const c8* attributeName, const core::vector3df& value) = 0;

	//! Sets a attribute as 3d vector
	virtual void setAttribute(const c8* attributeName, const core::vector3df& v) = 0;

	//! Gets an attribute as 3d vector
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual core::vector3df getAttributeAsVector3d(const c8* attributeName, const core::vector3df& defaultNotFound=core::vector3df(0,0,0)) const = 0;

	//! Gets an attribute as 3d vector
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual core::vector3df getAttributeAsVector3d(s32 index, const core::vector3df& defaultNotFound=core::vector3df(0,0,0)) const = 0;

	//! Sets an attribute as vector
	virtual void setAttribute(s32 index, const core::vector3df& v) = 0;

	/*
		Vector2d Attribute
	*/

	//! Adds an attribute as 2d vector
	virtual void addVector2d(const c8* attributeName, const core::vector2df& value) = 0;

	//! Sets a attribute as 2d vector
	virtual void setAttribute(const c8* attributeName, const core::vector2df& v) = 0;

	//! Gets an attribute as vector
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual core::vector2df getAttributeAsVector2d(const c8* attributeName, const core::vector2df& defaultNotFound=core::vector2df(0,0)) const = 0;

	//! Gets an attribute as position
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual core::vector2df getAttributeAsVector2d(s32 index, const core::vector2df& defaultNotFound=core::vector2df(0,0)) const = 0;

	//! Sets an attribute as 2d vector
	virtual void setAttribute(s32 index, const core::vector2df& v) = 0;

	/*
		Position2d Attribute
	*/

	//! Adds an attribute as 2d position
	virtual void addPosition2d(const c8* attributeName, const core::position2di& value) = 0;

	//! Sets a attribute as 2d position
	virtual void setAttribute(const c8* attributeName, const core::position2di& v) = 0;

	//! Gets an attribute as position
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual core::position2di getAttributeAsPosition2d(const c8* attributeName, const core::position2di& defaultNotFound=core::position2di(0,0)) const = 0;

	//! Gets an attribute as position
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual core::position2di getAttributeAsPosition2d(s32 index, const core::position2di& defaultNotFound=core::position2di(0,0)) const = 0;

	//! Sets an attribute as 2d position
	virtual void setAttribute(s32 index, const core::position2di& v) = 0;

	/*
		Rectangle Attribute
	*/

	//! Adds an attribute as rectangle
	virtual void addRect(const c8* attributeName, const core::rect<s32>& value) = 0;

	//! Sets an attribute as rectangle
	virtual void setAttribute(const c8* attributeName, const core::rect<s32>& v) = 0;

	//! Gets an attribute as rectangle
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual core::rect<s32> getAttributeAsRect(const c8* attributeName, const core::rect<s32>& defaultNotFound = core::rect<s32>()) const = 0;

	//! Gets an attribute as rectangle
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual core::rect<s32> getAttributeAsRect(s32 index, const core::rect<s32>& defaultNotFound = core::rect<s32>()) const = 0;

	//! Sets an attribute as rectangle
	virtual void setAttribute(s32 index, const core::rect<s32>& v) = 0;


	/*
		Dimension2d Attribute
	*/

	//! Adds an attribute as dimension2d
	virtual void addDimension2d(const c8* attributeName, const core::dimension2d<u32>& value) = 0;

	//! Sets an attribute as dimension2d
	virtual void setAttribute(const c8* attributeName, const core::dimension2d<u32>& v) = 0;

	//! Gets an attribute as dimension2d
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual core::dimension2d<u32> getAttributeAsDimension2d(const c8* attributeName, const core::dimension2d<u32>& defaultNotFound = core::dimension2d<u32>()) const = 0;

	//! Gets an attribute as dimension2d
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual core::dimension2d<u32> getAttributeAsDimension2d(s32 index, const core::dimension2d<u32>& defaultNotFound = core::dimension2d<u32>()) const = 0;

	//! Sets an attribute as dimension2d
	virtual void setAttribute(s32 index, const core::dimension2d<u32>& v) = 0;


	/*
		matrix attribute
	*/

	//! Adds an attribute as matrix
	virtual void addMatrix(const c8* attributeName, const core::matrix4& v) = 0;

	//! Sets an attribute as matrix
	virtual void setAttribute(const c8* attributeName, const core::matrix4& v) = 0;

	//! Gets an attribute as a matrix4
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual core::matrix4 getAttributeAsMatrix(const c8* attributeName, const core::matrix4& defaultNotFound=core::matrix4()) const = 0;

	//! Gets an attribute as matrix
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual core::matrix4 getAttributeAsMatrix(s32 index, const core::matrix4& defaultNotFound=core::matrix4()) const = 0;

	//! Sets an attribute as matrix
	virtual void setAttribute(s32 index, const core::matrix4& v) = 0;

	/*
		quaternion attribute
	*/

	//! Adds an attribute as quaternion
	virtual void addQuaternion(const c8* attributeName, const core::quaternion& v) = 0;

	//! Sets an attribute as quaternion
	virtual void setAttribute(const c8* attributeName, const core::quaternion& v) = 0;

	//! Gets an attribute as a quaternion
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual core::quaternion getAttributeAsQuaternion(const c8* attributeName, const core::quaternion& defaultNotFound=core::quaternion(0,1,0, 0)) const = 0;

	//! Gets an attribute as quaternion
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual core::quaternion getAttributeAsQuaternion(s32 index, const core::quaternion& defaultNotFound=core::quaternion(0,1,0, 0)) const = 0;

	//! Sets an attribute as quaternion
	virtual void setAttribute(s32 index, const core::quaternion& v) = 0;

	/*
		3d axis aligned bounding box attribute
	*/

	//! Adds an attribute as axis aligned bounding box
	virtual void addBox3d(const c8* attributeName, const core::aabbox3df& v) = 0;

	//! Sets an attribute as axis aligned bounding box
	virtual void setAttribute(const c8* attributeName, const core::aabbox3df& v) = 0;

	//! Gets an attribute as a axis aligned bounding box
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual core::aabbox3df getAttributeAsBox3d(const c8* attributeName, const core::aabbox3df& defaultNotFound=core::aabbox3df(0,0,0, 0,0,0)) const = 0;

	//! Gets an attribute as axis aligned bounding box
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual core::aabbox3df getAttributeAsBox3d(s32 index, const core::aabbox3df& defaultNotFound=core::aabbox3df(0,0,0, 0,0,0)) const = 0;

	//! Sets an attribute as axis aligned bounding box
	virtual void setAttribute(s32 index, const core::aabbox3df& v) = 0;

	/*
		plane
	*/

	//! Adds an attribute as 3d plane
	virtual void addPlane3d(const c8* attributeName, const core::plane3df& v) = 0;

	//! Sets an attribute as 3d plane
	virtual void setAttribute(const c8* attributeName, const core::plane3df& v) = 0;

	//! Gets an attribute as a 3d plane
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual core::plane3df getAttributeAsPlane3d(const c8* attributeName, const core::plane3df& defaultNotFound=core::plane3df(0,0,0, 0,1,0)) const = 0;

	//! Gets an attribute as 3d plane
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual core::plane3df getAttributeAsPlane3d(s32 index, const core::plane3df& defaultNotFound=core::plane3df(0,0,0, 0,1,0)) const = 0;

	//! Sets an attribute as 3d plane
	virtual void setAttribute(s32 index, const core::plane3df& v) = 0;


	/*
		3d triangle
	*/

	//! Adds an attribute as 3d triangle
	virtual void addTriangle3d(const c8* attributeName, const core::triangle3df& v) = 0;

	//! Sets an attribute as 3d triangle
	virtual void setAttribute(const c8* attributeName, const core::triangle3df& v) = 0;

	//! Gets an attribute as a 3d triangle
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual core::triangle3df getAttributeAsTriangle3d(const c8* attributeName, const core::triangle3df& defaultNotFound = core::triangle3df(core::vector3df(), core::vector3df(), core::vector3df())) const = 0;

	//! Gets an attribute as 3d triangle
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual core::triangle3df getAttributeAsTriangle3d(s32 index, const core::triangle3df& defaultNotFound = core::triangle3df(core::vector3df(), core::vector3df(), core::vector3df())) const = 0;

	//! Sets an attribute as 3d triangle
	virtual void setAttribute(s32 index, const core::triangle3df& v) = 0;


	/*
		line 2d attribute
	*/

	//! Adds an attribute as a 2d line
	virtual void addLine2d(const c8* attributeName, const core::line2df& v) = 0;

	//! Sets an attribute as a 2d line
	virtual void setAttribute(const c8* attributeName, const core::line2df& v) = 0;

	//! Gets an attribute as a 2d line
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual core::line2df getAttributeAsLine2d(const c8* attributeName, const core::line2df& defaultNotFound = core::line2df(0,0, 0,0)) const = 0;

	//! Gets an attribute as a 2d line
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual core::line2df getAttributeAsLine2d(s32 index, const core::line2df& defaultNotFound = core::line2df(0,0, 0,0)) const = 0;

	//! Sets an attribute as a 2d line
	virtual void setAttribute(s32 index, const core::line2df& v) = 0;


	/*
		line 3d attribute
	*/

	//! Adds an attribute as a 3d line
	virtual void addLine3d(const c8* attributeName, const core::line3df& v) = 0;

	//! Sets an attribute as a 3d line
	virtual void setAttribute(const c8* attributeName, const core::line3df& v) = 0;

	//! Gets an attribute as a 3d line
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual core::line3df getAttributeAsLine3d(const c8* attributeName, const core::line3df& defaultNotFound=core::line3df(0,0,0, 0,0,0)) const = 0;

	//! Gets an attribute as a 3d line
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual core::line3df getAttributeAsLine3d(s32 index, const core::line3df& defaultNotFound=core::line3df(0,0,0, 0,0,0)) const = 0;

	//! Sets an attribute as a 3d line
	virtual void setAttribute(s32 index, const core::line3df& v) = 0;


	/*
		Texture Attribute
	*/

	//! Adds an attribute as texture reference
	virtual void addTexture(const c8* attributeName, video::ITexture* texture, const io::path& filename = "") = 0;

	//! Sets an attribute as texture reference
	virtual void setAttribute(const c8* attributeName, video::ITexture* texture, const io::path& filename = "") = 0;

	//! Gets an attribute as texture reference
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	virtual video::ITexture* getAttributeAsTexture(const c8* attributeName, video::ITexture* defaultNotFound=0) const = 0;

	//! Gets an attribute as texture reference
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual video::ITexture* getAttributeAsTexture(s32 index, video::ITexture* defaultNotFound=0) const = 0;

	//! Sets an attribute as texture reference
	virtual void setAttribute(s32 index, video::ITexture* texture, const io::path& filename = "") = 0;


	/*
		User Pointer Attribute
	*/

	//! Adds an attribute as user pointer
	virtual void addUserPointer(const c8* attributeName, void* userPointer) = 0;

	//! Sets an attribute as user pointer
	virtual void setAttribute(const c8* attributeName, void* userPointer) = 0;

	//! Gets an attribute as user pointer
	//! \param attributeName: Name of the attribute to get.
	//! \param defaultNotFound Value returned when attributeName was not found
	virtual void* getAttributeAsUserPointer(const c8* attributeName, void* defaultNotFound = 0) const = 0;

	//! Gets an attribute as user pointer
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual void* getAttributeAsUserPointer(s32 index, void* defaultNotFound = 0) const = 0;

	//! Sets an attribute as user pointer
	virtual void setAttribute(s32 index, void* userPointer) = 0;

};

} // end namespace io
} // end namespace irr

#endif
