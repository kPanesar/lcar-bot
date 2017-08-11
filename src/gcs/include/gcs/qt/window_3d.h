/****************************************************************************
**
** Copyright (C) 2016 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/


/* 
 * File:   Window3D.h
 * Author: n8
 *
 * Created on July 16, 2017, 6:55 PM
 */

#ifndef WINDOW3D_H
#define WINDOW3D_H

#include <QtGui/QWindow>

namespace Qt3DCore {
class QAspectEngine;
class QAbstractAspect;
class QEntity;
}

namespace Qt3DRender {
class QCamera;
class QFrameGraphNode;
class QRenderAspect;
class QRenderSettings;
}

namespace Qt3DExtras {
class QForwardRenderer;
}

namespace Qt3DInput {
class QInputAspect;
class QInputSettings;
}

namespace Qt3DLogic {
class QLogicAspect;
}

class MultiViewportForwardRenderer;

class Window3D : public QWindow
{
    Q_OBJECT
public:
    Window3D(QScreen *screen = nullptr);
    virtual ~Window3D();
    
    inline Qt3DCore::QEntity * sceneRoot() const { return _scene_root; }
    void setSceneRoot(Qt3DCore::QEntity *root);
    inline Qt3DRender::QCamera * camera() const { return m_defaultCamera; }
    inline Qt3DRender::QCamera * miniMapCamera() const { return _mini_camera; }

    MultiViewportForwardRenderer * renderer()
    {
        return m_forwardRenderer;
    }
    
    void fixAspectRatio();
    
protected:
    void showEvent(QShowEvent *e) override;
    void resizeEvent(QResizeEvent *) override;
    
private:
    QScopedPointer<Qt3DCore::QAspectEngine> m_aspectEngine;

    // Aspects
    Qt3DRender::QRenderAspect *m_renderAspect;
    Qt3DInput::QInputAspect *m_inputAspect;
    Qt3DLogic::QLogicAspect *m_logicAspect;

    // Renderer configuration
    Qt3DRender::QRenderSettings *m_renderSettings;
    MultiViewportForwardRenderer *m_forwardRenderer;
    //Qt3DExtras::QForwardRenderer *m_forwardRenderer;
    Qt3DRender::QCamera *m_defaultCamera;
    Qt3DRender::QCamera * _mini_camera;

    // Input configurations
    Qt3DInput::QInputSettings *m_inputSettings;

    Qt3DCore::QEntity *_root;
    Qt3DCore::QEntity *_scene_root;

    bool _init;
};

#endif /* WINDOW3D_H */
