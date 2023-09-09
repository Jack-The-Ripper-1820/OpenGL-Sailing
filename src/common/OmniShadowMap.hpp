#pragma once

#include <ShadowMap.hpp>

class OmniShadowMap : public ShadowMap {
public:
	OmniShadowMap();

	bool Init(unsigned int width, unsigned int height);

	void Write();

	virtual void Read(GLenum textureUnit);

	~OmniShadowMap();
};