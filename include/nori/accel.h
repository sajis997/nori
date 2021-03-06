/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob

    Nori is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Nori is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <fstream>
#include <nori/mesh.h>
#include <nori/bvhprimitiveinfo.h>
#include <nori/bvhnode.h>

#include <algorithm>

NORI_NAMESPACE_BEGIN

/**
 * \brief Acceleration data structure for ray intersection queries
 *
 * The current implementation falls back to a brute force loop
 * through the geometry.
 */
class Accel {
public:

    Accel(uint32_t primPerNode = 8,
          SplitMethod split = SplitMethod::SPLIT_MIDDLE)
        : m_splitMethod(split),
          m_maxPrimitivesPerNode(primPerNode)
    {}

    ~Accel()
    {
        for_each(m_primitives.begin(),m_primitives.end(),[](auto p)
        {
            delete p;
        });

        m_primitives.clear();

        for_each(m_nodes.begin(),m_nodes.end(),[](auto p)
        {
            delete p;
        });

        m_nodes.clear();
    }
    /**
     * \brief Register a triangle mesh for inclusion in the acceleration
     * data structure
     *
     * This function can only be used before \ref build() is called
     */
    void addMesh(Mesh *mesh);

    /// Build the acceleration data structure (currently a no-op)
    void build();

    uint32_t buildRecursive(uint32_t left_index,
                            uint32_t right_index,
                            uint32_t depth);

    /// Return an axis-aligned box that bounds the scene
    const BoundingBox3f &getBoundingBox() const { return m_bbox; }

    /**
     * \brief Intersect a ray against all triangles stored in the scene and
     * return detailed intersection information
     *
     * \param ray
     *    A 3-dimensional ray data structure with minimum/maximum extent
     *    information
     *
     * \param its
     *    A detailed intersection record, which will be filled by the
     *    intersection query
     *
     * \param shadowRay
     *    \c true if this is a shadow ray query, i.e. a query that only aims to
     *    find out whether the ray is blocked or not without returning detailed
     *    intersection information.
     *
     * \return \c true if an intersection was found
     */
    bool rayIntersect(const Ray3f &ray, Intersection &its, bool shadowRay) const;




private:
    Mesh         *m_mesh = nullptr; ///< Mesh (only a single one for now)
    BoundingBox3f m_bbox;           ///< Bounding box of the entire scene

    SplitMethod m_splitMethod;

    uint32_t m_maxPrimitivesPerNode;
    std::vector<BvhPrimitiveInfo*> m_primitives;
    std::vector<BvhNode*> m_nodes;





};

NORI_NAMESPACE_END
