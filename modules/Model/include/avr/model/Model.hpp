#ifndef AVR_MODEL_HPP
#define AVR_MODEL_HPP

#include <avr/core/Core.hpp>

struct _GLMmodel;

namespace avr {

struct Size3D {
   float width, height, depth;

   Size3D() : width(0), height(0), depth(0) {/* default */}
   Size3D(float w, float h, float d) : width(w), height(h), depth(d) {/* init */}
   Size3D(float dims[3]): width(dims[0]), height(dims[1]), depth(dims[2]) {/* init */}
};

struct BBox {
   Point3f min, max;

   BBox() : min(Point3f(0, 0, 0)), max(Point3f(0, 0, 0)) {/* default */}
   BBox(const Point3f& min, const Point3f& max) : min(min), max(max) {/* init */}
   BBox(float minv[3], float maxv[3]) : min(Point3f(minv[0], minv[1], minv[2])), max(Point3f(maxv[3], maxv[4], maxv[5])) {/* init */}
   BBox(float minx, float miny, float minz, float maxx, float maxy, float maxz) : min(Point3f(minx, miny, minz)), max(Point3f(maxx, maxy, maxz)) {/* init */}
};

class Model {
public:
   // destructor
   ~Model();

   //! Changes the model's scale (default is 1)
   void Scale(float factor);

   /**
    * @brief Generates facet normals for a model.
    * Taking the cross product of the two vectors derived from the sides of each triangle.
    * @note Assumes a counter-clockwise winding.
    */
   void ComputeFacetNormals();

   /**
    * @brief Generates smooth vertex normals for a model.
    * First builds a list of all the triangles each vertex is in. Then loops through each vertex
    * in the the list averaging all the facet normals of the triangles each vertex is in. Finally,
    * sets the normal index in the triangle for the vertex to the generated smooth normal.
    * If the dot product of a facet normal and the facet normal associated with the first triangle
    * in the list of triangles the current vertex is in is greater than the cosine of the angle
    * parameter to the function, that facet normal is not added into the average normal calculation
    * and the corresponding vertex is given the facet normal. This tends to preserve hard edges.
    * @note The angle to use depends on the model, but 90 degrees is usually a good start.
    */
   void ComputeVertexNormals(const Deg& angle = Deg(90));

   //! Projection types to compute the model's texture coordinates
   enum ProjType { LINEAR_PROJECTION, SPHERICAL_PROJECTION };

   /**
    * @brief Generates texture coordinates according to a given projection of the texture map.
    * \li LINEAR_PROJECTION      generates these by linearly mapping the vertices onto a square.
    * \li SPHERICAL_PROJECTION   generates these by using the normal to calculate where that vertex would map onto a sphere.
    */
   void ComputeTextureCoords(const ProjType&);

   /**
    * @brief Reverse the polygon winding for all polygons in this model.
    * Default winding is counter-clockwise. Also changes the direction of the normals.
    */
   void Reverse();

   // TEMP [transferir para renderer]
   void Draw();
   void Compile();

   // Gets
   Size3D   GetDims() const;
   BBox     GetBBox() const;

private:
   Model() : index(-1), model(nullptr) {/* default */}

   int index;
   _GLMmodel* model;

   friend class FactoryModel;
   friend class Renderer;
};

class FactoryModel {
public:
   //! Creates a model by .obj file
   static SPtr<Model> OBJ(const std::string& filename);
   //! Creates a model by opengl description function
   static SPtr<Model> OGL(void (*glDraw)(void));
   //! Creates a sphere object of radious 1 (cuts is the number of circles in both direction)
   static SPtr<Model> Sphere(int cuts);
   //! Creates a cube object of size 1
   static SPtr<Model> Cube();
};

} // namespace avr

#endif // AVR_MODEL_HPP
