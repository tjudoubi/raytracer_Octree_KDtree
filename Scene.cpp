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

	primitive = new Sphere(1.00541309370346, Point(-3.54574366132457, -0.17492679379549, -3.52529458175862));
	primitive->SetMaterial(Materials[1]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.85641051335839, Point(-1.58627809371627, -0.323929374140558, -4.74170030784273));
	primitive->SetMaterial(Materials[2]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.729490168751577, Point(0.367334190290388, -0.450849718747371, -4.97808947714315));
	primitive->SetMaterial(Materials[3]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.62137946464292, Point(1.90825642044524, -0.558960422856028, -4.34040770674477));
	primitive->SetMaterial(Materials[4]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.529290805578231, Point(2.79616722001328, -0.651049081920717, -3.22535244639666));
	primitive->SetMaterial(Materials[5]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.450849718747371, Point(3.08727708634764, -0.729490168751577, -2.04351054609657));
	primitive->SetMaterial(Materials[6]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.384033629060538, Point(2.87749552687594, -0.79630625843841, -1.03397985245495));
	primitive->SetMaterial(Materials[7]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.327119707780159, Point(2.3038390139185, -0.853220179718789, -0.365420603840839));
	primitive->SetMaterial(Materials[7]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.278640450004206, Point(1.62028324058866, -0.901699437494742, -0.0513627603773734));
	primitive->SetMaterial(Materials[9]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.237345835582382, Point(0.980258044388319, -0.942994051916566, -0.0170817450049947));
	primitive->SetMaterial(Materials[10]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.202171097798072, Point(0.479437901391085, -0.978168789700877, -0.230635377721785));
	primitive->SetMaterial(Materials[11]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.172209268743165, Point(0.167061568198127, -1.00813061875578, -0.574972810713355));
	primitive->SetMaterial(Materials[12]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.146687793478156, Point(0.0237193138323233, -1.03365209402079, -0.944901788219927));
	primitive->SetMaterial(Materials[13]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.124948609982087, Point(0.00719170920714229, -1.05539127751686, -1.28252032420256));
	primitive->SetMaterial(Materials[14]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.106431181261041, Point(0.102680868181957, -1.07390870623791, -1.55352349783658));
	primitive->SetMaterial(Materials[15]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.0906580421042258, Point(0.261735710331161, -1.08968184539472, -1.74012705503053));
	primitive->SetMaterial(Materials[16]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.0772224878159845, Point(0.439777373377788, -1.10311739968296, -1.84983402675777));
	primitive->SetMaterial(Materials[17]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.0657780874821243, Point(0.610844296797831, -1.11456180001682, -1.9001169803364));
	primitive->SetMaterial(Materials[18]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.0560297513739306, Point(0.762545004027716, -1.12431013612502, -1.90861668696731));
	primitive->SetMaterial(Materials[19]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.047726122166103, Point(0.888755933391988, -1.13261376533285, -1.88076213548416));
	primitive->SetMaterial(Materials[20]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.0406530937789168, Point(0.9863186922521, -1.13968679372003, -1.82952666174767));
	primitive->SetMaterial(Materials[21]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.0346282907302952, Point(1.05752509391536, -1.14571159676865, -1.76826751274224));
	primitive->SetMaterial(Materials[22]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.0294963656498814, Point(1.10705278593008, -1.15084352184907, -1.70537826566795));
	primitive->SetMaterial(Materials[23]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.0251249937032075, Point(1.13990357379393, -1.15521489379574, -1.64559841234085));
	primitive->SetMaterial(Materials[24]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.0214014606436354, Point(1.16050002715239, -1.15893842685531, -1.59125267344645));
	primitive->SetMaterial(Materials[25]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.0182297565162216, Point(1.17240551144276, -1.16211013098273, -1.54319119252395));
	primitive->SetMaterial(Materials[26]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.0155281000757093, Point(1.17833716926224, -1.16481178742324, -1.50142841198435));
	primitive->SetMaterial(Materials[27]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.0132268300866599, Point(1.18029205596044, -1.16711305741229, -1.46554748174658));
	primitive->SetMaterial(Materials[28]);
	Primitives.push_back(primitive);

	primitive = new Sphere(0.0112666091336598, Point(1.1793026802878, -1.16907327836529, -1.43496076279161));
	primitive->SetMaterial(Materials[29]);
	Primitives.push_back(primitive);

	primitive = new Triangle(Point(-16, -1.18033988749895, -16), Point(-16, -1.18033988749895, 10), Point(14, -1.18033988749895, 10));
	primitive->SetMaterial(Materials[30]);
	Primitives.push_back(primitive);

	primitive = new Triangle(Point(-16, -1.18033988749895, -16), Point(14, -1.18033988749895, 10), Point(14, -1.18033988749895, -16));
	primitive->SetMaterial(Materials[30]);
	Primitives.push_back(primitive);

	Matrix4x4 mtx;
	mtx.Identity();
	mtx = Mat4x4Scale(18, 18, 18) * Mat4x4RotateY(PI / 4) * Mat4x4Translate(-1.65, -2, 0.5);
	LoadObjMesh("bunny_4.obj", Materials[2], mtx);


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