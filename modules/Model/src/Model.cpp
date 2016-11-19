#include <avr/model/Model.hpp>
#include <glm/glm.h>

using std::abs;

namespace avr {

Model::~Model() {
   glmDelete(this->model);
   // TEMP
   glDeleteLists(index, 1);
}

void Model::Scale(float factor) {
   glmScale(model, factor);
}

void Model::ComputeFacetNormals() {
   glmFacetNormals(model);
}

void Model::ComputeVertexNormals(const Deg& angle) {
   glmVertexNormals(model, angle.Get());
}

void Model::ComputeTextureCoords(const ProjType& type) {
   if(type == LINEAR_PROJECTION)
      glmLinearTexture(model);
   else
      glmSpheremapTexture(model);
}

void Model::Reverse() {
   glmReverseWinding(model);
}

///////////////////// TEMP //////////////////////
void Model::Draw() {
   if(index > -1)
      glCallList(index);
   else glmDraw(model, GLM_SMOOTH | GLM_MATERIAL);
}

void Model::Compile() {
   if(index < 0)
      index = glmList(model, GLM_SMOOTH | GLM_MATERIAL);
}
/////////////////////////////////////////////////

Size3D Model::GetDims() const {
   BBox box = this->GetBBox();
   Size3D sz;
   sz.width  = abs(box.max.x) + abs(box.min.x);
   sz.height = abs(box.max.x) + abs(box.min.y);
   sz.depth  = abs(box.max.z) + abs(box.min.z);
   return sz;
}

BBox Model::GetBBox() const {
	AVR_ASSERT(model != nullptr);
	AVR_ASSERT(model->vertices != nullptr);

	BBox box;
	box.max.x = box.min.x = model->vertices[3 + 0];
	box.max.y = box.min.y = model->vertices[3 + 1];
	box.max.z = box.min.z = model->vertices[3 + 2];

	for (uint i = 1; i <= model->numvertices; i++) {
		if (box.max.x < model->vertices[3 * i + 0])
			box.max.x = model->vertices[3 * i + 0];
		if (box.min.x > model->vertices[3 * i + 0])
			box.min.x = model->vertices[3 * i + 0];

		if (box.max.y < model->vertices[3 * i + 1])
			box.max.y = model->vertices[3 * i + 1];
		if (box.min.y > model->vertices[3 * i + 1])
			box.min.y = model->vertices[3 * i + 1];

		if (box.max.z < model->vertices[3 * i + 2])
			box.max.z = model->vertices[3 * i + 2];
		if (box.min.z > model->vertices[3 * i + 2])
			box.min.z = model->vertices[3 * i + 2];
	}

	return box;
}

//////////////////////////// Factory ////////////////////////////////
SPtr<Model> FactoryModel::OBJ(const string& filename) {
   GLMmodel* glm = glmReadOBJ((char*) filename.c_str());
   if(!glm) return nullptr;
   glmUnitize(glm);

   SPtr<Model> model = new Model;
   model->model = glm;
   return model;
}

SPtr<Model> FactoryModel::OGL(void (*glDraw)(void)) {
   AVR_ERROR(Cod::NotImplemented, "");
   return nullptr;
}

SPtr<Model> FactoryModel::Sphere(int cuts) {
   AVR_ERROR(Cod::NotImplemented, "");
   return nullptr;
}

SPtr<Model> FactoryModel::Cube() {
   AVR_ERROR(Cod::NotImplemented, "");
   return nullptr;
}

} // namespace avr
