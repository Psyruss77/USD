//
// Copyright 2019 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
#ifndef EXT_RMANPKG_24_0_PLUGIN_RENDERMAN_PLUGIN_HD_PRMAN_INT_RENDER_PASS_H
#define EXT_RMANPKG_24_0_PLUGIN_RENDERMAN_PLUGIN_HD_PRMAN_INT_RENDER_PASS_H

#include "pxr/pxr.h"
#include "pxr/base/gf/matrix4d.h"
#include "pxr/imaging/cameraUtil/framing.h"
#include "pxr/imaging/hd/renderPass.h"

#include "Riley.h"

#include <chrono>

PXR_NAMESPACE_OPEN_SCOPE

struct HdPrman_Context;
struct HdPrman_InteractiveContext;

class HdPrman_InteractiveRenderPass final : public HdRenderPass
{
public:
    HdPrman_InteractiveRenderPass(
        HdRenderIndex *index,
        HdRprimCollection const &collection,
        std::shared_ptr<HdPrman_Context> context);
    ~HdPrman_InteractiveRenderPass() override;

    bool IsConverged() const override;

protected:
    void _Execute(HdRenderPassStateSharedPtr const& renderPassState,
                  TfTokenVector const &renderTags) override;

private:
    std::shared_ptr<HdPrman_InteractiveContext> _interactiveContext;
    bool _converged;
    int _lastRenderedVersion;
    int _lastSettingsVersion;
    GfMatrix4d _lastProj;
    GfMatrix4d _lastViewToWorldMatrix;
    CameraUtilFraming _lastFraming;
    GfVec4f _lastCropWindow;
    uint32_t _lastCamPropertiesHash;

    std::chrono::steady_clock::time_point _frameStart;
    std::string _integrator;
    std::string _quickIntegrator;
    float _quickIntegrateTime;
    bool _quickIntegrate;
    bool _isPrimaryIntegrator;
    riley::IntegratorId _mainIntegratorId;
    riley::IntegratorId _quickIntegratorId;
    std::vector<riley::ClippingPlaneId> _clipPlanes;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // EXT_RMANPKG_24_0_PLUGIN_RENDERMAN_PLUGIN_HD_PRMAN_INT_RENDER_PASS_H
