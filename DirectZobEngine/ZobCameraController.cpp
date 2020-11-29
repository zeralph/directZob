#include "ZobCameraController.h"


ZobCameraController::ZobCameraController(Camera * c)
{
    m_zobCamera = c;
}

ZobCameraController::~ZobCameraController()
{
    m_zobCamera = NULL;
}

void ZobCameraController::Update()
{

}