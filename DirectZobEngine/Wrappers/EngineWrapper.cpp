#ifdef _WINDLL
#include "EngineWrapper.h"

namespace CLI
{
	static DirectZobType::RenderOptions m_renderOptions;
	static Line m_lines[NB_EDITOR_LINES];
	static Circle m_circles[NB_EDITOR_CIRCLES];

	EngineWrapper::EngineWrapper():ManagedObject(DirectZob::GetInstance()->GetEngine(), false)
	{
		m_renderOptions.zBuffered = false;
		m_renderOptions.bTransparency = true;
		m_renderOptions.cullMode = eCullMode_None;
		m_renderOptions.lightMode = DirectZobType::RenderOptions::eLightMode_none;
		m_nbTriangles = 0;
		m_triangleList = (Triangle*)malloc(sizeof(Triangle) * NB_EDITOR_TRIANGLES);
		m_vertices = (ZobVector3*)malloc(sizeof(ZobVector3) * NB_EDITOR_TRIANGLES * 3);
		m_uvs = (ZobVector2*)malloc(sizeof(ZobVector2) * NB_EDITOR_TRIANGLES * 3);
		m_projectedVertices = (ZobVector3*)malloc(sizeof(ZobVector3) * NB_EDITOR_TRIANGLES * 3);
		m_normals = (ZobVector3*)malloc(sizeof(ZobVector3) * NB_EDITOR_TRIANGLES * 3);
		int vi = 0;
		for (int i = 0; i < NB_EDITOR_TRIANGLES; i ++)
		{
			m_triangleList[i].verticeAIndex = vi;
			m_triangleList[i].verticeBIndex = vi + 1;
			m_triangleList[i].verticeCIndex = vi + 2;
			m_triangleList[i].va = &m_vertices[vi];
			m_triangleList[i].vb = &m_vertices[vi+1];
			m_triangleList[i].vc = &m_vertices[vi+2];
			m_triangleList[i].pa = &m_projectedVertices[vi];
			m_triangleList[i].pb = &m_projectedVertices[vi + 1];
			m_triangleList[i].pc = &m_projectedVertices[vi + 2];
			m_triangleList[i].na = &m_normals[vi];
			m_triangleList[i].nb = &m_normals[vi + 1];
			m_triangleList[i].nc = &m_normals[vi + 2];
			m_triangleList[i].ua = &m_uvs[vi];
			m_triangleList[i].ub = &m_uvs[vi + 1];
			m_triangleList[i].uc = &m_uvs[vi + 2];
			m_triangleList[i].n = &m_normals[i];
			m_triangleList[i].options = &m_renderOptions;
			vi += 3;
		}
	}

	EngineWrapper::~EngineWrapper()
	{
		free(m_triangleList);
		free(m_vertices);
		free(m_uvs);
		free(m_projectedVertices);
		free(m_normals);
	}

	int EngineWrapper::GetBufferWidth()
	{
		return m_Instance->GetBufferData()->width;
	}
	int EngineWrapper::GetBufferHeight()
	{
		return m_Instance->GetBufferData()->height;
	}
	void EngineWrapper::SetRenderOutput(int r)
	{
		if (r >= 0 && r < (int)eRenderOutput::__eRenderOutput_MAX__)
		{
			m_Instance->SetRenderOutput((eRenderOutput)r);
		}
	}
	void EngineWrapper::SetLightingPrecision(int r)
	{
		if (r >= 0 && r < (int)eLightingPrecision::__eLightingPrecision_MAX__)
		{
			m_Instance->SetLightingPrecision((eLightingPrecision)r);
		}
	}
	void EngineWrapper::SetRenderMode(int r)
	{
		if (r >= 0 && r < (int)eRenderMode::__eRenderMode_MAX__)
		{
			m_Instance->SetRenderMode((eRenderMode)r);
		}
	}
	bool EngineWrapper::GetProjectedCoords(ManagedVector3^ worldSpacePos)
	{
		ZobVector3 v = worldSpacePos->ToVector3();
		if (m_Instance->GetProjectedCoords(&v))
		{
			worldSpacePos->FromVector3(v);
			return true;
		}
		return false;
	}
	float EngineWrapper::GetDistanceToCamera(ManagedVector3^ worldSpacePos)
	{
		ZobVector3 v = worldSpacePos->ToVector3();
		return m_Instance->GetDistanceToCamera(&v);
	} 

	ZobObjectWrapper^ EngineWrapper::GetObjectAt2DCoords(float x, float y)
	{
		ZobObject* z = m_Instance->GetObjectAt2DCoords(x, y);
		if(z)
		{
			return gcnew ZobObjectWrapper(z);
		}
		return nullptr;
	}

	void EngineWrapper::QueueObjectsToRender()
	{
		Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		BufferData* bData = m_Instance->GetBufferData();
		float w = (float)bData->width / 2.0f;
		float h = (float)bData->height / 2.0f;
		if (c)
		{
			for (int i = 0; i < m_nbTriangles; i++)
			{
				Triangle* t = &m_triangleList[i];
				t->pa->Copy(t->va);
				t->pb->Copy(t->vb);
				t->pc->Copy(t->vc);
				c->ToViewSpace(t->pa);
				c->ToViewSpace(t->pb);
				c->ToViewSpace(t->pc);
				c->GetProjectionMatrix()->Mul(t->pa);
				c->GetProjectionMatrix()->Mul(t->pb);
				c->GetProjectionMatrix()->Mul(t->pc);
				t->pa->x = (t->pa->x / t->pa->z + 1) * w;
				t->pa->y = (t->pa->y / t->pa->z + 1) * h;
				t->pb->x = (t->pb->x / t->pb->z + 1) * w;
				t->pb->y = (t->pb->y / t->pb->z + 1) * h;
				t->pc->x = (t->pc->x / t->pc->z + 1) * w;
				t->pc->y = (t->pc->y / t->pc->z + 1) * h;
				t->draw = true;
				t->material = NULL;
				t->ComputeArea();
				t->clipMode = Triangle::eClip_3_in;
				m_Instance->QueueWorldTriangle(c, t);
			}
		}
		m_nbTriangles = 0;
		for (int i = 0; i < m_nbLines; i++)
		{
			m_Instance->QueueLine(c, &m_lines[i].p0, &m_lines[i].p1, m_lines[i].color, m_lines[i].bold, m_lines[i].noZ);
		}
		m_nbLines = 0;
		for (int i = 0; i < m_nbCircles; i++)
		{
			m_Instance->QueueEllipse(c, &m_circles[i].p, &m_circles[i].n, m_circles[i].r, m_circles[i].r, m_circles[i].color, m_circles[i].bold, m_circles[i].noZ);
		}
		m_nbCircles = 0;
	}

	void EngineWrapper::DrawTriangle(ManagedVector3^ p0, ManagedVector3^ p1, ManagedVector3^ p2, int color)
	{
		Triangle t = m_triangleList[m_nbTriangles];
		ZobVector3 pp0 = p0->ToVector3();
		ZobVector3 pp1 = p1->ToVector3();
		ZobVector3 pp2 = p2->ToVector3();
		t.va->Copy(&pp0);
		t.vb->Copy(&pp1);
		t.vc->Copy(&pp2);
		t.na = &ZobVector3(0, 1, 0);
		t.nb = &ZobVector3(0, 1, 0);
		t.nc = &ZobVector3(0, 1, 0);
//		t.options->colorization.x = 255;// = ZobVector3(color & 0xFF0000 >> 16, color & 0x00FF00 >> 8, color & 0x0000FF);
		m_nbTriangles++;
		//t.
//		m_Instance->QueueTriangle(&t);
	}

	void EngineWrapper::DrawCircle(ManagedVector3^ p0, ManagedVector3^ up, float r, int color, bool bold, bool noZ)
	{
		Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		if (c)
		{
			if (m_nbCircles < NB_EDITOR_LINES)
			{
				m_circles[m_nbCircles].p = p0->ToVector3();
				m_circles[m_nbCircles].n = up->ToVector3();
				m_circles[m_nbCircles].r = r;
				m_circles[m_nbCircles].color = color;
				m_circles[m_nbCircles].bold = bold;
				m_circles[m_nbCircles].noZ = noZ;
				m_nbCircles++;
			}
		}
	}

	void EngineWrapper::DrawLine(ManagedVector3^ p0, ManagedVector3^ p1, int color, bool bold, bool noZ)
	{
		Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		if (c)
		{
			if (m_nbLines < NB_EDITOR_LINES)
			{
				m_lines[m_nbLines].p0 = p0->ToVector3();
				m_lines[m_nbLines].p1 = p1->ToVector3();
				m_lines[m_nbLines].color = color;
				m_lines[m_nbLines].bold = bold;
				m_lines[m_nbLines].noZ = noZ;
				m_nbLines++;
			}
		}
	}
}
#endif //_WINDLL