#ifdef _WINDLL
#include "ZobObjectsEditor.h"
#include "../DirectZobWrapper.h"
#include "../ZobObjectManagerWrapper.h"
#include "../../DirectZobEngine//Behaviors/ZobBehaviorMesh.h"
#include <iostream>
#include <filesystem>
namespace CLI
{
	ZobObjectsEditor::ZobObjectsEditor()
	{
		m_gizmosGuid = 0;
		m_currentSelectedGizmo = NULL;
	}

	ZobObjectsEditor::~ZobObjectsEditor()
	{
		
	}

	void ZobObjectsEditor::SetParent(ZobObject* z)
	{
		ZobObjectManager* zm = DirectZob::GetInstance() ? DirectZob::GetInstance()->GetZobObjectManager() : NULL;
		if (zm)
		{
			ZobObject* g = zm->GetZobObjectFromlId(m_gizmosGuid);
			if (g)
			{
				g->SetParent(z);
				g->SetLocalPosition(0, 0, 0);
				g->SetLocalRotation(0, 0, 0);
			}
		}
	}

	void ZobObjectsEditor::AddEditorGizmos()
	{
		ZobObjectManager* zm = DirectZob::GetInstance()?DirectZob::GetInstance()->GetZobObjectManager():NULL;
		if (zm)
		{
			char lpTempPathBuffer[MAX_PATH];
			GetTempPath(MAX_PATH, lpTempPathBuffer);
			string arrowFile = string(lpTempPathBuffer);
			arrowFile.append("arrow.obj");
			std::FILE* tmpf = fopen(arrowFile.c_str(), "w");
			std::fputs(gArrow.c_str(), tmpf);
			std::rewind(tmpf);
			//tmpf-
			ZobObject* m_editorRoot = zm->CreateEditorZobObject(NULL);
			m_gizmosGuid = m_editorRoot->GetIdValue();
			m_editorRoot->SetName(EDITOR_OBJECT);
			m_translateX = zm->CreateEditorZobObject(m_editorRoot);
			m_translateX->SetName(EDITOR_ARROW_X);
			m_translateY = zm->CreateEditorZobObject(m_editorRoot);
			m_translateY->SetName(EDITOR_ARROW_Y);
			m_translateZ = zm->CreateEditorZobObject(m_editorRoot);
			m_translateZ->SetName(EDITOR_ARROW_Z);
			ZobFilePath zfp;
			zfp.file = "arrow.obj";
			zfp.path = lpTempPathBuffer;// "D:\n\\n\Git\n\\n\directZob\n\\n\resources\n\\n\_editor";
			zfp.name = "arrow.obj";
			zfp.bAbsolute = true;
			m_behaviorTranslateX = m_translateX->LoadMesh(zfp);
			m_behaviorTranslateY = m_translateY->LoadMesh(zfp);
			m_behaviorTranslateZ = m_translateZ->LoadMesh(zfp);

			m_behaviorTranslateX->GetRenderOptions()->color = ZobColor::Red;
			m_behaviorTranslateX->GetRenderOptions()->lightMode = Triangle::RenderOptions::eLightMode_none;
			m_behaviorTranslateX->GetRenderOptions()->zBuffered = false;
			m_behaviorTranslateY->GetRenderOptions()->color = ZobColor::Green;
			m_behaviorTranslateY->GetRenderOptions()->lightMode = Triangle::RenderOptions::eLightMode_none;
			m_behaviorTranslateY->GetRenderOptions()->zBuffered = false;
			m_behaviorTranslateZ->GetRenderOptions()->color = ZobColor::Blue;
			m_behaviorTranslateZ->GetRenderOptions()->lightMode = Triangle::RenderOptions::eLightMode_none;
			m_behaviorTranslateZ->GetRenderOptions()->zBuffered = false;
			m_translateX->SetLocalRotation(0, 90, 0);
			m_translateY->SetLocalRotation(-90, 0, 0);
		}
	}

	void ZobObjectsEditor::Select(ZobObject* z)
	{
		if (z == m_translateX)
		{
			m_currentSelectedGizmo = z;
			m_behaviorTranslateX->GetRenderOptions()->color = ZobColor::Yellow;
		}
		else if (z == m_translateY)
		{
			m_currentSelectedGizmo = z;
			m_behaviorTranslateY->GetRenderOptions()->color = ZobColor::Yellow;
		}
		else if (z == m_translateZ)
		{
			m_currentSelectedGizmo = z;
			m_behaviorTranslateZ->GetRenderOptions()->color = ZobColor::Yellow;
		}
	}

	void ZobObjectsEditor::UnSelect()
	{
		m_behaviorTranslateX->GetRenderOptions()->color = ZobColor::Red;
		m_behaviorTranslateY->GetRenderOptions()->color = ZobColor::Green;
		m_behaviorTranslateZ->GetRenderOptions()->color = ZobColor::Blue;
	}

	string ZobObjectsEditor::gArrow =
"# Blender v2.92.0 OBJ File : ''\n\
# www.blender.org\n\
#mtllib arrow.mtl\n\
o Cylinder\n\
v 0.019752 0.099298 0.800040\n\
v 0.079080 0.064899 0.800123\n\
v 0.101818 -0.010028 0.800104\n\
v 0.071586 -0.071586 0.800050\n\
v 0.029164 -0.096142 0.800012\n\
v -0.019751 -0.099296 0.800044\n\
v -0.079084 -0.064902 0.800113\n\
v -0.101818 0.010028 0.800104\n\
v -0.071586 0.071586 0.800050\n\
v -0.029164 0.096142 0.800012\n\
v 0.000000 -0.000000 1.000000\n\
v 0.009250 0.028675 0.000000\n\
v 0.019085 0.023255 0.000000\n\
v 0.026319 0.014667 0.000000\n\
v 0.029928 0.003479 0.000000\n\
v 0.028981 -0.008239 0.000000\n\
v 0.021998 -0.020930 0.000000\n\
v 0.006663 -0.029624 0.000000\n\
v -0.008239 -0.028981 0.000000\n\
v -0.020930 -0.021998 0.000000\n\
v -0.028884 -0.008762 0.000000\n\
v -0.029928 0.003479 0.000000\n\
v -0.024820 0.017492 0.000000\n\
v -0.013735 0.026817 0.000000\n\
v -0.002427 0.030032 0.000000\n\
v 0.010919 -0.028333 0.800000\n\
v -0.003479 -0.029928 0.800000\n\
v -0.029919 0.005181 0.800000\n\
v -0.023622 0.018703 0.800000\n\
v -0.014667 0.026319 0.800000\n\
v -0.000755 0.030355 0.800000\n\
v 0.021213 0.021213 0.800000\n\
v 0.026817 0.013735 0.800000\n\
v 0.013735 0.026817 0.800000\n\
v -0.016824 -0.025179 0.800000\n\
v -0.027755 -0.012314 0.800000\n\
v 0.022952 -0.019520 0.800000\n\
v 0.028675 -0.009250 0.800000\n\
v 0.030032 0.002427 0.800000\n\
vt 0.296080 0.482406\n\
vt 0.250000 0.250000\n\
vt 0.431082 0.398610\n\
vt 0.483127 0.227039\n\
vt 0.417637 0.082567\n\
vt 0.319333 0.021440\n\
vt 0.203635 0.017650\n\
vt 0.068918 0.101390\n\
vt 0.016873 0.272961\n\
vt 0.082567 0.417637\n\
vt 0.180667 0.478560\n\
vt 0.987280 0.277617\n\
vt 0.801987 0.018620\n\
vt 0.521440 0.180667\n\
vt 0.818200 0.227987\n\
vt 0.821432 0.255761\n\
vt 0.983127 0.227039\n\
vt 1.000000 0.500000\n\
vt 0.990983 0.994555\n\
vt 0.950317 0.500000\n\
vt 0.924683 0.994555\n\
vt 0.890625 0.500000\n\
vt 0.830933 0.500000\n\
vt 0.875000 0.994555\n\
vt 0.825317 0.994555\n\
vt 0.768433 0.500000\n\
vt 0.762817 0.994555\n\
vt 0.705933 0.500000\n\
vt 0.700317 0.994555\n\
vt 0.628903 0.500000\n\
vt 0.637817 0.994555\n\
vt 0.558402 0.993616\n\
vt 0.535153 0.500000\n\
vt 0.481567 0.986903\n\
vt 0.455933 0.500000\n\
vt 0.406169 0.983138\n\
vt 0.296875 0.500000\n\
vt 0.222848 0.983138\n\
vt 0.231567 0.500000\n\
vt 0.152152 0.500000\n\
vt 0.080933 0.994555\n\
vt 0.075317 0.500000\n\
vt 0.018433 0.994555\n\
vt 0.012817 0.500000\n\
vt 0.958649 0.366318\n\
vt 0.901521 0.434629\n\
vt 0.979787 0.184712\n\
vt 0.921773 0.086497\n\
vt 0.556574 0.386627\n\
vt 0.641138 0.462635\n\
vt 0.730796 0.488109\n\
vt 0.823378 0.477333\n\
vt 0.684712 0.020213\n\
vt 0.586497 0.078227\n\
vt 0.512720 0.277618\n\
vt 0.680667 0.478560\n\
vt 0.796080 0.482406\n\
vt 0.748158 0.321020\n\
vt 0.931082 0.398610\n\
vt 0.782659 0.313790\n\
vt 0.813790 0.282659\n\
vt 0.800912 0.300912\n\
vt 0.715105 0.312595\n\
vt 0.582567 0.417637\n\
vt 0.516873 0.272961\n\
vt 0.693807 0.294476\n\
vt 0.568918 0.101390\n\
vt 0.685058 0.221200\n\
vt 0.703635 0.017650\n\
vt 0.710480 0.190920\n\
vt 0.775477 0.183681\n\
vt 0.819333 0.021440\n\
vt 0.741715 0.178816\n\
vt 0.679985 0.262049\n\
vt 0.917637 0.082567\n\
vt 0.804584 0.203563\n\
vt 0.378903 0.500000\n\
vt 0.316405 0.983138\n\
vt 0.143433 0.987821\n\
vn 0.4519 0.7804 0.4322\n\
vn 0.8651 0.2624 0.4275\n\
vn 0.8093 -0.3978 0.4322\n\
vn 0.4497 -0.7776 0.4394\n\
vn 0.0581 -0.8964 0.4394\n\
vn -0.4519 -0.7804 0.4322\n\
vn -0.8651 -0.2624 0.4275\n\
vn -0.8093 0.3978 0.4322\n\
vn -0.4497 0.7776 0.4394\n\
vn -0.0581 0.8964 0.4394\n\
vn 0.0000 0.0000 -1.0000\n\
vn 0.0014 -0.0002 -1.0000\n\
vn 0.1155 0.9933 -0.0006\n\
vn 0.4826 0.8758 -0.0007\n\
vn 0.8002 0.5997 0.0002\n\
vn 0.9619 0.2735 0.0002\n\
vn 0.9933 -0.1155 0.0002\n\
vn 0.8735 -0.4868 -0.0002\n\
vn 0.5909 -0.8067 0.0007\n\
vn 0.1101 -0.9939 0.0010\n\
vn -0.3353 -0.9421 0.0009\n\
vn -0.9964 -0.0850 0.0002\n\
vn -0.9395 0.3425 -0.0007\n\
vn -0.6437 0.7652 -0.0003\n\
vn -0.2735 0.9619 0.0002\n\
vn 0.0005 0.0004 -1.0000\n\
vn 0.0016 0.0004 -1.0000\n\
vn 0.0016 0.0007 -1.0000\n\
vn 0.0015 0.0004 -1.0000\n\
vn 0.0014 0.0010 -1.0000\n\
vn -0.0001 0.0002 -1.0000\n\
vn -0.0009 -0.0000 -1.0000\n\
vn -0.0013 -0.0002 -1.0000\n\
vn -0.0004 0.0005 -1.0000\n\
vn -0.0016 -0.0006 -1.0000\n\
vn -0.0015 -0.0005 -1.0000\n\
vn -0.0006 -0.0004 -1.0000\n\
vn 0.0001 -0.0007 -1.0000\n\
vn -0.0002 -0.0006 -1.0000\n\
vn -0.0012 -0.0010 -1.0000\n\
vn 0.0011 0.0014 -1.0000\n\
vn 0.0001 0.0005 -1.0000\n\
vn -0.0015 -0.0002 -1.0000\n\
vn -0.0014 0.0007 -1.0000\n\
vn 0.0013 0.0003 -1.0000\n\
vn 0.0009 0.0000 -1.0000\n\
vn 0.0004 -0.0006 -1.0000\n\
vn 0.0014 -0.0008 -1.0000\n\
vn 0.2372 0.9715 0.0009\n\
vn 0.5997 0.8002 0.0004\n\
vn 0.7648 0.6442 -0.0004\n\
vn 0.9517 0.3070 -0.0002\n\
vn 0.9968 -0.0805 -0.0002\n\
vn 0.8761 -0.4821 -0.0003\n\
vn 0.4932 -0.8699 -0.0012\n\
vn -0.0431 -0.9991 -0.0009\n\
vn -0.4821 -0.8761 -0.0010\n\
vn -0.7621 -0.6475 0.0013\n\
vn -0.8571 -0.5151 -0.0011\n\
vn -0.9924 -0.1227 0.0009\n\
vn -0.9065 0.4222 0.0007\n\
vn -0.6479 0.7618 -0.0002\n\
vn -0.2786 0.9604 0.0003\n\
g Cylinder_Cylinder_None\n\
usemtl None\n\
s off\n\
f 1/1/1 11/2/1 2/3/1\n\
f 2/3/2 11/2/2 3/4/2\n\
f 3/4/3 11/2/3 4/5/3\n\
f 4/5/4 11/2/4 5/6/4\n\
f 5/6/5 11/2/5 6/7/5\n\
f 6/7/6 11/2/6 7/8/6\n\
f 7/8/7 11/2/7 8/9/7\n\
f 8/9/8 11/2/8 9/10/8\n\
f 9/10/9 11/2/9 10/11/9\n\
f 10/11/10 11/2/10 1/1/10\n\
f 15/12/11 18/13/11 21/14/11\n\
f 38/15/12 39/16/12 3/17/12\n\
f 25/18/13 31/19/13 12/20/13\n\
f 12/20/14 34/21/14 13/22/14\n\
f 14/23/15 32/24/15 33/25/15\n\
f 15/26/16 33/25/16 39/27/16\n\
f 16/28/17 39/27/17 38/29/17\n\
f 17/30/18 38/29/18 37/31/18\n\
f 17/30/19 37/31/19 26/32/19\n\
f 18/33/20 26/32/20 27/34/20\n\
f 19/35/21 27/34/21 35/36/21\n\
f 21/37/22 28/38/22 22/39/22\n\
f 22/39/23 28/38/23 23/40/23\n\
f 23/40/24 30/41/24 24/42/24\n\
f 24/42/25 31/43/25 25/44/25\n\
f 14/45/11 15/12/11 13/46/11\n\
f 16/47/11 17/48/11 15/12/11\n\
f 23/49/11 24/50/11 25/51/11\n\
f 25/51/11 12/52/11 13/46/11\n\
f 18/13/11 19/53/11 21/14/11\n\
f 19/53/11 20/54/11 21/14/11\n\
f 21/14/11 22/55/11 25/51/11\n\
f 22/55/11 23/49/11 25/51/11\n\
f 25/51/11 13/46/11 15/12/11\n\
f 15/12/11 17/48/11 18/13/11\n\
f 25/51/11 15/12/11 21/14/11\n\
f 10/56/26 1/57/26 31/58/26\n\
f 1/57/27 2/59/27 34/60/27\n\
f 2/59/28 3/17/28 33/61/28\n\
f 3/17/29 39/16/29 33/61/29\n\
f 2/59/30 33/61/30 32/62/30\n\
f 10/56/31 31/58/31 30/63/31\n\
f 9/64/32 10/56/32 30/63/32\n\
f 8/65/33 9/64/33 29/66/33\n\
f 29/66/34 9/64/34 30/63/34\n\
f 7/67/35 8/65/35 36/68/35\n\
f 6/69/36 7/67/36 35/70/36\n\
f 26/71/37 5/72/37 6/69/37\n\
f 27/73/38 26/71/38 6/69/38\n\
f 6/69/39 35/70/39 27/73/39\n\
f 7/67/40 36/68/40 35/70/40\n\
f 2/59/41 32/62/41 34/60/41\n\
f 1/57/42 34/60/42 31/58/42\n\
f 28/74/43 36/68/43 8/65/43\n\
f 8/65/44 29/66/44 28/74/44\n\
f 3/17/45 4/75/45 37/76/45\n\
f 4/75/46 5/72/46 26/71/46\n\
f 37/76/47 4/75/47 26/71/47\n\
f 3/17/48 37/76/48 38/15/48\n\
f 12/20/49 31/19/49 34/21/49\n\
f 13/22/50 34/21/50 32/24/50\n\
f 14/23/51 13/22/51 32/24/51\n\
f 15/26/52 14/23/52 33/25/52\n\
f 16/28/53 15/26/53 39/27/53\n\
f 17/30/54 16/28/54 38/29/54\n\
f 18/33/55 17/30/55 26/32/55\n\
f 19/35/56 18/33/56 27/34/56\n\
f 20/77/57 19/35/57 35/36/57\n\
f 20/77/58 35/36/58 36/78/58\n\
f 21/37/59 20/77/59 36/78/59\n\
f 21/37/60 36/78/60 28/38/60\n\
f 23/40/61 28/38/61 29/79/61\n\
f 23/40/62 29/79/62 30/41/62\n\
f 24/42/63 30/41/63 31/43/63";
}
#endif //_WINDLL