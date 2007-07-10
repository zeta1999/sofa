/*******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 1       *
*                (c) 2006-2007 MGH, INRIA, USTL, UJF, CNRS                     *
*                                                                              *
* This library is free software; you can redistribute it and/or modify it      *
* under the terms of the GNU Lesser General Public License as published by the *
* Free Software Foundation; either version 2.1 of the License, or (at your     *
* option) any later version.                                                   *
*                                                                              *
* This library is distributed in the hope that it will be useful, but WITHOUT  *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License  *
* for more details.                                                            *
*                                                                              *
* You should have received a copy of the GNU Lesser General Public License     *
* along with this library; if not, write to the Free Software Foundation,      *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.           *
*                                                                              *
* Contact information: contact@sofa-framework.org                              *
*                                                                              *
* Authors: J. Allard, P-J. Bensoussan, S. Cotin, C. Duriez, H. Delingette,     *
* F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza, M. Nesme, P. Neumann,        *
* and F. Poyer                                                                 *
*******************************************************************************/
#include <sofa/component/topology/RegularGridTopology.h>
#include <sofa/core/ObjectFactory.h>

namespace sofa
{

namespace component
{

namespace topology
{

using namespace sofa::defaulttype;
using std::cout;
using std::endl;

void RegularGridTopology::parse(core::objectmodel::BaseObjectDescription* arg)
{
    this->GridTopology::parse(arg);
    const char* xmin = arg->getAttribute("xmin",arg->getAttribute("min","0"));
    const char* ymin = arg->getAttribute("ymin",arg->getAttribute("min","0"));
    const char* zmin = arg->getAttribute("zmin",arg->getAttribute("min","0"));
    const char* xmax = arg->getAttribute("xmax",arg->getAttribute("max",arg->getAttribute("nx","1")));
    const char* ymax = arg->getAttribute("ymax",arg->getAttribute("max",arg->getAttribute("ny","1")));
    const char* zmax = arg->getAttribute("zmax",arg->getAttribute("max",arg->getAttribute("nz","1")));
    this->setPos(atof(xmin),atof(xmax),atof(ymin),atof(ymax),atof(zmin),atof(zmax));
}

SOFA_DECL_CLASS(RegularGridTopology)

int RegularGridTopologyClass = core::RegisterObject("Regular grid in 3D")
        .addAlias("RegularGrid")
        .add< RegularGridTopology >()
        ;

RegularGridTopology::RegularGridTopology(int nx, int ny, int nz)
    : GridTopology(nx, ny, nz)
{
}

RegularGridTopology::RegularGridTopology()
{
}

void RegularGridTopology::setPos(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax)
{
    setP0(Vec3(xmin,ymin,zmin));
    if (nx.getValue()>1)
        setDx(Vec3((xmax-xmin)/(nx.getValue()-1),0,0));
    else
        setDx(Vec3(0,0,0));
    if (ny.getValue()>1)
        setDy(Vec3(0,(ymax-ymin)/(ny.getValue()-1),0));
    else
        setDy(Vec3(0,0,0));
    if (nz.getValue()>1)
        setDz(Vec3(0,0,(zmax-zmin)/(nz.getValue()-1)));
    else
        setDz(Vec3(0,0,0));
}

unsigned RegularGridTopology::getIndex( int i, int j, int k ) const
{
    return nx.getValue()* ( ny.getValue()*k + j ) + i;
}

RegularGridTopology::Vec3 RegularGridTopology::getPoint(int i) const
{
    int x = i%nx.getValue(); i/=nx.getValue();
    int y = i%ny.getValue(); i/=ny.getValue();
    int z = i;
    return getPoint(x,y,z);
}

RegularGridTopology::Vec3 RegularGridTopology::getPoint(int x, int y, int z) const
{
    return p0+dx*x+dy*y+dz*z;
}

/// return the cube containing the given point (or -1 if not found).
int RegularGridTopology::findCube(const Vec3& pos)
{
    if (nx.getValue()<2 || ny.getValue()<2 || nz.getValue()<2) return -1;
    Vec3 p = pos-p0;
    double x = p*dx*inv_dx2;
    double y = p*dy*inv_dy2;
    double z = p*dz*inv_dz2;
    int ix = int(x+1000000)-1000000; // Do not round toward 0...
    int iy = int(y+1000000)-1000000;
    int iz = int(z+1000000)-1000000;
    if ((unsigned)ix<(unsigned)nx.getValue()-2 && (unsigned)iy<(unsigned)ny.getValue()-2 && (unsigned)iz<(unsigned)nz.getValue()-2)
    {
        return cube(ix,iy,iz);
    }
    else
    {
        return -1;
    }
}

/// return the nearest cube (or -1 if not found).
int RegularGridTopology::findNearestCube(const Vec3& pos)
{
    if (nx.getValue()<2 || ny.getValue()<2 || nz.getValue()<2) return -1;
    Vec3 p = pos-p0;
    double x = p*dx*inv_dx2;
    double y = p*dy*inv_dy2;
    double z = p*dz*inv_dz2;
    int ix = int(x+1000000)-1000000; // Do not round toward 0...
    int iy = int(y+1000000)-1000000;
    int iz = int(z+1000000)-1000000;
    if (ix<0) ix=0; else if (ix>nx.getValue()-2) ix=nx.getValue()-2;
    if (iy<0) iy=0; else if (iy>ny.getValue()-2) iy=ny.getValue()-2;
    if (iz<0) iz=0; else if (iz>nz.getValue()-2) iz=nz.getValue()-2;
    return cube(ix,iy,iz);
}

/// return the cube containing the given point (or -1 if not found),
/// as well as deplacements from its first corner in terms of dx, dy, dz (i.e. barycentric coordinates).
int RegularGridTopology::findCube(const Vec3& pos, double& fx, double &fy, double &fz)
{
    if (nx.getValue()<2 || ny.getValue()<2 || nz.getValue()<2) return -1;
    Vec3 p = pos-p0;
    double x = p*dx*inv_dx2;
    double y = p*dy*inv_dy2;
    double z = p*dz*inv_dz2;
    int ix = int(x+1000000)-1000000; // Do not round toward 0...
    int iy = int(y+1000000)-1000000;
    int iz = int(z+1000000)-1000000;
    if ((unsigned)ix<=(unsigned)nx.getValue()-2 && (unsigned)iy<=(unsigned)ny.getValue()-2 && (unsigned)iz<=(unsigned)nz.getValue()-2)
    {
        fx = x-ix;
        fy = y-iy;
        fz = z-iz;
        return cube(ix,iy,iz);
    }
    else
    {
        return -1;
    }
}

/// return the cube containing the given point (or -1 if not found),
/// as well as deplacements from its first corner in terms of dx, dy, dz (i.e. barycentric coordinates).
int RegularGridTopology::findNearestCube(const Vec3& pos, double& fx, double &fy, double &fz)
{
    if (nx.getValue()<2 || ny.getValue()<2 || nz.getValue()<2) return -1;
    Vec3 p = pos-p0;
    double x = p*dx*inv_dx2;
    double y = p*dy*inv_dy2;
    double z = p*dz*inv_dz2;
    int ix = int(x+1000000)-1000000; // Do not round toward 0...
    int iy = int(y+1000000)-1000000;
    int iz = int(z+1000000)-1000000;
    if (ix<0) ix=0; else if (ix>nx.getValue()-2) ix=nx.getValue()-2;
    if (iy<0) iy=0; else if (iy>ny.getValue()-2) iy=ny.getValue()-2;
    if (iz<0) iz=0; else if (iz>nz.getValue()-2) iz=nz.getValue()-2;
    fx = x-ix;
    fy = y-iy;
    fz = z-iz;
    return cube(ix,iy,iz);
}

} // namespace topology

} // namespace component

} // namespace sofa

