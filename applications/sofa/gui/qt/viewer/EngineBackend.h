/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2006 INRIA, USTL, UJF, CNRS, MGH                     *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program. If not, see <http://www.gnu.org/licenses/>.              *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_ENGINEBACKEND_H
#define SOFA_ENGINEBACKEND_H

#include <sofa/gui/PickHandler.h>
#include <SofaGraphComponent/ViewerSetting.h>

#include <sofa/helper/io/Image.h>

namespace sofa
{

namespace gui
{

namespace qt
{

namespace viewer
{

class EngineBackend
{
public:
    EngineBackend() {}

    virtual void setPickingMethod(sofa::gui::PickHandler* pick, sofa::component::configurationsetting::ViewerSetting* viewerConf) =0;
    virtual void setPrefix(const std::string& prefix) =0;
    virtual const std::string screenshotName() =0;
    virtual void screenshot(const std::string& filename, int compression_level = -1) =0;
    virtual void setBackgroundImage(helper::io::Image* image) =0;
    virtual void drawBackgroundImage(const int screenWidth, const int screenHeight)=0;

    virtual bool initRecorder(int width, int height, unsigned int framerate, unsigned int bitrate, const std::string& codecExtension="", const std::string& codecName="")  =0;
    virtual void endRecorder() =0;
    virtual void addFrameRecorder() =0;


private:

};

} // namespace viewer

} // namespace qt

} // namespace gui

} // namespace sofa

#endif // SOFA_ENGINEBACKEND_H
