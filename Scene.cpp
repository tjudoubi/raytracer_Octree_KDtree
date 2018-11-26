#include "stdafx.h"
#include<cassert>
#include<fstream>
#include<string>
#include "scene.h"

using std::ifstream;
using std::string;
using std::vector;

Scene::~Scene()
{
	for(size_t i=0;i<Lights.size();i++)
		delete Lights[i];

	for(size_t i=0;i<Primitives.size();i++)
		delete Primitives[i];

	for(size_t i=0;i<Materials.size();i++)
		delete Materials[i];

	delete kdtree;
}
////////////////////////////////////////////////////////////////////////////////////
//	计算场景的包围盒
////////////////////////////////////////////////////////////////////////////////////
void Scene::ComputeBoundingBox(double eps)
{
	BoundingBox.p1 = Vector3D( 1e20, 1e20, 1e20 );
	BoundingBox.p2 = Vector3D( -1e20, -1e20, -1e20 );
	for( int i=0; i<(int)Primitives.size(); i++ )
	{
		Primitive* p = Primitives[i];
		if( TRIANGLE == p->GetType() )
		{
			Triangle* tri = (Triangle*)p;
			for( int k=0; k<3; k++ ) // 3 channels
			{
				if( tri->v0[k] < BoundingBox.p1[k] )
					BoundingBox.p1[k] = tri->v0[k];

				if( tri->v0[k] > BoundingBox.p2[k] )
					BoundingBox.p2[k] = tri->v0[k];

				if( tri->v1[k] < BoundingBox.p1[k] )
					BoundingBox.p1[k] = tri->v1[k];

				if( tri->v1[k] > BoundingBox.p2[k] )
					BoundingBox.p2[k] = tri->v1[k];

				if( tri->v2[k] < BoundingBox.p1[k] )
					BoundingBox.p1[k] = tri->v2[k];

				if( tri->v2[k] > BoundingBox.p2[k] )
					BoundingBox.p2[k] = tri->v2[k];
			}
				

		}
		else if( SPHERE == p->GetType())
		{
			Sphere* s = (Sphere*)p;
			Point c = s->Center;
			double r = s->Radius;
			for( int k=0; k<3; k++ ) // 3 channels
			{
				if( c[k] - r  < BoundingBox.p1[k] )
					BoundingBox.p1[k] =c[k] - r;
				if( c[k] + r > BoundingBox.p2[k] )
					BoundingBox.p2[k] = c[k] + r;
			}

		}
	}

	BoundingBox.p1 -= Vector3D( eps, eps, eps);
	BoundingBox.p2 += Vector3D( eps, eps, eps);
}
////////////////////////////////////////////////////////////////////////////////////
//	初始化场景
////////////////////////////////////////////////////////////////////////////////////
void Scene::InitScene()
{
	//----------------------------- your code here --------------------------------

#pragma region Light

	Light* light = new Light();
	light->position = Point(2.0, 10.0, 3.0);
	light->ambient = Color(0.1, 0.1, 0.1);
	light->diffuse = Color(15.0, 15.0, 15.0);
	light->specular = Color(30.0, 30.0, 30.0);
	light->attenuation[0] = 1.8;
	light->attenuation[1] = 3.0;
	light->attenuation[2] = 0.0;
	Lights.push_back(light);

	light = new Light();
	light->position = Point(2.0, 5.0, -2.0);
	light->ambient = Color(0.1, 0.1, 0.1);
	light->diffuse = Color(10.0, 10.0, 10.0);
	light->specular = Color(30.0, 30.0, 30.0);
	light->attenuation[0] = 1.8;
	light->attenuation[1] = 3.0;
	light->attenuation[2] = 0.0;
	Lights.push_back(light);

#pragma endregion

#pragma region Material

	// Material 1
	Material* material = new Material();
	material->ambient = Color(0.0, 0.0, 1.0);
	material->diffuse = Color(0.0, 0.0, 1.0);
	Materials.push_back(material);

	// Material 2
	material = new Material();
	material->ambient = Color(0.148200357920757, 0.0, 0.851799642079243);
	material->diffuse = Color(0.148200357920757, 0.0, 0.851799642079243);
	Materials.push_back(material);

	// Material 3
	material = new Material();
	material->ambient = Color(0.274437369753673, 0.0, 0.725562630246327);
	material->diffuse = Color(0.274437369753673, 0.0, 0.725562630246327);
	Materials.push_back(material);

	// Material 4
	material = new Material();
	material->ambient = Color(0.381966011250105, 0.0, 0.618033988749895);
	material->diffuse = Color(0.381966011250105, 0.0, 0.618033988749895);
	Materials.push_back(material);

	// Material 5
	material = new Material();
	material->ambient = Color(0.473558869590032, 0.0, 0.526441130409968);
	material->diffuse = Color(0.473558869590032, 0.0, 0.526441130409968);
	Materials.push_back(material);

	// Material 6
	material = new Material();
	material->ambient = Color(0.551577633540998, 0.0, 0.448422366459002);
	material->diffuse = Color(0.551577633540998, 0.0, 0.448422366459002);
	Materials.push_back(material);

	// Material 7
	material = new Material();
	material->ambient = Color(0.618033988749895, 0.0, 0.381966011250105);
	material->diffuse = Color(0.618033988749895, 0.0, 0.381966011250105);
	Materials.push_back(material);

	// Material 8
	material = new Material();
	material->ambient = Color(0.674641488330724, 0.0, 0.325358511669276);
	material->diffuse = Color(0.674641488330724, 0.0, 0.325358511669276);
	Materials.push_back(material);

	// Material 9
	material = new Material();
	material->ambient = Color(0.722859736212676, 0.0, 0.277140263787324);
	material->diffuse = Color(0.722859736212676, 0.0, 0.277140263787324);
	Materials.push_back(material);

	// Material 10
	material = new Material();
	material->ambient = Color(0.76393202250021, 0.0, 0.23606797749979);
	material->diffuse = Color(0.76393202250021, 0.0, 0.23606797749979);
	Materials.push_back(material);

	// Material 11
	material = new Material();
	material->ambient = Color(0.798917381259308, 0.0, 0.201082618740692);
	material->diffuse = Color(0.798917381259308, 0.0, 0.201082618740692);
	Materials.push_back(material);

	// Material 12
	material = new Material();
	material->ambient = Color(0.828717897328322, 0.0, 0.171282102671678);
	material->diffuse = Color(0.828717897328322, 0.0, 0.171282102671678);
	Materials.push_back(material);

	// Material 13
	material = new Material();
	material->ambient = Color(0.854101966249684, 0.0, 0.145898033750316);
	material->diffuse = Color(0.854101966249684, 0.0, 0.145898033750316);
	Materials.push_back(material);

	// Material 14
	material = new Material();
	material->ambient = Color(0.875724107071416, 0.0, 0.124275892928584);
	material->diffuse = Color(0.875724107071416, 0.0, 0.124275892928584);
	Materials.push_back(material);

	// Material 15
	material = new Material();
	material->ambient = Color(0.894141838884354, 0.0, 0.105858161115646);
	material->diffuse = Color(0.894141838884354, 0.0, 0.105858161115646);
	Materials.push_back(material);

	// Material 16
	material = new Material();
	material->ambient = Color(0.909830056250526, 0.0, 0.0901699437494743);
	material->diffuse = Color(0.909830056250526, 0.0, 0.0901699437494743);
	Materials.push_back(material);

	// Material 17
	material = new Material();
	material->ambient = Color(0.923193274187892, 0.0, 0.0768067258121077);
	material->diffuse = Color(0.923193274187892, 0.0, 0.0768067258121077);
	Materials.push_back(material);

	// Material 18
	material = new Material();
	material->ambient = Color(0.934576058443968, 0.0, 0.0654239415560319);
	material->diffuse = Color(0.934576058443968, 0.0, 0.0654239415560319);
	Materials.push_back(material);

	// Material 19
	material = new Material();
	material->ambient = Color(0.944271909999159, 0.0, 0.0557280900008413);
	material->diffuse = Color(0.944271909999159, 0.0, 0.0557280900008413);
	Materials.push_back(material);

	// Material 20
	material = new Material();
	material->ambient = Color(0.952530832883524, 0.0, 0.0474691671164764);
	material->diffuse = Color(0.952530832883524, 0.0, 0.0474691671164764);
	Materials.push_back(material);

	// Material 21
	material = new Material();
	material->ambient = Color(0.959565780440386, 0.0, 0.0404342195596143);
	material->diffuse = Color(0.959565780440386, 0.0, 0.0404342195596143);
	Materials.push_back(material);

	// Material 22
	material = new Material();
	material->ambient = Color(0.965558146251367, 0.0, 0.0344418537486331);
	material->diffuse = Color(0.965558146251367, 0.0, 0.0344418537486331);
	Materials.push_back(material);

	// Material 23
	material = new Material();
	material->ambient = Color(0.970662441304369, 0.0, 0.0293375586956313);
	material->diffuse = Color(0.970662441304369, 0.0, 0.0293375586956313);
	Materials.push_back(material);

	// Material 24
	material = new Material();
	material->ambient = Color(0.975010278003583, 0.0, 0.0249897219964175);
	material->diffuse = Color(0.975010278003583, 0.0, 0.0249897219964175);
	Materials.push_back(material);

	// Material 25
	material = new Material();
	material->ambient = Color(0.978713763747792, 0.0, 0.0212862362522083);
	material->diffuse = Color(0.978713763747792, 0.0, 0.0212862362522083);
	Materials.push_back(material);

	// Material 26
	material = new Material();
	material->ambient = Color(0.981868391579155, 0.0, 0.0181316084208452);
	material->diffuse = Color(0.981868391579155, 0.0, 0.0181316084208452);
	Materials.push_back(material);

	// Material 27
	material = new Material();
	material->ambient = Color(0.984555502436803, 0.0, 0.0154444975631969);
	material->diffuse = Color(0.984555502436803, 0.0, 0.0154444975631969);
	Materials.push_back(material);

	// Material 28
	material = new Material();
	material->ambient = Color(0.986844382503575, 0.0, 0.0131556174964249);
	material->diffuse = Color(0.986844382503575, 0.0, 0.0131556174964249);
	Materials.push_back(material);

	// Material 29
	material = new Material();
	material->ambient = Color(0.988794049725214, 0.0, 0.0112059502747861);
	material->diffuse = Color(0.988794049725214, 0.0, 0.0112059502747861);
	Materials.push_back(material);

	// Material 30
	material = new Material();
	material->ambient = Color(0.990454775566779, 0.0, 0.00954522443322059);
	material->diffuse = Color(0.990454775566779, 0.0, 0.00954522443322059);
	Materials.push_back(material);

	// Material 31
	material = new Material();
	material->ambient = Color(0.1, 0.1, 0.1);
	material->diffuse = Color(2.0, 2.0, 2.0);
	material->specular = Color(0.0, 0.0, 0.0);
	material->emission = Color(0.0, 0.0, 0.0);
	material->shininess = 0;
	material->reflect = 0.6;
	Materials.push_back(material);

#pragma endregion

#pragma region Primitive

	Primitive* primitive;

	primitive = new Sphere(1.18033988749895, Point(-4.86132557848613, 0.0, -1.16940737981102));
	primitive->SetMaterial(Materials[0]);
	Primitives.push_back(primitive);

	primitive = new Sphere(1, Point(-3, 0, -3));
	primitive->SetMaterial(Materials[1]);
	Primitives.push_back(primitive);


	primitive = new Sphere(0.5, Point(-1, 0, -4));
	primitive->SetMaterial(Materials[2]);
	Primitives.push_back(primitive);

	
	primitive = new Sphere(0.4, Point(3, 0, -2));
	primitive->SetMaterial(Materials[6]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.3, Point(2, -0.7, -1));
	primitive->SetMaterial(Materials[7]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.32, Point(2, -0.8, -0.3));
	primitive->SetMaterial(Materials[7]);
	Primitives.push_back(primitive);


	primitive = new Sphere(2, Point(1, -1, -1));
	primitive->SetMaterial(Materials[23]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.1, Point(1.2, -1.1, -1.4));
	primitive->SetMaterial(Materials[29]);
	Primitives.push_back(primitive);

	primitive = new Triangle(Point(-16, -1, -16), Point(-16, -1, 10), Point(14, -1, 10));
	primitive->SetMaterial(Materials[30]);
	Primitives.push_back(primitive);

	primitive = new Triangle(Point(-16, -1, -16), Point(14, -1, 10), Point(14, -1, -16));
	primitive->SetMaterial(Materials[30]);
	Primitives.push_back(primitive);

	Matrix4x4 mtx;
	mtx.Identity();
	mtx = Mat4x4Scale(18, 18, 18) * Mat4x4RotateY(PI / 4) * Mat4x4Translate(-1.65, -2, 0.5);
	LoadObjMesh("bunny_1.obj", Materials[2], mtx);


#pragma endregion

	ComputeBoundingBox( 0.01 );
	kdtree = new KDTree();
	kdtree->Build( this );

	//----------------------------- end of your code ------------------------------
}

////////////////////////////////////////////////////////////////////////////////////
// 加载obj网格文件
// pFileName	- 网格文件文件名
// mat			- 网格模型对应的材质
// mtx			- 对网格模型施加的变换
////////////////////////////////////////////////////////////////////////////////////
bool Scene::LoadObjMesh( const char* pFileName, Material* mat, const Matrix4x4 &mtx)
{
	ifstream input( pFileName );
    if( !input ) return false;	
	
	Matrix4x4 mtx2 = mtx.Inverse().Transpose();

	vector<Point> vtxArray;
	vector<Normal> nrmArray;
	vector<int> vIdxArray;
	vector<int> nIdxArray;

	string buffer;
    for(;;)
    {
        input>>buffer;
        if( !input )
            break;
        if( buffer == "v" )
        {
            // Vertex Position
            double x, y, z;
            input >> x >> y >> z;
			vtxArray.push_back(Point(x, y, z));
        }
        else if( buffer == "vn" )
        {
            double x, y, z;
            input >> x >> y >> z;
			nrmArray.push_back(Normal(x, y, z));
        }
        else if( buffer == "f" )
        {
            // Face
            int iPosition, iTexCoord, iNormal;

            for(int iFace = 0; iFace < 3; iFace++ )
            {
                input >> iPosition;
				vIdxArray.push_back( iPosition - 1 );     

                if( '/' == input.peek() )
                {
                    input.ignore();

                    if( '/' != input.peek() )
                    {
                        input >> iTexCoord;
						// ignored
                    }

                    if( '/' == input.peek() )
                    {
                        input.ignore();
                        input >> iNormal;
						nIdxArray.push_back(iNormal - 1);
                    }
                }
            }
        }
        else
        {
            // Unimplemented or unrecognized command
        }

        input.ignore( 1000, '\n' );
    }
	input.close();
	
	// store data
	assert( vIdxArray.size() % 3 == 0 );
	assert( nIdxArray.size() % 3 == 0 );
	

	for( int i=0; i<(int)vtxArray.size(); i++ )
	{
		vtxArray[i] = vtxArray[i] * mtx;

		if( nrmArray[i].Length() < 0.5f )
			nrmArray[i] = Normal(0, 0, 1);
		nrmArray[i] = Mul3x3( nrmArray[i], mtx2 );
		nrmArray[i].Normalize();
	}
	for( int i=0; i<(int)vIdxArray.size()/3; i++ )
	{
		Triangle* p = new Triangle(vtxArray[vIdxArray[3*i]], vtxArray[vIdxArray[3*i+1]], vtxArray[vIdxArray[3*i+2]]);

		Normal n1 = nrmArray[nIdxArray[3*i]];
		Normal n2 = nrmArray[nIdxArray[3*i+1]];
		Normal n3 = nrmArray[nIdxArray[3*i+2]];
		Normal fn = (n1 + n2 + n3) / 3;
		fn.Normalize();

		p->normal = fn;
		p->SetMaterial(mat);

		Primitives.push_back(p);
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////
// 计算光线与场景的相交情况
// a_ray				- 光线
// intersectNormal		- 相交点的法向
// intersectMaterial	- 相交点的材质
// bAcceleration		- 是否启用加速
////////////////////////////////////////////////////////////////////////////////////
bool Scene::FindIntersection( const Ray& a_ray, double &t, Normal &intersectNormal, Material** intersectMaterial, bool bAcceleration)
{
	if(!bAcceleration)
		return IntersectDirectly(a_ray, t, intersectNormal, intersectMaterial);
	else
		return IntersectOctree(a_ray, t, intersectNormal, intersectMaterial);
}

////////////////////////////////////////////////////////////////////////////////////
// 直接计算相交
// a_ray				- 光线
// intersectNormal		- 相交点的法向
// intersectMaterial	- 相交点的材质
////////////////////////////////////////////////////////////////////////////////////
bool Scene::IntersectDirectly(const Ray& a_ray, double& t, Normal& intersectNormal, Material** intersectMaterial)
{
	bool bIntersect=false;
	for(size_t i = 0;i<Primitives.size();i++)
	{
		
		Primitive* pr = Primitives[i];
		if (pr->Intersect( a_ray, t )) 
		{
			bIntersect = true;
			intersectNormal = pr->GetNormal(a_ray(t));
			*intersectMaterial = pr->GetMaterial();
		}
	}

	return bIntersect;
}

////////////////////////////////////////////////////////////////////////////////////
// 利用八叉树计算相交
// a_ray				- 光线
// intersectNormal		- 相交点的法向
// intersectMaterial	- 相交点的材质
////////////////////////////////////////////////////////////////////////////////////
bool Scene::IntersectOctree(const Ray& a_ray, double& t, Normal& intersectNormal, Material** intersectMaterial)
{
	return kdtree->FindNearest(this, a_ray, t, intersectNormal, intersectMaterial);
}

////////////////////////////////////////////////////////////////////////////////////
// 检测光线是否与指定图元计算相交
// priIdx				- 图元索引
// a_ray				- 光线
// intersectNormal		- 相交点的法向
// intersectMaterial	- 相交点的材质
////////////////////////////////////////////////////////////////////////////////////
bool Scene::IntersectPrimitiveRay( int priIdx, const Ray& a_ray, double &t, Normal &intersectNormal, Material** intersectMaterial) const
{
	Primitive* pr = Primitives[priIdx];

	if(pr->Intersect(a_ray, t))
	{
		intersectNormal = pr->GetNormal(a_ray(t));
		*intersectMaterial = pr->GetMaterial();
		return true;
	}
	return false;
}

bool Scene::IntersectPrimitiveAABB( int priIdx, const AABB &box ) const
{
	Primitive* pr = Primitives[priIdx];
	return pr->IntersectAABB(box);
}