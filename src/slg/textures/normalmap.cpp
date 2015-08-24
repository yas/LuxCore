/***************************************************************************
 * Copyright 1998-2015 by authors (see AUTHORS.txt)                        *
 *                                                                         *
 *   This file is part of LuxRender.                                       *
 *                                                                         *
 * Licensed under the Apache License, Version 2.0 (the "License");         *
 * you may not use this file except in compliance with the License.        *
 * You may obtain a copy of the License at                                 *
 *                                                                         *
 *     http://www.apache.org/licenses/LICENSE-2.0                          *
 *                                                                         *
 * Unless required by applicable law or agreed to in writing, software     *
 * distributed under the License is distributed on an "AS IS" BASIS,       *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*
 * See the License for the specific language governing permissions and     *
 * limitations under the License.                                          *
 ***************************************************************************/

#include "slg/textures/normalmap.h"

using namespace std;
using namespace luxrays;
using namespace slg;

//------------------------------------------------------------------------------
// NormalMap textures
//------------------------------------------------------------------------------

void NormalMapTexture::Bump(HitPoint *hitPoint, const float sampleDistance) const {
    const Spectrum rgb = tex->GetSpectrumValue(*hitPoint).Clamp(0.f, 1.f);

	// Normal from normal map
	Vector n(rgb.c);
	n = 2.f * n - Vector(1.f, 1.f, 1.f);

	const Normal oldShadeN = hitPoint->shadeN;

	// Transform n from tangent to object space
	hitPoint->shadeN = Normal(Normalize(hitPoint->GetFrame().ToWorld(n)));
	hitPoint->shadeN *= (Dot(oldShadeN, hitPoint->shadeN) < 0.f) ? -1.f : 1.f;

	// Update dpdu and dpdv so they are still orthogonal to shadeN 
	hitPoint->dpdu = Cross(hitPoint->shadeN, Cross(hitPoint->dpdu, hitPoint->shadeN));
	hitPoint->dpdv = Cross(hitPoint->shadeN, Cross(hitPoint->dpdv, hitPoint->shadeN));
}

Properties NormalMapTexture::ToProperties(const ImageMapCache &imgMapCache) const {
	Properties props;
	
	const string name = GetName();
	props.Set(Property("scene.textures." + name + ".type")("normalmap"));
	props.Set(Property("scene.textures." + name + ".texture")(tex->GetName()));

	return props;
}
