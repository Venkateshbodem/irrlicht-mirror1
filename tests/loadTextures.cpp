// Copyright (C) 2008-2009 Colin MacDonald
// No rights reserved: this software is in the public domain.

#include "testUtils.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/** This tests verifies that textures opened from different places in the
	filesystem don't create duplicated textures. */
bool loadFromFileFolder(void)
{
	IrrlichtDevice *device =
		createDevice( video::EDT_NULL, dimension2di(160, 120));

	if (!device)
	{
		logTestString("Unable to create EDT_NULL device\n");
		return false;
	}

	IVideoDriver * driver = device->getVideoDriver();

	u32 numTexs = driver->getTextureCount();

	ITexture * tex1 = driver->getTexture("../media/tools.png");
	assert(tex1);
	if(!tex1)
		logTestString("Unable to open ../media/tools.png\n");
	if (driver->getTextureCount()!=numTexs+1)
	{
		logTestString("No additional texture in the texture cache");
		return false;
	}

	IReadFile * readFile = device->getFileSystem()->createAndOpenFile("../media/tools.png");
	assert(readFile);
	if(!readFile)
		logTestString("Unable to open ../media/tools.png\n");
	if (driver->getTextureCount()!=numTexs+1)
	{
		logTestString("Additional texture in the texture cache (1)\n");
		return false;
	}

	ITexture * tex2 = driver->getTexture(readFile);
	assert(tex2);
	if(!readFile)
		logTestString("Unable to create texture from ../media/tools.png\n");
	if (driver->getTextureCount()!=numTexs+1)
	{
		logTestString("Additional texture in the texture cache (2)\n");
		return false;
	}

	readFile->drop();

	// adding  a folder archive
	device->getFileSystem()->addFolderFileArchive( "../media/" );

	ITexture * tex3 = driver->getTexture("tools.png");
	assert(tex3);
	if(!tex3)
		logTestString("Unable to open tools.png\n");
	if (driver->getTextureCount()!=numTexs+2)
	{
		logTestString("No additional texture in the texture cache (3)\n");
		return false;
	}

	ITexture * tex4 = driver->getTexture("tools.png");
	assert(tex4);
	if(!tex4)
		logTestString("Unable to open tools.png\n");
	if (driver->getTextureCount()!=numTexs+2)
	{
		logTestString("Additional texture in the texture cache (4)\n");
		return false;
	}

	device->drop();
	// in 1.5 we create duplicates in file folders.
	return ((tex1 == tex2) && (tex3 == tex4));
}

bool loadTextures()
{
	bool result = true;
	result &= loadFromFileFolder();
	return result;
}
